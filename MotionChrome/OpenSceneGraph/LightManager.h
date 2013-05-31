/*##############################################################################
 File:       light_module.h
 Purpose:    This class encapsulates some basic lighting functionality provided
             by OSG. It allows for the creation of up to 8 lights which may
             be modified via the direct OSG::Light interface.
             
             Note that the user requests lights from this module, and should
             not instantiate them on their own, unless they have some way
             of keeping track of which lights have been previously enabled.
             
             Note that a maximum of 8 lights may be added to the scene
             using this module.
                
             Also note that unless light ONE is allocated here, the it will
             be used as the default SceneView light, as it has been in
             the previous tutorials. If light ONE is allocated using this
             class, then there will be no default SceneView light, leaving
             the scene dark save for objects illuminated by allocated lights.
Returns:    N/A
*/
#ifndef LIGHT_MANAGER
#define LIGHT_MANAGER

#include <vector>
using std::vector;
// LightSource - used to hold osg::Lights
#include <osg/LightSource>

// Light - represents the functionality of the OpenGL light model
#include <osg/Light>

class LightManager {
public:
    /*
        This enumeration represents the number of the light currently being allocated.
        OpenGL allows for a minimum, and usually maximum of 8 lights at any given
        time. This enumeration allows the user to safely request one of these 8
        without having to worry about stepping out of bounds. Note that they
        are 1 off, meaning 'ONE' actually represents the 0 light in OpenGL, while
        'EIGHT' represents the 7 light.
    */
    enum LIGHTNUMBER{
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT
    };
    
    /*//////////////////////////////////////////////////////////////////////////
        Default Constructor - This does not allocate any lights. Note that upon
        construction, this module is not yet ready for light allocation. The
        init() function must first be called.
    //////////////////////////////////////////////////////////////////////////*/
    LightManager();
    
    /*//////////////////////////////////////////////////////////////////////////
        Default Constructor - Empty
    //////////////////////////////////////////////////////////////////////////*/
    ~LightManager() {}

    /*//////////////////////////////////////////////////////////////////////////
        This function is necessary to grab the osg::Group that will contain
        any lights that are requested for allocation. Note that this osg::Group,
        for some reason, should be DIRECTLY under the SceneView. That is, there
        should not be any other nodes between the SceneView and this osg::Group.
        It has something to do with osg::SetStates, but I have yet to figure out
        what.
    //////////////////////////////////////////////////////////////////////////*/
    void init(osg::Group * root);
    
    /*//////////////////////////////////////////////////////////////////////////
        This function is used to request allocation of an osg::Light. The light
        is requested by passing the enumerated value representing the number
        of the light. If the light has not been previously allocated, it is created
        and added to the osg::Group for rendering. The osg::Light is returned
        as a pointer, which may be used to modify the light properties via
        the osg::Light interface.
        
        If the light has been previously allocated, it is returned as a pointer.
        
        Note that if this module has not been initialized with the init() function,
        NULL will be returned instead of an osg::Light.
    //////////////////////////////////////////////////////////////////////////*/
  osg::Light* getOrCreateLight(LightManager::LIGHTNUMBER number);
    
private:
    osg::Group * groupOfLights;
    /*
        Vector (really we're just using it as an array), of osg::LightSources,
        which themselves contain an individual osg::Light. The number defined
        by LightManager::LIGHTNUMBER will directly index a slot in this vector.
    */
    vector<osg::LightSource*>   lights;
    
    /*
        Boolean that represents the state of this class:
            false - this class has not been initialized yet
            true - the class has been initialized and is ready for light allocation
    */
    bool valid;
};
#endif
