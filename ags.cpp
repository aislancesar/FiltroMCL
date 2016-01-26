#include <ags.h>

AGS::AGS()
{
    // Desenha o campo
    this->setBackgroundBrush(Qt::darkGreen);
    QPen pena = QPen(Qt::white, 2);
    this->addRect(0, 0, 900, 600, pena, Qt::NoBrush);
    this->addRect(0, 150, 150, 300, pena, Qt::NoBrush);
    this->addRect(750, 150, 150, 300, pena, Qt::NoBrush);
    this->addLine(450, 0, 450, 600, pena);
    this->addEllipse(350, 200, 200, 200, pena, Qt::NoBrush);
    // Landmarks L
    for (int i = 0; i < 8; i++)
        this->addEllipse(LL[i][0]-10, LL[i][1]-10, 20, 20, QPen(QColor(0, 255, 255)), Qt::NoBrush);

    // Landmarks T
    for (int i = 0; i < 6; i++)
        this->addEllipse(LT[i][0]-10, LT[i][1]-10, 20, 20, QPen(QColor(255, 255, 0)), Qt::NoBrush);

    // Landmarks X
    for (int i = 0; i < 2; i++)
        this->addEllipse(LX[i][0]-10, LX[i][1]-10, 20, 20, QPen(QColor(255, 0, 255)), Qt::NoBrush);

    // Generates the new robot and particle set
    T = new robo("T", Qt::green, 100, 100, 50);
    P = new Particulas(QColor(0, 0, 255), 1000);

    // Set the new erros of the particles
    T->setVars(2, 2, 0.1);
    P->Erros(2, 2, 0.1);
   // It seens that bigger errors causes the AMCL to converges faster

    this->addItem(T);
    this->addItem(P);

    // Draws the robot over the particles (unneeded)
    T->setZValue(1000);
    P->setZValue(100);

    T->landmarks(LL, LT, LX);


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
        break;
    case Qt::Key_Q: // Turns counter-clock-wise and updates
        u[0] = 10.0;
        u[1] = -10.0;
        T->Andar(u);
        T->Medida(z);
//        P->Atualiza(u, z);
        break;
    case Qt::Key_E: // Turns counter-clock-wise and updates
        u[0] = 10.0;
        u[1] = 10.0;
        T->Andar(u);
        T->Medida(z);
//        P->Atualiza(u, z);
        break;
    case Qt::Key_A: // Turns counter-clock-wise and updates
        u[1] = -10.0;
        T->Andar(u);
        T->Medida(z);
//        P->Atualiza(u, z);
        break;
    case Qt::Key_D: // Turns clock-wise and updates
        u[1] = 10.0;
        T->Andar(u);
        T->Medida(z);
//        P->Atualiza(u, z);
        break;
    case Qt::Key_S: // Removes particles
        P->MudaQtd(50);
        break;
    case Qt::Key_X: // Adds particles
        P->MudaQtd(1000);
        break;
    case Qt::Key_I: // Just moves forward
        u[0] = 10.0;
        T->Andar(u);
        P->Move(u);
        break;
    case Qt::Key_J: // Just turns counter-clock-wise
        u[1] = -10.0;
        T->Andar(u);
        P->Move(u);
        break;
    case Qt::Key_L: // Just turns clock-wise
        u[1] = 10.0;
        T->Andar(u);
        P->Move(u);
        break;
    case Qt::Key_U: // Just turns counter-clock-wise
        u[0] = 10.0;
        u[1] = -10.0;
        T->Andar(u);
        P->Move(u);
        break;
    case Qt::Key_O: // Just turns clock-wise
        u[0] = 10.0;
        u[1] = 10.0;
        T->Andar(u);
        P->Move(u);
        break;
    case Qt::Key_Space: // Kidnappes the robot
        T->setX(qrand()%600);
        T->setY(qrand()%400);
        T->setRotation(qrand()%360);
        break;
    case Qt::Key_Enter: // Shows mean of the particles and the robot position
        qDebug("---");
        P->DesRobo(T->x(), T->y());
        qDebug("Robo: X.%g Y.%g R.%g", T->x(), T->y(), T->rotation());
        qDebug("---");
        break;
    case Qt::Key_K: // Measures the landmarks
        float z[16];
        T->Medida(z);
        qDebug("---");
        for (int i = 0; i <16; i++) qDebug("%g", z[i]);
        break;
    }
    this->update(-50, -50, 1000, 700);
}
