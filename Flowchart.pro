QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    diagram.cpp \
    drawarea.cpp \
    main.cpp \
    scrollarea.cpp \
    shapes/abstractshape.cpp \
    shapes/shapedecision.cpp \
    shapes/shapeend.cpp \
    shapes/shapefactory.cpp \
    shapes/shapeio.cpp \
    shapes/shapeprocess.cpp \
    shapes/shapestart.cpp \
    windowmain.cpp

HEADERS += \
    shapes/abstractshape.h \
    diagram.h \
    drawarea.h \
    scrollarea.h \
    shapes/shapedecision.h \
    shapes/shapeend.h \
    shapes/shapefactory.h \
    shapes/shapeio.h \
    shapes/shapeprocess.h \
    shapes/shapestart.h \
    shapes/shapetype.h \
    windowmain.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
  res.qrc
