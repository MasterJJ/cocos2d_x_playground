#include "GameScene.h"

USING_NS_CC;


Scene* GameScene::createScene()
{
	return GameScene::create();
}

GameScene::GameScene()
{
}


GameScene::~GameScene()
{
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	initBackGroundMap();
	initValue();

	if (Director::getInstance()->isPaused() == false)
	{
		initScore();
		//initMap();
		initPlayer();
		initTouch();
		initSchedule();
		initGameOverMenu();
	}

	if (Director::getInstance()->isPaused() == true)
	{
		Director::getInstance()->resume();

		((Menu*)getChildByTag(TAG_GAME_MN_GAME_OVER))->setVisible(false);
	}

	return true;
}


void GameScene::initValue() {

	_FrameSize = Director::getInstance()->getWinSize();
	_Itemcount = 0;


	if (Director::getInstance()->isPaused() == false)
	{

		_game_Items.clear();
		_player_misslie.clear();
		_enemyes.clear();
		_enemy_missiles.clear();

		_hi_score = UserDefault::getInstance()->getIntegerForKey("HIGH_SCORE");
	}
	else
	{
		((Sprite*)getChildByTag(_PlayerHP))->setTag(TAG_GAME_SP_PLAYER_FULL);
		_PlayerHP = TAG_GAME_SP_PLAYER_FULL;
	}

	_player_live = true;

	_score = 0;

	return;
}

void GameScene::initPlayer() {
	auto spr = Sprite::create("Player.png");
	_PlayerHP = TAG_GAME_SP_PLAYER_FULL;
	spr->setScale(0.3f);
	Point pos = Point(_FrameSize.width / 2, spr->getBoundingBox().size.height);
	spr->setPosition(pos);
	spr->setTag(_PlayerHP);

	addChild(spr, SEQUENCE_RENDER_PLAYER);
	return;
}


void GameScene::initTouch()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
}

bool GameScene::onTouchBegan(Touch* touch, Event* unusedEvent)
{
	if (_player_live == true)
	{
		auto player = (Sprite*)getChildByTag(_PlayerHP);

		_StartTouch = touch->getLocation();
		_StartPlayer= player->getPosition();
	}
	return true;
}


void GameScene::onTouchMoved(Touch* touch, Event* unusedEvent)
{
	if (_player_live == true)
	{
		auto player = (Sprite*)getChildByTag(_PlayerHP);

		Point location = touch->getLocation();
		Point changed = location - _StartTouch;

		Point move = _StartPlayer + changed;

		if (move.x <= player->getBoundingBox().size.width / 2)
		{
			move.x = player->getBoundingBox().size.width / 2;
		}
		else if (move.x >= _FrameSize.width - (player->getBoundingBox().size.width / 2))
		{
			move.x = _FrameSize.width - (player->getBoundingBox().size.width / 2);
		}

		if (move.y <= player->getBoundingBox().size.height / 2)
		{
			move.y = player->getBoundingBox().size.height / 2;
		}
		else if (move.y >= _FrameSize.height - (player->getBoundingBox().size.height / 2))
		{
			move.y = _FrameSize.height - (player->getBoundingBox().size.height / 2);
		}

		player->setPosition(move);
	}
}

void GameScene::initSchedule()
{
	schedule(schedule_selector(GameScene::setPlayerMissile), 0.08);
	schedule(schedule_selector(GameScene::setItem), 5);
	schedule(schedule_selector(GameScene::setEnemy), 0.3);

	scheduleUpdate();
}

void GameScene::resetPlayerMissile(Ref* sender)
{
	auto ball = (Sprite*)sender;
	_player_misslie.eraseObject(ball);
	removeChild(ball);
}



void GameScene::addScore(int nAdd)
{
	_score += nAdd;

	if (_score > _hi_score)
	{
		_hi_score = _score;
		UserDefault::getInstance()->setIntegerForKey("HIGH_SCORE", _hi_score);
		UserDefault::getInstance()->flush();
	}

	auto score = (Label*)getChildByTag(TAG_GAME_LB_SCORE);
	score->setPosition(Point(_FrameSize.width / 2, _FrameSize.height - 30));
	score->setString(StringUtils::format("SCORE:%d", _score));

	auto high = (Label*)getChildByTag(TAG_GAME_LB_SCORE_HIGHSCORE);
	high->setString(StringUtils::format("HIGH SCORE:%d", _hi_score));
}

