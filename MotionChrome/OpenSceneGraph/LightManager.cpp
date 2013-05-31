#include "lightManager.h"

LightManager:: LightManager()
  : groupOfLights(NULL),
    lights(8),
    valid(false) {
  /*
  Initialize our list to have all NULL values. This is done so when
  the user calls the get_or_create_light() function, the function
  can properly detect whether a light has been created for a particular
  slot.
  */
  for(unsigned int i = 0; i < lights.size(); ++i){
      lights[i] = NULL;
  }
        
}

void LightManager::init(osg::Group* root) {
  groupOfLights = root;
  valid = true;
}

osg::Light * LightManager::
getOrCreateLight(LightManager::LIGHTNUMBER number) {
  if(valid){
     if(lights[number]){
        return lights[number]->getLight();
     }
        
     //       Create a new light, and set the light number according to our parameter.
    osg::Light * light = new osg::Light();
    light->setLightNum(number);

        /*
    Create a new lightsource. This is the object which holds a light, there
    is a 1-1 correspondence between a light and a lightsource. The vector
    actually holds lightsources.
            
    Set the lightsource to contain our newly created light
        */
    osg::LightSource * lightsource = new osg::LightSource();
    lightsource->setLight(light);
        
    /*
    Now we need to enable the lightsource and thereby light in the SetState
    of the group node that will contain the lightsource.
    */
    lightsource->setStateSetModes(*groupOfLights->getOrCreateStateSet(), osg::StateAttribute::ON);
  
    /*
    Add the lightsource to the osg::Group
    */
    groupOfLights->addChild(lightsource);

    /*
    Now we stick the address of the lightsource in the proper slot in
    the vector of lights, so future calls with this number will extract
    this light, and not allocate a new one.
    */
    lights[number] = lightsource;
        
    // Return a pointer to the osg::Light so the user may modify the properties.
    return light;
  }
  return NULL;
}
