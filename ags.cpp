#include <ags.h>

AGS::AGS()
{
    // Generates the new robot and particle set
    T = new robo("T", Qt::green, 100, 100, 50);
    P = new Particulas(QColor(0, 0, 255), 1000);

    // Set the new erros of the particles
    P->Erros(5, 5, 2);
    // It seens that bigger errors causes the AMCL to converges faster

    this->addItem(T);
    this->addItem(P);

    // Draws the robot over the particles (unneeded)
    T->setZValue(1000);
    P->setZValue(30);
}

void AGS::keyPressEvent(QKeyEvent *event)
{
    // KeyBoard Controler
    float u[2] = {0.0, 0.0}; // Moviment command
    float z[2]; // Measure

    switch (event->key())
    {
    case Qt::Key_W: // Moves Forward
        u[0] = 10.0;
        T->Andar(u);
        T->Medida(z);
        P->Atualiza(u, z);
        P->update(-10, -10, 610, 410); // Redraw particles
        break;
    case Qt::Key_A: // Turns counter-clock-wise and updates
        u[1] = -10.0;
        T->Andar(u);
        T->Medida(z);
        P->Atualiza(u, z);
        P->update(-10, -10, 610, 410);
        break;
    case Qt::Key_D: // Turns clock-wise and updates
        u[1] = 10.0;
        T->Andar(u);
        T->Medida(z);
        P->Atualiza(u, z);
        P->update(-10, -10, 610, 410);
        break;
    case Qt::Key_S: // Removes particles
        P->MudaQtd(50);
        P->update(-10, -10, 610, 410);
        break;
    case Qt::Key_X: // Adds particles
        P->MudaQtd(1000);
        P->update(-10, -10, 610, 410);
        break;
    case Qt::Key_I: // Just moves forward
        u[0] = 10.0;
        T->Andar(u);
        P->Move(u);
        P->update(-10, -10, 610, 410);
        break;
    case Qt::Key_J: // Just turns counter-clock-wise
        u[1] = -10.0;
        T->Andar(u);
        P->Move(u);
        P->update(-10, -10, 610, 410);
        break;
    case Qt::Key_L: // Just turns clock-wise
        u[1] = 10.0;
        T->Andar(u);
        P->Move(u);
        P->update(-10, -10, 610, 410);
        break;
    case Qt::Key_Space: // Kidnappes the robot
        T->setX(qrand()%600);
        T->setY(qrand()%400);
        T->setRotation(qrand()%360);
        break;
    case Qt::Key_Enter: // Shows mean of the particles and the robot position
        P->DesRobo();
        qDebug("Robo: X.%g Y.%g", T->x(), T->y());
        qDebug("---");
        break;
    }
}