void GameScene::updateLabelHP(void)
{
	auto high = (Label*)getChildByTag(TAG_GAME_LB_PLAYER_LIFE);
	high->setString(StringUtils::format("Player Life : %d", _PlayerHP - TAG_GAME_SP_PLAYER_DIE));
}

void GameScene::initGameOverMenu(void)
{

	Point pt = getAnchorPoint();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GameScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	//replay
	auto item = MenuItemFont::create("RESTART", CC_CALLBACK_1(GameScene::menuRePlayCallback, this));
	item->setScale(2.0f);

	auto rePlay = Menu::create(item, NULL);
	rePlay->alignItemsHorizontally();
	rePlay->alignItemsVertically();
	rePlay->setVisible(false);
	rePlay->setTag(TAG_GAME_MN_GAME_OVER);
	addChild(rePlay, SEQUENCE_RENDER_SCORE);

}

void GameScene::menuRePlayCallback(cocos2d::Ref* sender)
{
	auto item = (MenuItemFont*)sender;

	init();
}

void GameScene::initScore()
{
	auto label = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);

	label->setPosition(Point(_FrameSize.width / 2, _FrameSize.height - 30));
	label->setTag(TAG_GAME_LB_SCORE);

	this->addChild(label, SEQUENCE_RENDER_SCORE);


	//hight Label
	auto label_high = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
	label_high->setPosition(Point(_FrameSize.width, _FrameSize.height - 30));
	label_high->setAnchorPoint(Point(1, 1));
	label_high->setTag(TAG_GAME_LB_SCORE_HIGHSCORE);

	this->addChild(label_high, SEQUENCE_RENDER_SCORE);

	//init socre
	addScore(0);

	//Player Life
	auto life = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
	life->setPosition(Point(0, _FrameSize.height - 30));
	life->setAnchorPoint(Point(0, 1));
	life->setTag(TAG_GAME_LB_PLAYER_LIFE);

	this->addChild(life, SEQUENCE_RENDER_SCORE);
	updateLabelHP();

}

void GameScene::resetEnemyMissile(Ref* sender)
{
	_enemy_missiles.eraseObject((Sprite*)sender);
	removeChild((Sprite*)sender);
}

void GameScene::checkEnemyMissilePos(Ref* sender)
{
	Sprite* enemyMissile = (Sprite*)sender;

	if (enemyMissile->getPosition().y <= -(enemyMissile->getContentSize().height))
	{
		resetEnemyMissile(enemyMissile);
	}
}


void GameScene::setBoomEffect(Point ptPos, float fScale)
{
	/*
	auto boom = ParticleSystemQuad::create("explosion.plist");
	boom->setPosition(ptPos);
	boom->setScale(fScale);
	boom->setRotation(rand() % 360);
	addChild(boom);

	auto act0 = DelayTime::create(1.0f);
	auto act1 = CallFuncN::create(CC_CALLBACK_1(GameScene::resetBoomEffect, this));
	auto act2 = Sequence::create(act0, act1, NULL);

	boom->runAction(act2);
	*/
}


void GameScene::resetBoomEffect(Ref* sender)
{
	auto* effect = (ParticleSystemQuad*)sender;

	removeChild(effect);
}

void GameScene::checkPlayer()
{
	auto removeEnemyMissile = Sprite::create();
	auto player = getChildByTag(_PlayerHP);
	Point ptPlayer = player->getPosition();
	float fPlayerRadius = player->getBoundingBox().size.height / 2;

	Point ptMissile;
	float fMissileRadius;

	for (Sprite* missile : _enemy_missiles)
	{
		ptMissile = missile->getPosition();
		fMissileRadius = missile->getBoundingBox().size.width;

		if (checkCollision(ptMissile, fMissileRadius, ptPlayer, fPlayerRadius) == true)
		{
			setBoomEffect(ptMissile, 0.3f);
			removeEnemyMissile = missile;
			_PlayerHP -= ENEMY_MISSILE_DAMAGE;
			player->setTag(_PlayerHP);
			updateLabelHP();
			if (player->getTag() == TAG_GAME_::TAG_GAME_SP_PLAYER_DIE)
			{
				_player_live = false;
				Director::getInstance()->pause();
				((Menu*)getChildByTag(TAG_GAME_MN_GAME_OVER))->setVisible(true);

			}
		}
	}

	if (_enemy_missiles.contains(removeEnemyMissile))
	{
		resetEnemyMissile(removeEnemyMissile);
	}

}

