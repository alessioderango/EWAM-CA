RESOURCES += images.qrc

HEADERS += mainwindow.h view.h \
    Coordinate.h \
    Junction.h \
    LinkResult.h \
    NodeResult.h \
    NodeTable.h \
    Pipe.h \
    Reservoir.h \
    cawdn.h \
    infojunction.h \
    line.h \
    ewamca.h \
    reservoirs.h \
    utilis.h
SOURCES += main.cpp \
    cawdn.cpp \
    infojunction.cpp \
    line.cpp \
    reservoirs.cpp
SOURCES += mainwindow.cpp view.cpp \
    ewamca.cpp

QT += widgets
qtHaveModule(printsupport): QT += printsupport
qtHaveModule(opengl): QT += opengl

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/chip
INSTALLS += target

DISTFILES += \
    reti/1_rete_permanente.inp \
    reti/1_rete_permanente.net \
    reti/2_rete_pattern.inp \
    reti/2_rete_pattern.net \
    reti/3_rete.inp \
    reti/3_rete.net \
    reti/4_rete_pattern_qualità.inp \
    reti/4_rete_pattern_qualità.net \
    reti/out/2_rete_pattern.out \
    reti/out/3_rete.out

FORMS += \
    infojunction.ui

