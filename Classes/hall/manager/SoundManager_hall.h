//
//  HallSoundManager.h
//
//  Created by on
//
//

#ifndef __SoundDataManager_hall__
#define __SoundDataManager_hall__


class HallSoundManager
{
public:
	HallSoundManager();
	~HallSoundManager();
	static HallSoundManager* instance();
	//设置游戏音效
	void setEffectConfig(bool flag);
	bool getEffectConfig();
	//设置背景音乐
	void setBgMusicConfig(bool flag);
	bool getBgMusicConfig();
	//设置声音大小
	void setBackgroundMusicVolume(float f);//0~1之间
	void setEffectSoundVolume(float f);//0~1之间
	//保存音量大小
	void saveBackgroundMusicVolume();
	void saveEffectSoundVolume();
	//获得音量大小
	float getBackgroundMusicVolume();
	float getEffectSoundVolume();
	//播放音效
	void playEffect(const char* sound);
	//播放点击音效
	void playClickEffect();
	//播放音乐
	void playHallBackgroundMusic();
	void playBackgroundMusic(const char* bgMusic);
	//停止背景音乐
	void stopBgMusic();
	//停止游戏音效
	void stopAllEffect();
protected:
	float m_fBgMusicVolume;
	float m_fEffectVolume;
	bool m_bBgMusicSwitch;
	bool m_bEffectSwitch;
private:
	static HallSoundManager* _instance;



};


#endif
