#include "MainScene.h"

const int ReceivesShadowTraversalMask = 0x1; //标识阴影接收对象（例如地面）的掩码
const int CastsShadowTraversalMask = 0x2;    //标识阴影投射对象（例如Body）的掩码。

MainScene::MainScene(QWidget * parent, const QGLWidget * shareWidget , WindowFlags f):
            OSGWidget(parent, shareWidget, f)
{
    rootNode = new osg::Group;

    //创建镜面高光对象
    osg::ref_ptr<osgFX::SpecularHighlights> shl = new osgFX::SpecularHighlights();
    shl->setTextureUnit(2);
    shl->setLightNumber(0);
    shl->setSpecularColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
    shl->setSpecularExponent(100.0f);

    //Create Ground
    ground = new OSGGround;

    shl->addChild(ground->getRootNode().get());

    //Create Skeleton
    skelRoot = new OSGSkeleton;
    skelRoot->createSkeleton("C:\\Users\\libin\\Desktop\\mocap\\1.bvh");

    rootNode->addChild(shl.get());
    osg::ref_ptr<osg::MatrixTransform> skelMat = new osg::MatrixTransform;
    skelMat->addChild(skelRoot->skeleton);
    skelMat->setMatrix(osg::Matrix::rotate(osg::inDegrees(90.0f),1.0f,0.0f,0.0f));
    rootNode->addChild(skelMat);

    //Camera
    this->getCamera()->setClearColor(osg::Vec4(131.0f/255, 169.0f/255, 201.0f/255, 1.0f));
    setCameraManipulator(new osgGA::TrackballManipulator);

    //Light
    osg::Light* light = new osg::Light;
    light->setLightNum(0);
    light->setPosition(osg::Vec4(0.0f,500.0f,500.0f,1.0f));
    light->setDirection(osg::Vec3(0.0f,0.0f,0.0f));
    osg::LightSource* lightSource = new osg::LightSource;
    lightSource->setLight(light);
    lightSource->setLocalStateSetModes(osg::StateAttribute::ON);
    osg::StateSet* stateset = rootNode->getOrCreateStateSet();
    lightSource->setStateSetModes(*stateset,osg::StateAttribute::ON);
    rootNode->addChild(lightSource);

    //Shadow
    osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;
    shadowedScene->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
    shadowedScene->setCastsShadowTraversalMask(CastsShadowTraversalMask);
    osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
    shadowedScene->setShadowTechnique(sm.get());
    skelMat->setNodeMask(CastsShadowTraversalMask);
    ground->getRootNode()->setNodeMask(ReceivesShadowTraversalMask);
    shadowedScene->addChild(ground->getRootNode().get());
    shadowedScene->addChild(skelMat.get());
    shadowedScene->addChild(lightSource);

    rootNode->addChild(shadowedScene);

    osgUtil::Optimizer optimizer;
    optimizer.optimize(rootNode.get());

    this->setSceneData(rootNode.get());

    //抗锯齿
    osg::DisplaySettings* ds = osg::DisplaySettings::instance();
    ds->setNumMultiSamples(2);
    setDisplaySettings(ds);

    AnimtkViewerModelController& mc   = AnimtkViewerModelController::instance();
    //mc.stop();

    //mc.update_l();
    mc.play();

    connect(&_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    _timer.start(0.01);
}
void MainScene::paintGL()
{
    frame();
}