void GameScene::checkEnemies()
{
	auto removeEnemy = Sprite::create();
	auto removeMissile = Sprite::create();

	Point ptEnemy, ptMissile;
	float fEnemyRadius, fMissileRadius;
	int HP = 0;

	for (Sprite *missile : _player_misslie)
	{
		ptMissile = missile->getPosition();
		fMissileRadius = missile->getBoundingBox().size.height / 2;

		for (Sprite *enemy : _enemyes)
		{
			ptEnemy = enemy->getPosition();
			fEnemyRadius = enemy->getBoundingBox().size.height / 2;

			if (checkCollision(ptEnemy, fEnemyRadius, ptMissile, fMissileRadius) == true)
			{
				removeMissile = missile;
				addScore(1);
				HP = enemy->getTag();
				HP -= PLYAER_MISSILE_DAMAGE;
				enemy->setTag(HP);
				int HalfEnemyHp = (TAG_GAME_SP_ENEMY_FULL - TAG_GAME_SP_ENEMY_DIY) >> 1;
				int enemyState = enemy->getTag() - TAG_GAME_SP_ENEMY_DIY;
				if (enemyState < HalfEnemyHp)
					setBoomEffect(missile->getPosition(), 0.3f);

				if (enemy->getTag() == TAG_GAME_SP_ENEMY_DIY)
				{
					removeEnemy = enemy;
					setBoomEffect(enemy->getPosition());
					addScore(10);
				}

			}
		}
	}

	if (_player_misslie.contains(removeMissile))
	{
		_player_misslie.eraseObject(removeMissile);
		resetPlayerMissile(removeMissile);
	}

	if (_enemyes.contains(removeEnemy))
	{
		_enemyes.eraseObject(removeEnemy);
		resetEnemy(removeEnemy);
	}

}

void GameScene::resetEnemy(Ref* sender)
{
	_enemyes.eraseObject((Sprite*)sender);
	removeChild((Sprite*)sender);
}

void GameScene::FireEnemyMissile(Ref* sender)
{
	int Fir = rand() % 3;

	if (Fir == 0)
	{
		Sprite* enemy = (Sprite*)sender;

		auto act0 = MoveBy::create(0.3f, Point(0, -200));
		auto act1 = CallFuncN::create(CC_CALLBACK_1(GameScene::checkEnemyMissilePos, this));
		auto act2 = Sequence::create(act0, act1, NULL);
		auto act3 = RepeatForever::create(act2);
		Sprite* missile = Sprite::create("EnemyBall.png");
		Point pt = enemy->getPosition();
		pt.y -= enemy->getBoundingBox().size.height;
		missile->setPosition(pt);

		missile->runAction(act3);
		addChild(missile);

		_enemy_missiles.pushBack(missile);
	}

}


void GameScene::checkEnemyPos(Ref* sender)
{
	Sprite* enemy = (Sprite*)sender;

	if (enemy->getPosition().y <= -(enemy->getContentSize().height))
	{
		resetEnemy(enemy);
	}
}

void GameScene::setEnemy(float delta)
{
	Sprite *pSprite = Sprite::create("Enemy1.png");

	Point pos;

	pos.x = pSprite->getBoundingBox().size.width / 2;
	pos.y = _FrameSize.height;

	pos.x = pos.x + rand() % (int)(_FrameSize.width - pos.x * 2);
	pSprite->setPosition(pos);
	//-(m_siFrame.height+pSprite->getContentSize().height

	auto act0 = MoveBy::create(0.3f, Point(0, -100));
	auto act1 = CallFuncN::create(CC_CALLBACK_1(GameScene::checkEnemyPos, this));
	auto act1_1 = CallFuncN::create(CC_CALLBACK_1(GameScene::FireEnemyMissile, this));
	auto act2 = Sequence::create(act0, act1, act1_1, NULL);
	auto act3 = RepeatForever::create(act2);

	pSprite->runAction(act3);
	pSprite->setTag(TAG_GAME_SP_ENEMY_FULL);

	_enemyes.pushBack(pSprite);
	addChild(pSprite, SEQUENCE_RENDER_ENEMY);
}


