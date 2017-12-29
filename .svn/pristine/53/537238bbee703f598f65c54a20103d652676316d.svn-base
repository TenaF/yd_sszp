#ifndef __cocos2dx_helpLayer_sicbo__
#define __cocos2dx_helpLayer_sicbo__

#include "sdk/sdk.h"
USING_NS_CC;
namespace sicbo
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

