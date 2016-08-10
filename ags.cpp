#include <ags.h>

AGS::AGS()
{
    // Draw field
    this->setBackgroundBrush(Qt::darkGreen);
    QPen pena = QPen(Qt::white, 2);
    this->addRect(0, 0, 900, 600, pena, Qt::NoBrush);
    this->addRect(0, 150, 150, 300, pena, Qt::NoBrush);
    this->addRect(750, 150, 150, 300, pena, Qt::NoBrush);
    this->addLine(450, 0, 450, 600, pena);
    this->addEllipse(350, 200, 200, 200, pena, Qt::NoBrush);

    this->addEllipse(L.LM[0]-10, L.LM[1]-10, 20, 20, QPen(QColor(150, 255, 150)));

    // Landmarks B
//    B = new Bola(&L, &BB);

    // Generates the new robot and particle set
    W = new robo(1, QColor(0, 255, 0), 0, 300, 0, true, &L, &BB);
//    T = new robo(0, QColor(0, 255, 255), 0, 300, 0, true, &L, &BB);

    P = new Particulas(QColor(0, 255, 0, 128), N, W, &L);

//    P->BB = &BB;

    // Set the new erros of the particles
    W->setVars(0, 0, 0.2);
//    T->setVars(2, 3, 0.2);
    P->Erros(4, 6, 0.2);
    // It seens that bigger errors causes the AMCL to converges faster

    this->addItem(W);
//    this->addItem(T);
    this->addItem(P);
//    this->addItem(B);

    // Draws the robot over the particles
    W->setZValue(1000);
//    T->setZValue(1001);

    P->setZValue(100);

//    P->Px[0] = W->x();
//    P->Py[0] = W->y();
//    P->Pr[0] = W->rotation();

//    B->setZValue(500);
}

void AGS::keyPressEvent(QKeyEvent *event)
{
    // KeyBoard Controler
    float u[2] = {0.0, 0.0}; // Moviment command
    Measures z; // Measure
    z.t_orient = false;

    switch (event->key())
    {
    case Qt::Key_W: // Moves Forward
        u[0] = 10.0;
        W->Andar(u);
        W->Medida(&z);
        XMCL(P, u, z);
        break;
    case Qt::Key_Q: // Turns counter-clock-wise and updates
        u[0] = 10.0;
        u[1] = -10.0;
        W->Andar(u);
        W->Medida(&z);
        XMCL(P, u, z);
        break;
    case Qt::Key_E: // Turns counter-clock-wise and updates
        u[0] = 10.0;
        u[1] = 10.0;
        W->Andar(u);
        W->Medida(&z);
        XMCL(P, u, z);
        break;
    case Qt::Key_A: // Turns counter-clock-wise and updates
        u[1] = -10.0;
        W->Andar(u);
        W->Medida(&z);
        XMCL(P, u, z);
        break;
    case Qt::Key_D: // Turns clock-wise and updates
        u[1] = 10.0;
        W->Andar(u);
        W->Medida(&z);
        XMCL(P, u, z);
        break;
    case Qt::Key_S: // Kidnappes the robot
        W->setX(UniRnd() * 600);
        W->setY(UniRnd() * 400);
        W->setRotation(UniRnd() * 360 - 180);
        break;

    case Qt::Key_Z: // Removes particles
        P->MudaQtd(1);
        break;
    case Qt::Key_X: // Adds particles
        P->MudaQtd(P->Qtd-100);
        break;
    case Qt::Key_C: // Adds particles
        P->MudaQtd(N);
        break;
    case Qt::Key_B:
        P->MudaQtd(1000); // Change number of particles to 1000
        break;
    case Qt::Key_Escape: // Reset simulation
        P->MudaQtd(0);
        P->MudaQtd(N);
        W->setX(450);
        W->setY(300);
        W->setRotation(0);
    case Qt::Key_F5: // Automatic Simulation
        Auto();
        break;
    }
    this->update(-50, -50, 1000, 700);
}

void AGS::Auto()
{
    // Vazio
}
