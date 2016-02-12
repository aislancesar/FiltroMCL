from mathaislan import *

def dist(x1, y1, x2, y2):
    return sqrt(pow(x1-x2, 2)+pow(y1-y2, 2))

N = 10000
Px = []
Py = []
Pw = []

for i in range(N):
    Px.append(gauss(100, 5))
    Py.append(gauss(100, 5))
    Pw.append(Gauss(0, 5, dist(100, 100, Px[i], Py[i])))

x = [100]
y = [100]
r = 100
scatter(x, y, c = ([0, 0, 1, 1]), s = r**2, edgecolor = 'face')
#plot(Px, Py)
axis([0, 900, 0, 600])
show()

