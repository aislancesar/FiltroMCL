#include <robo.h>

// Initializer
robo::robo (QString n, QColor c, float x, float y, float d)
{
    nome = n;
    cor = c;
    this->setX(x);
    this->setY(y);
    this->setRotation(d);
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
    QLineF linha(0, 0, 10, 0);
    linha.setAngle(45);
    painter->drawLine(linha);
    linha.setAngle(-45);
    painter->drawLine(linha);
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
    float c = 0.01745329251; // pi/180

    // Note that I'm using my Pseudo-Gauss-Random to generate the statistical errors of movement
    float r = GaussRnd(u[1], trnerr);

    // This is normalizing the angle
    if (this->rotation()+r > 360)
    {
        this->setRotation(this->rotation()+r-360.0);
    }else if(this->rotation()+r < -360){
        this->setRotation(this->rotation()+r+360.0);
    }else{
        this->setRotation(this->rotation()+r);
    }

    float x = GaussRnd(u[0], spderr)*cos(this->rotation()*c);
    float y = GaussRnd(u[0], spderr)*sin(this->rotation()*c);

    this->setX(this->x()+x);
    this->setY(this->y()+y);
}

void robo::Medida(float z[])
{
    // Returns the measured position with statistical error
    z[0] = GaussRnd(this->x(), meaerr);
    z[1] = GaussRnd(this->y(), meaerr);
}
