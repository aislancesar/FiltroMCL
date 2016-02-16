#ifndef FUN_H
#define FUN_H
#include <QWidget>
#include <math.h>
#include <QTime>

double GaussRnd(float mu, float sig);
double UniRnd();
float pi();
double Gaussian(float mu, float sig, float x);
void dist(float x1, float y1, float x2, float y2, float *d, float *r);
float hcc(float x1, float y1, float x2, float y2);
double min(double a, double b);
double max(double a, double b);
bool compAng(float ang, float base);

struct Landmarks
{
    // L shaped Landmarks
    float L [8][2] = {{0, 0}, {150, 150}, {150, 450}, {0, 600}, {750, 150}, {750, 450}, {900, 0}, {900, 600}};

    // T shaped Landmarks
    float T [6][2] = {{0, 150}, {0, 450}, {450, 0}, {450, 600}, {900, 150}, {900,450}};

    // X shaped Landmarks
    float X [2][2] = {{450, 200}, {450, 400}};

    // Friend Robots
    float F [3][2] = {{0, 0}, {0, 0}, {0, 0}};

    // Ball
    float B [2] = {450, 300};

    // Total of LandMarks
    int n = 17;
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

#endif // FUN_H
