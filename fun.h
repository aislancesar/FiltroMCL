#ifndef FUN_H
#define FUN_H
#include <QWidget>
#include <math.h>
#include <QTime>

float GaussRnd(float mu, float sig);
float UniRnd();
float pi();
double Gaussian(float mu, float sig, float x);

struct Landmarks
{
    float L [8][2] = {{0, 0}, {150, 150}, {150, 450}, {0, 600}, {750, 150}, {750, 450}, {900, 0}, {900, 600}};
    float T [6][2] = {{0, 150}, {0, 450}, {450, 0}, {450, 600}, {900, 150}, {900,450}};
    float X [2][2] = {{450, 200}, {450, 400}};
};

#endif // FUN_H
