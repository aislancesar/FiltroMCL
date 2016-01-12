#ifndef ROBO_H
#define ROBO_H

#include <QtWidgets>
#include <fun.h>

class robo : public QGraphicsItem
{
public:
    QString nome;
    QColor cor;
    int tipo;
    void Andar(float u[]);
//    void GiraAH();
//    void GiraH();
    float spd = 10, spderr = 1, trn = 10, trnerr = 1, meaerr = 1;
    void Medida(float z[]);

    robo (QString n, QColor c, float x, float y, float d);

    void setVars (float vel, float velerr, float rot, float roterr, float mederr);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem
               *option, QWidget *widget);
};
#endif // ROBO_H
