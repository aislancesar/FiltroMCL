from mathaislan import *

def NGauss(mu, sig, x):
	return exp(-(mu-x)**2/(2*sig**2))

x = arange(0, 600, 0.1)
y1 = []
y2 = []
for i in x:
	y1.append(NGauss(300, 30, i))
	y2.append(NGauss(300, 30, i)**9)

P = MyPlotter(x, [y1, y2])
FastPlot(P)