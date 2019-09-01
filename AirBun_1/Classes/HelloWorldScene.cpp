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

#include "HelloWorldScene.h"

#include "sound_base.hpp"

#define USE_JOYSTICK_MODE 1

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

SoundBase _sound_base;



// 1. 배경 넣기
// 2. 캐릭터 움직이기
// 3. 총알 나가기
// 4. 캐릭터 추가 하기
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

void HelloWorld::initJoystick() {
    _joystick = Joystick::create();
    
    this->addChild(_joystick, 2);
    _joystick->setVisible(false);
    return;
}


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    
    srand((int)time(NULL));
    

    // init game sence
    init_control();
    
    // rotate img
    init_bg();
    initGameData();
    // init player
    init_user();
    
    initTitle();
    
    init_gameover();
 
#if USE_JOYSTICK_MODE
    
    initJoystick();
#endif
    _sound_base.intro_play();
    
    return true;
}

void HelloWorld::init_control() {
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    _wsize = Director::getInstance()->getWinSize();
    
    return;
}


// init game over
void HelloWorld::init_gameover() {
   
    {
        auto label = Label::createWithTTF("GAME OVER", "fonts/Abberancy.ttf", 40);
        
        if (label == nullptr)
            return;
        label->setRotation(-90);
        // set color
        label->setTextColor(Color4B::RED);
        label->enableOutline(Color4B::YELLOW, 3);
        
        label->setPosition(Vec2(_wsize.width / 2, _wsize.height / 2));
        label->setVisible(false);
        label->setTag(TAG_LABEL_GAMEOVER);
        this->addChild(label, 1);
    }

    {
        auto label = Label::createWithTTF("PRESSS TO START", "fonts/Abberancy.ttf", 38);
        
        if (label == nullptr)
            return;
        label->setRotation(-90);
        // set color
        label->setTextColor(Color4B::RED);
        label->enableOutline(Color4B::YELLOW, 3);
        
        label->setPosition(Vec2(_wsize.width / 3, _wsize.height / 2));
        label->setVisible(false);
        label->setTag(TAG_LABEL_GAMEOVER_2);
        this->addChild(label, 1);
    }
    
    return;
}

void HelloWorld::setGameover() {
    _gameOver = true;
    
    
    {
        auto label = (Label*)this->getChildByTag(TAG_LABEL_GAMEOVER);
        label->runAction(Sequence::create(Show::create(),
                                          Spawn::create(
                                                        ScaleTo::create(1.0f, 1.0f),
                                                        RotateBy::create(1.0f, 360 * 5),
                                                        NULL),
                                          NULL));
    }
    
    {
        auto label = (Label*)this->getChildByTag(TAG_LABEL_GAMEOVER_2);
        label->runAction(Sequence::create(Show::create(),
                                          DelayTime::create(1.2f),
                                          ScaleTo::create(0.35f, 0.8f),
                                          NULL));
    }
    
    
    return;
}

// reset game over gameover re game
void HelloWorld::resetGameover() {
    {
        
        auto label = (Label*)this->getChildByTag(TAG_LABEL_GAMEOVER);
        label->stopAllActions();
        label->setScale(0.0f);
        label->setRotation(-90);
        label->setVisible(false);
    }
    {
        
        auto label = (Label*)this->getChildByTag(TAG_LABEL_GAMEOVER_2);
        label->stopAllActions();
        label->setScale(0.0f);
        label->setRotation(-90);
        label->setVisible(false);
    }
    
    
    for (SpriteEnemy *en : _enemies)
        this->removeChild(en);

    for (Sprite *att : _attacks)
        this->removeChild(att);
    
    for (Sprite *mi : _missies)
        this->removeChild(mi);
    
    //this->unscheduleAllSelectors();
    
    // initgame data
    initGameData();
    setTitle();
    
    return;
}


void HelloWorld::initGameData() {
    _titleOn = true;
    _user_die = false;
    _gameOver = false;
    
    _missies.clear();
    _enemies.clear();
    _attacks.clear();
    
    return;
}

// set title
void HelloWorld::setTitle() {
    init_sound();
    
    _titleOn = false;
    
    auto label = (Label*)this->getChildByTag(TAG_LABEL_TITLE);
    label->runAction(FadeOut::create(1.0f));
    
    auto spr = (Sprite*)this->getChildByTag(TAG_USER);
    spr->setPosition(Vec2(_wsize.width / 2, 200 - 250));
    spr->setVisible(true);
    spr->runAction(MoveBy::create(1.0f, Vec2(0, 250)));
    
    // schedule update
    this->scheduleUpdate();
    this->schedule(schedule_selector(HelloWorld::setMissile), 0.15f);
    this->schedule(schedule_selector(HelloWorld::set_enemy), 1.0f);
    
    
    return;
}

