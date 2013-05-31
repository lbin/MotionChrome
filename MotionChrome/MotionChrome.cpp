#include "MotionChrome.h"
#include "ui_MotionChromee.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDockWidget>

#include <osgDB/ReaderWriter>
#include <osgAnimation/Bone>
#include <osgAnimation/Skeleton>
#include <osgAnimation/UpdateBone>
#include <osgAnimation/StackedTransform>
#include <osgAnimation/StackedTranslateElement>
#include <osgAnimation/StackedQuaternionElement>
#include <osgAnimation/BasicAnimationManager>

#include <UI/LayoutDesign.h>
#include <OpenSceneGraph/OSGGround.h>
#include <string.h>
using namespace std;




MotionChrome::MotionChrome(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MotionChrome)
{
    ui->setupUi(this);

    //UI Design
    //add osg mainScene
    QDockWidget* displayDock = new QDockWidget(tr("Display"),this);
    displayDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    displayDock->setAllowedAreas(NULL);
    displayDock->setFloating(true);
    this->addDockWidget(Qt::LeftDockWidgetArea,displayDock);
    mainScene = new MainScene();
    displayDock->setWidget(mainScene);
    displayDock->widget()->show();

    //Beginning



}

MotionChrome::~MotionChrome()
{
    delete ui;
}
