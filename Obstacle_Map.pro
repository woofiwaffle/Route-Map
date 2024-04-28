QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/interface.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/map.cpp \
    src/moveitem.cpp \
    src/obstacle.cpp \
    src/parsingmap.cpp \
    src/route.cpp \

HEADERS += \
    headers/interface.h \
    headers/mainwindow.h \
    headers/map.h \
    headers/obstacle.h \
    headers/parsingmap.h \
    headers/route.h \
    src/moveitem.h

FORMS += \
    ui/interface.ui \
    ui/mainwindow.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    LICENCE \
    README.md \
    src/map.xml \
    docs/example.txt \
    reports/README.md


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
