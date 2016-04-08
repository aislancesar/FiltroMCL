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

    // Landmarks Goal
    for (int i = 0; i < 4; i++)
        this->addEllipse(L.G[i][0]-5, L.G[i][1]-5, 10, 10, QPen(QColor(200, 200, 200)), QBrush(QColor(200, 200, 200)));

    // Landmarks L
    for (int i = 0; i < 8; i++)
        this->addEllipse(L.L[i][0]-10, L.L[i][1]-10, 20, 20, QPen(QColor(0, 255, 255)), Qt::NoBrush);

    // Landmarks T
    for (int i = 0; i < 6; i++)
        this->addEllipse(L.T[i][0]-10, L.T[i][1]-10, 20, 20, QPen(QColor(255, 255, 0)), Qt::NoBrush);

    // Landmarks X
    for (int i = 0; i < 2; i++)
        this->addEllipse(L.X[i][0]-10, L.X[i][1]-10, 20, 20, QPen(QColor(255, 0, 255)), Qt::NoBrush);

    // Landmarks B
    B = new Bola(&L, &BB);

    // Generates the new robot and particle set
    W = new robo(1, QColor(0, 255, 0), 200, 500, 0, true, &L, &BB);
    T = new robo(0, QColor(0, 255, 255), 0, 300, 0, true, &L, &BB);

    L.Fknow[0] = true;
    L.Fknow[1] = true;

    L.Best[0][0] = 0;
    L.Best[0][1] = BolaX;
    L.Best[0][2] = BolaY;
    L.Best[0][3] = 0;
    L.Bknow[0] = true;
//    I = new robo(2, QColor(50, 50, 255), 300, 400, 0, false);

    P = new Particulas(QColor(0, 255, 0, 128), N, W, &L);

    P->BB = &BB;

    L.F[0][0] = 0;
    L.F[0][1] = 300;

    // Set the new erros of the particles
    W->setVars(0, 0, 0.2);
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

    //P->L = &L;
}

void AGS::keyPressEvent(QKeyEvent *event)
{
    // KeyBoard Controler
    float u[2] = {0.0, 0.0}; // Moviment command
    Measures z; // Measure

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
    case Qt::Key_V:
        qDebug() << P->Qtd;
        break;
    case Qt::Key_I: // Just moves forward
        u[0] = 10.0;
        T->Andar(u);
//        P->Move(u);
        break;
    case Qt::Key_J: // Just turns counter-clock-wise
        u[1] = -10.0;
        T->Andar(u);
//        P->Move(u);
        break;
    case Qt::Key_L: // Just turns clock-wise
        u[1] = 10.0;
        T->Andar(u);
//        P->Move(u);
        break;
    case Qt::Key_U: // Just turns counter-clock-wise
        u[0] = 10.0;
        u[1] = -10.0;
        T->Andar(u);
//        P->Move(u);
        break;
    case Qt::Key_O: // Just turns clock-wise
        u[0] = 10.0;
        u[1] = 10.0;
        T->Andar(u);
//        P->Move(u);
        break;
    case Qt::Key_Space: // Kidnappes the robot
        T->setX(UniRnd() * 600);
        T->setY(UniRnd() * 400);
        T->setRotation(UniRnd() * 360 - 180);
        break;
    case Qt::Key_Up:
        B->Move(0, -5);
        L.Bknow[0] = false;
        L.Bknow[1] = false;
        W->FindBall();
        T->FindBall();
        break;
    case Qt::Key_Down:
        B->Move(0, 5);
        L.Bknow[0] = false;
        L.Bknow[1] = false;
        W->FindBall();
        T->FindBall();
        break;
    case Qt::Key_Left:
        B->Move(-5, 0);
        L.Bknow[0] = false;
        L.Bknow[1] = false;
        W->FindBall();
        T->FindBall();
        break;
    case Qt::Key_Right:
        B->Move(5, 0);
        L.Bknow[0] = false;
        L.Bknow[1] = false;
        W->FindBall();
        T->FindBall();
        break;
    case Qt::Key_F5:
        Auto();
        break;
    }

//    qDebug() << L.Bknow[0] << L.Bknow[1] << L.Fknow[0] << L.Fknow[1] << L.B[0] << L.B[1];

    this->update(-50, -50, 1000, 700);
}

