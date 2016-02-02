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
    *nPx = 900*UniRnd();
    *nPy = 600*UniRnd();
    *nPr = 360*UniRnd()-180;
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
    if (nQtd < 1){
        Qtd = 1;
    }else{
        Qtd = nQtd;
    }
    qDebug("%d", Qtd);
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
    float z[L->n];
    float d, r;
    for (int c = 0; c < Qtd; c++)
    {
        int i = 0;
        double pw = 1;
        for (int j = 0; j < 16; j++)
            z[j] = 1000;

        for (; i < 8; i++)
        {
            d = sqrt(pow(L->L[i][0]-Px[c],2)+pow(L->L[i][1]-Py[c],2));
            r = atan2((L->L[i][1]-Py[c]),(L->L[i][0]-Px[c]))*180/pi();
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
        for (; i < 14; i++)
        {
            d = sqrt(pow(L->T[i-8][0]-Px[c],2)+pow(L->T[i-8][1]-Py[c],2));
            r = atan2((L->T[i-8][1]-Py[c]),(L->T[i-8][0]-Px[c]))*180/pi();
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
        for (; i < 16; i++)
        {
            d = sqrt(pow(L->X[i-14][0]-Px[c],2)+pow(L->X[i-14][1]-Py[c],2));
            r = atan2((L->X[i-14][1]-Py[c]),(L->X[i-14][0]-Px[c]))*180/pi();
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

        if(i == L->n - 1)
        {
            float d = sqrt(pow(L->B[0]-Px[c],2)+pow(L->B[1]-Py[c],2));
            float r = atan2((L->B[1]-Py[c]),(L->B[0]-Px[c]))*180/pi();
            //if (r < 0) r += 360;
            d = GaussRnd(d, MedErr*d/10);
            //qDebug("---- %g < %g ~ < %g", d, r, rotation());
            if((d < 600) && (r > rotation()-30) && (r < rotation()+30))
            {
                z[i] = d;
            }
        }

        for (int j = 0; j < L->n; j++)
        {
            pw *= Gaussian(zr[j], MedErr, z[j]);
        }

        // Normalizes weights
        if(pw < 1e-300) pw = 1e-300;
        Pw[c] = 301+log10(pw);
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

    painter->setBrush(QBrush(QColor(255, 255, 255, 128)));
    painter->drawEllipse(x-10, y-10, 20, 20);
    QLineF linha(x, y, x+10, y);
    linha.setAngle(-r+30);
    painter->drawLine(linha);
    linha.setAngle(-r-30);
    painter->drawLine(linha);

    option = option;
    widget = widget;
}
