#ifndef BOLA_H
#define BOLA_H

#include <QtWidgets>
#include <fun.h>

class Bola : public QGraphicsItem
{
public:
    Bola(Landmarks *lm);
    void Move();

    Landmarks *L;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem
                     *option, QWidget *widget);
};

#endif // BOLA_H
