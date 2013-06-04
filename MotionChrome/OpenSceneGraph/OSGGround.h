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

    osg::Drawable* createGround();
    osg::Drawable* getRootNode();

private:
    osg::Drawable* rootNode;
};

#endif // OSGGROUND_H
