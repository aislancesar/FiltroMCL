#include <fun.h>

float GaussRnd(float mu, float sig)
{
    int q = QTime::currentTime().msec();
    qsrand(q);
    float r[1000];
    float w[1000];
    float a = 3.716922*sig/1000;
    float rnd = 0;
    for (int i=0; i<1000; i++)
    {
        r[i] = a*i;
        w[i] = exp(-pow(a*i,2)/(2*pow(sig,2)));
        rnd += w[i];
    }
    rnd *= (qrand()%1000)/1000.0;
    for (int i=0; i<1000; i++)
    {
        if (w[i]>rnd)
            return mu+pow(-1, qrand()%2)*r[i];
        else
            rnd -= w[i];
    }
    return mu;
}