void HelloWorld::setMissile(float delta) {
    _sound_base.play_effect("shoot.wav");
    
    auto pl = (Sprite *)this->getChildByTag(TAG_USER);
    
    auto mi = Sprite::create("fire_0.png");
    mi->setRotation(-90);
    // sprite missile po player front + 20(y)
    mi->setPosition(pl->getPosition() + Vec2(-20, 0));
    
    // add sprite
    this->addChild(mi);
    
    _missies.pushBack(mi);
    
    mi->runAction(Sequence::create(
                                   MoveBy::create(2.0f, Vec2(-_wsize.width, 0)),
                                   CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetMissile, this)),
                                   NULL));

    delta = 0;
    return;
}

// init title
void HelloWorld::initTitle() {
    
    auto label = Label::createWithTTF("AirBun", "fonts/Abduction.ttf", 65);
    
    if (label == nullptr)
        return;
    label->setRotation(-90);
    // set color
    label->setTextColor(Color4B::RED);
    label->enableOutline(Color4B::YELLOW, 3);
    
    label->setPosition(Vec2(_wsize.width / 2, _wsize.height / 2));
    
    label->setTag(TAG_LABEL_TITLE);
    this->addChild(label);
    return;
}


void HelloWorld::resetMissile(Ref *sender) {
    auto sp = (Sprite*)sender;
    _missies.eraseObject(sp);
    this->removeChild(sp);
    
    return;
}


// user reset
void HelloWorld::reset_user() {
    _joystick->setVisible(false);
    
    _sound_base.play_effect("explosion.wav");
    
    _user_die = false;
    
    // get sprite by tag
    auto user = (Sprite*)this->getChildByTag(TAG_USER);
    
    user->setVisible(false);
    
    auto sp = Sprite::create();
    sp->setPosition(user->getPosition());
    this->addChild(sp);
    
    // ani
    auto ani = Animation::create();
    ani->setDelayPerUnit(0.05f);
    // load explosion texture
    auto tex = Director::getInstance()->getTextureCache()->addImage("Explosion.png");
    
    sp->setRotation(-90);
    for (int idx = 0; idx < 11; idx++) {
        
        ani->addSpriteFrameWithTexture(tex, Rect(idx % 5 * 75, idx / 5 * 75, 75, 75));
    }
    
    // animate
    
    auto animate = Animate::create(ani);
    sp->runAction(animate);
    
    sp->runAction(Sequence::create(
                                   DelayTime::create(0.55f),
                                   RemoveSelf::create(),
                                   CallFunc::create(CC_CALLBACK_0(HelloWorld::setGameover, this)),
                                   NULL));
    
    this->unschedule(schedule_selector(HelloWorld::setMissile));
    this->unschedule(schedule_selector(HelloWorld::set_enemy));
    return;
}


void HelloWorld::resetAttack(Ref *sender) {
    // get sender
    auto sp = (Sprite*)sender;
    _attacks.eraseObject(sp);
    this->removeChild(sp);
    
    return;
}


void HelloWorld::reset_enemy(Ref *sender) {
    
    // get sener sprite
    auto sp = (SpriteEnemy*)sender;
    _enemies.eraseObject(sp);
    this->removeChild(sp);
    
    
    return;
}


// init enemy
void HelloWorld::set_enemy(float delta) {
    float y = 50 + rand() % (500 - 50 * 2);
    
    auto spr = SpriteEnemy::create();
    spr->setRotation(-90);
    spr->setPosition(Vec2(-1, y)); //_wsize.height + 100));
    spr->isAttack = false;
    
    this->addChild(spr);
    
    int ran = rand() % 100;
    float speed;
    
    if (ran < 60) {
        spr->type = 1;
        spr->hp = 5;
        speed = 10.0f;
        
    }
    else if (ran < 90) {
        spr->type = 2;
        spr->hp = 10;
        speed = 15.0f;
        spr->setScaleX(1.6f);
        
    } else {
        spr->type = 3;
        spr->hp = 3;
        speed = 20.0f;
        spr->setScaleX(0.7f);
        spr->setScaleY(1.4f);
        
    }
    
    
    _enemies.pushBack(spr);
    
    // animation create
    auto animation = Animation::create();
    // animation speed
    animation->setDelayPerUnit(0.1f);
    auto tex = Director::getInstance()->getTextureCache()->addImage("enemy.png");
 
    for (int idx = 0; idx < 4; idx++) {
        
        animation->addSpriteFrameWithTexture(tex, Rect(idx * 45, 0, 40, 50));
    }
    
    auto animate = Animate::create(animation);
    
    
    spr->runAction(animate);
    
    
    spr->runAction(Sequence::create(
                                    MoveBy::create(speed, Vec2(_wsize.height + 200, 0)),
                                    
                                    CallFuncN::create(CC_CALLBACK_1(HelloWorld::reset_enemy, this)),
                                    NULL));
    
    
    
    return;
}


