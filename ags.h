#ifndef AGS_H
#define AGS_H

#include <robo.h>
#include <QtWidgets>
#include <fun.h>
#include <particulas.h>

// Minha classe de Cena
class AGS : public QGraphicsScene
{
public:
    AGS();
    robo *T;
    Particulas *P;
    QGraphicsTextItem *texto;
   // void AtualizaPart()

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
};
#endif // AGS_H
