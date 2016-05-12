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
    W = new robo(1, QColor(0, 255, 0), 450, 300, 0, true, &L, &BB);
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
    Measures_Mode(&z, true, true);

//    qDebug() << z.n;

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
    case Qt::Key_B:
        P->MudaQtd(1000);
        break;
    case Qt::Key_N:

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
    case Qt::Key_Escape:
        P->MudaQtd(0);
        P->MudaQtd(N);
        W->setX(450);
        W->setY(300);
        W->setRotation(0);
    }

//    qDebug() << L.Bknow[0] << L.Bknow[1] << L.Fknow[0] << L.Fknow[1] << L.B[0] << L.B[1];

    this->update(-50, -50, 1000, 700);
}

void AGS::Auto()
{
    float u[2] = {0, 0}; // Moviment command
    Measures *z;
    bool Dyn = true;
    bool Stt = true;
    int Qtd;
    std::string nome = "All";

    QTime tempo;
    tempo.start();

    QTime total;
    total.start();

    for (int j = 0; j < 5; j++){
        if (j == 0)
            Qtd = 10000;
        else if(j == 1)
            Qtd = 5000;
        else if(j == 2)
            Qtd = 2000;
        else if(j == 3)
            Qtd = 1000;
        else
            Qtd = 500;

        for (int k = 0; k < 3; k++){
            if (k == 0){
                Dyn = true;
                Stt = true;
                nome = "All";
            }else if (k == 1){
                Dyn = false;
                Stt = true;
                nome = "Stt";
            }else{
                Dyn = true;
                Stt = false;
                nome = "Dyn";
            }


            std::ofstream TimeFile ("../Data/Incoming/Time-" + nome + "-" + std::to_string(Qtd) + ".txt");

            for (int i = 0; i < 100; i++){
                P->MudaQtd(0);
                P->MudaQtd(Qtd);
                W->setX(450);
                W->setY(300);
                W->setRotation(0);

                int c = 0;

                std::ofstream DataFile ("../Data/Incoming/Data-" + nome + "-" + std::to_string(Qtd) + "-" + std::to_string(i) + ".txt");

                tempo.restart();

                u[0] = 0;
                u[1] = 10;

                for (; c < 36; c++){
                    z = new Measures;
                    Measures_Mode(z, Dyn, Stt);
                    W->Andar(u);
                    W->Medida(z);
                    XMCL(P, u, *z);
//                    qDebug() << j+1 << "/ 5 -" << k+1 << "/ 3 -" << i+1 << "/ 100 -" << c+1 << "/ 100";
                    DataFile << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            //        this->update(-50, -50, 1000, 700);
            //        QTest::qWait(100);
                }

                u[0] = 10;
                u[1] = 0;

                for (; c < 41; c++){
                    z = new Measures;
                    Measures_Mode(z, Dyn, Stt);
                    W->Andar(u);
                    W->Medida(z);
                    XMCL(P, u, *z);
//                    qDebug() << j+1 << "/ 5 -" << k+1 << "/ 3 -" << i+1 << "/ 100 -" << c+1 << "/ 100";
                    DataFile << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            //        this->update(-50, -50, 1000, 700);
            //        QTest::qWait(100);
                }

                u[0] = 10;
                u[1] = 10;

                for (; c < 60; c++){
                    z = new Measures;
                    Measures_Mode(z, Dyn, Stt);
                    W->Andar(u);
                    W->Medida(z);
                    XMCL(P, u, *z);
//                    qDebug() << j+1 << "/ 5 -" << k+1 << "/ 3 -" << i+1 << "/ 100 -" << c+1 << "/ 100";
                    DataFile << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            //        this->update(-50, -50, 1000, 700);
            //        QTest::qWait(100);
                }

                u[0] = 10;
                u[1] = 0;

                for (; c < 100; c++){
                    z = new Measures;
                    Measures_Mode(z, Dyn, Stt);
                    W->Andar(u);
                    W->Medida(z);
                    XMCL(P, u, *z);
//                    qDebug() << j+1 << "/ 5 -" << k+1 << "/ 3 -" << i+1 << "/ 100 -" << c+1 << "/ 100";
                    DataFile << c << " " << W->x() << " " << W->y() << " " << L.F[1][0] << " " << L.F[1][1] << "\n";
            //        this->update(-50, -50, 1000, 700);
            //        QTest::qWait(100);
                }

                int t = tempo.elapsed();

                qDebug() << j+1 << "/ 5 -" << k+1 << "/ 3 -" << i+1 << "/ 100 - Time:" << t;
                TimeFile << i << " " << t << "\n";

                DataFile.close();
            }
            qDebug() << "\n";
            TimeFile.close();
        }
    }
    int totaltime = total.elapsed();
    int hours = totaltime/(60*60*1000);
    int minutes = totaltime/(60*1000)-hours*60;
    int seconds = totaltime/(1000)-minutes*60-hours*60*60;
    qDebug() << "Done in" << hours << "hours" << minutes << "minutes" << seconds << "seconds!";
}
