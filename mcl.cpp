#include <mcl.h>

// AMCL adaptation coeficients
#define yf 0.1
#define ys 0.05

// ANMCL max distance
#define DIST 100

// How is the position estimation
#define TP 0 // Pondered mean
//#define TP 1 // Greatest weighed particle
//#define TP 2 // Normal mean


// Choose the algorithm to use
void XMCL(Particulas *P, float u[], Measures z)
{
    MCL(P, u, z);
//    AMCL(P, u, z);
}

// Monte-Carlo Localization
void MCL(Particulas *P, float u[], Measures z)
{
    P->Move(u);
    double Sum = P->Mede(z);

    float Pnx[P->Qtd], Pny[P->Qtd], Pnr[P->Qtd];
    double Psw[P->Qtd], Pnw[P->Qtd];

    Psw[0] = P->Pw[0];

    for (int i = 1; i < P->Qtd; i++) Psw[i] = Psw[i-1]+P->Pw[i];

    int c = 0;
    for (int i = 0; i < P->Qtd; i++)
    {
        while (Psw[c] < Sum*(i+1)/(P->Qtd+1)) c++;
        Pnx[i] = P->Px[c];
        Pny[i] = P->Py[c];
        Pnr[i] = P->Pr[c];
        Pnw[i] = P->Pw[c];
    }

    for (int i = 0; i < P->Qtd; i++)
    {
        P->Px[i] = Pnx[i];
        P->Py[i] = Pny[i];
        P->Pr[i] = Pnr[i];
        P->Pw[i] = Pnw[i];
    }

    P->EstRobo(TP);
}

// Augmented Monte-Carlo Localization
void AMCL(Particulas *P, float u[], Measures z)
{
    static float ws = 0, wf = 0;
    double wa = 0;
    float Pnx[P->Qtd], Pny[P->Qtd], Pnr[P->Qtd];
    double Psw[P->Qtd], Pnw[P->Qtd];

    P->Move(u);
    double Sum = P->Mede(z);

    wa = Sum/P->Qtd;
    ws += ys*(wa-ws);
    wf += yf*(wa-wf);

    Psw[0] = P->Pw[0];

    for (int i = 1; i < P->Qtd; i++) Psw[i] = Psw[i-1]+P->Pw[i];

    int c = 0;
    for (int i = 0; i < P->Qtd; i++)
    {
        if(UniRnd() < 1-wf/ws)
        {
            P->Nova(&Pnx[i], &Pny[i], &Pnr[i], &Pnw[i]);
        }else{
            while (Psw[c] < Sum*(i+1)/(P->Qtd+1)) c++;
            Pnx[i] = P->Px[c];
            Pny[i] = P->Py[c];
            Pnr[i] = P->Pr[c];
            Pnw[i] = P->Pw[c];
        }
    }

    for (int i = 0; i < P->Qtd; i++)
    {
        P->Px[i] = Pnx[i];
        P->Py[i] = Pny[i];
        P->Pr[i] = Pnr[i];
        P->Pw[i] = Pnw[i];
    }

    P->EstRobo(TP);
}
