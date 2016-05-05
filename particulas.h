#ifndef PARTICULAS_H
#define PARTICULAS_H
#include <QtWidgets>
#include <math.h>
#include <fun.h>
#include <robo.h>

class Particulas : public QGraphicsItem
{
public:
    int Qtd; // Quantity of particles, max 1000
    QColor cor; // Color used to draw the particle

    // Here are the vector which holds the informations about the group of particles
    float Px[N]; // X position
    float Py[N]; // Y position
    float Pr[N]; // Rotation
    double Pw[N]; // Weight, initialises as zeros

    // Erros used to generatre the drift of particles
    float MovErr = 1; // The moviment error
    float RotErr = 1; // The rotation error
    float MedErr = 1; // The measurement error
    float OGVar = PI/6;

    // To which robot the particles are attached
    int rob;

    // LandMarks
    Landmarks *L;

    // Black Board
    BlackBoard *BB;

    // Regions
    Regiao Reg[2];

    // Initializer
    Particulas(QColor c, int Q, robo *ROB, Landmarks *l);

    // Set of functions
    void Move(float u[]); // Aplies the movement model
    double Mede(Measures zr); // Aplies the measurement model (updates the weights)
    void Erros(float Mov, float Rot, float Med); // Changes the errors
    void MudaQtd(int nQtd); // Changes the Quantity of particles
    void EstRobo(int tp); // Control function (ignore this one)
    void Nova(float *nPx, float *nPy, float *nPr, double *nPw); // Create new particle

    // These are needed to draw the particles
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem
                     *option, QWidget *widget);

};

#endif // PARTICULAS_H
