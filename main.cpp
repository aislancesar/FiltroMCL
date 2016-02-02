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

    QGraphicsView view(&cena);
    view.resize(1000, 700);
    view.show();

    return app.exec();
}
