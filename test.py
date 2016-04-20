from mathaislan import *

x = arange(0, 11, 1.0)
y = []
sig = 1/sqrt(2*pi)

for j in x:
	y.append(Gauss(5, sig, j))

FastPlot(MyPlotter(x, [y]))