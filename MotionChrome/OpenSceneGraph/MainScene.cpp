#include "MainScene.h"

#include <osg/Stencil>
#include <osg/ClipNode>
#include <osg/ClipPlane>
#include <osg/BlendFunc>
#include <osgGA/NodeTrackerManipulator>

const int ReceivesShadowTraversalMask = 0x1; //标识阴影接收对象（例如地面）的掩码
const int CastsShadowTraversalMask = 0x2;    //标识阴影投射对象（例如Body）的掩码。

MainScene::MainScene(QWidget * parent, const QGLWidget * shareWidget , WindowFlags f):
            OSGWidget(parent, shareWidget, f)
{
    rootNode = new osg::Group;

    //Creat Ground
    ground = new OSGGround;
    osg::ref_ptr<osg::MatrixTransform> groundMat = new osg::MatrixTransform;
    osg::Geode* groundGeode = new osg::Geode;
    groundGeode->addDrawable(ground->getRootNode());
    groundMat->addChild(groundGeode);


    //Create Skeleton
    skelRoot = new OSGSkeleton;
    skelRoot->createSkeleton("D:\\mocap\\BVH\\1.bvh");
    //Wating~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //skinRoot = dynamic_cast<osg::MatrixTransform*>(osgDB::readNodeFile("D:\\mocap\\Model\\1.osg"));
    //Wating~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    osg::ref_ptr<osg::MatrixTransform> skelMat = new osg::MatrixTransform;
    skelMat->addChild(skelRoot->skeleton);
    skelMat->setMatrix(osg::Matrix::rotate(osg::inDegrees(90.0f),1.0f,0.0f,0.0f));
    //rootNode->addChild(skelMat);

    //Camera
    this->getCamera()->setClearColor(osg::Vec4(0.0f/255, 0.0f/255, 0.0f/255, 1.0f));
    //
//    this->setCameraManipulator(new osgGA::TrackballManipulator);
    //
    //Track
    osgGA::NodeTrackerManipulator *ntm = new osgGA::NodeTrackerManipulator();
    osgGA::NodeTrackerManipulator::TrackerMode trackerMode = osgGA::NodeTrackerManipulator::NODE_CENTER_AND_AZIM;
    ntm->setTrackerMode(trackerMode);
    osgGA::NodeTrackerManipulator::RotationMode rotationMode = osgGA::NodeTrackerManipulator::TRACKBALL;
    ntm->setRotationMode( rotationMode );
    ntm->setTrackNode(skelMat.get());
    this->setCameraManipulator(ntm);
    //Track


    //Mirror
    rootNode->addChild(createMirroredScene(skelMat.get()));



    osgUtil::Optimizer optimizer;
    optimizer.optimize(rootNode.get());

    this->setSceneData(rootNode.get());

    //抗锯齿
    osg::DisplaySettings* ds = osg::DisplaySettings::instance();
    ds->setNumMultiSamples(1);
    setDisplaySettings(ds);

    AnimtkViewerModelController& mc   = AnimtkViewerModelController::instance();
    //mc.stop();
    mc.play();

    connect(&_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    _timer.start(0.01);
}
void MainScene::paintGL()
{
    frame();
}

osg::Node* MainScene::createMirroredScene(osg::Node* model)
{
    float lightHigth = 1000.0f;
    float lightHigthPlus = 0.0f;
    float lightX = 500.0f;
    float lightY = 500.0f;
    //Light
    osg::Light* light = new osg::Light;
    light->setLightNum(0);
    light->setPosition(osg::Vec4(lightX,lightY,lightHigth,1.0f));
    light->setDirection(osg::Vec3(0.0f,0.0f,0.0f));
    osg::LightSource* lightSource = new osg::LightSource;
    lightSource->setLight(light);
    lightSource->setLocalStateSetModes(osg::StateAttribute::ON);
    osg::StateSet* stateset = rootNode->getOrCreateStateSet();
    stateset->setMode(GL_LIGHT0, osg::StateAttribute::ON );
    lightSource->setStateSetModes(*stateset,osg::StateAttribute::ON);

    //Lithgt1  此后三个灯光暂时未使用
    osg::Light* light1 = new osg::Light;
    light1->setLightNum(1);
    light1->setPosition(osg::Vec4(-lightX,-lightY,lightHigth + lightHigthPlus,1.0f));
    light1->setDirection(osg::Vec3(0.0f,0.0f,0.0f));
    osg::LightSource* lightSource1 = new osg::LightSource;
    lightSource1->setLight(light1);
    lightSource1->setLocalStateSetModes(osg::StateAttribute::ON);
    stateset = rootNode->getOrCreateStateSet();
    stateset->setMode(GL_LIGHT1, osg::StateAttribute::ON );
    lightSource1->setStateSetModes(*stateset,osg::StateAttribute::ON);

    //Lithgt2
    osg::Light* light2 = new osg::Light;
    light2->setLightNum(2);
    light2->setPosition(osg::Vec4(lightX,-lightY,lightHigth + lightHigthPlus,1.0f));
    light2->setDirection(osg::Vec3(0.0f,0.0f,0.0f));
    osg::LightSource* lightSource2 = new osg::LightSource;
    lightSource2->setLight(light2);
    lightSource2->setLocalStateSetModes(osg::StateAttribute::ON);
    stateset = rootNode->getOrCreateStateSet();
    stateset->setMode(GL_LIGHT2, osg::StateAttribute::ON );
    lightSource2->setStateSetModes(*stateset,osg::StateAttribute::ON);

    //Lithgt3
    osg::Light* light3 = new osg::Light;
    light3->setLightNum(3);
    light3->setPosition(osg::Vec4(-lightX,lightY,lightHigth + lightHigthPlus,1.0f));
    light3->setDirection(osg::Vec3(0.0f,0.0f,0.0f));
    osg::LightSource* lightSource3 = new osg::LightSource;
    lightSource3->setLight(light3);
    lightSource3->setLocalStateSetModes(osg::StateAttribute::ON);
    stateset = rootNode->getOrCreateStateSet();
    stateset->setMode(GL_LIGHT3, osg::StateAttribute::ON );
    lightSource3->setStateSetModes(*stateset,osg::StateAttribute::ON);



    //Shadow
    osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;
    shadowedScene->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
    shadowedScene->setCastsShadowTraversalMask(CastsShadowTraversalMask);
    osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
    shadowedScene->setShadowTechnique(sm.get());



    osg::Drawable* mirror = ground->getRootNode();

    osg::MatrixTransform* mirNode = new osg::MatrixTransform;


    // make sure that the global color mask exists.
    osg::ColorMask* rootColorMask = new osg::ColorMask;
    rootColorMask->setMask(true,true,true,true);

    // set up depth to be inherited by the rest of the scene unless
    // overrideen. this is overridden in bin 3.
    osg::Depth* rootDepth = new osg::Depth;
    rootDepth->setFunction(osg::Depth::LESS);
    rootDepth->setRange(0.0,1.0);

    osg::StateSet* rootStateSet = new osg::StateSet();
    rootStateSet->setAttribute(rootColorMask);
    rootStateSet->setAttribute(rootDepth);

    mirNode->setStateSet(rootStateSet);


    // bin1  - set up the stencil values and depth for mirror.
    {

        // set up the stencil ops so that the stencil buffer get set at
        // the mirror plane
        osg::Stencil* stencil = new osg::Stencil;
        stencil->setFunction(osg::Stencil::ALWAYS,1,~0u);
        stencil->setOperation(osg::Stencil::KEEP, osg::Stencil::KEEP, osg::Stencil::REPLACE);

        // switch off the writing to the color bit planes.
        osg::ColorMask* colorMask = new osg::ColorMask;
        colorMask->setMask(false,false,false,false);

        osg::StateSet* statesetBin1 = new osg::StateSet();
        statesetBin1->setRenderBinDetails(1,"RenderBin");
        statesetBin1->setMode(GL_CULL_FACE,osg::StateAttribute::OFF);
        statesetBin1->setAttributeAndModes(stencil,osg::StateAttribute::ON);
        statesetBin1->setAttribute(colorMask);

        // set up the mirror geode.
        osg::Geode* geode = new osg::Geode;
        geode->addDrawable(mirror);
        geode->setStateSet(statesetBin1);
        mirNode->addChild(geode);

    }

    // bin one - draw scene without mirror or reflection, unset
    // stencil values where scene is infront of mirror and hence
    // occludes the mirror.可能会影响性能  因为加载了两次
    {
        osg::Stencil* stencil = new osg::Stencil;
        stencil->setFunction(osg::Stencil::ALWAYS,0,~0u);
        stencil->setOperation(osg::Stencil::KEEP, osg::Stencil::KEEP, osg::Stencil::REPLACE);

        osg::StateSet* statesetBin2 = new osg::StateSet();
        statesetBin2->setRenderBinDetails(2,"RenderBin");
        statesetBin2->setAttributeAndModes(stencil,osg::StateAttribute::ON);


        osg::Group* groupBin2 = new osg::Group();
        groupBin2->setStateSet(statesetBin2);
        groupBin2->addChild(model);

        mirNode->addChild(groupBin2);
    }

    // bin3  - set up the depth to the furthest depth value
    {

        // set up the stencil ops so that only operator on this mirrors stencil value.
        osg::Stencil* stencil = new osg::Stencil;
        stencil->setFunction(osg::Stencil::EQUAL,1,~0u);
        stencil->setOperation(osg::Stencil::KEEP, osg::Stencil::KEEP, osg::Stencil::KEEP);

        // switch off the writing to the color bit planes.
        osg::ColorMask* colorMask = new osg::ColorMask;
        colorMask->setMask(false,false,false,false);

        // set up depth so all writing to depth goes to maximum depth.
        osg::Depth* depth = new osg::Depth;
        depth->setFunction(osg::Depth::ALWAYS);
        depth->setRange(1.0,1.0);

        osg::StateSet* statesetBin3 = new osg::StateSet();
        statesetBin3->setRenderBinDetails(3,"RenderBin");
        statesetBin3->setMode(GL_CULL_FACE,osg::StateAttribute::OFF);
        statesetBin3->setAttributeAndModes(stencil,osg::StateAttribute::ON);
        statesetBin3->setAttribute(colorMask);
        statesetBin3->setAttribute(depth);

        // set up the mirror geode.
        osg::Geode* geode = new osg::Geode;
        geode->addDrawable(mirror);
        geode->setStateSet(statesetBin3);

        mirNode->addChild(geode);

    }

    // bin4  - draw the reflection.
    {
        osg::ClipPlane* clipplane = new osg::ClipPlane;
        clipplane->setClipPlane(0.0,0.0,-1.0,0.0f);
        clipplane->setClipPlaneNum(0);

        osg::ClipNode* clipNode = new osg::ClipNode;
        clipNode->addClipPlane(clipplane);


        osg::StateSet* dstate = clipNode->getOrCreateStateSet();
        dstate->setRenderBinDetails(4,"RenderBin");
        dstate->setMode(GL_CULL_FACE,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);

        osg::Stencil* stencil = new osg::Stencil;
        stencil->setFunction(osg::Stencil::EQUAL,1,~0u);
        stencil->setOperation(osg::Stencil::KEEP, osg::Stencil::KEEP, osg::Stencil::KEEP);
        dstate->setAttributeAndModes(stencil,osg::StateAttribute::ON);

        osg::MatrixTransform* reverseMatrix = new osg::MatrixTransform;
        reverseMatrix->setStateSet(dstate);
        reverseMatrix->preMult(osg::Matrix::translate(0.0f,0.0f,0.0f)*
                     osg::Matrix::scale(1.0f,1.0f,-1.0f)*
                     osg::Matrix::translate(0.0f,0.0f,0.0f));

        reverseMatrix->addChild(model);

        clipNode->addChild(reverseMatrix);

        mirNode->addChild(clipNode);

    }


    // bin5  - draw the textured mirror and blend it with the reflection.
    {

        // set up depth so all writing to depth goes to maximum depth.
        osg::Depth* depth = new osg::Depth;
        depth->setFunction(osg::Depth::ALWAYS);

        osg::Stencil* stencil = new osg::Stencil;
        stencil->setFunction(osg::Stencil::EQUAL,1,~0u);
        stencil->setOperation(osg::Stencil::KEEP, osg::Stencil::KEEP, osg::Stencil::ZERO);

        // set up additive blending.
        osg::BlendFunc* trans = new osg::BlendFunc;
        trans->setFunction(osg::BlendFunc::ONE,osg::BlendFunc::ONE);

        osg::StateSet* statesetBin5 = new osg::StateSet();
        //osg::StateSet* statesetBin5 = createMirrorTexturedState("Images/tank.rgb");

        statesetBin5->setRenderBinDetails(5,"RenderBin");
        statesetBin5->setMode(GL_CULL_FACE,osg::StateAttribute::OFF);
        statesetBin5->setAttributeAndModes(stencil,osg::StateAttribute::ON);
        statesetBin5->setAttributeAndModes(trans,osg::StateAttribute::ON);
        statesetBin5->setAttribute(depth);

        // set up the mirror geode.
        osg::Geode* geode = new osg::Geode;
        geode->addDrawable(mirror);
        geode->setStateSet(statesetBin5);

        model->setNodeMask(CastsShadowTraversalMask);
        geode->setNodeMask(ReceivesShadowTraversalMask);
        shadowedScene->addChild(geode);
        shadowedScene->addChild(model);
        shadowedScene->addChild(lightSource);
        shadowedScene->addChild(lightSource1);
//        shadowedScene->addChild(lightSource2);
//        shadowedScene->addChild(lightSource3);


        mirNode->addChild(shadowedScene);
    }

    return mirNode;
}



