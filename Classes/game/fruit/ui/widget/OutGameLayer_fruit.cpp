﻿#include "OutGameLayer_fruit.h"
#include "game/fruit/message/PokerMessageController_fruit.h"
namespace fruit
{

	enum UI_OUT
	{
		SETTING_CLOSE = 7457192,			//
		SURE_TAG = 7457183,
		CANCEL_TAG = 7457182,

	};

	OutGameLayer::OutGameLayer()
	{
		//schedule(schedule_selector(OutGameLayer::update), 0.3f);
	}

	OutGameLayer::~OutGameLayer()
	{
		//unschedule(schedule_selector(OutGameLayer::update));
		instance = nullptr;
	}

	bool OutGameLayer::init()
	{
		CCSize visibleSize = Director::getInstance()->getVisibleSize();
		setTouchEnabled(true);
		auto pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(SETPATH("fruit/outLayer.json").c_str());
		pRoot->setAnchorPoint(CCPoint(0.5f, 0.5f));
		pRoot->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		this->addChild(pRoot);

		/*Widget * Panel = static_cast<Widget *>(pRoot->getChildByName("Panel"));
		Panel->setTag(SETTING_CLOSE);
		Panel->addTouchEventListener(this, toucheventselector(OutGameLayer::touchButton));*/

		bg = static_cast<ImageView *>(pRoot->getChildByName("bg"));
		bg->setTag(1);
		bg->setTouchEnabled(true);

		Button * sure_Btn = static_cast<Button *>(bg->getChildByName("sure_Btn"));
		sure_Btn->setTag(SURE_TAG);
		sure_Btn->addTouchEventListener(this, toucheventselector(OutGameLayer::touchButton));

		Button * cancel_Btn = static_cast<Button *>(bg->getChildByName("cancel_Btn"));
		cancel_Btn->setTag(CANCEL_TAG);
		cancel_Btn->addTouchEventListener(this, toucheventselector(OutGameLayer::touchButton));

		Button * close_Btn = static_cast<Button *>(bg->getChildByName("close_Btn"));
		close_Btn->setTag(CANCEL_TAG);
		close_Btn->addTouchEventListener(this, toucheventselector(OutGameLayer::touchButton));
		return true;
	}

	OutGameLayer * OutGameLayer::instance = nullptr;

	void OutGameLayer::createObject()
	{
		if (!instance)
		{
			Scene * pScene = Director::sharedDirector()->getRunningScene();
			instance = OutGameLayer::create();
			pScene->addChild(instance, 10000);
		}
		else
		{
			instance->removeFromParent();
			instance = nullptr;
		}
	}

	void OutGameLayer::clearSetting()
	{
		if (instance)
		{
			instance->removeFromParent();
			instance = nullptr;
		}
	}

	void OutGameLayer::touchButton(Object * obj, cocos2d::ui::TouchEventType eventType)
	{
		Button * pButton = (Button *)obj;
		if (eventType == cocos2d::ui::TouchEventType::TOUCH_EVENT_ENDED)
		{
			switch (pButton->getTag()){
			case SURE_TAG:
			{
				clearSetting(); 
				SendMsg::sendStandUpReq(NetworkManager::FLAG_GAME, UserInfo::instance()->getTableId(), UserInfo::instance()->getSeatId(), 1, 0);
			}
				break;

			case CANCEL_TAG:
			{
				clearSetting();
			}
				break;

			case SETTING_CLOSE:
			{
				clearSetting();
			}
				break;
			}
		}
	}

	void OutGameLayer::update(float dt)
	{
		switch (bg->getTag())
		{
		case 1:
			bg->setTag(2);
			break;
		case 2:
			bg->setTag(1);
			break;
		}
	}
}

