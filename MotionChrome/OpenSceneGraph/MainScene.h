#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <OpenSceneGraph/OSGWidget.h>
#include <QtCore/QTimer>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>
#include <osgFX/SpecularHighlights>
#include <osgUtil/Optimizer>
#include <osgDB/ReaderWriter>
#include <osgDB/ReadFile>

#include <OpenSceneGraph/OSGGround.h>

#include <OpenSceneGraph/OSGSkeleton.h>
#include <OpenSceneGraph/LightManager.h>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowVolume>
#include <osgShadow/ShadowTexture>
#include <osgShadow/ShadowMap>
#include <osgShadow/SoftShadowMap>
#include <osgShadow/ParallelSplitShadowMap>
#include <osgShadow/LightSpacePerspectiveShadowMap>
#include <osgShadow/StandardShadowMap>

class MainScene : public OSGWidget
{
    Q_OBJECT
public:
    MainScene(QWidget * parent = 0, const QGLWidget * shareWidget = 0, WindowFlags f = 0);

    virtual void paintGL();

    OSGGround* ground;
    OSGSkeleton* skelRoot;

    LightManager lightManager;


    osg::ref_ptr<osg::Group> rootNode;

protected:
    QTimer _timer;

};

#endif // MAINSCENE_H
