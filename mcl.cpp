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
    double Max = P->Mede(z);

    float Pnx[N], Pny[N], Pnr[N];
    double t = 1;

    for (int j = 0; j < P->L->n; j++) t *= Gaussian(z[j], P->MedErr, z[j])*exp(-1);

    Regiao A;
    for(int i = 0; i < Rg; i++) P->Reg[i] = A;

    float rnd = 0;
    int c = P->Qtd*UniRnd();

    //qDebug() << Max << t << t*exp(-17);

    int k = 0;
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

        if(P->Pw[c] < t*exp(-17))
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
        P->Px[i] = Pnx[i];
        P->Py[i] = Pny[i];
        P->Pr[i] = Pnr[i];
    }
    qDebug() << k;
}
