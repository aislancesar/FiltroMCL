#ifndef AGS_H
#define AGS_H

#include <robo.h>
#include <QtWidgets>
#include <fun.h>
#include <particulas.h>

// Class used to hold the objects on screen
class AGS : public QGraphicsScene
{
public:
    AGS();
    robo *T; // Holds the robot
    Particulas *P; // Holds the particles

protected:
    // Keyboard control
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
};
#endif // AGS_H
