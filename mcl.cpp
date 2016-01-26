#include <mcl.h>

void MCL(Particulas *P, float u[], float z[])
{
    P->Move(u);
    P->Mede(z);
    float Pnx[1000], Pny[1000], Pnr[1000], Pnw[1000];

    float Max = 0;
    for (int i = 0; i < P->Qtd; i++)
    {
        if (P->Pw[i] > Max)
            Max = P->Pw[i];
    }

    qDebug("Max: %g", Max);

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
