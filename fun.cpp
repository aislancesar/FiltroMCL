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
    return exp(-pow(mu-x, 2)/(2*pow(sig*mu, 2)))/(sqrt(2*PI)*sig*mu);
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
    float ax = cos(ang*PI/180);
    float ay = sin(ang*PI/180);

    float bx = cos(base*PI/180);
    float by = sin(base*PI/180);

    float cx = ax-bx;
    float cy = ay-by;
    float c = pow(cx,2)+pow(cy,2);

    float da = base+30;
    float dx = cos(da*PI/180)-bx;
    float dy = sin(da*PI/180)-by;
    float d = pow(dx,2)+pow(dy,2);

    return c < d;
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
    return ret;
}

