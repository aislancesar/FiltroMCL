QT +=   \
        widgets\
        core \
        gui

SOURCES += main.cpp \
    robo.cpp \
    ags.cpp \
    particulas.cpp \
    fun.cpp \
    mcl.cpp

HEADERS += \
    robo.h \
    ags.h \
    fun.h \
    particulas.h \
    mcl.h

DISTFILES +=

QMAKE_CXXFLAGS += -std=c++11
