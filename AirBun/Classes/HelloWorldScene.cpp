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
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
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

	auto wlayer = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(wlayer);

	initBackGroundMap();

	auto pMenuItem1 = MenuItemImage::create(
		"Images/btn-play-normal.png",
		"Images/btn-play-selected.png",
		CC_CALLBACK_1(HelloWorld::doClick1, this));

	auto pMenuItem2 = MenuItemImage::create(
		"Images/btn-highscores-normal.png",
		"Images/btn-highscores-selected.png",
		CC_CALLBACK_1(HelloWorld::doClick2, this));

	auto pMenuItem3 = MenuItemImage::create(
		"Images/btn-about-normal.png",
		"Images/btn-about-selected.png",
		CC_CALLBACK_1(HelloWorld::doClick3, this));

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, pMenuItem3, nullptr);

	pMenu->alignItemsVertically();

	this->addChild(pMenu);

	

    return true;
}

void HelloWorld::doClick1(Ref* sender) {
	log("Select doClick1");

}
void HelloWorld::doClick2(Ref* sender) {
	log("Select doClick2");

}
void HelloWorld::doClick3(Ref* sender) {
	log("Select doClick3");

}

void HelloWorld::initBackGroundMap() {
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