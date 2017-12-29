#include "SettingLayer_yxx.h"
#include "hall/manager/SoundManager_hall.h"
#include "game/yxx/message/PokerMessageController_yxx.h"
#include "game/yxx/data/SoundManager_yxx.h"
namespace yxx
{
	enum UI_SETTING
	{
		SETTING_CLOSE = 0,			//
		SETTING_MODIFY_PASS,
		SETTING_SWITCH,
		SETTING_MUSIC,
		SETTING_SOUND
	};

	SettingLayer::SettingLayer()
	{
		//schedule(schedule_selector(SettingLayer::update), 0.3f);
	}

	SettingLayer::~SettingLayer()
	{
		//unschedule(schedule_selector(SettingLayer::update));
		instance = nullptr;
	}

	bool SettingLayer::init()
	{
		CCSize visibleSize = Director::getInstance()->getVisibleSize();
		setTouchEnabled(true);
		auto pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(SETPATH("yxx/Setting.json").c_str());
		pRoot->setAnchorPoint(CCPoint(0.5f, 0.5f));
		pRoot->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		this->addChild(pRoot);

		auto planel = static_cast<Widget *>(pRoot->getChildByName("planel"));
		bg = static_cast<ImageView *>(planel->getChildByName("bg"));
		bg->setTouchEnabled(true);
		bg->setTag(1);
		music_Btn = static_cast<Button *>(pRoot->getChildByName("music_Btn"));
		music_Btn->setTag(SETTING_MUSIC);
		music_Btn->addTouchEventListener(this, toucheventselector(SettingLayer::touchButton));

		if (!HallSoundManager::instance()->getBgMusicConfig())//isSound == true是关闭音效
		{
			music_Btn->loadTextureNormal(SETPATH("yxx/setting/off.png").c_str());
		}
		else
		{
			music_Btn->loadTextureNormal(SETPATH("yxx/setting/no.png").c_str());
		}
		sound_Btn = static_cast<Button *>(pRoot->getChildByName("sound_Btn"));
		sound_Btn->setTag(SETTING_SOUND);
		sound_Btn->addTouchEventListener(this, toucheventselector(SettingLayer::touchButton));

		if (!HallSoundManager::instance()->getEffectConfig())//isSound == true是关闭音效
		{
			sound_Btn->loadTextureNormal(SETPATH("yxx/setting/off.png").c_str());
		}
		else
		{
			sound_Btn->loadTextureNormal(SETPATH("yxx/setting/no.png").c_str());
		}

		auto close_Btn = static_cast<Button *>(pRoot->getChildByName("close_Btn"));
		close_Btn->setTag(SETTING_CLOSE);
		close_Btn->addTouchEventListener(this, toucheventselector(SettingLayer::touchButton));

		Widget * bg = static_cast<Widget *>(pRoot->getChildByName("planel"));
		bg->setTag(SETTING_CLOSE);
		bg->addTouchEventListener(this, toucheventselector(SettingLayer::touchButton));
		return true;
	}

	SettingLayer * SettingLayer::instance = nullptr;

	void SettingLayer::createSetting()
	{
		if (!instance)
		{
			Scene * pScene = Director::sharedDirector()->getRunningScene();
			instance = SettingLayer::create();
			pScene->addChild(instance, 10000);
		}
		else
		{
			instance->removeFromParent();
			instance = nullptr;
		}
	}

	void SettingLayer::clearSetting()
	{
		if (instance)
		{
			instance->removeFromParent();
			instance = nullptr;
		}
	}

	void SettingLayer::touchButton(Object * obj, cocos2d::ui::TouchEventType eventType)
	{
		Button * pButton = (Button *)obj;
		if (eventType == cocos2d::ui::TouchEventType::TOUCH_EVENT_ENDED)
		{
			switch (pButton->getTag()){
			case SETTING_SOUND:
			{
				isSound = HallSoundManager::instance()->getEffectConfig();
				HallSoundManager::instance()->setEffectConfig(!isSound);
				if (isSound)//isSound == true是关闭音效
				{
					sound_Btn->loadTextureNormal(SETPATH("yxx/setting/off.png").c_str());
				}
				else
				{
					sound_Btn->loadTextureNormal(SETPATH("yxx/setting/no.png").c_str());
				}
			}
				break;

			case SETTING_MUSIC:
			{
				isMusic = HallSoundManager::instance()->getBgMusicConfig();
				HallSoundManager::instance()->setBgMusicConfig(!isMusic);
				if (isMusic)//isMusic == true是关闭音乐
				{
					SoundManager::instance()->stopBgMusic();
					music_Btn->loadTextureNormal(SETPATH("yxx/setting/off.png").c_str());
				}
				else
				{
					SoundManager::instance()->playBgMusic(SETPATH("yxx/sound/BetBk.mp3").c_str());
					music_Btn->loadTextureNormal(SETPATH("yxx/setting/no.png").c_str());
				}
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

	void SettingLayer::update(float dt)
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
		bg->loadTexture(__String::createWithFormat(SETPATH("big_bg_%d.png").c_str(), bg->getTag())->getCString());
	}
}

