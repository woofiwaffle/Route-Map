QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/interface.cpp \
    src/main.cpp \
    src/map.cpp \
    src/obstacle.cpp \
    src/parsingmap.cpp \
    src/route.cpp \

HEADERS += \
    headers/interface.h \
    headers/map.h \
    headers/obstacle.h \
    headers/parsingmap.h \
    headers/route.h \

RESOURCES += \
    resource.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    LICENCE \
    README.md \
    src/map.xml \
    docs/example.txt \
    reports/README.md
