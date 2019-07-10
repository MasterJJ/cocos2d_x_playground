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

	auto wlayer = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(wlayer);
	initBackGroundMap();

	return true;
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