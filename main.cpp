#include <QtWidgets>
#include <robo.h>
#include <ags.h>
#include <fun.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Creates a scene
    AGS cena;

    QGraphicsView view(&cena);
    view.resize(1000, 700);
    view.show();

    return app.exec();
}
