#ifndef MCL_H
#define MCL_H
#include <fun.h>
#include <particulas.h>

void XMCL(Particulas *P, float u[], Measures z);
void MCL(Particulas *P, float u[], Measures z);
void AMCL(Particulas *P, float u[], Measures z);
void ANMCL(Particulas *P, float u[], Measures z);

#endif // MCL_H
