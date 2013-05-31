#-------------------------------------------------
#
# Project created by lbin 2013-04-19 14:59:03
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MotionChrome
TEMPLATE = app

LIBS +=  D:\Library\Osg3.0.1MinGw\lib\libosgd.dll.a \
         D:\Library\Osg3.0.1MinGw\lib\libosgViewerd.dll.a \
         D:\Library\Osg3.0.1MinGw\lib\libosgGAd.dll.a \
         D:\Library\Osg3.0.1MinGw\lib\libosgManipulatord.dll.a \
         D:\Library\Osg3.0.1MinGw\lib\libosgUtild.dll.a \
         D:\Library\Osg3.0.1MinGw\lib\libosgDBd.dll.a \
         D:\Library\Osg3.0.1MinGw\lib\libOpenThreadsd.dll.a \
         D:\Library\Osg3.0.1MinGw\lib\libosgAnimationd.dll.a \
         D:\Library\Osg3.0.1MinGw\lib\libosgShadowd.dll.a \
         D:\Library\Osg3.0.1MinGw\lib\libosgTextd.dll.a \
         D:\Library\Osg3.0.1MinGw\lib\libosgFXd.dll.a \
         D:\Qt\5.0.2\qtbase\lib\libQt5OpenGLd.a \
         D:\Library\OpenCV\lib\libopencv_core245.dll.a
INCLUDEPATH += D:\Library\Osg3.0.1MinGw\include \
               D:\Library\OpenCV\include

SOURCES += main.cpp\
        MotionChrome.cpp \
    OpenSceneGraph/OSGWidget.cpp \
    OpenSceneGraph/MainScene.cpp \
    UI/LayoutDesign.cpp \
    OpenSceneGraph/OSGGround.cpp \
    OpenSceneGraph/OSGSkeleton.cpp \
    OpenSceneGraph/LightManager.cpp

HEADERS  += MotionChrome.h \
    OpenSceneGraph/OSGWidget.h \
    OpenSceneGraph/MainScene.h \
    UI/LayoutDesign.h \
    OpenSceneGraph/OSGGround.h \
    OpenSceneGraph/OSGSkeleton.h \
    OpenSceneGraph/AnimtkViewerModelController.h \
    OpenSceneGraph/LightManager.h

FORMS    += MotionChrome.ui
