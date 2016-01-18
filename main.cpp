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
    // Borders
    cena.addRect(0, 0, 600, 400, Qt::DashLine, Qt::NoBrush);
    cena.addEllipse(-3, -3, 6, 6, Qt::SolidLine, Qt::black);
    QGraphicsView view(&cena);
    view.resize(700, 500);
    view.show();        

    return app.exec();
}
