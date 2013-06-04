#include "OSGGround.h"
#include <osg/Vec3>


#include <osg/Geometry>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>
#include <osg/Sequence>
#include <vector>




OSGGround::OSGGround(void)
{
    rootNode = createGround();
}

OSGGround::~OSGGround(void)
{
}
osg::Drawable* OSGGround::createGround()
{
    osg::Vec3 center(0.0f,0.0f,-1.0f);
    float radius = 1000.0f;
    int numTilesX = 100;
    int numTilesY = 100;
    float width = 2*radius;
    float height = 2*radius;
    osg::Vec3 v000(center - osg::Vec3(width*0.5f,height*0.5f,0.0f));
    osg::Vec3 dx(osg::Vec3(width/((float)numTilesX),0.0,0.0f));
    osg::Vec3 dy(osg::Vec3(0.0f,height/((float)numTilesY),0.0f));


    osg::Vec3Array* coords = new osg::Vec3Array;
    int iy;
    for(iy=0;iy<=numTilesY;++iy)
    {
        for(int ix=0;ix<=numTilesX;++ix)
        {
            coords->push_back(v000+dx*(float)ix+dy*(float)iy);
        }
    }
    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(100.0f/255,112.0f/255,199.0f/255,1.0f)); // white

    colors->push_back(osg::Vec4(60.0f/255,60.0f/255,60.0f/255,1.0f));
    int numColors=colors->size();


    int numIndicesPerRow=numTilesX+1;
    osg::UIntArray* coordIndices = new osg::UIntArray; // assumes we are using less than 256 points...
    osg::UIntArray* colorIndices = new osg::UIntArray;
    for(iy=0;iy<numTilesY;++iy)
    {
        for(int ix=0;ix<numTilesX;++ix)
        {
            coordIndices->push_back(ix + (iy+1)*numIndicesPerRow);
            coordIndices->push_back(ix + iy*numIndicesPerRow);
            coordIndices->push_back((ix + 1) + iy*numIndicesPerRow);
            coordIndices->push_back((ix + 1)+(iy + 1)*numIndicesPerRow);
            colorIndices->push_back((ix + iy) % numColors);
        }
    }
    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
    osg::Geometry* geom = new osg::Geometry;
    geom->setVertexArray(coords);
    geom->setVertexIndices(coordIndices);

    geom->setColorArray(colors);
    geom->setColorIndices(colorIndices);
    geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
    geom->setNormalArray(normals);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,coordIndices->size()));
//    osg::Geode* geode = new osg::Geode;
//    geode->addDrawable(geom);
//    rootNode->addChild(geode);
//    rootNode->setDataVariance(osg::Object::DYNAMIC);
    return geom;

}

osg::Drawable* OSGGround::getRootNode()
{
    return rootNode;
}
