#include "bola.h"

Bola::Bola(Landmarks *lm)
{
    L = lm;
    setX(L->B[0]);
    setY(L->B[1]);
}

void Bola::Move()
{
    setX(L->B[0]);
    setY(L->B[1]);
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

    option = option;
    widget = widget;
}