void AGS::Auto()
{
    float u[2] = {0, 0}; // Moviment command
    Measures *z;
    int time[100];
    QTime tempo;
    tempo.start();

    for (int i = 0; i < 100; i++){
        P->MudaQtd(0);
        P->MudaQtd(N);

        qDebug() << "Move 1";
        std::ofstream myfile1 ("../Data/Incoming/M-1-" + std::to_string(i) + ".txt");
        int c = 0;

        u[1] = -6;
        W->setX(300);
        W->setY(200);
        W->setRotation(30);
//      this->update(-50, -50, 1000, 700);
//      QTest::qWait(100);

        for(;c < 40; c++){
            z = new Measures; // Measure
            W->Andar(u);
            W->Medida(z);
            XMCL(P, u, *z);
            myfile1 << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            qDebug() << i << "/100 - " << 1 << "/5 - " << c << "/100";
//          this->update(-50, -50, 1000, 700);
//          QTest::qWait(100);
        }

        u[0] = 10;
        u[1] = 0;
        for(; c < 60; c++){
            z = new Measures; // Measure
            W->Andar(u);
            W->Medida(z);
            XMCL(P, u, *z);
            myfile1 << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            qDebug() << i << "/100 - " << 1 << "/5 - " << c << "/100";
//          this->update(-50, -50, 1000, 700);
//          QTest::qWait(100);
        }

        u[0] = 0;
        u[1] = -6;
        for (; c < 80; c++){
            z = new Measures; // Measure
            W->Andar(u);
            W->Medida(z);
            XMCL(P, u, *z);
            myfile1 << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            qDebug() << i << "/100 - " << 1 << "/5 - " << c << "/100";
//          this->update(-50, -50, 1000, 700);
//          QTest::qWait(100);
        }

        u[0] = 10;
        u[1] = 0;
        for (; c < 100; c++){
            z = new Measures; // Measure
            W->Andar(u);
            W->Medida(z);
            XMCL(P, u, *z);
            myfile1 << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            qDebug() << i << "/100 - " << 1 << "/5 - " << c << "/100";
//          this->update(-50, -50, 1000, 700);
//          QTest::qWait(100);
        }

        myfile1.close();

        P->MudaQtd(0);
        P->MudaQtd(N);

        qDebug() << "Move 2";
        std::ofstream myfile2 ("../Data/Incoming/M-2-" + std::to_string(i) + ".txt");

        u[0] = 6;
        u[1] = 0.6;
        W->setX(700);
        W->setY(100);
        W->setRotation(120);
//        this->update(-50, -50, 1000, 700);
//        QTest::qWait(100);

        for (c = 0; c < 100; c++){
            z = new Measures; // Measure
            W->Andar(u);
            W->Medida(z);
            XMCL(P, u, *z);
            myfile2 << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            qDebug() << i << "/100 - " << 2 << "/5 - " << c << "/100";
//                    this->update(-50, -50, 1000, 700);
//                    QTest::qWait(100);
        }

        myfile2.close();

        P->MudaQtd(0);
        P->MudaQtd(N);

        qDebug() << "Move 3";
        std::ofstream myfile3 ("../Data/Incoming/M-3-" + std::to_string(i) + ".txt");

        u[0] = 7.5;
        u[1] = 0;
        W->setX(200);
        W->setY(50);
        W->setRotation(120);
//      this->update(-50, -50, 1000, 700);
//      QTest::qWait(100);

        for(c = 0; c < 13; c++){
            z = new Measures; // Measure
            W->Andar(u);
            W->Medida(z);
            XMCL(P, u, *z);
            myfile3 << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            qDebug() << i << "/100 - " << 3 << "/5 - " << c << "/100";
//                    this->update(-50, -50, 1000, 700);
//                    QTest::qWait(100);
        }

        u[0] = 0;
        u[1] = -5;
        for(; c < 31; c++){
            z = new Measures; // Measure
            W->Andar(u);
            W->Medida(z);
            XMCL(P, u, *z);
            myfile3 << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            qDebug() << i << "/100 - " << 3 << "/5 - " << c << "/100";
//                    this->update(-50, -50, 1000, 700);
//                    QTest::qWait(100);
        }

        u[0] = 7.5;
        u[1] = 0;
        for(; c < 100; c++){
            z = new Measures; // Measure
            W->Andar(u);
            W->Medida(z);
            XMCL(P, u, *z);
            myfile3 << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            qDebug() << i << "/100 - " << 3 << "/5 - " << c << "/100";
//                    this->update(-50, -50, 1000, 700);
//                    QTest::qWait(100);
        }

        myfile3.close();

        P->MudaQtd(0);
        P->MudaQtd(N);

        qDebug() << "Move 4";
        std::ofstream myfile4 ("../Data/Incoming/M-4-" + std::to_string(i) + ".txt");

        u[0] = 12;
        u[1] = -7;
        W->setX(300);
        W->setY(500);
        W->setRotation(0);
//            this->update(-50, -50, 1000, 700);
//            QTest::qWait(100);

        for (c = 0; c < 100; c++){
            z = new Measures; // Measure
            W->Andar(u);
            W->Medida(z);
            XMCL(P, u, *z);
            myfile4 << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            qDebug() << i << "/100 - " << 4 << "/5 - " << c << "/100";
//                    this->update(-50, -50, 1000, 700);
//                    QTest::qWait(100);
        }

        myfile4.close();

        P->MudaQtd(0);
        P->MudaQtd(N);

        qDebug() << "Move 5";
        std::ofstream myfile5 ("../Data/Incoming/M-5-" + std::to_string(i) + ".txt");

        u[0] = 7.5;
        u[1] = 0;
        W->setX(750);
        W->setY(100);
        W->setRotation(90);
//            this->update(-50, -50, 1000, 700);
//            QTest::qWait(1000);

        for (c = 0; c < 40; c++){
            z = new Measures; // Measure
            W->Andar(u);
            W->Medida(z);
            XMCL(P, u, *z);
            myfile5 << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            qDebug() << i << "/100 - " << 5 << "/5 - " << c << "/100";
//                    this->update(-50, -50, 1000, 700);
//                    QTest::qWait(100);
        }

        u[1] = 4.5;
        for (; c < 60; c++){
            z = new Measures; // Measure
            W->Andar(u);
            W->Medida(z);
            XMCL(P, u, *z);
            myfile5 << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            qDebug() << i << "/100 - " << 5 << "/5 - " << c << "/100";
//                    this->update(-50, -50, 1000, 700);
//                    QTest::qWait(100);
        }

        u[1] = 0;
        for (; c < 100; c++){
            z = new Measures; // Measure
            W->Andar(u);
            W->Medida(z);
            XMCL(P, u, *z);
            myfile5 << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            qDebug() << i << "/100 - " << 5 << "/5 - " << c << "/100";
//                    this->update(-50, -50, 1000, 700);
//                    QTest::qWait(100);
        }
        myfile5.close();

        time[i] = tempo.elapsed();
        tempo.restart();
    }

    std::ofstream myfile ("../Data/Incoming/Times.txt");
    for (int i = 0; i < 100; i++)
        myfile << i << " " << time[i] << "\n";
    myfile.close();

    qDebug() << "Done!";
}
