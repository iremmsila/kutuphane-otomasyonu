QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    kitap_islemleri.cpp \
    main.cpp \
    mainwindow.cpp \
    odunc_alma.cpp \
    odunc_teslim_etme.cpp \
    uye_islemleri.cpp

HEADERS += \
    kitap_islemleri.h \
    mainwindow.h \
    odunc_alma.h \
    odunc_teslim_etme.h \
    uye_islemleri.h

FORMS += \
    kitap_islemleri.ui \
    mainwindow.ui \
    odunc_alma.ui \
    odunc_teslim_etme.ui \
    uye_islemleri.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    sources.qrc \
    sources.qrc
