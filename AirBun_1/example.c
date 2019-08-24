//----------------------------------GameScene.h------------------------------//

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

 

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

 

USING_NS_CC;

using namespace CocosDenshion;

 

#define TAG_LABEL_TITLE               0
#define TAG_SPRITE_PLAYER             1
#define TAG_ACTION_PLAYER_MOVE        2
#define TAG_LABEL_GAMEOVER_1          3
#define TAG_LABEL_GAMEOVER_2          4

 

class SpriteEnemy : public Sprite

{

public:

 CREATE_FUNC(SpriteEnemy);

 

 int type;
 int hp;
 bool isAttack;

};

 

class GameScene : public cocos2d::Layer

{

public:

 static cocos2d::Scene* createScene();
 virtual bool init();
 CREATE_FUNC(GameScene);


 Size winSize;
 bool isTitle, isPlayerDie, isGameover;

 Vector<Sprite*> missiles, attacks;

 Vector<SpriteEnemy*> enemies;

 void update(float delta);


 void initGameData();

 void initBG();

 

 void initTitle();
 void setTitle();

 

 void initPlayer();
 void resetPlayer();

 void setMissile(float delta);
 void resetMissile(Ref* sender);


 void setEnemy(float delta);

 void resetEnemy(Ref* sender);

 void attackEnemy_2(Vec2 pos);

 void attackEnemy_3(Vec2 pos);

 void resetAttack(Ref* sender);

 

 void initGameover();

 void setGameover();

 void resetGameover();

 

 bool onTouchBegan(Touch* touch, Event* unused_event);

};

 

#endif

 

 

 

//----------------------------------GameScene.cpp----------------------------//

 

#include "GameScene.h"

 

Scene* GameScene::createScene()

{

 auto scene = Scene::create();

 

 auto layer = GameScene::create();

 scene->addChild(layer);

 

 return scene;

}

 

bool GameScene::init()

{

 if (!Layer::init())

 {

  return false;

 }

 

 SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/menu_bg.mp3");

 

 auto listener = EventListenerTouchOneByOne::create();

 listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);

 Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

 

 winSize = Director::getInstance()->getWinSize();

 srand((int)time(NULL));

 

 initGameData();

 

 initBG();

 initPlayer();

 

 initTitle();

 

 initGameover();

 

 return true;

}

 

void GameScene::update(float delta)

{

 if (isPlayerDie) return;

 

 auto sprPlayer = (Sprite*)this->getChildByTag(TAG_SPRITE_PLAYER);

 Rect rectPlayer = Rect(sprPlayer->getPositionX() - 10, sprPlayer->getPositionY() - 20, 20, 40);

 

 for (SpriteEnemy* enemy : enemies) {

 

  if (enemy->type == 2) {

   if (!enemy->isAttack && enemy->getPositionY() < winSize.height * 3 / 4) {

    enemy->isAttack = true;

    attackEnemy_2(enemy->getPosition());

   }

  }

  else if (enemy->type == 3) {

   if (!enemy->isAttack

    && sprPlayer->getPositionX()>enemy->getPositionX() - 20

    && sprPlayer->getPositionX()<enemy->getPositionX() + 20) {

    enemy->isAttack = true;

    attackEnemy_3(enemy->getPosition());

   }

  }

 }

 

 bool isCrash = false;

 

 for (Sprite* missile : missiles) {

  Rect rectMissile = missile->getBoundingBox();

 

  for (SpriteEnemy* enemy : enemies) {

   Rect rectEnemy = enemy->getBoundingBox();

 

   if (rectMissile.intersectsRect(rectEnemy)) {

 

    SimpleAudioEngine::getInstance()->playEffect("Sounds/boom.wav");

 

    enemy->hp--;

 

    if (enemy->hp <= 0) {

     SimpleAudioEngine::getInstance()->playEffect("Sounds/explosion.wav");

 

     auto particle = ParticleSystemQuad::create("explosion.plist");

     particle->setPosition(enemy->getPosition());

     this->addChild(particle);

 

     particle->runAction(Sequence::create(

      DelayTime::create(2.0f),

      RemoveSelf::create(),

      NULL));

 

     enemies.eraseObject(enemy);

     this->removeChild(enemy);

    }

 

    missiles.eraseObject(missile);

    this->removeChild(missile);

 

    isCrash = true;

    break;

   }

  }

 

  if (isCrash) {

   isCrash = false;

   break;

  }

 }

 

 for (Sprite* attack : attacks) {

  Rect rectAttack = attack->getBoundingBox();

 

  if (rectPlayer.intersectsRect(rectAttack)) {

 

   attacks.eraseObject(attack);

   this->removeChild(attack);

 

   resetPlayer();

   return;

  }

 }

 

 for (Sprite* enemy : enemies) {

  Rect rectEnemy = enemy->getBoundingBox();

 

  if (rectPlayer.intersectsRect(rectEnemy)) {

 

   SimpleAudioEngine::getInstance()->playEffect("Sounds/explosion.wav");

 

   auto particle = ParticleSystemQuad::create("explosion.plist");

   particle->setPosition(enemy->getPosition());

   this->addChild(particle);

 

   particle->runAction(Sequence::create(

    DelayTime::create(2.0f),

    RemoveSelf::create(),

    NULL));

 

   attacks.eraseObject(enemy);

   this->removeChild(enemy);

 

   resetPlayer();

   return;

  }

 }

}

 

