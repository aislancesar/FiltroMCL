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
        linha = new QLineF(0, 0, 600, 0);
        painter->drawArc(-600, -600, 1200, 1200, -30*16, 60*16);
        painter->drawArc(-200, -200, 400, 400, -30*16, 60*16);
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

    float x = GaussRnd(u[0], spderr)*cos(this->rotation()*pi()/180);
    float y = GaussRnd(u[0], spderr)*sin(this->rotation()*pi()/180);

    this->setX(this->x()+x);
    this->setY(this->y()+y);

    BB->x[nome] = this->x();
    BB->y[nome] = this->y();
    BB->r[nome] = this->rotation();
}

void robo::Medida(float z[])
{
    // Orientation
    orienterr = GaussRnd(orienterr, 0.3);
    if (pow(orienterr, 2) > pow(30, 2))
    {
        orienterr = 0;
        qDebug() << "-=-=-=-=-=-=- IMU Reset -=-=-=-=-=-=-";
    }
    z[0] = rotation() + orienterr;

    // Ball

    FindBall();

    float d, r;
    dist(BB->B[0], BB->B[1], x(), y(), &d, &r);
    d = GaussRnd(d, meaerr*d/10);
    if((d < 600) && (d > 10) && compAng(r, rotation()) && (L->Bknow[0] || L->Bknow[1]))
    {
        z[1] = d;
    }else{
        z[1] = 600;
    }

    // Goalkeeper
    int k;

    if(nome == 0) k = 1;
    else k = 0;

    dist(BB->x[k], BB->y[k], x(), y(), &d, &r);
    d = GaussRnd(d, meaerr*d/10);

    if((d < 600) && (d > 10) && compAng(r, rotation()))
    {
        z[2] = d;
    }else{
        z[2] = 600;
    }

//    qDebug() << BB->x[0] << BB->y[0];
}

void robo::FindBall()
{
    float d, r;
    dist(BB->B[0], BB->B[1], x(), y(), &d, &r);
    d = GaussRnd(d, meaerr*d/10);
    if(!((d < 600) && (d > 10) && compAng(r, rotation())) || !L->Fknow[nome])
    {
        d = 600;
        L->Bknow[nome] = false;
        return;
    }

    r = r*pi()/180;
    L->Best[nome][0] = x();
    L->Best[nome][1] = y();
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

    if(Y1 == 0) qDebug() << X1 << X2 << Y1 << Y2;

    if (X1 != X2 && Y1 != Y2)
    {
        X1 = x()+d*cos(r);
        Y1 = y()+d*sin(r);
    }

    L->B[0] = X1;
    L->B[1] = Y1;
}