void GameScene::checkItem()
{
	auto remove = Sprite::create();

	auto player = (Sprite*)getChildByTag(_PlayerHP);

	Rect rcPlayer = player->getBoundingBox();
	Rect rcMissile;

	Point ptPlayer = player->getPosition();
	float fPlayerRadius = rcPlayer.size.height / 2;

	Point ptItem;
	float fItemRadius;

	for (Sprite * item : _game_Items)
	{
		rcMissile = item->getBoundingBox();
		ptItem = item->getPosition();
		fItemRadius = rcMissile.size.height / 2;

		if (checkCollision(ptPlayer, fPlayerRadius, ptItem, fItemRadius) == true)
		{
			_PlayerHP = TAG_GAME_SP_PLAYER_FULL;
			player->setTag(_PlayerHP);

			remove = item;
			updateLabelHP();
			break;
		}
	}

	if (_game_Items.contains(remove))
	{
		++_Itemcount;
		_game_Items.eraseObject(remove);
		removeChild(remove);
	}

	//rcPlayer.intersectsCircle()

}

void GameScene::update(float delta)
{
	checkItem();
	checkEnemies();
	checkPlayer();
}

void GameScene::resetItem(Ref* sender)
{
	Sprite* item = (Sprite*)sender;

	_game_Items.eraseObject(item);
	removeChild(item);
}


void GameScene::setItem(float delta)
{
	auto item = Sprite::create("item.png");
	item->setScale(2.0f);

	Point pos;

	pos.x = item->getBoundingBox().size.width / 2;
	pos.y = _FrameSize.height;

	pos.x = pos.x + rand() % (int)(_FrameSize.width - pos.x * 2);
	item->setPosition(pos);

	auto act0 = MoveBy::create(3.0f, Point(0, -_FrameSize.height));
	auto act1 = CallFuncN::create(CC_CALLBACK_1(GameScene::resetItem, this));
	auto act2 = Sequence::create(act0, act1, NULL);

	item->runAction(act2);

	_game_Items.pushBack(item);
	addChild(item, SEQUENCE_RENDER_ITEM);
}
void GameScene::setPlayerMissile(float delta)
{
	if (_Itemcount >= 0 && _Itemcount < 5)
	{
		auto player = (Sprite*)getChildByTag(_PlayerHP);
		auto ball = Sprite::create("PlayerBall.png");
		Point ptP = player->getPosition();
		ball->setPosition(Point(ptP.x, ptP.y + ball->getBoundingBox().size.height + player->getBoundingBox().size.height / 2));

		auto act0 = MoveBy::create(1.0, Point(0, _FrameSize.height));
		auto act1 = CallFuncN::create(CC_CALLBACK_1(GameScene::resetPlayerMissile, this));
		auto act2 = Sequence::create(act0, act1, NULL);

		ball->runAction(act2);

		addChild(ball, SEQUENCE_RENDER_PLAYER_MISSILE);
		_player_misslie.pushBack(ball);
	}
	else if (_Itemcount >= 5 && _Itemcount < 10)
	{
		auto player = (Sprite*)getChildByTag(_PlayerHP);
		auto ball0 = Sprite::create("PlayerBall.png");
		Point ptP = player->getPosition();
		ball0->setPosition(Point(ptP.x - ball0->getBoundingBox().size.width, ptP.y + ball0->getBoundingBox().size.height + player->getBoundingBox().size.height / 2));

		auto act0 = MoveBy::create(1.0, Point(0, _FrameSize.height));
		auto act1 = CallFuncN::create(CC_CALLBACK_1(GameScene::resetPlayerMissile, this));
		auto act2 = Sequence::create(act0, act1, NULL);

		ball0->runAction(act2);

		addChild(ball0, SEQUENCE_RENDER_PLAYER_MISSILE);

		auto ball1 = Sprite::create("PlayerBall.png");
		//Point ptP = player->getPosition();
		ball1->setPosition(Point(ptP.x + ball0->getBoundingBox().size.width, ptP.y + ball1->getBoundingBox().size.height + player->getBoundingBox().size.height / 2));

		auto act3 = MoveBy::create(1.0, Point(0, _FrameSize.height));
		auto act4 = CallFuncN::create(CC_CALLBACK_1(GameScene::resetPlayerMissile, this));
		auto act5 = Sequence::create(act3, act4, NULL);

		ball1->runAction(act5);

		addChild(ball1, SEQUENCE_RENDER_PLAYER_MISSILE);

		_player_misslie.pushBack(ball0);
		_player_misslie.pushBack(ball1);
	}
	else
	{
		auto player = (Sprite*)getChildByTag(_PlayerHP);
		auto ball0 = Sprite::create("PlayerBall.png");
		Point ptP = player->getPosition();
		ball0->setPosition(Point(ptP.x - ball0->getBoundingBox().size.width, ptP.y + ball0->getBoundingBox().size.height + player->getBoundingBox().size.height / 2));

		auto act0 = MoveBy::create(1.0, Point(0, _FrameSize.height));
		auto act1 = CallFuncN::create(CC_CALLBACK_1(GameScene::resetPlayerMissile, this));
		auto act2 = Sequence::create(act0, act1, NULL);

		ball0->runAction(act2);

		addChild(ball0, SEQUENCE_RENDER_PLAYER_MISSILE);

		auto ball1 = Sprite::create("PlayerBall.png");
		//Point ptP = player->getPosition();
		ball1->setPosition(Point(ptP.x + ball0->getBoundingBox().size.width, ptP.y + ball1->getBoundingBox().size.height + player->getBoundingBox().size.height / 2));

		auto act3 = MoveBy::create(1.0, Point(0, _FrameSize.height));
		auto act4 = CallFuncN::create(CC_CALLBACK_1(GameScene::resetPlayerMissile, this));
		auto act5 = Sequence::create(act3, act4, NULL);

		ball1->runAction(act5);

		addChild(ball1, SEQUENCE_RENDER_PLAYER_MISSILE);

		//
		auto ball2 = Sprite::create("PlayerBall.png");
		//Point ptP = player->getPosition();
		ball2->setPosition(Point(ptP.x + ball0->getBoundingBox().size.width * 2, ptP.y - ball2->getBoundingBox().size.height + player->getBoundingBox().size.height / 2));

		auto act6 = MoveBy::create(1.0, Point(90, _FrameSize.height));
		auto act7 = CallFuncN::create(CC_CALLBACK_1(GameScene::resetPlayerMissile, this));
		auto act8 = Sequence::create(act6, act7, NULL);

		ball2->runAction(act8);

		addChild(ball2, SEQUENCE_RENDER_PLAYER_MISSILE);

		//
		auto ball3 = Sprite::create("PlayerBall.png");
		//Point ptP = player->getPosition();
		ball3->setPosition(Point(ptP.x - ball0->getBoundingBox().size.width * 2, ptP.y - ball3->getBoundingBox().size.height + player->getBoundingBox().size.height / 2));

		auto act9 = MoveBy::create(1.0, Point(-90, _FrameSize.height));
		auto act10 = CallFuncN::create(CC_CALLBACK_1(GameScene::resetPlayerMissile, this));
		auto act11 = Sequence::create(act9, act10, NULL);

		ball3->runAction(act11);

		addChild(ball3, SEQUENCE_RENDER_PLAYER_MISSILE);

		_player_misslie.pushBack(ball0);
		_player_misslie.pushBack(ball1);
		_player_misslie.pushBack(ball2);
		_player_misslie.pushBack(ball3);
	}
}

