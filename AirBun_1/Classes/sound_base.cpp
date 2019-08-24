//
//  sound_base.cpp
//  AirBun_1-mobile
//
//  Created by masterj on 02/08/2019.
//

#include "sound_base.hpp"
USING_NS_CC;
#include <SimpleAudioEngine.h>
using namespace CocosDenshion;


void SoundBase::init(){
    
    
    return;
}


int SoundBase::play_effect(std::string file) {
    
    if (file.length() <= 0)
        return 0;
    
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->playEffect(file.c_str());
    
    
    return 0;
}



int SoundBase::intro_play() {
    
    
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->preloadBackgroundMusic("nbgm.mp3");
    audio->playBackgroundMusic("nbgm.mp3");
    
    return 0;
}


int SoundBase::play() {
    
    
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
//    audio->preloadBackgroundMusic("audio/dimensions.mp3");
//    audio->playBackgroundMusic("audio/dimensions.mp3");
    audio->preloadBackgroundMusic("nnbgm.mp3");
    audio->playBackgroundMusic("nnbgm.mp3");

    return 0;
}
