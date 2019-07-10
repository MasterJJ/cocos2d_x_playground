#pragma once


#include "cocos2d.h"
#include "defined_game.h"

class GameScene : public cocos2d::Scene
{
public:
	GameScene();
	~GameScene();

	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	void initBackGroundMap();

};

