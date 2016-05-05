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
    robo *W, *T, *I; // Holds the robots
    Particulas *P; // Holds the particles

    void Auto();

protected:
    // Keyboard control
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
};
#endif // AGS_H
