#include <QtWidgets>
#include <robo.h>
#include <ags.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GaussRnd(0,0);
    //Criando o stage
    AGS cena;
    cena.addRect(0, 0, 600, 400, Qt::DashLine, Qt::NoBrush);
    cena.addEllipse(-3, -3, 6, 6, Qt::SolidLine, Qt::black);

    //for (int i = 0; i < 1000; i++)
        //qDebug("f(%f) = %g", i*0.1, qExp(-(qPow(i*0.1, 2.0)/(qPow(1, 2.0)/2.0)))*qExp(-(qPow(i*0.1, 2.0)/(qPow(1, 2.0)/2.0)))/(2*3.14159265359*qPow(1,2.0)));
//    qDebug("%g", qExp(-(qPow(1, 2.0)/(qPow(1, 2.0)/2.0)))/(2*3.14159265359*qPow(1,2.0))*qExp(-(qPow(1, 2.0)/(qPow(1, 2.0)/2.0)))/(2*3.14159265359*qPow(1,2.0)));
//    qDebug("%g", qExp(-(qPow(100, 2.0)/(qPow(1, 2.0)/2.0)))/(2*3.14159265359*qPow(1,2.0))*qExp(-(qPow(100, 2.0)/(qPow(1, 2.0)/2.0)))/(2*3.14159265359*qPow(1,2.0)));

    //Caixa de Texto
//    QGraphicsTextItem texto;
//    texto.setPos(0, -30);
//    cena.addItem(&texto);



    //Exibir o stage
    QGraphicsView view(&cena);
    view.resize(700, 500);
    view.show();

    //cena.R = &D;

    return app.exec();
}
