#include <robo.h>

robo::robo (QString n, QColor c, float x, float y, float d)
{
    nome = n;
    cor = c;
    this->setX(x);
    this->setY(y);
    this->setRotation(d);
};

QRectF robo::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
                  20 + penWidth, 20 + penWidth);
}

void robo::setVars (float vel, float velerr, float rot, float roterr, float mederr)
{
    this->spd = vel;
    this->spderr = velerr;
    this->trn = rot;
    this->trnerr = roterr;
    this->meaerr = mederr;
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
}

void robo::Andar(float u[])
{
    float c = 0.01745329251; // pi/180
    float x = GaussRnd(u[0],this->spderr)*cos(this->rotation()*c);
    float y = GaussRnd(u[0],this->spderr)*sin(this->rotation()*c);
    float r = GaussRnd(u[1], this->trnerr);
    this->setX(this->x()+x);
    this->setY(this->y()+y);
    this->setRotation(this->rotation()+r);
}

//void robo::GiraAH()
//{

//    this->setRotation(this->rotation()-r);
//}

//void robo::GiraH()
//{
//    float r = GaussRnd(this->trn, this->trnerr);
//    this->setRotation(this->rotation()+r);
//}

void robo::Medida(float z[])
{
    z[0] = GaussRnd(this->x(), this->meaerr);
    z[1] = GaussRnd(this->y(), this->meaerr);
}
