#ifndef BOLA_H
#define BOLA_H

#include <QtWidgets>
#include <fun.h>

class Bola : public QGraphicsItem
{
public:
    Bola(Landmarks *lm, BlackBoard *b);
    void Move(float X, float Y);

    Landmarks *L;
    BlackBoard *B;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem
                     *option, QWidget *widget);
};

#endif // BOLA_H