void GameScene::initBackGroundMap() {
	//map
	{
		auto act0 = MoveBy::create(10, Point(0, -1600));
		auto act1 = Place::create(Point::ZERO);
		auto act2 = Sequence::create(act0, act1, NULL);
		auto act3 = RepeatForever::create(act2);

		auto spr0 = Sprite::create("air_map1.jpg");
		spr0->setPosition(Point::ZERO);
		spr0->runAction(act3);

		auto act4 = MoveBy::create(10, Point(0, -1600));
		auto act5 = Place::create(Point(0, 800));
		auto act6 = Sequence::create(act4, act5, NULL);
		auto act7 = RepeatForever::create(act6);

		auto spr1 = Sprite::create("air_map2.jpg");
		spr1->setPosition(Point(0, 800));
		spr1->runAction(act7);

		auto act8 = MoveBy::create(10, Point(0, -1600));
		auto act9 = Place::create(Point(0, 1600));
		auto act10 = Sequence::create(act8, act9, NULL);
		auto act11 = RepeatForever::create(act10);

		auto spr2 = Sprite::create("air_map1.jpg");
		spr2->setPosition(Point(0, 1600));
		spr2->runAction(act11);

		spr0->setAnchorPoint(Point(0, 0));
		spr1->setAnchorPoint(Point(0, 0));
		spr2->setAnchorPoint(Point(0, 0));

		addChild(spr0, SEQUENCE_RENDER_MAP);
		addChild(spr1, SEQUENCE_RENDER_MAP);
		addChild(spr2, SEQUENCE_RENDER_MAP);
	}

	return;
}

void GameScene::menuCloseCallback(Ref* pSender)
{

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

}