void GameScene::initGameData()

{

 isTitle = true;

 isPlayerDie = false;

 isGameover = false;

 

 missiles.clear();

 enemies.clear();

 attacks.clear();

}

 

void GameScene::initBG()

{

 auto node = ParallaxNode::create();

 this->addChild(node);

 

 node->runAction(RepeatForever::create(Sequence::create(

  MoveTo::create(20.0f, Vec2(0, -720)),

  Place::create(Vec2::ZERO),

  NULL)));

 

 for (int i = 0; i < 2; i++) {

  auto spr = Sprite::create("BG/bg_back.png");

  spr->setAnchorPoint(Vec2::ZERO);

  node->addChild(spr, 0, Vec2(0, 1), Vec2(0, 720 * i));

 }

 

 for (int i = 0; i < 3; i++) {

  auto spr = Sprite::create("BG/bg_middle.png");

  spr->setAnchorPoint(Vec2::ZERO);

  node->addChild(spr, 1, Vec2(0, 2), Vec2(0, 720 * i));

 }

 

 for (int i = 0; i < 5; i++) {

  auto spr = Sprite::create("BG/bg_front.png");

  spr->setAnchorPoint(Vec2::ZERO);

  node->addChild(spr, 2, Vec2(0, 4), Vec2(0, 720 * i));

 }

}

 

void GameScene::initTitle()

{

 auto label = Label::createWithTTF("Fligth360", "Fonts/Schwarzwald Regular.ttf", 140);

 label->setTextColor(Color4B::BLUE);

 label->enableOutline(Color4B::WHITE, 3);

 label->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

 label->setTag(TAG_LABEL_TITLE);

 this->addChild(label);

}

 

void GameScene::setTitle()

{

 SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/bgm.mp3");

 

 isTitle = false;

 

 auto label = (Label*)this->getChildByTag(TAG_LABEL_TITLE);

 label->runAction(FadeOut::create(1.0f));

 

 auto spr = (Sprite*)this->getChildByTag(TAG_SPRITE_PLAYER);

 spr->setPosition(Vec2(winSize.width / 2, 200 - 250));

 spr->setVisible(true);

 spr->runAction(MoveBy::create(1.0f, Vec2(0, 250)));

 

 this->scheduleUpdate();

 this->schedule(schedule_selector(GameScene::setMissile), 0.15f);

 this->schedule(schedule_selector(GameScene::setEnemy), 1.0f);

}

 

void GameScene::initPlayer()

{

 auto spr = Sprite::create();

 spr->setPosition(Vec2(winSize.width / 2, 200 - 250));

 spr->setTag(TAG_SPRITE_PLAYER);

 this->addChild(spr);

 

 auto texture = Director::getInstance()->getTextureCache()->addImage("Sprites/Spaceship.png");

 

 auto animation = Animation::create();

 animation->setDelayPerUnit(0.1f);

 for (int i = 0; i < 4; i++) {

  animation->addSpriteFrameWithTexture(texture, Rect(i * 96, 0, 96, 104));

 }

 

 auto animate = Animate::create(animation);

 spr->runAction(RepeatForever::create(animate));

}

 

void GameScene::resetPlayer()