void HelloWorld::init_user() {
    
    auto spr = Sprite::create();
    spr->setPosition(Vec2(_wsize.width +100, 100));
    spr->setTag(TAG_USER);
    spr->setRotation(-90);
    this->addChild(spr);
    
    auto tex = Director::getInstance()->getTextureCache()->addImage("Spaceship.png");
    
    auto ani = Animation::create();
    ani->setDelayPerUnit(0.1f);
    
    
    
    for (int idx = 0; idx < 4; idx++) {
        ani->addSpriteFrameWithTexture(tex, Rect(idx * 45, 0, 40, 50));
    }
    
    auto animate = Animate::create(ani);
    spr->runAction(RepeatForever::create(animate));

}

void HelloWorld::update(float delta) {
    
    if (_user_die) return;
    
    auto player = (Sprite*) this->getChildByTag(TAG_USER);
    Rect player_rect = Rect(player->getPositionX() - 10, player->getPositionY() - 20, 20, 40);
                            

    for (SpriteEnemy *en : _enemies) {
        if (en->type == 2) {
            if (!en->isAttack && en->getPositionX() < _wsize.width * 3 / 4) {
      
                en->isAttack = true;
                
                attackEnemy_2(en->getPosition());
            }
            
        } else if (en->type == 3) {
            if (!en->isAttack
                && player->getPositionY() > en->getPositionY() - 20
                && player->getPositionY() < en->getPositionY() + 20) {
                
                en->isAttack = true;
                
                attackEnemy_3(en->getPosition());
            }
        }
    }
    
    
    bool crash = false;
    
    // check player missile
    for (Sprite * mi : _missies) {
        // get boundingBox rect
        
        Rect mi_r = mi->getBoundingBox();
        
        // en crash check
        for (SpriteEnemy *en : _enemies) {
            Rect en_r  = en->getBoundingBox();
            
            // check crash
            if (mi_r.intersectsRect(en_r)) {
                
                // effect sound
                _sound_base.play_effect("boom.wav");
                
                en->hp--;
                
                if (en->hp <= 0) {
                    _sound_base.play_effect("explosion.wav");
                    
                    auto particle = ParticleSystemQuad::create("explosion.plist");
                    
                    this->addChild(particle);
                    particle->setPosition(en->getPosition());
                    
                    // run action
                    
                    particle->runAction(Sequence::create(
                                                         DelayTime::create(2.0f),
                                                         RemoveSelf::create(),
                                                         NULL));
                    
                    _enemies.eraseObject(en);
                    this->removeChild(en);
                }
                
                
                _missies.eraseObject(mi);
                this->removeChild(mi);
                
                crash = true;
                break;
            }
            
        }
        
        
        
        if (crash) {
            crash = false;
            break;
        }
        
    }
    
    
    // atack enermy missiles
    for (Sprite *attack : _attacks) {
        // get bounding box
        Rect attack_r = attack->getBoundingBox();
        // check crash
        if (player_rect.intersectsRect(attack_r)) {
            // eraseobject
            _attacks.eraseObject(attack);
            this->removeChild(attack);
            
            reset_user();
            
            return;
        }
        
        
        // enemies crash to player
        for (Sprite *en : _enemies) {
            Rect en_r = en->getBoundingBox();
            
            if (!player_rect.intersectsRect(en_r))
                continue;
            
            _sound_base.play_effect("explosion.wav");
            
            auto parti = ParticleSystemQuad::create("explosion.plist");
            
            parti->setPosition(en->getPosition());
            this->addChild(parti);
            
            parti->runAction(Sequence::create(
                                            DelayTime::create(2.0f),
                                            RemoveSelf::create(),
                                             NULL));
            
            _attacks.eraseObject(en);
            this->removeChild(en);
            
            reset_user();
            
            return;
        }
        
        
    }
    
    
    
    // move user
#if USE_JOYSTICK_MODE
    auto spr = (Sprite*)this->getChildByTag(TAG_USER);

    float width = spr->getContentSize().width / 2;
    float height = spr->getContentSize().height / 2;
    
    float vx = spr->getPosition().x + _joystick->getVelocity().x * 10;
    float vy = spr->getPosition().y + _joystick->getVelocity().y * 10;
    
    if ((vx < width) || (vx >(480 - width)))
    {
        vx = spr->getPosition().x;
    }
    
    if ((vy < height) || (vy >(320 - height)))
    {
        vy = spr->getPosition().y;
    }

    spr->setPosition(Point(vx, vy));
   
/*
    Vec2 pos = spr->getPosition();
    Vec2 location = touch->getLocation();
    float distance = sqrtf((location.x - pos.x) * (location.x - pos.x) + (location.y - pos.y) * (location.y - pos.y));
    auto action = MoveTo::create(distance / 500, location);
    action->setTag(TAG_USER_MOVE);
    spr->runAction(action);
  */
#endif
    
    
    
    
    
    delta = 0;
    //
    return;
}



