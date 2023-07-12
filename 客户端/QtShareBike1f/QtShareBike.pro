TEMPLATE = app
TARGET = minibrowser
QT += gui webchannel widgets websockets
QT += androidextras
QT += core network
QT += quick
QT += qml webview

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        bike.pb.cc \
        main.cpp

RESOURCES += \
    qrc.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/../protobuf_arm_3.6.0_clang/lib/ -lprotobuf

INCLUDEPATH += $$PWD/../protobuf_arm_3.6.0_clang/include
DEPENDPATH += $$PWD/../protobuf_arm_3.6.0_clang/include

HEADERS += \
    bike.pb.h

ANDROID_EXTRA_LIBS = $$PWD/../protobuf_arm_3.6.0_clang/lib/libprotobuf.so

DISTFILES += \
    android/AndroidManifest.xml \
    android/assets/bikeManage.html \
    android/assets/images/bike_32.ico \
    android/assets/images/bike_48.ico \
    android/assets/images/end.ico \
    android/assets/images/start.ico \
    android/assets/map.html \
    android/assets/qwebchannel.js \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/values/libs.xml \
    android/src/an/qt/qtWithJava/MyJavaClass.java \
    ios/Info.plist

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
