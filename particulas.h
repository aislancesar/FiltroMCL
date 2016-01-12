#ifndef PARTICULAS_H
#define PARTICULAS_H
#include <QtWidgets>
#include <math.h>
#include <fun.h>

class Particulas : public QGraphicsItem
{
    int Qtd;
public:
    QColor cor;
    float Px[1000];
    float Py[1000];
    float Pr[1000];
    float Pw[1000];
    float MovErr = 1;
    float RotErr = 1;
    float MedErr = 1;
    float wf = 0.5, ws = 0.1;


    Particulas(QColor c, int Q);

    void Atualiza(float z[]);
    void Move(float u[]);
    void Mede(float z[]);
    void Erros(float Mov, float Rot, float Med);
    void MudaQtd(int nQtd);
    void DesRobo();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem
                     *option, QWidget *widget);

};

#endif // PARTICULAS_H
