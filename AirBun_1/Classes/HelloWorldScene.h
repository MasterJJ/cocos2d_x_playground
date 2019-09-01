/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Joystick.h"
USING_NS_CC;


enum {
    TAG_USER = 0,
    TAG_USER_MOVE,
    TAG_LABEL_TITLE,
    TAG_LABEL_GAMEOVER,
    TAG_LABEL_GAMEOVER_2,
    TAG_GAME_MAX
    
} TAG_GAME;

class SpriteEnemy : public  Sprite
{
public:
    CREATE_FUNC(SpriteEnemy);
    
    int type;
    int hp;
    bool isAttack;
    
    
};

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // init bg
    void init_bg();
    void init_sound();


    bool _titleOn, _gameOver, _user_die;
    Vector<Sprite*> _missies, _attacks;
    Vector<SpriteEnemy*> _enemies;
    void update(float delta);
    
    // game init
    void initGameData();
    void initTitle();
    void setTitle();
    
    void init_control();
    bool onTouchBegan(Touch* touch, Event* unused_event);
    
    
    // init user
    
    void init_user();
    void reset_user();

    // game play init
    void setMissile(float delta);
    void resetMissile(Ref* sender);
    
    
    // init emnermy
    void set_enemy(float delta);
    void reset_enemy(Ref* sender);
    void attackEnemy(Ref* sender);
    void attackEnemy_2(Vec2 pos);
    void attackEnemy_3(Vec2 pos);
    void resetAttack(Ref* sender);
    
    void init_gameover();
    void setGameover();
    void resetGameover();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    
private:
   
    Joystick* _joystick;
    Size _wsize;  // window size

    void initJoystick();
    
    
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
