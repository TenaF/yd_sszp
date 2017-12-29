
#ifndef __hello2dx__OutGameLayer_sszp__
#define __hello2dx__OutGameLayer_sszp__
#include "game/sszp/message/CMD_Game_sszp.h"
USING_NS_CC;

namespace sszp
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

