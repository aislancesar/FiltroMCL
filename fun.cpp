#include <fun.h>
#include <random>

int q = sqrt(pow(QTime::currentTime().msec()*QTime::currentTime().second(), 3));
std::default_random_engine rnd(q);

double UniRnd()
{
    std::uniform_real_distribution<double> dist (0.0, 1.0);

    return dist(rnd);
}

double GaussRnd(float mu, float sig)
{
    std::normal_distribution<double> dist (mu, sig);

    return dist(rnd);
}

float pi()
{
    return 3.14159265358979323846;
}

double Gaussian(float mu, float sig, float x)
{
//    return exp(-pow(mu-x, 2)/(2*pow(sig*mu/10, 2)))/(sqrt(2*pi())*sig*mu/10);
    return exp(-pow(mu-x, 2)/(2*pow(sig*mu, 2)))/(sqrt(2*pi())*sig*mu);
}

void dist(float x1, float y1, float x2, float y2, float *d, float *r)
{
    *d = hcc(x1, y1, x2, y2);
    *r = atan2((y1-y2), (x1-x2))*180/pi();
}

float hcc(float x1, float y1, float x2, float y2)
{
    return sqrt(pow(x1-x2, 2)+pow(y1-y2, 2));
}

double min(double a, double b)
{
    if(a < b) return a;
    else return b;
}

double max(double a, double b)
{
    if(a > b) return a;
    else return b;
}

bool compAng(float ang, float base)
{
    if(base > 150 || base < -150)
    {
        if(ang > 0 && base < 0)
            return (ang < base + 360 + 30) && (ang > base + 360 - 30);
        else if(ang < 0 && base > 0)
            return (ang < base - 360 + 30) && (ang > base - 360 - 30);
    }
    return (ang < base + 30) && (ang > base - 30);
}

int myround(double a)
{
    if(a-int(a) > int(a+1)-a) return int((a+1));
    else return int(a);
}
