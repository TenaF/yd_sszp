//
//  SoundDataManager.h
//  WEILEPAOYAO
//
//  Created by on
//
//

#ifndef __WEILEPAOYAO__SoundDataManager_sicbo__
#define __WEILEPAOYAO__SoundDataManager_sicbo__

//#include <iostream>
//#include "CommonData.h"
using namespace std;
USING_NS_CC;
namespace sicbo
{
	#define CONFIG_SOUND_MUTE  "sound_mute" //声音开关
	#define SOUND_VALUE   "sound_value" //音量
	#define CONFIG_BG_MUSIC  "sound_bg_onoff" //背景音开关
	#define CONFIG_GAME_MUSIC  "sound_game_onoff" //游戏音效开关
	#define CONFIG_CHAT  "chat_onoff" //游戏人声
	#define NOTFIRST_USE  "notfirst_use" //首次使用
	#define MUSIC "MUSIC"
	#define SOUND "SOUND"

	class SoundManager
	{
	public:
		~SoundManager();
		static SoundManager* instance();
		void setSoundConfig(bool flag);
		bool getSoundConfig();
		void setFirstUse(bool flag);
		bool getFirstUse();
		void setAudioInfo(bool bSwitch);

		void setBgSoundVolume(float f);//0~1之间
		void setEffectSoundVolume(float f);//0~1之间
		void playEffect(const char* sound);
		void playBgMusic();
		void playBgMusic(const char* bgMusic);
		void unplayBgMusic(const char* bgMusic);
		void stopBgMusic();
		void stopAllEffect();
	private:
		SoundManager();
		static SoundManager* _instance;
		unsigned int m_SoundId;
	};
}

#endif /* defined(__WEILEPAOYAO__SoundDataManager__) */
