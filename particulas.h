#ifndef PARTICULAS_H
#define PARTICULAS_H
#include <QtWidgets>
#include <math.h>
#include <fun.h>

class Particulas : public QGraphicsItem
{
    int Qtd; // Quantity of particles, max 1000
    // Note that only MudaQtd can change this one!
public:
    QColor cor; // Color used to draw the particle
    // Here are the vector which holds the informations about the group of particles
    float Px[1000]; // X position
    float Py[1000]; // Y position
    float Pr[1000]; // Rotation
    float Pw[1000]; // Weight, initialises as zeros
    // Erros used to generatre the drift of particles
    float MovErr = 1; // The moviment error
    float RotErr = 1; // The rotation error
    float MedErr = 1; // The measurement error
    // Vars used in AMCL to generate new random particles
    float wf = 1; // Sensible to fast changes of the particles weight
    float ws = 0.001; // Sensible to slow changes of the particles weight

    // Initializer
    Particulas(QColor c, int Q);

    // Set of functions
    void Atualiza(float u[], float z[]); // Updates the particle set
    void Move(float u[]); // Aplies the movement model
    void Mede(float z[]); // Aplies the measurement model (updates the weights)
    void Erros(float Mov, float Rot, float Med); // Changes the errors
    void MudaQtd(int nQtd); // Changes the Quantity of particles
    void DesRobo(float rx, float ry); // Control function (ignore this one)

    // These are needed to draw the particles
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem
                     *option, QWidget *widget);

};

#endif // PARTICULAS_H
