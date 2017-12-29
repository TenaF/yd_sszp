#include "sdk/sdk.h"
#include "SoundManager_hall.h"
#include "SimpleAudioEngine.h"
#include "base/CCUserDefault.h"
#include "cocos2d.h"
using namespace cocos2d;
using namespace CocosDenshion;

#define TAG "HallSoundManager"
 
#define HALL_CONFIG_BG_MUSIC_VOLUME  "hall_bg_music_volume" //背景音量
#define HALL_CONFIG_EFFECT_VOLUME   "hall_effect_volume" //音效音量

#define HALL_CONFIG_BG_MUSIC  "hall_sound_bg_music_switch" //背景音乐开关
#define HALL_CONFIG_EFFECT_SOUND  "hall_effect_sound_switch" //游戏音效开关

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//大厅背景音乐无需加游戏写入路径
#define SOUND_EFFECT_DATING      "PlazaRes/bg_music/dating.ogg"
#else
//大厅背景音乐无需加游戏写入路径
#define SOUND_EFFECT_DATING    "PlazaRes/bg_music/dating.mp3"
#endif

	HallSoundManager* HallSoundManager::_instance = 0;

	HallSoundManager::HallSoundManager()
	{
		//设置本地存储声音音量大小
		m_fBgMusicVolume = this->getBackgroundMusicVolume();
		m_fEffectVolume = this->getEffectSoundVolume();

		m_bBgMusicSwitch = this->getBgMusicConfig();
		m_bEffectSwitch = this->getEffectConfig();

		if (!m_bBgMusicSwitch)
		{
			SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.f);
		}
		//	setBackgroundMusicVolume(0.0f);
		if (!m_bEffectSwitch)
		{
			SimpleAudioEngine::getInstance()->setEffectsVolume(0.f);
		}
		NYInfo(TAG, "MusicVolume:%f,EffectVolume:%f,EffectS:%d,MusicS:%d", m_fBgMusicVolume, m_fEffectVolume, m_bBgMusicSwitch, m_bEffectSwitch);
	}

	HallSoundManager* HallSoundManager::instance()
	{
		if(!_instance)
		{
			_instance = new HallSoundManager();
		}
		return _instance;
	}

	HallSoundManager::~HallSoundManager()
	{

	}

	// 游戏音效开关
	void HallSoundManager::setEffectConfig(bool flag)
	{
		m_bEffectSwitch = flag;
		UserDefault::getInstance()->setBoolForKey(HALL_CONFIG_EFFECT_SOUND, m_bEffectSwitch);
		UserDefault::getInstance()->flush();
	}

	bool HallSoundManager::getEffectConfig()
	{
		m_bEffectSwitch = UserDefault::getInstance()->getBoolForKey(HALL_CONFIG_EFFECT_SOUND, true);
		return m_bEffectSwitch;
	}

	// 背景音乐开关
	void HallSoundManager::setBgMusicConfig(bool flag)
	{
// 		if (!flag)
// 		{
// 			SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
// 		}
// 		else
// 		{
// 			SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
// 		}
		int s = flag;
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume((float)s);
		m_bBgMusicSwitch = flag;
		UserDefault::getInstance()->setBoolForKey(HALL_CONFIG_BG_MUSIC, m_bBgMusicSwitch);
		UserDefault::getInstance()->flush();
	}

	bool HallSoundManager::getBgMusicConfig()
	{
		m_bBgMusicSwitch = UserDefault::getInstance()->getBoolForKey(HALL_CONFIG_BG_MUSIC, true);
		return m_bBgMusicSwitch;
	}

	void HallSoundManager::setBackgroundMusicVolume(float f)
	{
		m_fBgMusicVolume = f;
		if (m_bBgMusicSwitch)
			SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(f);
	}
	void HallSoundManager::setEffectSoundVolume(float f)
	{
		m_fEffectVolume = f;
		SimpleAudioEngine::getInstance()->setEffectsVolume(f);
	}

	float HallSoundManager::getBackgroundMusicVolume()
	{
		auto str = UserDefault::getInstance()->getStringForKey(HALL_CONFIG_BG_MUSIC_VOLUME, "1.0");
		m_fBgMusicVolume = (float)std::atof(str.c_str());
		return m_fBgMusicVolume;
	}

	float HallSoundManager::getEffectSoundVolume()
	{
		auto str = UserDefault::getInstance()->getStringForKey(HALL_CONFIG_EFFECT_VOLUME, "1.0");
		m_fEffectVolume = (float)std::atof(str.c_str());
		return m_fEffectVolume;
	}

	void HallSoundManager::saveBackgroundMusicVolume(){
		string str = StringUtils::format("%f", m_fBgMusicVolume);
		UserDefault::getInstance()->setStringForKey(HALL_CONFIG_BG_MUSIC_VOLUME, str.c_str());
		UserDefault::getInstance()->flush();
	}

	void HallSoundManager::saveEffectSoundVolume(){
		string str = StringUtils::format("%f", m_fEffectVolume);
		UserDefault::getInstance()->setStringForKey(HALL_CONFIG_EFFECT_VOLUME, str.c_str());
		UserDefault::getInstance()->flush();
	}


	void HallSoundManager::playHallBackgroundMusic()
  	{
		auto audioManager = SimpleAudioEngine::getInstance();
		if (audioManager->isBackgroundMusicPlaying())
			return;
		audioManager->playBackgroundMusic(SOUND_EFFECT_DATING, true);
// 		if (!m_bBgMusicSwitch)
// 		{
// 			audioManager->pauseBackgroundMusic();
// 		}
// 		else
// 		{
// 			audioManager->resumeBackgroundMusic();
// 		}
	}

	void HallSoundManager::playBackgroundMusic(const char* bgMusic)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic(bgMusic, true);
// 		if (!m_bBgMusicSwitch)
// 		{
// 			SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
// 		}
// 		else
// 		{
// 			SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
// 		}
	}

	void HallSoundManager::playEffect(const char* sound)
	{
		if (!m_bEffectSwitch)return;
		SimpleAudioEngine::getInstance()->playEffect(sound);
	}
	void HallSoundManager::playClickEffect()
	{
		this->playEffect("PlazaRes/bg_music/click.mp3");
	}

	void HallSoundManager::stopBgMusic()
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
	void HallSoundManager::stopAllEffect()
	{
		SimpleAudioEngine::getInstance()->stopAllEffects();
	}