{

 SimpleAudioEngine::getInstance()->playEffect("Sounds/explosion.wav");

 

 isPlayerDie = true;

 

 auto sprPlayer = (Sprite*)this->getChildByTag(TAG_SPRITE_PLAYER);

 sprPlayer->setVisible(false);

 

 auto spr = Sprite::create();

 spr->setPosition(sprPlayer->getPosition());

 this->addChild(spr);

 

 auto animation = Animation::create();

 animation->setDelayPerUnit(0.05f);

 auto texture = Director::getInstance()->getTextureCache()->addImage("Sprites/Explosion.png");

 for (int i = 0; i < 11; i++) {

  animation->addSpriteFrameWithTexture(texture, Rect(i % 5 * 160, i / 5 * 160, 160, 160));

 }

 

 auto animate = Animate::create(animation);

 spr->runAction(animate);

 

 spr->runAction(Sequence::create(

  DelayTime::create(0.55f),

  RemoveSelf::create(),

  CallFunc::create(CC_CALLBACK_0(GameScene::setGameover, this)),

  NULL));

 

 this->unschedule(schedule_selector(GameScene::setMissile));

}

 

void GameScene::setMissile(float delta)

{

 SimpleAudioEngine::getInstance()->playEffect("Sounds/shoot.wav");

 

 auto sprPlayer = (Sprite*)this->getChildByTag(TAG_SPRITE_PLAYER);

 

 auto sprMissile = Sprite::create("Sprites/fire_0.png");

 sprMissile->setPosition(sprPlayer->getPosition() + Vec2(0, 20));

 this->addChild(sprMissile);

 

 missiles.pushBack(sprMissile);

 

 sprMissile->runAction(Sequence::create(

  MoveBy::create(2.0f, Vec2(0, winSize.height)),

  CallFuncN::create(CC_CALLBACK_1(GameScene::resetMissile, this)),

  NULL));

}

 

void GameScene::resetMissile(Ref* sender)

{

 auto spr = (Sprite*)sender;

 

 missiles.eraseObject(spr);

 this->removeChild(spr);

}

 

void GameScene::setEnemy(float delta)

{

 float x = 50 + rand() % (1280 - 50 * 2);

 

 auto spr = SpriteEnemy::create();

 spr->setPosition(Vec2(x, winSize.height + 100));

 spr->isAttack = false;

 this->addChild(spr);

 

 int random = rand() % 100;

 float speed;

 

 if (random < 60) {

  spr->type = 1;

  spr->hp = 5;

  speed = 10.0f;

 }

 else if (random < 90) {

  spr->type = 2;

  spr->hp = 10;

  spr->setScaleX(1.6f);

  speed = 15.0f;

 }

 else {

  spr->type = 3;

  spr->hp = 3;

  spr->setScaleX(0.7f);

  spr->setScaleY(1.4f);

  speed = 20.0f;

 }

 

 enemies.pushBack(spr);

 

 auto animation = Animation::create();

 animation->setDelayPerUnit(0.1f);

 auto texture = Director::getInstance()->getTextureCache()->addImage("Sprites/enemy.png");

 for (int i = 0; i < 4; i++) {

  animation->addSpriteFrameWithTexture(texture, Rect(i * 96, 0, 50, 72));

 }

 

 auto animate = Animate::create(animation);

 spr->runAction(RepeatForever::create(animate));

 

 spr->runAction(Sequence::create(

  MoveBy::create(speed, Vec2(0, -winSize.height - 200)),

  CallFuncN::create(CC_CALLBACK_1(GameScene::resetEnemy, this)),

  NULL));

}

 

void GameScene::resetEnemy(Ref* sender)

{

 auto spr = (SpriteEnemy*)sender;

 enemies.eraseObject(spr);

 this->removeChild(spr);

}

 

void GameScene::attackEnemy_2(Vec2 pos)

{

 SimpleAudioEngine::getInstance()->playEffect("Sounds/enemy_shoot.wav");

 

 for (int i = 0; i < 5; i++) {

  auto spr = Sprite::create("Sprites/fire_1.png");

  spr->setPosition(pos);

  this->addChild(spr);

 

  attacks.pushBack(spr);

 

  spr->runAction(Sequence::create(

   MoveBy::create(4.0f, Vec2(-400 + i * 200, -winSize.height)),

   CallFuncN::create(CC_CALLBACK_1(GameScene::resetAttack, this)),

   NULL));

 }

}

 

