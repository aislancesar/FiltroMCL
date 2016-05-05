#include <particulas.h>

Particulas::Particulas(QColor c, int Q, robo *ROB, Landmarks *l)
{
    cor = c;
    Qtd = Q;
    L = l;
    rob = ROB->nome;

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

void Particulas::EstRobo(int tp)
{
    Regiao A;
    for(int i = 0; i < 2; i++) Reg[i] = A;

    if (tp == 0){
        for (int i = 0; i < Qtd; i++){
            Reg[0].cx += Pw[i]*Px[i];
            Reg[0].cy += Pw[i]*Py[i];
            Reg[0].pw += Pw[i];
            Reg[0].rc += cos(Pr[i]*PI/180)*Pw[i];
            Reg[0].rs += sin(Pr[i]*PI/180)*Pw[i];
        }
    }else if(tp == 1){
        double p = 0;
        for (int i = 0; i < Qtd; i++){
            if (Pw[i] > p) {
                Reg[0].cx = Px[i]*Pw[i];
                Reg[0].cy = Py[i]*Pw[i];
                Reg[0].pw = Pw[i];
                Reg[0].rc = cos(Pr[i]*PI/180)*Pw[i];
                Reg[0].rs = sin(Pr[i]*PI/180)*Pw[i];
                p = Pw[i];
            }
        }
    }else if(tp == 2){
        for (int i = 0; i < Qtd; i++){
            Reg[0].cx += Px[i];
            Reg[0].cy += Py[i];
            Reg[0].pw += 1;
            Reg[0].rc += cos(Pr[i]*PI/180);
            Reg[0].rs += sin(Pr[i]*PI/180);
        }
    }

    L->F[rob][0] = Reg[0].cx/Reg[0].pw;
    L->F[rob][1] = Reg[0].cy/Reg[0].pw;
    L->Fknow[rob] = true;

    Reg[0].d = 20;
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
//    qDebug() << Qtd;
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
        Px[i] += GaussRnd(u[0], MovErr)*cos(Pr[i]*PI/180);
        Py[i] += GaussRnd(u[0], MovErr)*sin(Pr[i]*PI/180);
    }
}

double Particulas::Mede(Measures zr)
{
    float d, r;

    double Sum = 0;

    for (int c = 0; c < Qtd; c++)
    {
        Measures z;
        double pw = 1;

        // Orientation
        if (zr.t_orient){
            z.orientation = Pr[c];
            pw *= AngGaussian(zr.orientation, OGVar, z.orientation);
        }

        // Ball
        if (zr.t_ball){
            dist(L->B[0], L->B[1], Px[c], Py[c], &d, &r);
            d = GaussRnd(d, MedErr*d/10);
            if((d < LDIST) && (d > 10) && compAng(r, Pr[c]) && (L->Bknow[0] || L->Bknow[1]))
            {
                z.ball = d;
            }
            pw *= Gaussian(zr.ball, MedErr, z.ball);
        }

        // GoalKeeper
        int k;

        if (rob == 0) k = 1;
        else k = 0;

        if(zr.t_robo){
            dist(L->F[k][0], L->F[k][1], Px[c], Py[c], &d, &r);
            d = GaussRnd(d, MedErr*d/10);
            if((d < LDIST) && (d > 10) && compAng(r, Pr[c]))
            {
                z.robo = d;
            }
            pw *= Gaussian(zr.robo, MedErr, z.robo);
        }

        // Landmark L
        if (zr.t_lmL){
            for (int i = 0; i < 8; i++)
            {
                dist(L->L[i][0], L->L[i][1], Px[c], Py[c], &d, &r);
                d = GaussRnd(d, MedErr*d/10);

                if((d < SDIST) && (d > 10) && compAng(r, Pr[c]) && (d < z.lmL))
                    z.lmL = d;
            }
            pw *= Gaussian(zr.lmL, MedErr, z.lmL);
        }

        // Landmark T
        if (zr.lmT){
            for (int i = 0; i < 6; i++)
            {
                dist(L->T[i][0], L->T[i][1], Px[c], Py[c], &d, &r);
                d = GaussRnd(d, MedErr*d/10);

                if((d < SDIST) && (d > 10) && compAng(r, Pr[c]) && (d < z.lmT))
                    z.lmT = d;
            }
            pw *= Gaussian(zr.lmT, MedErr, z.lmT);
        }

        // Landmark X
        if (zr.lmX){
            for (int i = 0; i < 2; i++)
            {
                dist(L->X[i][0], L->X[i][1], Px[c], Py[c], &d, &r);
                d = GaussRnd(d, MedErr*d/10);

                if((d < SDIST) && (d > 10) && compAng(r, Pr[c]) && (d < z.lmX))
                    z.lmX = d;
            }
            pw *= Gaussian(zr.lmX, MedErr, z.lmX);
        }

        // Goal Poles
        if (zr.t_goal){
            for (int i = 0; i < 4; i++)
            {
                dist(L->G[i][0], L->G[i][1], Px[c], Py[c], &d, &r);
                d = GaussRnd(d, MedErr*d/10);

                if((d < LDIST) && (d > 10) && compAng(r, Pr[c]))
                {
                    if (d < z.goal1)
                    {
                        z.goal2 = z.goal1;
                        z.goal1 = d;
                    }else if(d < z.goal2){
                        z.goal2 = d;
                    }
                }
            }
            pw *= Gaussian(zr.goal1, MedErr, z.goal1);
            pw *= Gaussian(zr.goal2, MedErr, z.goal2);
        }

        Pw[c] = max(1e-300, pw);

        Sum += Pw[c];
    }
    return Sum;
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
        painter->setPen(QPen(cor));
        QLineF linha(Px[i], Py[i], Px[i]+5, Py[i]);
        linha.setAngle(-Pr[i]);
        painter->drawLine(linha);    
    }

    painter->setPen(QPen(QColor(0, 0, 255)));
    painter->setBrush(Qt::NoBrush);
    int M = 0;
    for(int i = 0; i < 2; i++)
    {
        painter->drawEllipse(Reg[i].cx/Reg[i].pw-Reg[i].d, Reg[i].cy/Reg[i].pw-Reg[i].d, 2*Reg[i].d, 2*Reg[i].d);
        if (max(Reg[M].pw, Reg[i].pw) == Reg[i].pw) M = i;
    }
    x = Reg[M].cx/Reg[M].pw;
    y = Reg[M].cy/Reg[M].pw;
    r = atan2(Reg[M].rs, Reg[M].rc)*180/PI;

    painter->setBrush(QBrush(QColor(0, 255, 0, 128)));
    painter->drawEllipse(x-10, y-10, 20, 20);

    QLineF linha(x, y, x+10, y);
    linha.setAngle(-r+30);
    painter->drawLine(linha);
    linha.setAngle(-r-30);
    painter->drawLine(linha);

    option = option;
    widget = widget;
}
