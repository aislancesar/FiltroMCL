#include <robo.h>

// Initializer
robo::robo (int n, QColor c, float x, float y, float d, bool v)
{
    nome = n;
    cor = c;
    this->setX(x);
    this->setY(y);
    this->setRotation(d);
    visao = v;
};

// Needed for dawing
QRectF robo::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
                  20 + penWidth, 20 + penWidth);
}

void robo::paint(QPainter *painter, const QStyleOptionGraphicsItem
                 *option, QWidget *widget)
{
    painter->setBrush(QBrush(cor));
    painter->drawEllipse(-10, -10, 20, 20);
    QLineF *linha;
    if(visao)
    {
        linha = new QLineF(0, 0, 600, 0);
        painter->drawArc(-600, -600, 1200, 1200, -30*16, 60*16);
    }else{
        linha = new QLineF(0, 0, 10, 0);
    }

    linha->setAngle(30);
    painter->drawLine(*linha);
    linha->setAngle(-30);
    painter->drawLine(*linha);


    option = option;
    widget = widget;
}

// ---
void robo::setVars (float velerr, float roterr, float mederr)
{
    spderr = velerr;
    trnerr = roterr;
    meaerr = mederr;
}

void robo::Andar(float u[])
{
    // I'm using the x, y and rotation of the class QGraphicsItem

    // Note that I'm using my Pseudo-Gauss-Random to generate the statistical errors of movement
    float r = GaussRnd(u[1], trnerr);

    // This is normalizing the angle
    if (this->rotation()+r > 180)
    {
        this->setRotation(this->rotation()+r-360.0);
    }else if(this->rotation()+r < -180){
        this->setRotation(this->rotation()+r+360.0);
    }else{
        this->setRotation(this->rotation()+r);
    }

    float x = GaussRnd(u[0], spderr)*cos(this->rotation()*pi()/180);
    float y = GaussRnd(u[0], spderr)*sin(this->rotation()*pi()/180);

    this->setX(this->x()+x);
    this->setY(this->y()+y);
}

void robo::Medida(float z[])
{
    for (int i = 0; i < L->n; i++) z[i] = 600;
//    orienterr = GaussRnd(orienterr, 0.3);
//    if (pow(orienterr, 2) > pow(30, 2))
//    {
//        orienterr = 0;
//        qDebug() << "IMU Reset";
//    }
//    z[L->n] = rotation() + orienterr;
    // Returns the measured position with statistical error
    int i = 0;
    for (; i < 8; i++)
    {
        float d, r;
        dist(L->L[i][0], L->L[i][1], x(), y(), &d, &r);
        d = GaussRnd(d, meaerr*d/10);
        if((d < 600) && (d > 10) && compAng(r, rotation()))
        {
            z[i] = d;
            int a = i;
            while (a > 0)
            {
                if (z[a-1] > z[a])
                {
                    z[a] = z[a-1];
                    z[a-1] = d;
                }
                a--;
            }
        }
    }
    for (; i < 14; i++)
    {
        float d, r;
        dist(L->T[i-8][0], L->T[i-8][1], x(), y(), &d, &r);
        d = GaussRnd(d, meaerr*d/10);
        if((d < 600) && (d > 10) && compAng(r, rotation()))
        {
            z[i] = d;
            int a = i;
            while (a > 8)
            {
                if (z[a-1] > z[a])
                {
                    z[a] = z[a-1];
                    z[a-1] = d;
                }
                a--;
            }
        }
    }
    for (; i < 16; i++)
    {

        float d, r;
        dist(L->X[i-14][0], L->X[i-14][1], x(), y(), &d, &r);
        d = GaussRnd(d, meaerr*d/10);
        if((d < 600) && (d > 10) && compAng(r, rotation()))
        {
            z[i] = d;
            int a = i;
            while (a > 14)
            {
                if (z[a-1] > z[a])
                {
                    z[a] = z[a-1];
                    z[a-1] = d;
                }
                a--;
            }
        }
    }
    if(i == L->n - 1)
    {
        float d, r;
        dist(L->B[0], L->B[1], x(), y(), &d, &r);
        d = GaussRnd(d, meaerr*d/10);
        if((d < 600) && (d > 10) && compAng(r, rotation()))
        {
            z[i] = d;
        }
    }
}

/* --- One multidimensional distance
float robo::Medida()
{
    float z = 0;
    // Returns the measured position with statistical error
    int i = 0;
    for (; i < 8; i++)
    {
        float d, r;
        dist(L->L[i][0], L->L[i][1], x(), y(), &d, &r);
        d = GaussRnd(d, meaerr*d/10);
        if((d < 600) && (d > 10) && (r > rotation()-30) && (r < rotation()+30))
        {
            z += pow(d, 2);
        }else{
            z += pow(600, 2);
        }
    }
    for (; i < 14; i++)
    {
        float d, r;
        dist(L->T[i-8][0], L->T[i-8][1], x(), y(), &d, &r);
        d = GaussRnd(d, meaerr*d/10);
        if((d < 600) && (d > 10) && (r > rotation()-30) && (r < rotation()+30))
        {
            z += pow(d, 2);
        }else{
            z += pow(600, 2);
        }
    }
    for (; i < 16; i++)
    {

        float d, r;
        dist(L->X[i-14][0], L->X[i-14][1], x(), y(), &d, &r);
        d = GaussRnd(d, meaerr*d/10);
        if((d < 600) && (d > 10) && (r > rotation()-30) && (r < rotation()+30))
        {
            z += pow(d, 2);
        }else{
            z += pow(600, 2);
        }
    }
    if(i == L->n - 1)
    {
        float d, r;
        dist(L->B[0], L->B[1], x(), y(), &d, &r);
        d = GaussRnd(d, meaerr*d/10);
        if((d < 600) && (d > 10) && (r > rotation()-30) && (r < rotation()+30))
        {
            z += pow(d, 2);
        }else{
            z += pow(600, 2);
        }
    }
    return sqrt(z);
    //qDebug() << z[16];
    //qDebug("Robo:[%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g]", z[0], z[1], z[2], z[3], z[4], z[5], z[6], z[7], z[8], z[9], z[10], z[11], z[12], z[13], z[14], z[15]);
}
*/
