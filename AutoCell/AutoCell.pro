QT += widgets

QMAKE_CXXFLAGS = -std=c++11
QMAKE_LFLAGS = -std=c++11

SOURCES += \
    main.cpp \
    cell.cpp \
    mainwindow.cpp

HEADERS += \
    cell.h \
    mainwindow.h

DISTFILES += \
    ../../../../../../Downloads/autoCell icons/fast-backward-full.svg \
    ../../../../../../Downloads/autoCell icons/fast-backward.svg \
    ../../../../../../Downloads/autoCell icons/fast-forward-full.svg \
    ../../../../../../Downloads/autoCell icons/fast-forward.svg \
    ../../../../../../Downloads/autoCell icons/open.svg \
    ../../../../../../Downloads/autoCell icons/play-full.svg \
    ../../../../../../Downloads/autoCell icons/play.svg

RESOURCES += \
    resources.qrc
