#ifndef BOLA_H
#define BOLA_H

#include <QtWidgets>
#include <fun.h>

#define BolaX 550
#define BolaY 300

class Bola : public QGraphicsItem
{
public:
    Bola(Landmarks *lm, BlackBoard *b);
    void Move(float X, float Y);

    Landmarks *L;
    BlackBoard *BB;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem
                     *option, QWidget *widget);
};

#endif // BOLA_H
