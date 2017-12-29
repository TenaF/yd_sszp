
#ifndef __hello2dx__SettingLayer_fruit__
#define __hello2dx__SettingLayer_fruit__
#include "game/fruit/message/CMD_Game_fruit.h"
USING_NS_CC;

namespace fruit
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

