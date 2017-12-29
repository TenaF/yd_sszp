
#ifndef __hello2dx__OutGameLayer_yxx__
#define __hello2dx__OutGameLayer_yxx__
#include "game/yxx/message/CMD_Game_yxx.h"
USING_NS_CC;

namespace yxx
{
	class OutGameLayer : public Layout
	{
	public:
		OutGameLayer();
		~OutGameLayer();

		ImageView * bg;

		virtual bool init();
		static OutGameLayer * instance;
		static void createObject(bool istrue);
		void clearSetting();
		void touchButton(Object * obj, cocos2d::ui::TouchEventType eventType);
		void update(float dt);
		CREATE_FUNC(OutGameLayer);
	private:

	};

}
#endif

