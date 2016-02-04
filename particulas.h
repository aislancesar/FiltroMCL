#ifndef PARTICULAS_H
#define PARTICULAS_H
#include <QtWidgets>
#include <math.h>
#include <fun.h>

#define N 2000

class Particulas : public QGraphicsItem
{
    //const int N = 2000;
public:
    int Qtd; // Quantity of particles, max 1000
    QColor cor; // Color used to draw the particle

    // Here are the vector which holds the informations about the group of particles
    float Px[N]; // X position
    float Py[N]; // Y position
    float Pr[N]; // Rotation
    float Pw[N]; // Weight, initialises as zeros

    // Erros used to generatre the drift of particles
    float MovErr = 1; // The moviment error
    float RotErr = 1; // The rotation error
    float MedErr = 1; // The measurement error

    // Vars used in AMCL to generate new random particles
    float wf = 1; // Sensible to fast changes of the particles weight
    float ws = 0.001; // Sensible to slow changes of the particles weight

    // LandMarks
    Landmarks *L;

    // Regions
    float R[4][5];

    // Initializer
    Particulas(QColor c, int Q);

    // Set of functions
    //void Atualiza(float u[], float z[]); // Updates the particle set
    void Move(float u[]); // Aplies the movement model
    void Mede(float z[]); // Aplies the measurement model (updates the weights)
    void Erros(float Mov, float Rot, float Med); // Changes the errors
    void MudaQtd(int nQtd); // Changes the Quantity of particles
    void DesRobo(float rx, float ry); // Control function (ignore this one)
    void Nova(float *nPx, float *nPy, float *nPr, float *nPw);

    // These are needed to draw the particles
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem
                     *option, QWidget *widget);

};

#endif // PARTICULAS_H
