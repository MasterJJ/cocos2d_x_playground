#pragma once


#include "cocos2d.h"
#include "defined_game.h"
#include <vector>
USING_NS_CC;


class GameScene : public cocos2d::Scene
{
public:
	GameScene();
	~GameScene();

	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	/////////////
	/// in game var
	Point _StartTouch;
	Point _StartPlayer;
	Size  _FrameSize;
	int   _Itemcount;
	int   _PlayerHP;
	int   _score;
	int   _hi_score;
	bool  _player_live;

#define evec Vector<Sprite *> 

	evec _game_Items;
	evec _player_misslie;
	evec _enemyes;
	evec _enemy_missiles;


	void initValue();

	////////////
	// function
	void initSchedule();
	void initBackGroundMap();
	void initTouch();
	void initPlayer();
	void initGameOver();
	void initGameOverMenu();

	void initScore();
	void addScore(int nAdd);
	void updateLabelHP();

	/////////////
	// player
	void setPlayerMissile(float delta);
	void resetPlayerMissile(Ref* sender);

	void setItem(float delta);
	void resetItem(Ref* sender);
	void setEnemy(float delta);
	void checkEnemyPos(Ref* sender);
	void FireEnemyMissile(Ref* sender);
	void resetEnemy(Ref* sender);
	void checkEnemyMissilePos(Ref* sender);
	void resetEnemyMissile(Ref* sender);

	void checkItem();
	void checkEnemies();
	void checkPlayer();


	void resetBoomEffect(Ref* sender);
	void setBoomEffect(Point ptPos, float fScale = 1.0f);

	void menuRePlayCallback(cocos2d::Ref* pSender);


	virtual bool onTouchBegan(Touch* touch, Event* unusedEvent);
	virtual void onTouchMoved(Touch* touch, Event* unusedEvent);
	virtual void update(float delta);


	
	void menuCloseCallback(cocos2d::Ref* pSender);

	bool checkCollision(Point point1, float fRad1, Point point2, float fRad2)
	{
		float fTemp = sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));

		if (fTemp <= fRad1 + fRad2)
			return true;

		return false;
	}
};

