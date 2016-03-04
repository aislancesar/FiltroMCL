#ifndef FUN_H
#define FUN_H
#include <QtWidgets>
#include <math.h>
#include <QTime>
#include <random>

#define PI 3.14159265358979323846

// Max Number of particles
#define N 10000
// Max Number of regions for ANMCL
#define Rg 4

// Vision parameters
#define LDIST 600 // Long distance
#define SDIST 200 // Short distance

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
    // Used to measure the particles possible position
    // L shaped Landmarks
    float L [8][2] = {{0, 0}, {150, 150}, {150, 450}, {0, 600}, {750, 150}, {750, 450}, {900, 0}, {900, 600}};

    // T shaped Landmarks
    float T [6][2] = {{0, 150}, {0, 450}, {450, 0}, {450, 600}, {900, 150}, {900,450}};

    // X shaped Landmarks
    float X [2][2] = {{450, 200}, {450, 400}};

    // Friend Robots
    float F [2][2] = {{0, 0}, {0, 0}};
    bool Fknow [2];

    // Ball
    float B [2] = {450, 300};
    float Best [2][4];
    bool Bknow [2];

    // Goal Poles
    float G[4][2] = {{0, 200}, {0, 400}, {900, 200}, {900, 400}};

    // Total of LandMarks
    int n = 9;
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
    float orientation;
    float ball = 600;
    float robo = 600;
    float goal1 = 600;
    float goal2 = 600;
    float lmL = 200;
    float lmT = 200;
    float lmX = 200;
    int n = 9;
};

#endif // FUN_H
