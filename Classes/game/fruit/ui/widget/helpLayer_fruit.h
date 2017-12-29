#ifndef __cocos2dx_helpLayer_fruit__
#define __cocos2dx_helpLayer_fruit__

#include "sdk/sdk.h"
USING_NS_CC;
namespace fruit
{
	using namespace ui;
	class helpLayer : public Layout
	{
	public:
		helpLayer();
		~helpLayer();
	public:
		virtual bool init();
		static helpLayer * instance;
		static void createObject();
		void RootTouch(Ref * obj, cocos2d::ui::TouchEventType eventType);
		void update(float dt);

		ImageView * bg;
	private:
		Widget *pRoot;
	};
}

#endif 

