#include <fun.h>

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

double Gaussian(float mu, float sig, float x)
{
//    return exp(-pow(mu-x, 2)/(2*pow(sig*mu/10, 2)))/(sqrt(2*pi())*sig*mu/10);
//    return exp(-pow(mu-x, 2)/(2*pow(sig*mu, 2)))/(sqrt(2*PI)*sig*mu);
//    return exp(-pow(mu-x, 2)/(2*pow(sig*mu, 2)));
    return exp(-pow(mu-x, 2)/(2*pow(sig,2)))/sqrt(2*PI*pow(sig,2));
}

void dist(float x1, float y1, float x2, float y2, float *d, float *r)
{
    *d = hcc(x1, y1, x2, y2);
    *r = atan2((y1-y2), (x1-x2))*180/PI;
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

double AngGaussian(float mu, float sig, float x)
{
    float radmu = mu*PI/180;
    float radx = x*PI/180;

    double cosmu = cos(radmu);
    double sinmu = sin(radmu);
    double cosx = cos(radx);
    double sinx = sin(radx);

    double ret = exp(-(pow(cosmu-cosx,2)+pow(sinmu-sinx,2))/(2*pow(sig, 2)))/sqrt(2*PI*pow(sig, 2));
//    ret *= exp(-pow(cosmu-cosx, 2)/(2*pow(sig, 2)))/sqrt(2*PI*pow(sig, 2));
//    ret *= exp(-pow(sinmu-sinx, 2)/(2*pow(sig, 2)))/sqrt(2*PI*pow(sig, 2));
    return ret;
}

void Dynamics_off(Measures *z)
{
    z->t_ball = false;
    z->t_robo = false;
    z->t_orient = false;
}

void Statics_off(Measures *z)
{
    z->t_goal = false;
    z->t_lmL = false;
    z->t_lmT = false;
    z->t_lmX = false;
}

void Measures_Mode(Measures *z, bool Dynamic, bool Static)
{
    if (!Dynamic) {
        Dynamics_off(z);
    }
    if (!Static) {
        Statics_off(z);
    }
    z->n = 0;
    if (z->t_orient) z->n += 2;
    if (z->t_ball) z->n++;
    if (z->t_robo) z->n++;
    if (z->t_lmL) z->n++;
    if (z->t_lmT) z->n++;
    if (z->t_lmX) z->n++;
    if (z->t_goal) z->n += 2;
}
