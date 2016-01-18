QT +=   \
        widgets\
        core \
        gui

SOURCES += main.cpp \
    robo.cpp \
    ags.cpp \
    gaussrnd.cpp \
    particulas.cpp

HEADERS += \
    robo.h \
    ags.h \
    fun.h \
    particulas.h

DISTFILES +=

QMAKE_CXXFLAGS += -std=c++11
