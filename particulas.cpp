#include <particulas.h>


Particulas::Particulas(QColor c, int Q)
{
    cor = c;
    Qtd = Q;

    for(int i = 0; i < Q; i++)
    {
        // Note that this generates random particles, but all of them are in a Int state space, even though they are float.
        Nova(&Px[i], &Py[i], &Pr[i], &Pw[i]);
    }
}

void Particulas::Nova(float *nPx, float *nPy, float *nPr, float *nPw)
{
    *nPx = qrand()%900;
    *nPy = qrand()%600;
    *nPr = qrand()%360-180;
    *nPw = 0;
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
            Nova(&Px[i], &Py[i], &Pr[i], &Pw[i]);
        }
    }
    // If not just ignore the particles out of reach
    Qtd = nQtd;
}

void Particulas::landmarks(float rLL[8][2], float rLT[6][2], float rLX[2][2])
{
    for (int i = 0; i < 8; i++)
    {
        LL[i][0] = rLL[i][0];
        LL[i][1] = rLL[i][1];
    }
    for (int i = 0; i < 6; i++)
    {
        LT[i][0] = rLT[i][0];
        LT[i][1] = rLT[i][1];
    }
    for (int i = 0; i < 2; i++)
    {
        LX[i][0] = rLX[i][0];
        LX[i][1] = rLX[i][1];
    }
}

//void Particulas::Atualiza(float u[], float z[])
//{
//    this->Move(u);
//    this->Mede(z);
//    // Generates a new temporary set of particles
//    float Pnx[1000], Pny[1000], Pnr[1000], Pnw[1000];
//    // These are constants for the old set of particles
//    float Max = 0;
//    float wa = 0;
//    //float pi = 2*3.14159265359;
//    // Finds out the biggest weight of the particles set and the average of all weights
//    for(int i = 0; i < Qtd; i++)
//    {
//        if (Pw[i] > Max)
//        {
//            Max = Pw[i];
//        }
//        wa += Pw[i];
//    }
//    wa /= Qtd;
//    // Finds the current values of ws and wf to aply the AMCL algorithm
//    wf += 0.5*(wa-wf);
//    ws += 0.3*(wa-ws);
//    qDebug("- wf %g - ws %g - wa %g -", wf, ws, wa);
//    // This part selects the particles
//    float rnd = 0;
//    int c = qrand()%Qtd;
//    for(int i = 0; i < Qtd; i++)
//    {
//        if((qrand()%1000)/1000.0 < 1-wf/ws)
//        {
//            Nova(&Pnx[i], &Pny[i], &Pnr[i], &Pnw[i]);
//        }else{
//            rnd += 2* Max * (qrand()%1000000)/1000000.0;
//            while (Pw[c] < rnd)
//            {
//                rnd -= Pw[c];
//                c = (c+1)%Qtd;
//            }
//            Pnx[i] = Px[c];
//            Pny[i] = Py[c];
//            Pnr[i] = Pr[c];
//            Pnw[i] = Pw[c];
//        }
//    }
//    // This updates the particles set
//    for(int i = 0; i < Qtd; i++)
//    {
//        Px[i] = Pnx[i];
//        Py[i] = Pny[i];
//        Pr[i] = Pnr[i];
//        Pw[i] = Pnw[i];
//    }
//    // This uses the ws factor to update the number of particles
//    //MudaQtd(Qtd - C);
//}

void Particulas::Erros(float Mov, float Rot, float Med)
{
    MedErr = Med;
    MovErr = Mov;
    RotErr = Rot;
}

void Particulas::Move(float u[])
{
    // It moves the particles using a Pseudo-Gauss-Random generator
    for(int i=0; i<Qtd; i++)
    {
        Pr[i] += GaussRnd(u[1], RotErr);
        if (Pr[i] > 180) Pr[i] -= 360;
        if (Pr[i] < -180) Pr[i] += 360;
        Px[i] += GaussRnd(u[0], MovErr)*cos(Pr[i]*pi()/180);
        Py[i] += GaussRnd(u[0], MovErr)*sin(Pr[i]*pi()/180);
    }
}

