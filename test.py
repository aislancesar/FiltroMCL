from mathaislan import *

def AngGauss(alpha, sigma, beta):
	mucos = cos(alpha)
	musin = sin(alpha)
	xcos = cos(beta)
	xsin = sin(beta)

	Ret = Gauss(mucos, sigma, xcos)
	Ret *= Gauss(musin, sigma, xsin)

	return Ret

def newAngGauss(alpha, sigma, beta):
	mucos = cos(alpha)
	musin = sin(alpha)
	xcos = cos(beta)
	xsin = sin(beta)

	return exp(-(pow(mucos-xcos, 2) + pow(musin-xsin, 2))/(2*sigma**2))/sqrt(2*pi*sigma**2)

sig1 = pi/6 # 30 GRAUS
sig2 = pi/9 # 20 graus

x = arange(-pi, pi, 0.01)
y1 = []
y2 = []
y3 = []
y4 = []

for i in x:
	y1.append(AngGauss(0, sig1, i))
	y2.append(AngGauss(0, sig2, i))
	y3.append(newAngGauss(0, sig1, i))
	y4.append(newAngGauss(0, sig2, i))
# var1 = AngGauss(0, sig1, sig1)
# v1 = CtePlot(x, var1)

# var11 = AngGauss(0, 2*sig1, 2*sig1)
# v11 = CtePlot(x, var11)

# var2 = AngGauss(0, sig2, sig2)
# v2 = CtePlot(x, var2)

# var22 = AngGauss(0, 2*sig2, 2*sig2)
# v22 = CtePlot(x, var22)

# P = MyPlotter(x, [y1, y2])
# FastPlot(P)

plot(x, y1, 'g', linewidth = 2)
plot(x, y2, 'b', linewidth = 2)
plot(x, y3, 'r', linewidth = 2)
plot(x, y4, 'y', linewidth = 2)

# plot([-pi/6, -pi/6], [0, 1.4], 'g')
# plot([pi/6, pi/6], [0, 1.4], 'g')
# plot([-pi/9, -pi/9], [0, 1.4], 'b')
# plot([pi/9, pi/9], [0, 1.4], 'b')

# plot([-2*pi/6, -2*pi/6], [0, 1.4], 'g')
# plot([2*pi/6, 2*pi/6], [0, 1.4], 'g')
# plot([-2*pi/9, -2*pi/9], [0, 1.4], 'b')
# plot([2*pi/9, 2*pi/9], [0, 1.4], 'b')

# plot(x, v1, 'g')
# plot(x, v2, 'b')
# plot(x, v11, 'g')
# plot(x, v22, 'b')

plot([-pi/2, -pi/2], [0, 1.4], 'k')
plot([pi/2, pi/2], [0, 1.4], 'k')

show()