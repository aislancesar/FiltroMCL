#include <fun.h>
#include <random>

float GaussRnd(float mu, float sig)
{
    // Pseudo-Gaussian-Random!!!
    // Changes the qrand() base based on the elapsed miliseconds
    int q = sqrt(pow(QTime::currentTime().msec()*QTime::currentTime().second(), 3));
    qsrand(q);
    std::default_random_engine rnd (q);
    std::normal_distribution<double> dist (mu, sig);

    return dist(rnd);

//    // Starts it
//    float r[1000];
//    float w[1000];
//    float a = 3.716922*sig/1000;
//    float rnd = 0;

//    // Generates a vector of weights
//    for (int i=0; i<1000; i++)
//    {
//        r[i] = a*i;
//        w[i] = exp(-pow(a*i,2)/(2*pow(sig,2)));
//        rnd += w[i];
//    }

//    // Takes a "random" number between 0 and 1;
//    rnd *= (qrand()%1000)/1000.0;

//    // Select a position of the vectors and returns it
//    for (int i=0; i<1000; i++)
//    {
//        if (w[i]>rnd)
//            return mu+pow(-1, qrand()%2)*r[i];
//        else
//            rnd -= w[i];
//    }

//    // If it goes wrong return mean
//    return mu;
}

float pi()
{
    return 3.14159265358979323846;
}

