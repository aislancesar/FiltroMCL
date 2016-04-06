#ifndef AGS_H
#define AGS_H

#include <robo.h>
#include <QtWidgets>
#include <fun.h>
#include <particulas.h>
#include <mcl.h>
#include <bola.h>
#include <QtTest/QtTest>
#include <string>
#include <fstream>

// Class used to hold the objects on screen
class AGS : public QGraphicsScene
{
public:
    AGS();
    Landmarks L;
    BlackBoard BB;
    Bola *B;
//    float LL [8][2] = {{0, 0}, {150, 150}, {150, 450}, {0, 600}, {750, 150}, {750, 450}, {900, 0}, {900, 600}};
//    float LT [6][2]= {{0, 150}, {0, 450}, {450, 0}, {450, 600}, {900, 150}, {900,450}};
//    float LX [2][2] = {{450, 200}, {450, 400}};
    robo *W, *T, *I; // Holds the robots
    Particulas *P; // Holds the particles

    void Auto();

protected:
    // Keyboard control
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
};
#endif // AGS_H
