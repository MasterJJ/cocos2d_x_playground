//
//  sound_base.hpp
//  AirBun_1-mobile
//
//  Created by masterj on 02/08/2019.
//

#ifndef __SOUND_BASE_H__
#define __SOUND_BASE_H__

#include <stdio.h>

#include "SimpleAudioEngine.h"

class SoundBase
{
public:
    void init();
    int play();
    int intro_play();
    
    int play_effect(std::string file);
    
private:
    
    
};

#endif /* sound_base_hpp */
