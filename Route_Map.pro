QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/interfacemap.cpp \
    src/interfaceroute.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/obstacle.cpp \
    src/route.cpp


HEADERS += \
    headers/interfacemap.h \
    headers/interfaceroute.h \
    headers/mainwindow.h \
    headers/obstacle.h \
    headers/route.h \

FORMS += \
    ui/interfaceroute.ui \
    ui/interfacemap.ui \
    ui/mainwindow.ui

DISTFILES += \
    LICENCE \
    README.md \
    src/inputMap.xml \
    src/savedRoute.xml \
    screenshots/screenshot1.png \
    screenshots/screenshot2.png \
    screenshots/screenshot3.png \
    screenshots/screenshot4.png \
    screenshots/screenshot5.png \
    reports/README.md


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
