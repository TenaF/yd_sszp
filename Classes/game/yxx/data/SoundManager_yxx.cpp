//
//  SoundDataManager.cpp
//  WEILEPAOYAO
//
//  Created by 
//
//
#include "sdk/sdk.h"
#include "SoundManager_yxx.h"
#include "SimpleAudioEngine.h"
#include "base/CCUserDefault.h"
#include "cocos2d.h"
using namespace cocos2d;
using namespace CocosDenshion;

#define TAG "SoundManager"
#include "hall/manager/SoundManager_hall.h"


namespace yxx
{
	SoundManager* SoundManager::_instance = 0;

	SoundManager::SoundManager()
	{
		m_SoundId = 0;
	}

	SoundManager* SoundManager::instance()
	{
		if(!_instance)
		{
			_instance = new SoundManager();
		}
		return _instance;
	}

	SoundManager::~SoundManager()
	{

	}

	// 声音开关
	void SoundManager::setSoundConfig(bool flag)
	{
		UserDefault::getInstance()->setBoolForKey(CONFIG_SOUND_MUTE, flag);
		UserDefault::getInstance()->flush();
	}

	bool SoundManager::getSoundConfig()
	{
		return UserDefault::getInstance()->getBoolForKey(CONFIG_SOUND_MUTE,true);
    
	}
	void SoundManager::setBgSoundVolume(float f)
	{
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(f);
	}
	void SoundManager::setEffectSoundVolume(float f)
	{
		SimpleAudioEngine::getInstance()->setEffectsVolume(f);
	}
	// 是否第一个进入
	void SoundManager::setFirstUse(bool flag)
	{
	//    CCLOG("setFirst = %d",flag);
		UserDefault::getInstance()->setBoolForKey(NOTFIRST_USE,flag);
		UserDefault::getInstance()->flush();
	}

	bool SoundManager::getFirstUse()
	{
	//    CCLOG("first use = %d",UserDefault::getInstance()->getBoolForKey(NOTFIRST_USE,true));
	//    CCLOG("first use = %d",UserDefault::getInstance()->getBoolForKey(NOTFIRST_USE,false));    
		return UserDefault::getInstance()->getBoolForKey(NOTFIRST_USE,true);
    
	}

	void SoundManager::setAudioInfo(bool bSwitch)
	{
		//设置声音
		SimpleAudioEngine *pSimAudio=SimpleAudioEngine::getInstance();
		if(bSwitch)
		{
			pSimAudio->stopAllEffects();
		}
	}

	void SoundManager::playBgMusic()
	{
		//SimpleAudioEngine::getInstance()->playBackgroundMusic(PEARL_SOUND_BG_MUSIC.c_str(), true);
	}
	void SoundManager::playBgMusic(const char* bgMusic)
	{
		if (!HallSoundManager::instance()->getBgMusicConfig()){
			this->stopBgMusic();
			return;
		}
		SimpleAudioEngine::getInstance()->playBackgroundMusic(bgMusic, true);
	}
	void SoundManager::stopBgMusic()
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}

	void SoundManager::playEffect(const char* sound)
	{
		if (!HallSoundManager::instance()->getEffectConfig()) return;
		SimpleAudioEngine *pSimAudio=SimpleAudioEngine::getInstance();
		pSimAudio->playEffect(sound);
	}

	void SoundManager::stopAllEffect()
	{
		SimpleAudioEngine *pSimAudio=SimpleAudioEngine::getInstance();
		pSimAudio->stopAllEffects();
	}
}