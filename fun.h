#ifndef FUN_H
#define FUN_H
#include <QtWidgets>
#include <math.h>
#include <QTime>
#include <random>

#define PI 3.14159265358979323846

// Max Number of particles
#define N 10000

// Vision parameters
#define LDIST 600 // Long distance
#define SDIST 500 // Short distance

double GaussRnd(float mu, float sig);
double UniRnd();
double Gaussian(float mu, float sig, float x);
void dist(float x1, float y1, float x2, float y2, float *d, float *r);
float hcc(float x1, float y1, float x2, float y2);
double min(double a, double b);
double max(double a, double b);
bool compAng(float ang, float base);
double AngGaussian(float mu, float sig, float x);

struct Landmarks
{
    float LM[2] = {450, 300};
};

struct BlackBoard
{
    // Absolute position of moving objects
    // Used for vision measures
    float x[2];
    float y[2];
    float r[2];

    float B[2];
};

struct Regiao
{
    float cx = 0;
    float cy = 0;
    double pw = 0;
    float rc = 0;
    float rs = 0;
    float d = 0;
    int i = 0;
};

struct Measures
{
    bool t_orient = true;
    bool t_lm = true;

    float orientation;
    float lm = LDIST;

    int n = 2;
};

//void Dynamics_off(Measures *z);
//void Statics_off(Measures *z);
//void Measures_Mode(Measures *z, bool Dynamic, bool Static);

#endif // FUN_H
