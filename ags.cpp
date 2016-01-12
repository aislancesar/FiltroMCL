#include <ags.h>

AGS::AGS()
{
    T = new robo("M", Qt::green, 100, 100, 50);
    P = new Particulas(QColor(0, 0, 255), 1000);
    P->Erros(5, 5, 2);

//    D = new robo("C", Qt::red, 0, 200, 200, 0);

    texto = new QGraphicsTextItem();

    this->addItem(T);
    this->addItem(P);
//    this->addItem(D);
    T->setZValue(1000);
//    D->setZValue(1);
    P->setZValue(30);
    texto->setPos(0, -30);
    this->addItem(texto);
}

void AGS::keyPressEvent(QKeyEvent *event)
{
    float u[2] = {0.0, 0.0};
    float z[2];
    //R->setX(R->x()+10);
    switch (event->key())
    {
    case Qt::Key_W: //Andar para frente e atualiza particulas
        u[0] = 10.0;
        T->Andar(u);
        T->Medida(z);
        P->Move(u);
        P->Mede(z);
        P->Atualiza(z);
        P->update(-10, -10, 610, 410);
        break;
    case Qt::Key_A: //Girar anti-horario e atualiza particulas
        u[1] = -10.0;
        T->Andar(u);
        T->Medida(z);
        P->Move(u);
        P->Mede(z);
        P->Atualiza(z);
        P->update(-10, -10, 610, 410);
        break;
    case Qt::Key_D: //Girar horario e atualiza particulas
        u[1] = 10.0;
        T->Andar(u);
        T->Medida(z);
        P->Move(u);
        P->Mede(z);
        P->Atualiza(z);
        P->update(-10, -10, 610, 410);
        break;
    case Qt::Key_S: //Deleta particula
        P->MudaQtd(50);
        P->update(-10, -10, 610, 410);
        break;
    case Qt::Key_X: //Adiciona particulas
        P->MudaQtd(1000);
        P->update(-10, -10, 610, 410);
        break;
    case Qt::Key_I: //Anda para frente
        u[0] = 10.0;
        T->Andar(u);
        P->Move(u);
        P->update(-10, -10, 610, 410);
        break;
    case Qt::Key_J: // Gira anti-horário
        u[1] = -10.0;
        T->Andar(u);
        P->Move(u);
        P->update(-10, -10, 610, 410);
        break;
    case Qt::Key_L: //Girar horario
        u[1] = 10.0;
        T->Andar(u);
        P->Move(u);
        P->update(-10, -10, 610, 410);
        break;
    case Qt::Key_Space: // Sequestra o robo
        T->setX(qrand()%600);
        T->setY(qrand()%400);
        T->setRotation(qrand()%360);
        break;
    case Qt::Key_Enter: // Mostra o ponto médio das particulas e do robo
        P->DesRobo();
        qDebug("Robo: X.%g Y.%g", T->x(), T->y());
        qDebug("---");
        break;
    }

}
