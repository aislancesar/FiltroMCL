QT +=   \
        widgets\
        core \
        gui \
        testlib

SOURCES += main.cpp \
    robo.cpp \
    ags.cpp \
    particulas.cpp \
    fun.cpp \
    mcl.cpp \
    bola.cpp

HEADERS += \
    robo.h \
    ags.h \
    fun.h \
    particulas.h \
    mcl.h \
    bola.h

DISTFILES +=

QMAKE_CXXFLAGS += -std=c++11