void GameScene::attackEnemy_3(Vec2 pos)
{
 SimpleAudioEngine::getInstance()->playEffect("Sounds/enemy_shoot.wav");
 
 for (int i = 0; i < 3; i++) {
  auto spr = Sprite::create("Sprites/fire_1.png");
  spr->setPosition(pos + Vec2(0, -30 * i));
  this->addChild(spr);
  attacks.pushBack(spr);

  spr->runAction(Sequence::create(
   DelayTime::create(0.1*i),
   MoveBy::create(1.0f, Vec2(0, -winSize.height)),
   CallFuncN::create(CC_CALLBACK_1(GameScene::resetAttack, this)),
   NULL));
 }
}

 

void GameScene::resetAttack(Ref* sender)

{

 auto spr = (Sprite*)sender;

 attacks.eraseObject(spr);

 this->removeChild(spr);

}

 

void GameScene::initGameover()

{

 auto label_1 = Label::createWithTTF("GAME OVER", "Fonts/SAM_5C_27TRG_.TTF", 120);

 label_1->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

 label_1->setTextColor(Color4B::RED);

 label_1->setScale(0.0f);

 label_1->setVisible(false);

 label_1->setTag(TAG_LABEL_GAMEOVER_1);

 this->addChild(label_1, 1);

 

 auto label_2 = Label::createWithTTF("PRESS TO START", "Fonts/SAM_5C_27TRG_.TTF", 40);

 label_2->setPosition(Vec2(winSize.width / 2, 100));

 label_2->setScale(0.0f);

 label_2->setVisible(false);

 label_2->setTag(TAG_LABEL_GAMEOVER_2);

 this->addChild(label_2, 1);

}

 

void GameScene::setGameover()
{
 isGameover = true;

 auto label_1 = (Label*)this->getChildByTag(TAG_LABEL_GAMEOVER_1);
 label_1->runAction(Sequence::create(
  Show::create(),
  Spawn::create(
  ScaleTo::create(1.0f, 1.0f),
  RotateBy::create(1.0f, 360 * 5),
  NULL),
  NULL));


 auto label_2 = (Label*)this->getChildByTag(TAG_LABEL_GAMEOVER_2);
 label_2->runAction(Sequence::create(
  Show::create(),
  DelayTime::create(1.2f),
  ScaleTo::create(0.4f, 1.0f),
  NULL));
}

 

void GameScene::resetGameover()
{
 auto label_1 = (Label*)this->getChildByTag(TAG_LABEL_GAMEOVER_1);
 label_1->stopAllActions();
 label_1->setScale(0.0f);
 label_1->setRotation(0);
 label_1->setVisible(false);

 

 auto label_2 = (Label*)this->getChildByTag(TAG_LABEL_GAMEOVER_2);
 label_2->stopAllActions();
 label_2->setScale(0.0f);
 label_2->setVisible(false);

 

 for (SpriteEnemy* enemy : enemies) {
  this->removeChild(enemy);
 }

 

 for (Sprite* attack : attacks) {
  this->removeChild(attack);
 }

 for (Sprite* missile : missiles) {
  this->removeChild(missile);
 }
 this->unscheduleAllSelectors();
 initGameData();
 setTitle();
}

 

bool GameScene::onTouchBegan(Touch* touch, Event* unused_event)
{

 if (isGameover) {
  resetGameover();
  return true;
 }

 else if (isPlayerDie) {
  return true;
 }

 else if (isTitle) {
  setTitle();
  return true;
 }

 auto spr = (Sprite*)this->getChildByTag(TAG_SPRITE_PLAYER);

 if (spr->getActionByTag(TAG_ACTION_PLAYER_MOVE)) {
  spr->stopActionByTag(TAG_ACTION_PLAYER_MOVE);
 }

 Vec2 pos = spr->getPosition();
 Vec2 location = touch->getLocation();

 float distance = sqrtf((location.x - pos.x)*(location.x - pos.x) + (location.y - pos.y)*(location.y - pos.y));
 auto action = MoveTo::create(distance / 500, location);
 action->setTag(TAG_ACTION_PLAYER_MOVE);
 spr->runAction(action);
 return true;

}


