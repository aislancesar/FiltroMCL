#include <QtWidgets>
#include <robo.h>
#include <ags.h>
#include <fun.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // Changes the base of qrand()
    GaussRnd(0,0);

    // Creates a scene
    AGS cena;

    QGraphicsView view(&cena);
    view.resize(1000, 700);
    view.show();

//    cena.addEllipse(200, 200, 200, 200, Qt::SolidLine, Qt::NoBrush);
//    cena.addEllipse(100, 000, 400, 400, Qt::SolidLine, Qt::NoBrush);
//    cena.addEllipse(000, 100, 400, 400, Qt::SolidLine, Qt::NoBrush);
//    cena.addEllipse(200, 100, 400, 400, Qt::SolidLine, Qt::NoBrush);
//    cena.addEllipse(100, 200, 400, 400, Qt::SolidLine, Qt::NoBrush);

    return app.exec();
}
