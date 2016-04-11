#include <robo.h>

// Initializer
robo::robo (int n, QColor c, float x, float y, float d, bool v, Landmarks *lm, BlackBoard *bb)
{
    nome = n;
    cor = c;
    this->setX(x);
    this->setY(y);
    this->setRotation(d);
    visao = v;
    BB = bb;
    BB->x[nome] = this->x();
    BB->y[nome] = this->y();
    BB->r[nome] = this->rotation();
    L = lm;
};

// Needed for dawing
QRectF robo::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
                  20 + penWidth, 20 + penWidth);
}

void robo::paint(QPainter *painter, const QStyleOptionGraphicsItem
                 *option, QWidget *widget)
{
    painter->setBrush(QBrush(cor));
    painter->drawEllipse(-10, -10, 20, 20);
    QLineF *linha;
    if(visao)
    {
        linha = new QLineF(0, 0, LDIST, 0);
        painter->drawArc(-LDIST, -LDIST, 2*LDIST, 2*LDIST, -30*16, 60*16);
        painter->drawArc(-SDIST, -SDIST, 2*SDIST, 2*SDIST, -30*16, 60*16);
    }else{
        linha = new QLineF(0, 0, 10, 0);
    }

    linha->setAngle(30);
    painter->drawLine(*linha);
    linha->setAngle(-30);
    painter->drawLine(*linha);

    option = option;
    widget = widget;
}

// ---
void robo::setVars (float velerr, float roterr, float mederr)
{
    spderr = velerr;
    trnerr = roterr;
    meaerr = mederr;
}

void robo::Andar(float u[])
{
    // I'm using the x, y and rotation of the class QGraphicsItem

    // Note that I'm using my Pseudo-Gauss-Random to generate the statistical errors of movement
    float r = GaussRnd(u[1], trnerr);

    // This is normalizing the angle
    if (this->rotation()+r > 180)
    {
        this->setRotation(this->rotation()+r-360.0);
    }else if(this->rotation()+r < -180){
        this->setRotation(this->rotation()+r+360.0);
    }else{
        this->setRotation(this->rotation()+r);
    }

    float x = GaussRnd(u[0], spderr)*cos(this->rotation()*PI/180);
    float y = GaussRnd(u[0], spderr)*sin(this->rotation()*PI/180);

    this->setX(this->x()+x);
    this->setY(this->y()+y);

    BB->x[nome] = this->x();
    BB->y[nome] = this->y();
    BB->r[nome] = this->rotation();
}

void robo::Medida(Measures *z)
{

    if (z->t_orient) {
        // Orientation
        orienterr = GaussRnd(orienterr, 0.3);
        if (pow(orienterr, 2) > pow(30, 2))
        {
            orienterr = 0;
            qDebug() << "-=-=-=-=-=-=- IMU Reset -=-=-=-=-=-=-";
        }
        z->orientation = rotation() + orienterr;
    }
    // Ball

//    FindBall();

    float d, r;

    if (z->t_ball) {
        dist(BB->B[0], BB->B[1], x(), y(), &d, &r);
        d = GaussRnd(d, meaerr*d/10);
        if((d < LDIST) && (d > 10) && compAng(r, rotation()) && (L->Bknow[0] || L->Bknow[1]))
        {
            z->ball = d;
        }
    }


    // Goalkeeper
    int k;

    if(nome == 0) k = 1;
    else k = 0;

    if (z->t_robo) {
        dist(BB->x[k], BB->y[k], x(), y(), &d, &r);
        d = GaussRnd(d, meaerr*d/10);

        if((d < LDIST) && (d > 10) && compAng(r, rotation()))
        {
            z->robo = d;
        }
    }

    // Landmark L
    if (z->t_lmL){
        for (int i = 0; i < 8; i++)
        {
            dist(L->L[i][0], L->L[i][1], x(), y(), &d, &r);
            d = GaussRnd(d, meaerr*d/10);

            if((d < SDIST) && (d > 10) && compAng(r, rotation()) && (d < z->lmL))
                z->lmL = d;
        }
    }
    // Landmark T
    if (z->t_lmT){
        for (int i = 0; i < 6; i++)
        {
            dist(L->T[i][0], L->T[i][1], x(), y(), &d, &r);
            d = GaussRnd(d, meaerr*d/10);

            if((d < SDIST) && (d > 10) && compAng(r, rotation()) && (d < z->lmT))
                z->lmT = d;
        }
    }

    // Landmark X
    if (z->t_lmX){
        for (int i = 0; i < 2; i++)
        {
            dist(L->X[i][0], L->X[i][1], x(), y(), &d, &r);
            d = GaussRnd(d, meaerr*d/10);

            if((d < SDIST) && (d > 10) && compAng(r, rotation()) && (d < z->lmX))
                z->lmX = d;
        }
    }

    // Goal Poles
    if(z->t_goal){
        for (int i = 0; i < 4; i++)
        {
            dist(L->G[i][0], L->G[i][1], x(), y(), &d, &r);
            d = GaussRnd(d, meaerr*d/10);

            if((d < LDIST) && (d > 10) && compAng(r, rotation()))
            {
                if (d < z->goal1)
                {
                    z->goal2 = z->goal1;
                    z->goal1 = d;
                }else if(d < z->goal2){
                    z->goal2 = d;
                }
            }
        }
    }

//    qDebug() << BB->x[0] << BB->y[0];
}

void robo::FindBall()
{
//    qDebug() << "Find Ball:" << nome << L->Fknow[nome] << L->Bknow[nome];
    float d, r;
    dist(BB->B[0], BB->B[1], x(), y(), &d, &r);
    d = GaussRnd(d, meaerr*d/10);
    if(!((d < LDIST) && (d > 10) && compAng(r, rotation())) || !L->Fknow[nome])
    {
        L->Bknow[nome] = false;
        return;
    }

    r = r*PI/180;

    L->Best[nome][0] = L->F[nome][0];
    L->Best[nome][1] = L->F[nome][1];
    L->Best[nome][2] = d;
    L->Best[nome][3] = r;
    L->Bknow[nome] = true;

    float X1 = 0;
    float Y1 = 0;
    float X2 = 1;
    float Y2 = 1;

    if (L->Bknow[0] && L->Bknow[1])
    {
        float Ax = L->Best[0][0];
        float Ay = L->Best[0][1];
        float A = L->Best[0][3];
        float Bx = L->Best[1][0];
        float By = L->Best[1][1];
        float B = L->Best[1][3];
        X1 = Bx+(cos(A)*(By-Ay)+sin(A)*(Ax-Bx))/(sin(A)*cos(B)-sin(B)*cos(A))*cos(B);
        Y1 = By+(cos(A)*(By-Ay)+sin(A)*(Ax-Bx))/(sin(A)*cos(B)-sin(B)*cos(A))*sin(B);
        X2 = Ax+((Ax-Bx)*sin(B)+(By-Ay)*cos(B))/(sin(A)*cos(B)-cos(A)*sin(B))*cos(A);
        Y2 = Ay+((Ax-Bx)*sin(B)+(By-Ay)*cos(B))/(sin(A)*cos(B)-cos(A)*sin(B))*sin(A);
    }

//    qDebug() << nome << X1 << X2 << Y1 << Y2;

    if (X1 != X2 || Y1 != Y2)
    {
        X1 = x()+d*cos(r);
        Y1 = y()+d*sin(r);
    }

    L->B[0] = X1;
    L->B[1] = Y1;
}
