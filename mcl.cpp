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
//    ANMCL(P, u, z);
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

    qDebug() << ws << wf << 1-wf/ws;

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

// All-New Augmented Monte-Carlo Localization
void ANMCL(Particulas *P, float u[], Measures z)
{
    P->Move(u);
    double Sum = P->Mede(z);
    double Pnw[N];
    bool Pn[N];

    float Pnx[N], Pny[N], Pnr[N];
    double t = 1;

//    t *= AngGaussian(z.orientation, P->OGVar, z.orientation);
//    t *= Gaussian(z.ball, P->MedErr, z.ball);
//    t *= Gaussian(z.goal1, P->MedErr, z.goal1);
//    t *= Gaussian(z.goal2, P->MedErr, z.goal2);
//    t *= Gaussian(z.lmL, P->MedErr, z.lmL);
//    t *= Gaussian(z.lmT, P->MedErr, z.lmT);
//    t *= Gaussian(z.lmX, P->MedErr, z.lmX);
//    t *= Gaussian(z.robo, P->MedErr, z.robo);

    Pnw[0] = P->Pw[0];
    for (int i = 1; i < P->Qtd; i++)
        Pnw[i] = P->Pw[i]+Pnw[i-1];

    // Changing number of particles
    if(P->Reg[0].i > 0)
    {
        float s = 0;
        for(int i = 0; i < Rg; i++) if(P->Reg[i].i > 0) s += 1.0/max(0.00005, min(1, 1.25-0.0125*P->Reg[i].d));
        s = 1.0/s;
        P->MudaQtd(min(N, 4/3*(100-N)*s+100+4/3*(N-100)));
    }else{
        P->MudaQtd(N);
    }

    Regiao A;
    for(int i = 0; i < Rg; i++) P->Reg[i] = A;

    int c = 0;

    for (int i = 0; i < P->Qtd; i++){

        Pn[i] = false;

        while (Pnw[c] < Sum*(i+1)/(P->Qtd+1)) c++;

        if(P->Pw[c] > t*exp(-P->L->n))
        {
            for(int j = 0; j < Rg; j++)
            {
                float cx = P->Reg[j].cx+P->Pw[c]*P->Px[c];
                float cy = P->Reg[j].cy+P->Pw[c]*P->Py[c];
                double pw = P->Reg[j].pw+P->Pw[c];
                float d = hcc(cx/pw, cy/pw, P->Px[c], P->Py[c]);
                if(d <= DIST)
                {
                    P->Reg[j].cx = cx;
                    P->Reg[j].cy = cy;
                    P->Reg[j].pw = pw;
                    P->Reg[j].rc += cos(P->Pr[c]*PI/180)*P->Pw[c];
                    P->Reg[j].rs += sin(P->Pr[c]*PI/180)*P->Pw[c];
                    P->Reg[j].d = max(d, P->Reg[j].d);
                    P->Reg[j].i++;
                    break;
                }
            }
        }

        if(P->Pw[c] < t*exp(-2*P->L->n))
            P->Nova(&Pnx[i], &Pny[i], &Pnr[i], &Pnw[i]);
        else
        {
            Pnx[i] = P->Px[c];
            Pny[i] = P->Py[c];
            Pnr[i] = P->Pr[c];
        }
    }

    for (int i = 0; i < P->Qtd; i++)
    {
        c = P->Qtd * UniRnd();
        while(Pn[c]) c = (c+1)%P->Qtd;
        P->Px[c] = Pnx[i];
        P->Py[c] = Pny[i];
        P->Pr[c] = Pnr[i];
        Pn[c] = true;
    }

    P->L->Fknow[P->rob] = false;
    if(P->Reg[1].i == 0)
    {
        P->L->F[P->rob][0] = P->Reg[0].cx/P->Reg[0].pw;
        P->L->F[P->rob][1] = P->Reg[0].cy/P->Reg[0].pw;
        P->L->Fknow[P->rob] = true;
    }
}
