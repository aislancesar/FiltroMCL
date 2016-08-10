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

    float d, r;

    if (z->t_lm){
        dist(L->LM[0], L->LM[1], x(), y(), &d, &r);
        d = GaussRnd(d, meaerr*d/10);

        if((d < SDIST) && (d > 10) && compAng(r, rotation()) && (d < z->lm))
             z->lm = d;
    }
}

void robo::FindBall()
{
    // Vazio
}
