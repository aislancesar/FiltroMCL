#ifndef MCL_H
#define MCL_H
#include <fun.h>
#include <particulas.h>

void XMCL(Particulas *P, float u[], float z[]);
void MCL(Particulas *P, float u[], float z[]);
void AMCL(Particulas *P, float u[], float z[]);
void ANMCL(Particulas *P, float u[], float z[]);

#endif // MCL_H
