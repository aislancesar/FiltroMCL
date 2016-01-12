#include <particulas.h>


Particulas::Particulas(QColor c, int Q)
{
    this->cor = c;
    this->Qtd = Q;

    for(int i = 0; i < Q; i++)
    {
        this->Px[i] = qrand()%600;
        this->Py[i] = qrand()%400;
//        this->Px[i] = qrand()%20 + 90;
//        this->Py[i] = qrand()%20 + 90;
        this->Pr[i] = qrand()%360;
    }
}

void Particulas::MudaQtd(int nQtd)
{
    if (nQtd > Qtd)
    {
        for(int i = Qtd; i < nQtd; i++)
        {
            this->Px[i] = qrand()%600;
            this->Py[i] = qrand()%400;
            this->Pr[i] = qrand()%360;
        }
    }
    Qtd = nQtd;
}

void Particulas::Atualiza()
{
    float Pnx[1000], Pny[1000], Pnr[1000], Pnw[1000];
    float Max = 0;
    for(int i = 0; i < this->Qtd; i++)
    {
        if (Pw[i] > Max)
        {
            Max = Pw[i];
        }
    }
    qDebug("%g", Max);
    Max *= 2;
    float rnd;
    int c = 0;
    for(int i = 0; i < this->Qtd; i++)
    {
        rnd = Max * (qrand()%1000000)/1000000.0;

        while (this->Pw[c] < rnd)
        {
            rnd -= Pw[c];
            c = (c+1)%Qtd;
        }
        Pnx[i] = this->Px[c];
        Pny[i] = this->Py[c];
        Pnr[i] = this->Pr[c];
        Pnw[i] = this->Pw[c];
        //qDebug("%g", Pw[c]);
    }
    //qDebug("---");
    for(int i = 0; i < this->Qtd; i++)
    {
        this->Px[i] = Pnx[i];
        this->Py[i] = Pny[i];
        this->Pr[i] = Pnr[i];
        this->Pw[i] = Pnw[i];
    }
}

void Particulas::Erros(float Mov, float Rot, float Med)
{
    this->MedErr = Med;
    this->MovErr = Mov;
    this->RotErr = Rot;
}

void Particulas::Move(float u[])
{
    float c = 0.01745329251; // pi/180
    for(int i=0; i<this->Qtd; i++)
    {
        Px[i] += GaussRnd(u[0],this->MovErr)*cos(this->Pr[i]*c);
        Py[i] += GaussRnd(u[0],this->MovErr)*sin(this->Pr[i]*c);
        Pr[i] += GaussRnd(u[1],this->RotErr);
    }
}

void Particulas::Mede(float z[])
{
    float pi = 2*3.14159265359;
    for (int i=0; i<this->Qtd; i++)
    {        
        this->Pw[i] = exp(-(pow((this->Px[i]-z[0])/10.0, 2.0)/(pow(this->MedErr, 2.0)/2.0)))*exp(-(pow((this->Py[i]-z[1])/10.0, 2.0)/(pow(this->MedErr, 2.0)/2.0)))/(pi*pow(this->MedErr,2.0));
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
    for(int i = 0; i<this->Qtd; i++)
    {
        //painter->setBrush(QBrush(this->cor));
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(this->cor));
        painter->drawEllipse(this->Px[i]-1.5, this->Py[i]-1.5, 3, 3);
        QLineF linha(this->Px[i], this->Py[i], this->Px[i]+5, this->Py[i]);
        linha.setAngle(-this->Pr[i]);
        painter->drawLine(linha);
    }
}
