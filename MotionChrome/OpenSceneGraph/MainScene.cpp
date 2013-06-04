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
    skelRoot->createSkeleton("C:\\Users\\libin\\Desktop\\mocap\\1.bvh");


    osg::ref_ptr<osg::MatrixTransform> skelMat = new osg::MatrixTransform;
    skelMat->addChild(skelRoot->skeleton);
    skelMat->setMatrix(osg::Matrix::rotate(osg::inDegrees(90.0f),1.0f,0.0f,0.0f));
    //rootNode->addChild(skelMat);

    //Camera
    this->getCamera()->setClearColor(osg::Vec4(0.0f/255, 0.0f/255, 0.0f/255, 1.0f));
    //setCameraManipulator(new osgGA::TrackballManipulator);
    //setCameraManipulator(new osgGA::NodeTrackerManipulator);

    osgGA::NodeTrackerManipulator *ntm = new osgGA::NodeTrackerManipulator();
    ntm->setTrackNode(skelRoot->skeleton);
    this->setCameraManipulator(ntm);


    //Mirror
    rootNode->addChild(createMirroredScene(skelMat.get()));



    osgUtil::Optimizer optimizer;
    optimizer.optimize(rootNode.get());

    this->setSceneData(rootNode.get());

    //抗锯齿
    osg::DisplaySettings* ds = osg::DisplaySettings::instance();
    ds->setNumMultiSamples(2);
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
    //Light
    osg::Light* light = new osg::Light;
    light->setLightNum(0);
    light->setPosition(osg::Vec4(500.0f,0.0f,500.0f,1.0f));
    light->setDirection(osg::Vec3(0.0f,0.0f,0.0f));
    osg::LightSource* lightSource = new osg::LightSource;
    lightSource->setLight(light);
    lightSource->setLocalStateSetModes(osg::StateAttribute::ON);
    osg::StateSet* stateset = rootNode->getOrCreateStateSet();
    lightSource->setStateSetModes(*stateset,osg::StateAttribute::ON);

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

        mirNode->addChild(shadowedScene);
    }

    return mirNode;
}