void HelloWorld::attackEnemy_2(Vec2 pos) {
    
    _sound_base.play_effect("enemy_shoot.wav");
    
    for ( int idx = 0; idx < 5; idx++) {
        auto sp = Sprite::create("fire_1.png");
        sp->setPosition(pos);
        sp->setRotation(-90);
        _attacks.pushBack(sp);
        this->addChild(sp);
        sp->runAction(Sequence::create(
                                       MoveBy::create(4.0f, Vec2(_wsize.width,   -300 + idx * 100)),
                                       CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetAttack, this)),
                                       NULL));
        
        
    }
    
}

void HelloWorld::attackEnemy_3(Vec2 pos) {
    
    _sound_base.play_effect("enemy_shoot.wav");
    
    
    // 3 shot
    for ( int idx = 0; idx < 3; idx++) {
        auto sp = Sprite::create("fire_1.png");
        sp->setRotation(-90);
        sp->setPosition(pos + Vec2(-30 * idx, 0));
        this->addChild(sp);
        
        _attacks.pushBack(sp);
        
        sp->runAction(Sequence::create(
                                       DelayTime::create(0.1 * idx),
                                       MoveBy::create(1.5f, Vec2(_wsize.width, 0)),
                                       CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetAttack, this)),
                                       NULL));
        
        
    }
    
}



void HelloWorld::init_sound() {

    _sound_base.play();
    
    return;
}



bool HelloWorld::onTouchBegan(Touch* touch, Event* unused_event) {
    
    
    if (_gameOver) {
        resetGameover();
        _joystick->setVisible(true);
        return true;
        
    } else if (_user_die) {
        
        return true;
    }  else if (_titleOn) {
        setTitle();
        _joystick->setVisible(true);
        return true;
    }
    
    


    
#if USE_JOYSTICK_MODE

#else
    auto spr = (Sprite*)this->getChildByTag(TAG_USER);
    if (spr->getActionByTag(TAG_USER_MOVE)) {
        spr->stopActionByTag(TAG_USER_MOVE);
    }
    
    Vec2 pos = spr->getPosition();
    Vec2 location = touch->getLocation();
    float distance = sqrtf((location.x - pos.x) * (location.x - pos.x) + (location.y - pos.y) * (location.y - pos.y));
    auto action = MoveTo::create(distance / 500, location);
    action->setTag(TAG_USER_MOVE);
    spr->runAction(action);
#endif
    
    
    return true;
}

void HelloWorld::init_bg() {
    auto node = ParallaxNode::create();
    this->addChild(node);
    
    node->runAction(RepeatForever::create(
                                          
                                          // create Sequence
                                          Sequence::create(
                                                           // ?? MoveTo time, position
                               
                                                           MoveBy::create(5.0f, Vec2(660, 0)),
                                                           
                                                           //  set offset (start pos)
                                                           //Place::create(Vec2::ZERO),
                                                           Place::create(Vec2(0, 0)),
                                                           
                                                           
                                                           NULL)
                                          )
                    );
    
    
    
   
    
    for (int idx = -2; idx < 3; idx++) {
        auto spr = Sprite::create("BG/bg_back.png");
        spr->setRotation(-90);
        
        spr->setScale(0.5, 1);
        spr->setAnchorPoint(Vec2::ZERO);
        node->addChild(spr, 0,
                       
                       // parallaxratio  x, y move speed    zero is stop ;;
                       Vec2(1, 1),
                       
                       Vec2(-330 * idx, 0));
    }
    
#if 1
    for (int idx = -3; idx < 4; idx++) {
        auto spr = Sprite::create("BG/bg_middle.png");
        spr->setRotation(-90);

        spr->setScale(0.5, 1);
        spr->setAnchorPoint(Vec2::ZERO);
        node->addChild(spr, 1,
                       
                       // parallaxratio  x, y move speed    zero is stop ;;
                       Vec2(2, 0),
                       
                       Vec2(-330 * idx, 0));
    }
        
    for (int idx = -3; idx < 8; idx++) {
        auto spr = Sprite::create("BG/bg_front.png");
        spr->setRotation(-90);
        
        spr->setScale(0.7, 1);
        spr->setAnchorPoint(Vec2::ZERO);
        node->addChild(spr, 2,
                       
                       // parallaxratio  x, y move speed    zero is stop ;;
                       Vec2(4, 0),
                       
                       Vec2(-330 * idx, 0));
    }
        
#endif
    
    return;
    
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
