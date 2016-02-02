#include <fun.h>
#include <random>

int q = sqrt(pow(QTime::currentTime().msec()*QTime::currentTime().second(), 3));
std::default_random_engine rnd(q);

float UniRnd()
{
    std::uniform_real_distribution<double> dist (0.0, 1.0);

    return dist(rnd);
}

float GaussRnd(float mu, float sig)
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
    return exp(-pow(mu-x, 2)/(2*pow(sig*mu/10, 2)))/(sqrt(2*pi())*sig*mu/10);
    //return exp(-pow(mu-x, 2)/(2*pow(sig*mu, 2)))/(sqrt(2*pi())*sig*mu);
}
