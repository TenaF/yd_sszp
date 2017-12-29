//
//  SceneBase.cpp
//  hello2dx
//
//  Created by  on 14-7-4.
//
//  统一处理一些基类事件:
//      @返回键

#include "SceneBase.h"
#include "sdk/sdk.h"
#include "sdk/ui/PromptLayer.h"
#include <time.h>
#include "hall/ui/LoadingScene.h"

USING_NS_CC;
#define TAG "SceneBase"

Scene* SceneBase::createScene(SceneBase* layer)
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
Scene* SceneBase::createSceneWithPhysics(SceneBase* layer)
{
	auto scene = Scene::createWithPhysics();
    if (layer)
    {
		scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    	layer->initView();
		layer->setPhysicsWorld(scene->getPhysicsWorld());
		scene->addChild(layer);
		layer->addListener();
    }
    return scene;
}

SceneBase::SceneBase()
{
    Layer::init();
}

void SceneBase::onEnter()
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

void SceneBase::cleanup()
{
    Layer::cleanup();
}

void SceneBase::onExit()
{
	Layer::onExit();
}

void SceneBase::onBackPressed()
{
//	JniUtil::finishApk();
	Director::getInstance()->popScene();
}

void SceneBase::setBg(const char* resPath)
{
    auto bg = Sprite::create(resPath);
    //bg->setScale(VisibleRect::getScale());
    bg->setPosition(VisibleRect::center());
    this->addChild(bg);
}

void SceneBase::addListener()
{
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event){
	   NYInfo(TAG,"onKeyPressed keyCode4=%d", keyCode);
	};
	listener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event){
	   NYInfo(TAG,"onKeyReleased keyCode3=%d", keyCode);

	   if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	   {
		   onBackPressed();
	   }
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
