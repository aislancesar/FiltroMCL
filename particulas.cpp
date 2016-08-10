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
    // Vazio
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

        if (zr.t_lm){
            dist(L->LM[0], L->LM[1], Px[c], Py[c], &d, &r);
            d = GaussRnd(d, MedErr*d/10);

            if((d < SDIST) && (d > 10) && compAng(r, Pr[c]) && (d < z.lm))
                z.lm = d;

            pw *= Gaussian(zr.lm, MedErr, z.lm);
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
