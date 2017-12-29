#include "helpLayer_yxx.h"
#include "hall/data/DataManager.h"
#include "game/yxx/data/SoundManager_yxx.h"
#include "game/yxx/message/CMD_Game_yxx.h"
#include "game/yxx/message/PokerMessageController_yxx.h"
namespace yxx
{
	using namespace std;
	using namespace ui;
	helpLayer::helpLayer()
	{
		//schedule(schedule_selector(helpLayer::update), 0.3f);
	}

	helpLayer::~helpLayer()
	{
		//unschedule(schedule_selector(helpLayer::update));
		instance = nullptr;
	}

	bool helpLayer::init()
	{
		if (!Layout::init())
		{
			return false;
		}
		auto visbleSize = Director::getInstance()->getVisibleSize();
		auto pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(SETPATH("yxx/helpLayer.json").c_str());
		CCSize visibleSize = Director::getInstance()->getVisibleSize();
		pRoot->setAnchorPoint(CCPoint(0.0f, 0.0f));
		pRoot->setPosition(Vec2(0, 0));
		this->addChild(pRoot);
		pRoot->addTouchEventListener(this, toucheventselector(helpLayer::RootTouch));

		auto close_Btn = static_cast<Button*>(pRoot->getChildByName("close"));
		close_Btn->addTouchEventListener(this, toucheventselector(helpLayer::RootTouch));

		bg = static_cast<ImageView*>(pRoot->getChildByName("bg"));
		bg->setTag(1);
		auto listview = static_cast<ListView*>(pRoot->getChildByName("ListView"));
		auto help_bg = Sprite::create(SETPATH("yxx/rule/help_bg.png").c_str());
		if (help_bg)
		{
			help_bg->setAnchorPoint(Vec2(0, 0));
			auto layer = Layout::create();
			layer->setContentSize(CCSize(help_bg->getContentSize()));
			layer->addChild(help_bg);
			listview->addChild(layer);
		}
		
		return true;
	}

	helpLayer * helpLayer::instance = nullptr;

	void helpLayer::createObject()
	{
		Scene * pScene = Director::getInstance()->getRunningScene();
		if (!instance)
		{
			instance = new helpLayer();
			instance->init();
			pScene->addChild(instance,10000);
		}
		else
		{
			instance->removeFromParent();
			instance = nullptr;
		}
	}

	void helpLayer::RootTouch(Ref * obj, cocos2d::ui::TouchEventType eventType)
	{
		if (eventType == cocos2d::ui::TouchEventType::TOUCH_EVENT_ENDED)
		{
			if (instance)
			{
				instance->removeFromParent();
				instance = nullptr;
			}
		}
	}

	void helpLayer::update(float dt)
	{
		bg = static_cast<ImageView*>(pRoot->getChildByName("bg"));
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