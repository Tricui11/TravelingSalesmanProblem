QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    annealing.cpp \
    main.cpp \
    dialog.cpp \
    point.cpp \
    random.cpp \
    tsp_helper.cpp \
    tsp_instance.cpp \
    tsp_solution.cpp

HEADERS += \
    annealing.h \
    dialog.h \
    random.h \
    tsp_helper.h

FORMS += \
    dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
