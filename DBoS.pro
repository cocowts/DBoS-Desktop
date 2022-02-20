QT       += core gui multimedia multimediawidgets charts serialport concurrent network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arcprogressbar/arcprogressbar.cpp \
    backlightwidget/backlightwidget.cpp \
    calculatorwidget/calculatorwidget.cpp \
    camerawidget/camerawidget.cpp \
    colordashboard/colordashboard.cpp \
    dynamicline/dynamicline.cpp \
    electricitywidget/electricitywidget.cpp \
    illuminationwidget/illuminationthread.cpp \
    illuminationwidget/illuminationwidget.cpp \
    infraredwidget/infraredwidget.cpp \
    keywidget/keywidget.cpp \
    main.cpp \
    mainwindowctrl.cpp \
    mainwindowui.cpp \
    mapwidget/mapwidget.cpp \
    musicwidget/musicwidget.cpp \
    oledwidget/drawwidget.cpp \
    oledwidget/oledwidget.cpp \
    other/other.cpp \
    photosensitivewidget/photosensitivewidget.cpp \
    recorderwidget/recorderwidget.cpp \
    remotecontrolwidget/remotectrlwidget.cpp \
    simplemessagebox/simplemessagebox.cpp \
    sliderwidget/sliderwidget.cpp \
    systemwidget/systemwidget.cpp \
    temperaturewidget/temperaturethread.cpp \
    temperaturewidget/temperaturewidget.cpp \
    topwidget/topwidget.cpp \
    ultrasonicwavewidget/ultrasonicwavethread.cpp \
    ultrasonicwavewidget/ultrasonicwavewidget.cpp \
    videowidget/videowidget.cpp \
    wareprogressbar/wareprogressbar.cpp \
    weatherwidget/weatherwidget.cpp

HEADERS += \
    arcprogressbar/arcprogressbar.h \
    backlightwidget/backlightwidget.h \
    calculatorwidget/calculatorwidget.h \
    camerawidget/camerawidget.h \
    colordashboard/colordashboard.h \
    commonhelper.h \
    dynamicline/dynamicline.h \
    electricitywidget/electricitywidget.h \
    illuminationwidget/illuminationthread.h \
    illuminationwidget/illuminationwidget.h \
    infraredwidget/infraredwidget.h \
    keywidget/keywidget.h \
    mainwindow.h \
    mapwidget/mapwidget.h \
    musicwidget/musicwidget.h \
    oledwidget/drawwidget.h \
    oledwidget/oledwidget.h \
    other/other.h \
    photosensitivewidget/photosensitivewidget.h \
    recorderwidget/recorderwidget.h \
    remotecontrolwidget/remotectrlwidget.h \
    simplemessagebox/simplemessagebox.h \
    sliderwidget/sliderwidget.h \
    systemwidget/systemwidget.h \
    temperaturewidget/temperaturethread.h \
    temperaturewidget/temperaturewidget.h \
    topwidget/topwidget.h \
    ultrasonicwavewidget/ultrasonicwavethread.h \
    ultrasonicwavewidget/ultrasonicwavewidget.h \
    videowidget/videowidget.h \
    wareprogressbar/wareprogressbar.h \
    weatherwidget/weatherwidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES +=
