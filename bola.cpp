//#include "bola.h"

//Bola::Bola(Landmarks *lm, BlackBoard *b)
//{
//    L = lm;
//    BB = b;
//    setX(BolaX);
//    setY(BolaY);
//    BB->B[0] = BolaX;
//    BB->B[1] = BolaY;
//    L->B[0] = BolaX;
//    L->B[1] = BolaY;
//}

//void Bola::Move(float X, float Y)
//{
//    setX(x()+X);
//    setY(y()+Y);
//    BB->B[0] = x();
//    BB->B[1] = y();
//}

//QRectF Bola::boundingRect() const
//{

//    return QRectF(-5, -5, 10, 10);
//}

//void Bola::paint(QPainter *painter, const QStyleOptionGraphicsItem
//                 *option, QWidget *widget)
//{
//    painter->setBrush(QColor(255, 255, 0));
//    painter->setPen(QPen(QColor(255, 255, 0)));
//    painter->drawEllipse(-5, -5, 10, 10);

//    float X = L->B[0]-x();
//    float Y = L->B[1]-y();

//    QLineF linha(X, Y-10, X, Y+10);
//    QLineF linha2(X-10, Y, X+10, Y);
//    painter->drawLine(linha);
//    painter->drawLine(linha2);

//    option = option;
//    widget = widget;
//}
