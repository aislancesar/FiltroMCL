#include "bola.h"

Bola::Bola(Landmarks *lm, BlackBoard *b)
{
    L = lm;
    B = b;
    setX(450);
    setY(300);
    B->B[0] = 450;
    B->B[1] = 300;
    L->B[0] = 450;
    L->B[1] = 300;
}

void Bola::Move(float X, float Y)
{
    setX(x()+X);
    setY(y()+Y);
    B->B[0] = x()+X;
    B->B[1] = y()+Y;
}

QRectF Bola::boundingRect() const
{

    return QRectF(-5, -5, 10, 10);
}

void Bola::paint(QPainter *painter, const QStyleOptionGraphicsItem
                 *option, QWidget *widget)
{
    painter->setBrush(QColor(255, 255, 0));
    painter->setPen(QPen(QColor(255, 255, 0)));
    painter->drawEllipse(-5, -5, 10, 10);

    float X = L->B[0]-x();
    float Y = L->B[1]-y();

    QLineF linha(X, Y-10, X, Y+10);
    QLineF linha2(X-10, Y, X+10, Y);
    painter->drawLine(linha);
    painter->drawLine(linha2);

    option = option;
    widget = widget;
}
