#ifndef ROBO_H
#define ROBO_H

#include <QtWidgets>
#include <fun.h>

class robo : public QGraphicsItem
{
public:
    // Sets name and color of the robot
    int nome;
    QColor cor;
    bool visao;

    // Functions
    void Andar(float u[]); // Moves the robot
    float spderr = 1; // Movement Error
    float trnerr = 1; // Turn Error
    float meaerr = 1; // Measurement Error
    float orienterr = 0;

    // LandMarks
    Landmarks *L;

    // BlackBoard
    BlackBoard *BB;

    void Medida(Measures *z); // Measures the position
    void setVars (float velerr, float roterr, float mederr); // No comments
    void FindBall();

    // Needed functions
    robo (int n, QColor c, float x, float y, float d, bool v, Landmarks *lm, BlackBoard *bb);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem
               *option, QWidget *widget);
};
#endif // ROBO_H
