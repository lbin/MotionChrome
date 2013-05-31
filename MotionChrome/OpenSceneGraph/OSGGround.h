#ifndef OSGGROUND_H
#define OSGGROUND_H

#include <osgViewer/Viewer>
#include <osg/Group>
#include <osgFX/SpecularHighlights>

class OSGGround
{
public:
    OSGGround();
    ~OSGGround();

    int createGround();
    osg::ref_ptr<osg::MatrixTransform> getRootNode();

private:
    osg::ref_ptr<osg::MatrixTransform> rootNode;
};

#endif // OSGGROUND_H
