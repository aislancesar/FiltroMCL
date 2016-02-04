from math import *
from matplotlib.pyplot import *
from numpy import *

def Gauss(mu, sig, x):
    return exp(-pow(mu-x,2)/(2*sig**2))/sqrt(2*pi*sig**2)

def MyGauss(mu, sig, x):
    return Gauss(mu, mu*sig, x)

def ToPlotGauss(mu, sig, x, f = False):
    y = []
    if f:
        for i in x:
            y.append(MyGauss(mu, sig, i))
    else:
        for i in x:
            y.append(Gauss(mu, sig, i))
    return y

def Int(mu, sig, a, b, step = 0.01, f = False):
    I = 0
    x = arange(a, b, step)
    if f:
        for i in x:
            I += Gauss(mu, sig, i)*step
    else:
        for i in x:
            I += MyGauss(mu, sig, i)*step
    return I

def CtePlot(x, cte = 0):
    y = []
    for i in x:
        y.append(cte)
    return y

def MyPlotter(x, y):
    c = ['k', 'b', 'r', 'm', 'g', 'c', 'y']
    k = 0
    Plotter = []
    for i in y:
        Plotter.append([x, i, c[k]])
        k = (k+1)%7
    return Plotter

def FastPlot(Plotter):
    for i in Plotter:
        plot(i[0], i[1], i[2])
    show()

def Help(code = 0):
    if code in [1, "Gauss"]:
        print
        print "Gauss(mu, sig, x)"
        print "    mu, sig and x are scalars"
        print "    returns a scalar"
        print
    elif code in [2, "MyGauss"]:
        print
        print "MyGauss(mu, sig, x)"
        print "    mu, sig and x are scalars"
        print "    returns a scalar"
        print
    elif code in [3, "ToPlotGauss"]:
        print
        print "ToPlotGauss(mu, sig, x, f = False)"
        print "    mu and sig are scalars"
        print "    x is a vector"
        print "    f = True to use MyGauss function"
        print "    uses Gauss function otherwise"
        print "    returns a vector"
        print
    elif code in [4, "Int"]:
        print
        print "Int(mu, sig, a, b, step = 0.01, f = False)"
        print "    mu, sig, a, b and step are scalars"
        print "    f = True to use MyGauss function"
        print "    uses Gauss function otherwise"
        print "    returns a scalar"
        print
    elif code in [5, "CtePlot"]:
        print
        print "CtePlot(x, cte = 0)"
        print "    x is a vector"
        print "    cte is a scalar"
        print "    returns a vector"
        print
    elif code in [6, "MyPlotter"]:
        print
        print "MyPlotter(x, y)"
        print "    x is a vector"
        print "    y is a vector of vectors"
        print "    returns a Plotter"
        print
    elif code in [7, "FastPlot"]:
        print
        print "FastPlot(Plotter)"
        print "    Plotter is vector"
        print "    (e.g. Plotter = [[x, y, 'k'], [x, z, 'b']])"
        print "    Plots a graph"
        print
    elif code in [8, "Help"]:
        print
        print "Help(code = 0)"
        print "    code is an integer"
        print "    returns a help guide for a function"
        print "    (e.g. Help(1) will return the help for Gauss function)"
        print
    else:
        print
        print "Available functions:"
        print "    1. Gauss(mu, sig, x)"
        print "    2. MyGauss(mu, sig, x)"
        print "    3. ToPlotGauss(mu, sig, x, f = False)"
        print "    4. Int(mu, sig, a, b, step = 0.01, f = False)"
        print "    5. CtePlot(x, cte = 0)"
        print "    6. MyPlotter(x, y)"
        print "    7. FastPlot(Plotter)"
        print "    8. Help(code = 0)"
        print
        print " >>> use Help(8) for more information <<<"
        print

print
print "+-----------------------------+"
print "|  mathAislan import success  |"
print "| use Help() for my functions |"
print "+-----------------------------+"
print
