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

void Particulas::Nova(float *nPx, float *nPy, float *nPr, double *nPw)
{
    *nPx = 900*UniRnd();
    *nPy = 600*UniRnd();
    *nPr = 360*UniRnd()-180;
    *nPw = 0;
}

void Particulas::TendNova(float *nPx, float *nPy, float *nPr, float *nPw)
{
    int c = UniRnd()*Rg;
    while(Reg[c][2] == 0) c = (c+1)%Rg;
    *nPx = GaussRnd(Reg[c][0]/Reg[c][2], Reg[c][5]);
    *nPy = GaussRnd(Reg[c][1]/Reg[c][2], Reg[c][5]);
    *nPr = GaussRnd(atan2(Reg[c][4], Reg[c][3])*180/pi(), 1);
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

double Particulas::Mede(float zr[])
{
    float z[L->n];
    float d, r;

    double Max = 0;

    for (int c = 0; c < Qtd; c++)
    {
        int i = 0;

        for (int j = 0; j < L->n; j++) z[j] = 600;

        for (; i < 8; i++)
        {
            dist(L->L[i][0], L->L[i][1], Px[c], Py[c], &d, &r);
            d = GaussRnd(d, MedErr*d/10);
            if((d < 600) && (d > 10) && compAng(r, Pr[c]))
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
            dist(L->T[i-8][0], L->T[i-8][1], Px[c], Py[c], &d, &r);
            d = GaussRnd(d, MedErr*d/10);
            if((d < 600) && (d > 10) && compAng(r, Pr[c]))
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
            dist(L->X[i-14][0], L->X[i-14][1], Px[c], Py[c], &d, &r);
            d = GaussRnd(d, MedErr*d/10);
            if((d < 600) && (d > 10) && compAng(r, Pr[c]))
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

        if(i == L->n - 1){
            dist(L->B[0], L->B[1], Px[c], Py[c], &d, &r);
            d = GaussRnd(d, MedErr*d/10);
            if((d < 600) && (d > 10) && compAng(r, Pr[c]))
            {
                z[i] = d;
            }
        }

        double pw = 1;
        for (int j = 0; j < L->n; j++)
            pw *= Gaussian(zr[j], MedErr, z[j]);
        Pw[c] = pw;

        Max = max(Max, pw);

//        if(c == 0){
//            for (int j = 0; j < 17; j++)
//                qDebug("A.append([%g, %g])", zr[j], z[j]);
//        }
        //qDebug() << pw;
    }
    return Max;
}


// Needed to draw the particles
QRectF Particulas::boundingRect() const
{
    return QRectF(-10, -10, 610, 410);
}

void Particulas::paint(QPainter *painter, const QStyleOptionGraphicsItem
                 *option, QWidget *widget)
{
    float x, y, r;

    for(int i = 0; i<Qtd; i++)
    {
//        x += Px[i]*Pw[i];
//        y += Py[i]*Pw[i];
//        rc += cos(Pr[i]*pi()/180)*Pw[i];
//        rs += sin(Pr[i]*pi()/180)*Pw[i];
//        w += Pw[i];
        painter->setPen(QPen(cor));
        QLineF linha(Px[i], Py[i], Px[i]+5, Py[i]);
        linha.setAngle(-Pr[i]);
        painter->drawLine(linha);    
    }

//    x /= w;
//    y /= w;
//    float r = atan2(rs, rc)*180.0/pi();

    painter->setPen(QPen(QColor(0, 0, 255)));
    painter->setBrush(Qt::NoBrush);
    int M = 0;
    for(int i = 0; i < Rg; i++)
    {
        painter->drawEllipse(Reg[i][0]/Reg[i][2]-Reg[i][5], Reg[i][1]/Reg[i][2]-Reg[i][5], 2*Reg[i][5], 2*Reg[i][5]);
        if (max(Reg[M][2], Reg[i][2]) == Reg[i][2]) M = i;
    }
    x = Reg[M][0]/Reg[M][2];
    y = Reg[M][1]/Reg[M][2];
    r = atan2(Reg[M][4], Reg[M][3])*180/pi();

    painter->setBrush(QBrush(QColor(0, 255, 0, 128)));
    painter->drawEllipse(x-10, y-10, 20, 20);
//    painter->drawEllipse(x-30, y-30, 60, 60);
//    painter->drawEllipse(x-100, y-100, 200, 200);
    QLineF linha(x, y, x+10, y);
    linha.setAngle(-r+30);
    painter->drawLine(linha);
    linha.setAngle(-r-30);
    painter->drawLine(linha);

    //qDebug() << Gaussian(2400, MedErr, 2400)*exp(-1);
    option = option;
    widget = widget;
}

/* -- Using only multidimensional distance measure...
void Particulas::Mede(float zr)
{
    float z = 0;
    float d, r;

    for (int c = 0; c < Qtd; c++)
    {
        int i = 0;
        for (; i < 8; i++)
        {
            dist(L->L[i][0], L->L[i][1], Px[c], Py[c], &d, &r);
            d = GaussRnd(d, MedErr*d/10);
            if((d < 600) && (d > 10) && (r > Pr[c]-30) && (r < Pr[c]+30))
            {
                z += pow(d, 2);
            }else{
                z += pow(600, 2);
            }
        }
        for (; i < 14; i++)
        {
            dist(L->T[i-8][0], L->T[i-8][1], Px[c], Py[c], &d, &r);
            d = GaussRnd(d, MedErr*d/10);
            if((d < 600) && (d > 10) && (r > Pr[c]-30) && (r < Pr[c]+30))
            {
                z += pow(d, 2);
            }else{
                z += pow(600, 2);
            }
        }
        for (; i < 16; i++)
        {
            dist(L->X[i-14][0], L->X[i-14][1], Px[c], Py[c], &d, &r);
            d = GaussRnd(d, MedErr*d/10);
            if((d < 600) && (d > 10) && (r > Pr[c]-30) && (r < Pr[c]+30))
            {
                z += pow(d, 2);
            }else{
                z += pow(600, 2);
            }
        }

        if(i == L->n - 1){
            dist(L->B[0], L->B[1], Px[c], Py[c], &d, &r);
            d = GaussRnd(d, MedErr*d/10);
            if((d < 600) && (d > 10) && (r > Pr[c]-30) && (r < Pr[c]+30))
            {
                z += pow(d, 2);
            }else{
                z += pow(600, 2);
            }
        }

        z = sqrt(z);
        Pw[c] = Gaussian(zr, MedErr, z);
    }
}*/
