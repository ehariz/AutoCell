QT += widgets core
QMAKE_CXXFLAGS = -std=c++11
QMAKE_LFLAGS = -std=c++11

SOURCES += \
    main.cpp \
    cell.cpp \
    mainwindow.cpp \
    cellhandler.cpp \
    creationdialog.cpp

HEADERS += \
    cell.h \
    mainwindow.h \
    cellhandler.h \
    creationdialog.h

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