void Particulas::Mede(float zr[])
{
    float z[16];
    float d, r;
    for (int c = 0; c < Qtd; c++)
    {
        Pw[c] = 0;
        for (int i = 0; i < 16; i++)
            z[i] = 600;

        for (int i = 0; i < 8; i++)
        {
            d = sqrt(pow(LL[i][0]-Px[c],2)+pow(LL[i][1]-Py[c],2));
            r = atan2((LL[i][1]-Py[c]),(LL[i][0]-Px[c]))*180/pi();
            //if (r < 0) r += 360;
            d = GaussRnd(d, MedErr*d/10);

            if((d < 600) && (r > Pr[c]-30) && (r < Pr[c]+30))
            {
                z[i] = d;
                int a = i;
                while (a > 0)
                {
                    if (z[a-1] > z[a])
                    {
                        z[a] = z[a-1];
                        z[a-1] = d;
                    }
                    a--;
                }
            }
        }
        for (int i = 8; i < 14; i++)
        {
            d = sqrt(pow(LT[i-8][0]-Px[c],2)+pow(LT[i-8][1]-Py[c],2));
            r = atan2((LT[i-8][1]-Py[c]),(LT[i-8][0]-Px[c]))*180/pi();
            //if (r < 0) r += 360;
            d = GaussRnd(d, MedErr*d/10);
            //qDebug("---- %g < %g ~ < %g", d, r, rotation());
            if((d < 600) && (r > Pr[c]-30) && (r < Pr[c]+30))
            {
                z[i] = d;
                int a = i;
                while (a > 8)
                {
                    if (z[a-1] > z[a])
                    {
                        z[a] = z[a-1];
                        z[a-1] = d;
                    }
                    a--;
                }
            }
        }
        for (int i = 14; i < 16; i++)
        {
            d = sqrt(pow(LX[i-14][0]-Px[c],2)+pow(LX[i-14][1]-Py[c],2));
            r = atan2((LX[i-14][1]-Py[c]),(LX[i-14][0]-Px[c]))*180/pi();
            //if (r < 0) r += 360;
            d = GaussRnd(d, MedErr*d/10);
            //qDebug("---- %g < %g ~ < %g", d, r, rotation());
            if((d < 600) && (r > Pr[c]-30) && (r < Pr[c]+30))
            {
                z[i] = d;
                int a = i;
                while (a > 14)
                {
                    if (z[a-1] > z[a])
                    {
                        z[a] = z[a-1];
                        z[a-1] = d;
                    }
                    a--;
                }
            }
        }

        float n = 0;

        for (int i = 0; i < 16; i++)
        {
            n += zr[i];
            if (zr[i] < 600)
                Pw[c] += Gaussian(z[i], MedErr, zr[i]);
                //qDebug("%g", z);
        }
        if (n == 600*16) Pw[c] = Gaussian(600, MedErr, 600);
        qDebug("[%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g]", z[0], z[1], z[2], z[3], z[4], z[5], z[6], z[7], z[8], z[9], z[10], z[11], z[12], z[13], z[14], z[15]);
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
    float x=0, y=0, rc=0, rs=0, w=0;
    for(int i = 0; i<Qtd; i++)
    {
        x += Px[i]*Pw[i];
        y += Py[i]*Pw[i];
        rc += cos(Pr[i]*pi()/180)*Pw[i];
        rs += sin(Pr[i]*pi()/180)*Pw[i];
        w += Pw[i];
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(cor));
        painter->drawEllipse(Px[i]-1.5, Py[i]-1.5, 3, 3);
        QLineF linha(Px[i], Py[i], Px[i]+5, Py[i]);
        linha.setAngle(-Pr[i]);
        painter->drawLine(linha);
    }

    x /= w;
    y /= w;
    float r = atan2(rs, rc)*180.0/pi();

    painter->setBrush(QBrush(Qt::white));
    painter->drawEllipse(x-10, y-10, 20, 20);
    QLineF linha(x, y, x+10, y);
    linha.setAngle(-r+30);
    painter->drawLine(linha);
    linha.setAngle(-r-30);
    painter->drawLine(linha);

    option = option;
    widget = widget;
}
