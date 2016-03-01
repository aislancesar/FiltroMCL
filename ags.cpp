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
//    for (int i = 0; i < 8; i++)
//        this->addEllipse(L.L[i][0]-10, L.L[i][1]-10, 20, 20, QPen(QColor(0, 255, 255)), Qt::NoBrush);

    // Landmarks T
//    for (int i = 0; i < 6; i++)
//        this->addEllipse(L.T[i][0]-10, L.T[i][1]-10, 20, 20, QPen(QColor(255, 255, 0)), Qt::NoBrush);

    // Landmarks X
//    for (int i = 0; i < 2; i++)
//        this->addEllipse(L.X[i][0]-10, L.X[i][1]-10, 20, 20, QPen(QColor(255, 0, 255)), Qt::NoBrush);

    // Landmarks B
    B = new Bola(&L);

    // Generates the new robot and particle set
    W = new robo(1, QColor(0, 255, 0), 200, 300, 0, true, &L, &BB);
    T = new robo(0, QColor(0, 255, 255), 0, 300, 0, true, &L, &BB);
//    I = new robo(2, QColor(50, 50, 255), 300, 400, 0, false);

    P = new Particulas(QColor(0, 255, 0, 128), N, W);

    L.F[0][0] = 0;
    L.F[0][1] = 300;

    // Set the new erros of the particles
    T->setVars(2, 3, 0.2);
    P->Erros(4, 6, 0.2);
    // It seens that bigger errors causes the AMCL to converges faster

    this->addItem(W);
    this->addItem(T);
    //this->addItem(I);
    this->addItem(P);
    this->addItem(B);

    // Draws the robot over the particles
    W->setZValue(1000);
    T->setZValue(1001);
//    I->setZValue(1002);

    P->setZValue(100);

    P->Px[0] = W->x();
    P->Py[0] = W->y();
    P->Pr[0] = W->rotation();

    B->setZValue(500);

    //qDebug() << B->x() << L.B[0];

//    W->L = &L;
//    T->L = &L;
//    I->L = &L;

    P->L = &L;
}

void AGS::keyPressEvent(QKeyEvent *event)
{
    // KeyBoard Controler
    float u[2] = {0.0, 0.0}; // Moviment command
    float z[L.n+1]; // Measure

    switch (event->key())
    {
    case Qt::Key_W: // Moves Forward
        u[0] = 10.0;
        W->Andar(u);
        W->Medida(z);
        XMCL(P, u, z);
        break;
    case Qt::Key_Q: // Turns counter-clock-wise and updates
        u[0] = 10.0;
        u[1] = -10.0;
        W->Andar(u);
        W->Medida(z);
        XMCL(P, u, z);
        break;
    case Qt::Key_E: // Turns counter-clock-wise and updates
        u[0] = 10.0;
        u[1] = 10.0;
        W->Andar(u);
        W->Medida(z);
        XMCL(P, u, z);
        break;
    case Qt::Key_A: // Turns counter-clock-wise and updates
        u[1] = -10.0;
        W->Andar(u);
        W->Medida(z);
        XMCL(P, u, z);
        break;
    case Qt::Key_D: // Turns clock-wise and updates
        u[1] = 10.0;
        W->Andar(u);
        W->Medida(z);
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
    case Qt::Key_V:
        qDebug() << P->Qtd;
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
        T->setX(UniRnd() * 600);
        T->setY(UniRnd() * 400);
        T->setRotation(UniRnd() * 360 - 180);
        break;
    case Qt::Key_Enter: // Shows mean of the particles and the robot position
        qDebug("---");
        P->DesRobo(T->x(), T->y());
        qDebug("Robo: X.%g Y.%g R.%g", T->x(), T->y(), T->rotation());
        qDebug("---");
        break;
//    case Qt::Key_K: // Measures the landmarks
//        float z[16];
//        T->Medida(z);
//        qDebug("---");
//        for (int i = 0; i <16; i++) qDebug("%g", z[i]);
//        break;
    case Qt::Key_Up:
        L.B[1] -= 5;
        break;
    case Qt::Key_Down:
        L.B[1] += 5;
        break;
    case Qt::Key_Left:
        L.B[0] -= 5;
        break;
    case Qt::Key_Right:
        L.B[0] += 5;
        break;
    }
    B->Move();
    this->update(-50, -50, 1000, 700);
}
