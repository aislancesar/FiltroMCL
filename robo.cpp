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
    float d, r;
    dist(L->B[0], L->B[1], x(), y(), &d, &r);
    d = GaussRnd(d, meaerr*d/10);
    if((d < 600) && (d > 10) && compAng(r, rotation()))
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
