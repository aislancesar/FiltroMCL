from mathaislan import *

z = []
zr = []
#-------------------------
z.append(600)
zr.append(600)
z.append(600)
zr.append(600)
z.append(600)
zr.append(600)
z.append(600)
zr.append(600)
z.append(600)
zr.append(600)
z.append(600)
zr.append(600)
z.append(600)
zr.append(600)
z.append(600)
zr.append(600)
z.append(276.155)
zr.append(600)
z.append(600)
zr.append(600)
z.append(600)
zr.append(600)
z.append(600)
zr.append(600)
z.append(600)
zr.append(600)
z.append(600)
zr.append(600)
z.append(84.7567)
zr.append(244.385)
z.append(600)
zr.append(269.772)
z.append(600)
zr.append(221.577)
#--------------------------
r = 0
p = 0
for i in range(len(z)):
    p += z[i]**2
    r += zr[i]**2
p = sqrt(p)
r = sqrt(r)
print p, r, MyGauss(r, 0.2, p)
