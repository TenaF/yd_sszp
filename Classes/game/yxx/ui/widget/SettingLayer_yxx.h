
#ifndef __hello2dx__SettingLayer_yxx__
#define __hello2dx__SettingLayer_yxx__
#include "game/yxx/message/CMD_Game_yxx.h"
USING_NS_CC;

namespace yxx
{
	class SettingLayer : public Layout
	{
	public:
		SettingLayer();
		~SettingLayer();
		virtual bool init();
		static SettingLayer * instance;
		static void createSetting();
		void clearSetting();
		void touchButton(Object * obj, cocos2d::ui::TouchEventType eventType);
		void update(float);
		CREATE_FUNC(SettingLayer);
	public:
		bool isMusic;
		bool isSound;
	public:
		Button * sound_Btn;
		Button * music_Btn;
	private:
		ImageView *bg;
	};

}
#endif

