#include "OSGSkeleton.h"

osg::ref_ptr<osg::Group> createBody(osg::Vec3 offset)
{
    // blue color
    float bonew = 0.5f;
    float ballw = ballw*2+1;
    osg::Vec4 boneColor(85.0f/255, 146.0f/255, 237.0f/255, 1.0f);
    osg::Vec4 jointColor(150.0f/255, 210.0f/255, 255.0f/255, 1.0f);
    osg::ref_ptr<osg::Group> group = new osg::Group;

    osg::Vec3 startPt = osg::Vec3(0.0f,0.0f,0.0f);
    osg::Vec3 t = offset;
    float len=t.length();
    t/=2;

    osg::Cylinder *unitCylinder = new osg::Cylinder(osg::Vec3(0,0,0),bonew,len);
    osg::ShapeDrawable* unitCylinderDrawable = new osg::ShapeDrawable(unitCylinder);

    osg::Sphere* startSphere = new osg::Sphere(startPt, bonew);
    osg::Sphere* endSphere = new osg::Sphere(offset, bonew);
    osg::ShapeDrawable* startSphereDrawable=new osg::ShapeDrawable(startSphere);
    osg::ShapeDrawable* endSphereDrawable=new osg::ShapeDrawable(endSphere);

    unitCylinderDrawable->setColor(boneColor);
    startSphereDrawable->setColor(jointColor);
    endSphereDrawable->setColor(jointColor);

    osg::Geode* geode1=new osg::Geode;
    osg::Geode* geode2=new osg::Geode;   // geode2是骨头两端的球体，是不需要做tranform变换的

    osg::MatrixTransform* transform=new osg::MatrixTransform;
    transform->addChild(geode1);

    osg::Vec3 axis=osg::Vec3(0,0,1)^t;//旋转轴

    float theta=acos(t.z()/t.length());//旋转角度

    //创建变化实例
    osg::Matrixd mat1,mat2;
    mat1.makeRotate(osg::Quat(theta,axis));
    mat2.makeTranslate(t);
    transform->setMatrix(mat1*mat2);

    geode1->addDrawable(unitCylinderDrawable);
    geode2->addDrawable(startSphereDrawable);
    geode2->addDrawable(endSphereDrawable);
    group->addChild(transform);
    group->addChild(geode2);
    return group;
}


osg::ref_ptr<osg::Geode> createHead(osg::Vec3 offset)
{
    osg::Vec4 jointColor(255.0f/255, 255.0f/255, 255.0f/255, 1.0f);
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::Vec3 centerPosOfHead = (offset - osg::Vec3(0.0f,0.0f,0.0f)) / 2;

    osg::Sphere* headSphere = new osg::Sphere(centerPosOfHead, 1.35);   // the param sets the radius
    osg::ShapeDrawable* headSphereDrawable=new osg::ShapeDrawable(headSphere);

    headSphereDrawable->setColor(jointColor);

    geode->addDrawable(headSphereDrawable);

    return geode;
}

struct AnimationManagerFinder : public osg::NodeVisitor
{
    osg::ref_ptr<osgAnimation::BasicAnimationManager> _am;
    AnimationManagerFinder() : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {}
    void apply(osg::Node& node) {
        if (_am.valid())
            return;
        if (node.getUpdateCallback()) {
            osgAnimation::AnimationManagerBase* b = dynamic_cast<osgAnimation::AnimationManagerBase*>(node.getUpdateCallback());
            if (b) {
                _am = new osgAnimation::BasicAnimationManager(*b);
                return;
            }
        }
        traverse(node);
    }
};


struct AddHelperBone : public osg::NodeVisitor
{
    AddHelperBone() : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {}
    void apply(osg::Transform& node) {
        osgAnimation::Bone* bone = dynamic_cast<osgAnimation::Bone*>(&node);
        if (bone){
            unsigned int numChild = bone->getNumChildren();
            for(unsigned int i = 0; i< numChild; ++i)
            {
                osg::ref_ptr<osgAnimation::Bone> childBone = dynamic_cast<osgAnimation::Bone*>(bone->getChild(i));
                osg::Vec3 offset = childBone->getMatrixInSkeletonSpace().getTrans();
                if(bone->getName()!="SkelRoot")
                bone->addChild(createBody(offset));
            }
        }
        traverse(node);
    }
};


OSGSkeleton::OSGSkeleton()
{
}

void OSGSkeleton::createSkeleton(string str)
{
    skeleton = dynamic_cast<osg::Group*>(osgDB::readNodeFile(str));
    AnimationManagerFinder finder;
    skeleton->accept(finder);
    if (finder._am.valid()) {
        skeleton->setUpdateCallback(finder._am.get());
        AnimtkViewerModelController::setModel(finder._am.get());
    }
    AddHelperBone addHelper;
    skeleton->getChild(0)->accept(addHelper);
}





