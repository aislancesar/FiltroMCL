#include <QtWidgets>
#include <robo.h>
#include <ags.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // Changes the base of qrand()
    GaussRnd(0,0);

    // Creates a scene
    AGS cena;
//    QGraphicsScene cena;
//    cena.addRect(0, 0, 1056, 576);
//    cena.addLine(0, 152, 384, 152);
//    cena.addLine(384, 152, 384, 576);
//    cena.addLine(512, 264, 1056, 264);
//    cena.addLine(512, 128, 512, 264);
//    cena.addLine(672, 264, 672, 576);
    // Borders
//    cena.addRect(0, 0, 600, 400, Qt::DashLine, Qt::NoBrush);
//    cena.addEllipse(-3, -3, 6, 6, Qt::SolidLine, Qt::black);
    QGraphicsView view(&cena);
    view.resize(1000, 700);
    view.show();

    return app.exec();
}
