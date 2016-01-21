#include <particulas.h>


Particulas::Particulas(QColor c, int Q)
{
    cor = c;
    Qtd = Q;

    for(int i = 0; i < Q; i++)
    {
        // Note that this generates random particles, but all of them are in a Int state space, even though they are float.
        Px[i] = qrand()%600;
        Py[i] = qrand()%400;
        Pr[i] = qrand()%360;
        Pw[i] = 0;
    }
}

void Particulas::DesRobo(float rx, float ry)
{
    float x = 0, y = 0, rc = 0, rs = 0, w = 0;
    // Takes the average of the particles and gives the aproximated position of the Robot given by the particles
    for (int i = 0; i < Qtd; i++)
    {
        x += Px[i]*Pw[i];
        y += Py[i]*Pw[i];
        rc += cos(Pr[i])*Pw[i];
        rs += sin(Pr[i])*Pw[i];
        w += Pw[i];
    }
    x /= w;
    y /= w;
    float r = atan2(rs, rc)*180.0/3.1416;
    float d = sqrt(pow(x-rx,2)+pow(y-ry,2));
    qDebug("Erro: %g", d);
    qDebug("Particulas: x.%g y.%g r.%g", x, y, r);
}

void Particulas::MudaQtd(int nQtd)
{
    if (nQtd > Qtd)
    {
        // If the quantity is bigger than the older one, we have to generate new random particles
        for(int i = Qtd; i < nQtd; i++)
        {
            Px[i] = qrand()%600;
            Py[i] = qrand()%400;
            Pr[i] = qrand()%360;
            Pw[i] = 0;
        }
    }
    // If not just ignore the particles out of reach
    Qtd = nQtd;
}

void Particulas::Atualiza(float u[], float z[])
{
    this->Move(u);
    this->Mede(z);
    // Generates a new temporary set of particles
    float Pnx[1000], Pny[1000], Pnr[1000], Pnw[1000];
    // These are constants for the old set of particles
    float Max = 0;
    float wa = 0;
    int C = 0;
    //float pi = 2*3.14159265359;
    // Finds out the biggest weight of the particles set and the average of all weights
    for(int i = 0; i < Qtd; i++)
    {
        if (Pw[i] > Max)
        {
            Max = Pw[i];
        }else if (Pw[i] < 0.053990967/MedErr)
        {
            C++;
            Pw[i] = 0;
        }
        wa += Pw[i];
    }
    wa /= Qtd;
    // Finds the current values of ws and wf to aply the AMCL algorithm
    wf += 0.5*(wa-wf);
    ws += 0.3*(wa-ws);
    qDebug("- wf %g - ws %g - wa %g -", wf, ws, wa);
    // This part selects the particles
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
            rnd += 2* Max * (qrand()%1000000)/1000000.0;
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
    }
    // This updates the particles set
    for(int i = 0; i < Qtd; i++)
    {
        Px[i] = Pnx[i];
        Py[i] = Pny[i];
        Pr[i] = Pnr[i];
        Pw[i] = Pnw[i];
    }
    // This uses the ws factor to update the number of particles
    //MudaQtd(Qtd - C);
}

void Particulas::Erros(float Mov, float Rot, float Med)
{
    MedErr = Med;
    MovErr = Mov;
    RotErr = Rot;
}

void Particulas::Move(float u[])
{
    // It moves the particles using a Pseudo-Gauss-Random generator
    float c = 0.01745329251; // pi/180
    for(int i=0; i<Qtd; i++)
    {
        Pr[i] += GaussRnd(u[1], RotErr);
        Px[i] += GaussRnd(u[0], MovErr)*cos(Pr[i]*c);
        Py[i] += GaussRnd(u[0], MovErr)*sin(Pr[i]*c);
    }
}

void Particulas::Mede(float z[])
{
    // Calculates the weight of the particles in the set
    float pi = 2*3.14159265359;
    float w;
    for (int i=0; i<Qtd; i++)
    {
        w = 1;
        w *= exp(-(pow((Px[i]-z[0]), 2.0)/(pow(MedErr, 2.0)/2.0)));
        w *= exp(-(pow((Py[i]-z[1]), 2.0)/(pow(MedErr, 2.0)/2.0)));
        Pw[i] = w/(sqrt(pi)*MedErr);
    }
}


// Needed to draw the particles
QRectF Particulas::boundingRect() const
{
    return QRectF(-10, -10, 610, 410);
}

void Particulas::paint(QPainter *painter, const QStyleOptionGraphicsItem
                 *option, QWidget *widget)
{
    for(int i = 0; i<Qtd; i++)
    {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(cor));
        painter->drawEllipse(Px[i]-1.5, Py[i]-1.5, 3, 3);
        QLineF linha(Px[i], Py[i], Px[i]+5, Py[i]);
        linha.setAngle(-Pr[i]);
        painter->drawLine(linha);
    }
    option = option;
    widget = widget;
}
