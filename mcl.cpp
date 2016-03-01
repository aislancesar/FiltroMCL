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
    double Max = P->Mede(z);

    float Pnx[P->Qtd], Pny[P->Qtd], Pnr[P->Qtd];

    float rnd = 0;
    int c = P->Qtd*UniRnd(), k = 0;
    for (int i = 0; i < P->Qtd; i++)
    {
        rnd += 2 * Max * UniRnd();
        while (P->Pw[c] < rnd)
        {
            rnd -= P->Pw[c];
            c = (c+1)%P->Qtd;
            k++;
            if(k > 3*P->Qtd)
            {
                k = 0;
                break;
            }
        }
        Pnx[i] = P->Px[c];
        Pny[i] = P->Py[c];
        Pnr[i] = P->Pr[c];
    }

    qDebug() << k << Max;

    for (int i = 0; i < P->Qtd; i++)
    {
        P->Px[i] = Pnx[i];
        P->Py[i] = Pny[i];
        P->Pr[i] = Pnr[i];
        P->Pw[i] = 0;
    }
}

// Augmented Monte-Carlo Localization
void AMCL(Particulas *P, float u[], float z[])
{
    static float ws = 0, wf = 0;
    //static int k = 0;
    float ys = 0.01, yf = 0.1, wa = 0;
    float Pnx[N], Pny[N], Pnr[N];

    double Max;

    P->Move(u);
    Max = P->Mede(z);

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
    int c = UniRnd() * P->Qtd, k = 0;
    for (int i = 0; i < P->Qtd; i++)
    {
        if(UniRnd() < 1-wf/ws)
        {
            P->Nova(&Pnx[i], &Pny[i], &Pnr[i]);
        }
        else
        {
            rnd += 2 * Max * UniRnd();
            while (P->Pw[c] < rnd)
            {
                rnd -= P->Pw[c];
                c = (c+1)%P->Qtd;
                k++;
                if(k > 3*P->Qtd)
                {
                    k = 0;
                    break;
                }
            }
            Pnx[i] = P->Px[c];
            Pny[i] = P->Py[c];
            Pnr[i] = P->Pr[c];
        }
    }

    for (int i = 0; i < P->Qtd; i++)
    {
        P->Px[i] = Pnx[i];
        P->Py[i] = Pny[i];
        P->Pr[i] = Pnr[i];
        P->Pw[i] = 0;
    }
}

// All-New Augmented Monte-Carlo Localization
void ANMCL(Particulas *P, float u[], float z[])
{
    P->Move(u);
    P->Mede(z);
    double Max, Pnw[N];
    bool Pn[N];

    float Pnx[N], Pny[N], Pnr[N];
    double t = 1;

    t *= Gaussian(z[0], 20.0/z[0], z[0])*exp(-1);
    t *= Gaussian(z[1], P->MedErr, z[1])*exp(-1);
    t *= Gaussian(z[2], P->MedErr, z[2])*exp(-1);

    Pnw[0] = P->Pw[0];
    for (int i = 1; i < P->Qtd; i++)
        Pnw[i] = P->Pw[i]+Pnw[i-1];

    Max = Pnw[P->Qtd-1];

    // Changing number of particles
    if(P->Reg[0].i > 0)
    {
        float s = 0;
        for(int i = 0; i < Rg; i++) if(P->Reg[i].i > 0) s += 1.0/max(0.00005, min(1, 1.25-0.0125*P->Reg[i].d));
        s = 1.0/s;
        P->MudaQtd(min(N, 4/3*(100-N)*s+100+4/3*(N-100)));
    }

    Regiao A;
    for(int i = 0; i < Rg; i++) P->Reg[i] = A;

    int c = 0;

    for (int i = 0; i < P->Qtd; i++){

        Pn[i] = false;

        while (Pnw[c] < Max*(i+1)/(P->Qtd+1)) c++;

        if(P->Pw[c] > t)
        {
            for(int j = 0; j < Rg; j++)
            {
                float cx = P->Reg[j].cx+P->Pw[c]*P->Px[c];
                float cy = P->Reg[j].cy+P->Pw[c]*P->Py[c];
                double pw = P->Reg[j].pw+P->Pw[c];
                float d = hcc(cx/pw, cy/pw, P->Px[c], P->Py[c]);
                if(d <= 100)
                {
                    P->Reg[j].cx = cx;
                    P->Reg[j].cy = cy;
                    P->Reg[j].pw = pw;
                    P->Reg[j].rc += cos(P->Pr[c]*pi()/180)*P->Pw[c];
                    P->Reg[j].rs += sin(P->Pr[c]*pi()/180)*P->Pw[c];
                    P->Reg[j].d = max(d, P->Reg[j].d);
                    P->Reg[j].i++;
                    break;
                }
            }
        }

        if(P->Pw[c] < t*exp(-(3)))
            P->Nova(&Pnx[i], &Pny[i], &Pnr[i]);
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
}
