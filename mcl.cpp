#include <mcl.h>


// Choose the algorithm to use
void XMCL(Particulas *P, float u[], float z[])
{
//    MCL(P, u, z);
//    AMCL(P, u, z);
    ANMCL(P, u, z);
}

// Monte-Carlo Localization
void MCL(Particulas *P, float u[], float z[])
{
    P->Move(u);
    P->Mede(z);
    float Pnx[N], Pny[N], Pnr[N];
    double Pnw[N];

    double Max = 0;
    for (int i = 0; i < P->Qtd; i++)
    {
        if (P->Pw[i] > Max)
            Max = P->Pw[i];
    }

    //qDebug("Max: %g", Max);

    float rnd = 0;
    int c = P->Qtd*UniRnd();
    for (int i = 0; i < P->Qtd; i++)
    {
        rnd += 2 * Max * UniRnd();
        while (P->Pw[c] < rnd)
        {
            rnd -= P->Pw[c];
            c = (c+1)%P->Qtd;
        }
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
}

// Augmented Monte-Carlo Localization
void AMCL(Particulas *P, float u[], float z[])
{
    static float ws = 0, wf = 0;
    //static int k = 0;
    float ys = 0.01, yf = 0.1, wa = 0;
    float Pnx[N], Pny[N], Pnr[N];
    double Pnw[N];

    P->Move(u);
    P->Mede(z);

    for (int i = 0; i < P->Qtd; i++)
    {
        wa += P->Pw[i];
    }

    wa /= P->Qtd;
    ws += ys*(wa-ws);
    wf += yf*(wa-wf);

    //qDebug() << Max;
    //k++;
    //qDebug("wa.append(%g) # %d", wa, k);


    float rnd = 0;
    int c = UniRnd() * P->Qtd;
    for (int i = 0; i < P->Qtd; i++)
    {
        if(UniRnd() < 1-wf/ws)
        {
            P->Nova(&Pnx[i], &Pny[i], &Pnr[i], &Pnw[i]);
        }
        else
        {
            rnd += 2 * 300 * UniRnd();
            while (P->Pw[c] < rnd)
            {
                rnd -= P->Pw[c];
                c = (c+1)%P->Qtd;
            }
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
}

// All-New Augmented Monte-Carlo Localization
void ANMCL(Particulas *P, float u[], float z[])
{
    P->Move(u);
    P->Mede(z);
    float Pnx[N], Pny[N], Pnr[N];
    double Pnw[N], Ppw[N];
    int Pq[N];

    double Max = 0;
    for (int i = 0; i < P->Qtd; i++)
    {
        Ppw[i] = P->Pw[i];
        Pq[i] = 0;
        if (P->Pw[i] > Max)
            Max = P->Pw[i];
    }

    //qDebug("Max: %g", Max);
//    int k = 0;
    float rnd = 0;
    int c = P->Qtd*UniRnd();
    for (int i = 0; i < P->Qtd; i++)
    {
        rnd += 2 * Max * UniRnd();
        while (Ppw[c] < rnd)
        {
            rnd -= Ppw[c];
            //qDebug() << i << c << Ppw[c];
            c = (c+1)%P->Qtd;
//            k++;
        }
        Pnx[i] = P->Px[c];
        Pny[i] = P->Py[c];
        Pnr[i] = P->Pr[c];
        Pnw[i] = P->Pw[c];
        Pq[c] += 1;
        //Ppw[c] = Ppw[c]/2.0;
        //qDebug() << i << c << "peso:" << Ppw[c] << "/ max:" << Max;
    }
    //qDebug() << "iter." << k;
//    for(int i = 0; i < N; i++) if(Pq[i] > 0) qDebug() << i << Pq[i];
//    qDebug();
    for (int i = 0; i < P->Qtd; i++)
    {
        P->Px[i] = Pnx[i];
        P->Py[i] = Pny[i];
        P->Pr[i] = Pnr[i];
        P->Pw[i] = Pnw[i];
    }
}


