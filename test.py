from mathaislan import *

def AngGauss(alpha, sigma, beta):
	mucos = cos(alpha)
	musin = sin(alpha)
	xcos = cos(beta)
	xsin = sin(beta)

	Ret = Gauss(mucos, sigma, xcos)
	Ret *= Gauss(musin, sigma, xsin)

	return Ret

sig = pi/6

x = arange(-pi, pi, 0.01)
y = []
for i in x:
	y.append(AngGauss(0, sig, i))

var1 = AngGauss(0, sig, sig)
v1 = CtePlot(x, var1)

var2 = AngGauss(0, sig, 2*sig)
v2 = CtePlot(x, var2)

P = MyPlotter(x, [y, v1, v2])
FastPlot(P)