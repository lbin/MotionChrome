#ifndef ANIMTKVIEWERMODELCONTROLLER_H
#define ANIMTKVIEWERMODELCONTROLLER_H

#include <osg/Node>
#include <osgDB/ReadFile>
#include <osgAnimation/BasicAnimationManager>
#include <string.h>
using namespace std;

class AnimtkViewerModelController
{
public:
    typedef std::vector<std::string> AnimationMapVector;

    static AnimtkViewerModelController& instance()
    {
        static AnimtkViewerModelController avmc;
        return avmc;
    }

    static bool setModel(osgAnimation::BasicAnimationManager* model)
    {
        AnimtkViewerModelController& self = instance();
        self._model = model;
        for (osgAnimation::AnimationList::const_iterator it = self._model->getAnimationList().begin(); it != self._model->getAnimationList().end(); it++)
            self._map[(*it)->getName()] = *it;

        for(osgAnimation::AnimationMap::iterator it = self._map.begin(); it != self._map.end(); it++)
            self._amv.push_back(it->first);

        return true;
    }

    bool list()
    {
        std::cout << "Animation List:" << std::endl;
        for(osgAnimation::AnimationMap::iterator it = _map.begin(); it != _map.end(); it++)
            std::cout << it->first << std::endl;
        return true;
    }

    bool play()
    {
        if(_focus < _amv.size())
        {
            std::cout << "Play " << _amv[_focus] << std::endl;
            _model->playAnimation(_map[_amv[_focus]].get());
            return true;
        }

        return false;
    }

    bool update_l()
    {
        if(_focus < _amv.size())
        {
            osgAnimation::Animation* anmi = _map[_amv[_focus]].get();
            anmi->setPlayMode(osgAnimation::Animation::LOOP);
            for(osgAnimation::ChannelList::const_iterator it = anmi->getChannels().begin(); it!=anmi->getChannels().end();++it)
            {
                if ((*it)->getName() == "position")
                {
                    osgAnimation::Vec3KeyframeContainer* pos = dynamic_cast<osgAnimation::Vec3KeyframeContainer*>((*it)->getSampler()->getKeyframeContainer());
                    for(size_t i = 0; i< pos->size();++i)
                    {
                        osgAnimation::Vec3Keyframe a = pos->at(6);
                        a.setValue(osg::Vec3f(0.0f,0.0f,0.0f));
                        pos->at(i).setValue(osg::Vec3f(0.0f,0.0f,0.0f));
                    }

                }
            }
            return true;
        }

        return false;

    }

    bool stop()
    {
        if(_focus < _amv.size())
        {
            std::cout << "Stop " << _amv[_focus] << std::endl;
            _model->stopAnimation(_map[_amv[_focus]].get());
            return true;
        }
        return false;
    }

    bool next()
    {
        _focus = (_focus + 1) % _map.size();
        std::cout << "Current now is " << _amv[_focus] << std::endl;
        return true;
    }

    bool previous()
    {
        _focus = (_map.size() + _focus - 1) % _map.size();
        std::cout << "Current now is " << _amv[_focus] << std::endl;
        return true;
    }

    bool playByName(const std::string& name)
    {
        for(unsigned int i = 0; i < _amv.size(); i++) if(_amv[i] == name) _focus = i;
        _model->playAnimation(_map[name].get());
        return true;
    }

    const std::string& getCurrentAnimationName() const
    {
        return _amv[_focus];
    }

    const AnimationMapVector& getAnimationMap() const
    {
        return _amv;
    }

private:
    osg::ref_ptr<osgAnimation::BasicAnimationManager> _model;
    osgAnimation::AnimationMap                   _map;
    AnimationMapVector                     _amv;
    unsigned int                           _focus;

    AnimtkViewerModelController():
        _model(0),
        _focus(0) {}
};


#endif // ANIMTKVIEWERMODELCONTROLLER_H
