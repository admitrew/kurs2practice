QT += core gui network testlib sql webenginewidgets
QMAKE_CXXFLAGS += -pipe

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ChatClient.cpp \
    ChatWindow.cpp \
#    test_chatclient.cpp \ -- для автоматизированного тестирования
    LoginWindow.cpp \
    UserManager.cpp \
    VideoCallWindow.cpp \
    main.cpp

HEADERS += \
    ChatClient.h \
#    ChatClientTest.h \ -- для автоматизированного тестирования
    ChatWindow.h \
    LoginWindow.h \
    UserManager.h \
    VideoCallWindow.h

FORMS += \
    ChatWindow.ui \
    loginwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
