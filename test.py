from mathaislan import *

x = arange(0, 11, 1.0)
y = []

for j in x:
	y.append(Gauss(5, 0.5, j))



FastPlot(MyPlotter(x, [y]))