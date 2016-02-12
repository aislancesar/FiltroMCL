#include <mcl.h>


// Choose the algorithm to use
void XMCL(Particulas *P, float u[], float z)
{
//    MCL(P, u, z);
//    AMCL(P, u, z);
    ANMCL(P, u, z);
}

// Monte-Carlo Localization
void MCL(Particulas *P, float u[], float z)
{
    P->Move(u);
    P->Mede(z);
    float Pnx[N], Pny[N], Pnr[N], Pnw[N], Max = 0;

    for (int i = 0; i < P->Qtd; i++)
    {
        Max = max(Max, P->Pw[i]);
    }

    qDebug() << Max;

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
void AMCL(Particulas *P, float u[], float z)
{
    static float ws = 0, wf = 0;
    //static int k = 0;
    float ys = 0.01, yf = 0.1, wa = 0;
    float Pnx[N], Pny[N], Pnr[N], Pnw[N];

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
void ANMCL(Particulas *P, float u[], float z)
{
    P->Move(u);
    P->Mede(z);
    float Pnx[N], Pny[N], Pnr[N], Pnw[N], Max = 0;
    float t = Gaussian(z, P->MedErr, z)/sqrt(2);
    bool Pr[N];

    for (int i = 0; i < P->Qtd; i++)
    {
        Pr[i] = true;
        Max = max(Max, P->Pw[i]);
    }

    for(int i = 0; i < Rg; i++) for(int c = 0; c < vRg; c++) P->Reg[i][c] = 0;

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

        if(P->Pw[c] > t)
        {
            for(int j = 0; j < Rg; j++)
            {
                float cx = P->Reg[j][0]+P->Pw[c]*P->Px[c];
                float cy = P->Reg[j][1]+P->Pw[c]*P->Py[c];
                float pw = P->Reg[j][2]+P->Pw[c];
                float d = hcc(cx/pw, cy/pw, P->Px[c], P->Py[c]);
                if(d <= 100)
                {
                    P->Reg[j][0] = cx;
                    P->Reg[j][1] = cy;
                    P->Reg[j][2] = pw;
                    P->Reg[j][3] += cos(P->Pr[c]*pi()/180)*P->Pw[c];
                    P->Reg[j][4] += sin(P->Pr[c]*pi()/180)*P->Pw[c];
                    P->Reg[j][5] = max(d, P->Reg[j][5]);
                    P->Reg[j][6]++;
                    break;
                }
            }
            //Pr[c] = false;
        }
        //if(P->Pw[c] > t/2) Pr[c] = false;
        Pr[i] = false;
//        Pr[c] = false;
        Pnx[i] = P->Px[c];
        Pny[i] = P->Py[c];
        Pnr[i] = P->Pr[c];
        Pnw[i] = P->Pw[c];
    }

    int k = 0;
    for (int i = 0; i < P->Qtd; i++)
    {
        if(Pr[i])
        {
            P->TendNova(&Pnx[i], &Pny[i], &Pnr[i], &Pnw[i]);
            k++;
        }
        P->Px[i] = Pnx[i];
        P->Py[i] = Pny[i];
        P->Pr[i] = Pnr[i];
        P->Pw[i] = Pnw[i];
    }
    qDebug() << k;
}
