#include <robo.h>

// Initializer
robo::robo (QString n, QColor c, float x, float y, float d)
{
    nome = n;
    cor = c;
    this->setX(x);
    this->setY(y);
    this->setRotation(d);
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
    QLineF linha(0, 0, 600, 0);
    linha.setAngle(30);
    painter->drawLine(linha);
    linha.setAngle(-30);
    painter->drawLine(linha);
    painter->drawArc(-600, -600, 1200, 1200, -30*16, 60*16);

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

void robo::landmarks(float rLL[8][2], float rLT[6][2], float rLX[2][2])
{
    for (int i = 0; i < 8; i++)
    {
        LL[i][0] = rLL[i][0];
        LL[i][1] = rLL[i][1];
    }
    for (int i = 0; i < 6; i++)
    {
        LT[i][0] = rLT[i][0];
        LT[i][1] = rLT[i][1];
    }
    for (int i = 0; i < 2; i++)
    {
        LX[i][0] = rLX[i][0];
        LX[i][1] = rLX[i][1];
    }
}

void robo::Medida(float z[])
{
    // Returns the measured position with statistical error
    for (int i = 0; i < 16; i++)
        z[i] = 600;

    for (int i = 0; i < 8; i++)
    {
        float d = sqrt(pow(LL[i][0]-x(),2)+pow(LL[i][1]-y(),2));
        float r = atan2((LL[i][1]-y()),(LL[i][0]-x()))*180/pi();
        //if (r < 0) r += 360;
        d = GaussRnd(d, meaerr*d/10);
        //qDebug("---- %g < %g ~ < %g", d, r, rotation());
        if((d < 600) && (r > rotation()-30) && (r < rotation()+30))
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
    for (int i = 8; i < 14; i++)
    {
        float d = sqrt(pow(LT[i-8][0]-x(),2)+pow(LT[i-8][1]-y(),2));
        float r = atan2((LT[i-8][1]-y()),(LT[i-8][0]-x()))*180/pi();
        //if (r < 0) r += 360;
        d = GaussRnd(d, meaerr*d/10);
        //qDebug("---- %g < %g ~ < %g", d, r, rotation());
        if((d < 600) && (r > rotation()-30) && (r < rotation()+30))
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
    for (int i = 14; i < 16; i++)
    {
        float d = sqrt(pow(LX[i-14][0]-x(),2)+pow(LX[i-14][1]-y(),2));
        float r = atan2((LX[i-14][1]-y()),(LX[i-14][0]-x()))*180/pi();
        //if (r < 0) r += 360;
        d = GaussRnd(d, meaerr*d/10);
        //qDebug("---- %g < %g ~ < %g", d, r, rotation());
        if((d < 600) && (r > rotation()-30) && (r < rotation()+30))
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
}
