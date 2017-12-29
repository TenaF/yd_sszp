//
//  BaseScene.cpp
//  hello2dx
//
//  Created by  on 14-7-4.
//
//  统一处理一些基类事件:
//      @返回键

#include "sdk/scene/BaseScene.h"
#include "sdk/sdk.h"
#include "sdk/ui/PromptLayer.h"
#include <time.h>
#include "hall/ui/ExitLayer.h"
#include "hall/ui/LoadingScene.h"

USING_NS_CC;

#define TAG "BaseScene"

Scene* BaseScene::createScene(BaseScene* layer)
{
    auto scene = Scene::create();
    if (layer)
    {
		layer->initView();
        scene->addChild(layer);
        layer->addListener();
    }

    return scene;
}

BaseScene::BaseScene()
{
    Layer::init();
}

void BaseScene::onEnter()
{
    Layer::onEnter();

//  time_t cur_time = time(NULL);
//  NYInfo(TAG,"  curtime = %ld ,lasttime = %ld",cur_time,last_time);
//  if(cur_time > last_time && cur_time - last_time < 1000)		//间隔1秒
//	{
//		Director::getInstance()->end();
//	}else{
//		PromptLayer::createObject("再按一次将退出程序");
//		last_time = cur_time;
//	}
}

void BaseScene::cleanup()
{
    Layer::cleanup();
}

void BaseScene::onBackPressed()
{
	ExitLayer::createObject();
}

void BaseScene::setBg(const char* resPath)
{
    auto bg = Sprite::create(resPath);
    bg->setScale(VisibleRect::getScale());
    bg->setPosition(VisibleRect::center());
    this->addChild(bg);
}

void BaseScene::addListener()
{
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event){
	   NYInfo(TAG,"onKeyPressed keyCode=%d", keyCode);
	};
	listener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event){
	   NYInfo(TAG,"onKeyReleased keyCode1=%d", keyCode);

	   if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	   {
		   onBackPressed();
	   }
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
