#include <particulas.h>


Particulas::Particulas(QColor c, int Q)
{
    cor = c;
    Qtd = Q;

    for(int i = 0; i < Q; i++)
    {
        Px[i] = qrand()%600;
        Py[i] = qrand()%400;
//        this->Px[i] = qrand()%20 + 90;
//        this->Py[i] = qrand()%20 + 90;
        Pr[i] = qrand()%360;
        Pw[i] = 0;
    }
}

void Particulas::DesRobo()
{
    float x = 0, y = 0, rc = 0, rs = 0, w = 0;
    for (int i = 0; i < Qtd; i++)
    {
        x += Px[i]*Pw[i];
        y += Py[i]*Pw[i];
//        rc += cos(Pr[i]);
//        rs += sin(Pr[i]);
        w += Pw[i];
    }
    x /= w;
    y /= w;
    // Não faz sentido fazer a média do ângulo, já que ele não é considerado no cálculo do peso.
    //float r = atan2(rs, rc);
    qDebug("Particulas: x.%g y.%g", x, y);
}

void Particulas::MudaQtd(int nQtd)
{
    if (nQtd > Qtd)
    {
        for(int i = Qtd; i < nQtd; i++)
        {
            Px[i] = qrand()%600;
            Py[i] = qrand()%400;
            Pr[i] = qrand()%360;
            Pw[i] = 0;
        }
    }
    Qtd = nQtd;
}

void Particulas::Atualiza(float z[])
{
    float Pnx[1000], Pny[1000], Pnr[1000], Pnw[1000];
    float Max = 0;
    float wa = 0;
    float pi = 2*3.14159265359;
    for(int i = 0; i < Qtd; i++)
    {
        if (Pw[i] > Max)
        {
            Max = Pw[i];
        }
        wa += Pw[i];
    }
    wa /= Qtd;
    wf += 0.5*(wa-wf);
    ws += 0.1*(wa-ws);
    //qDebug("%g", 1-wf/ws);
    //qDebug("%g", Max);
    Max *= 2;
    float rnd = 0;
    int c = qrand()%Qtd;
    for(int i = 0; i < Qtd; i++)
    {
        if((qrand()%1000)/1000.0 < 1-wf/ws)
        {
            Pnx[i] = qrand()%600;
            Pny[i] = qrand()%400;
            Pnr[i] = qrand()%360;
            Pnw[i] = 0;
        }else{
            rnd += Max * (qrand()%1000000)/1000000.0;
            while (Pw[c] < rnd)
            {
                rnd -= Pw[c];
                c = (c+1)%Qtd;
            }
            Pnx[i] = Px[c];
            Pny[i] = Py[c];
            Pnr[i] = Pr[c];
            Pnw[i] = Pw[c];
        }
        //qDebug("%g", Pw[c]);
    }
    //qDebug("---");
    for(int i = 0; i < Qtd; i++)
    {
        Px[i] = Pnx[i];
        Py[i] = Pny[i];
        Pr[i] = Pnr[i];
        Pw[i] = Pnw[i];
    }
}

void Particulas::Erros(float Mov, float Rot, float Med)
{
    MedErr = Med;
    MovErr = Mov;
    RotErr = Rot;
}

void Particulas::Move(float u[])
{
    float c = 0.01745329251; // pi/180
    for(int i=0; i<Qtd; i++)
    {
        Px[i] += GaussRnd(u[0], MovErr)*cos(Pr[i]*c);
        Py[i] += GaussRnd(u[0], MovErr)*sin(Pr[i]*c);
        Pr[i] += GaussRnd(u[1], RotErr);
    }
}

void Particulas::Mede(float z[])
{
    float pi = 2*3.14159265359;
    for (int i=0; i<Qtd; i++)
    {        
        Pw[i] = exp(-(pow((Px[i]-z[0])/10.0, 2.0)/(pow(MedErr, 2.0)/2.0)))*exp(-(pow((Py[i]-z[1])/10.0, 2.0)/(pow(MedErr, 2.0)/2.0)))/(pi*pow(MedErr,2.0));
        //qDebug("%f %f - %f %f = %g", Px[i], Py[i], z[0], z[1], Pw[i]);
    }
    //for(int i = 0; i < this->Qtd; i++) qDebug("%f", exp(-(pow(this->Px[i]-z[0], 2)/(pow(this->MedErr, 2)/2.0)))/(pi*pow(this->MedErr,2))*exp(-(pow(this->Py[i]-z[1], 2)/(pow(this->MedErr, 2)/2.0)))/(pi*pow(this->MedErr,2)));
}

QRectF Particulas::boundingRect() const
{
    return QRectF(-10, -10, 610, 410);
}

void Particulas::paint(QPainter *painter, const QStyleOptionGraphicsItem
                 *option, QWidget *widget)
{
    for(int i = 0; i<Qtd; i++)
    {
        //painter->setBrush(QBrush(this->cor));

        painter->setBrush(Qt::NoBrush);
        //painter->setPen(QPen(QColor(int(255*(1-Pw[i]/0.0397887)), 0, int(255*(Pw[i]/0.0397887)))));
        painter->setPen(QPen(cor));
        painter->drawEllipse(Px[i]-1.5, Py[i]-1.5, 3, 3);
        QLineF linha(Px[i], Py[i], Px[i]+5, Py[i]);
        linha.setAngle(-Pr[i]);
        painter->drawLine(linha);
    }
}
