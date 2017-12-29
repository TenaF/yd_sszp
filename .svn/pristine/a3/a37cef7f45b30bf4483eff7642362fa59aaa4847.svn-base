
#ifndef __hello2dx__OutGameLayer_fruit__
#define __hello2dx__OutGameLayer_fruit__
#include "game/fruit/message/CMD_Game_fruit.h"
USING_NS_CC;

namespace fruit
{
	class OutGameLayer : public Layout
	{
	public:
		OutGameLayer();
		~OutGameLayer();

		ImageView * bg;

		virtual bool init();
		static OutGameLayer * instance;
		static void createObject();
		void clearSetting();
		void touchButton(Object * obj, cocos2d::ui::TouchEventType eventType);
		void update(float dt);
		CREATE_FUNC(OutGameLayer);
	private:

	};

}
#endif

