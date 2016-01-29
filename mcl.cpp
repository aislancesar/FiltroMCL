#include <mcl.h>


// Choose the algorithm to use
void XMCL(Particulas *P, float u[], float z[])
{
    //MCL(P, u, z);
    //AMCL(P, u, z);
    ANMCL(P, u, z);
}

// Monte-Carlo Localization
void MCL(Particulas *P, float u[], float z[])
{
    P->Move(u);
    P->Mede(z);
    float Pnx[N], Pny[N], Pnr[N], Pnw[N];

    float Max = 0;
    for (int i = 0; i < P->Qtd; i++)
    {
        if (P->Pw[i] > Max)
            Max = P->Pw[i];
    }

    //qDebug("Max: %g", Max);

    float rnd = 0;
    int c = qrand()%P->Qtd;
    for (int i = 0; i < P->Qtd; i++)
    {
        rnd += 1 * Max * (qrand()%1000000)/1000000.0;
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
    static int k = 0;
    float ys = 0.01, yf = 0.1, wa = 0, Max = 0;
    float Pnx[N], Pny[N], Pnr[N], Pnw[N];

    P->Move(u);
    P->Mede(z);

    for (int i = 0; i < P->Qtd; i++)
    {
        wa += P->Pw[i];
        if (P->Pw[i] > Max)
            Max = P->Pw[i];
    }

    wa /= P->Qtd;
    ws += ys*(wa-ws);
    wf += yf*(wa-wf);

    qDebug("wa.append(%g) # %d", wa, k);
    k++;

    float rnd = 0;
    int c = qrand()%P->Qtd;
    for (int i = 0; i < P->Qtd; i++)
    {
        if((qrand()%1000000)/1000000.0 < 1-wf/ws)
        {
            P->Nova(&Pnx[i], &Pny[i], &Pnr[i], &Pnw[i]);
        }
        else
        {
            rnd += 1 * Max * (qrand()%1000000)/1000000.0;
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
    static float ws = 0, wf = 0;
    static int k = 0;
    float yf = 0.1, ys = 0.01, wa = 0, Max = 0;
    float Pnx[N], Pny[N], Pnr[N], Pnw[N];

    P->Move(u);
    P->Mede(z);

    for (int i = 0; i < P->Qtd; i++)
    {
        wa += P->Pw[i];
        if (P->Pw[i] > Max)
            Max = P->Pw[i];
    }

    wa /= P->Qtd;
    wf += yf*(1.0*wa-wf);
    ws += ys*(0.5*wa-ws);

    qDebug("wa.append(%g) # %d", wa, k);
    k++;
//    qDebug("ws.append(%g)", ws);
//    qDebug("wf.append(%g)", wf);

    float rnd = 0;
    int c = qrand()%P->Qtd;
    for (int i = 0; i < P->Qtd; i++)
    {
        if((qrand()%1000000)/1000000.0 < 1-wf/ws)
        {
            P->Nova(&Pnx[i], &Pny[i], &Pnr[i], &Pnw[i]);
        }
        else
        {
            rnd += 1 * Max * (qrand()%1000000)/1000000.0;
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


