//
//  LoginScene.h
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#ifndef __hello2dx__SettingLayer__
#define __hello2dx__SettingLayer__

#include "sdk/sdk.h"

class SettingLayer : public LayerColor
{
	public :
		static SettingLayer * s_pInstance;
		Widget * pRoot;

	public:

		static void createObject();
		static void hideModal();

		SettingLayer();
		~SettingLayer();

		//void onSchedule(float tick);

		virtual bool init();
		virtual void onEnter();
		virtual void onExit();
	
		void touchButton(Ref * obj ,TouchEventType eventType);
		void sliderChangeEvent(Ref * pSender , SliderEventType type);

		CREATE_FUNC(SettingLayer);
};

#endif /* defined(__hello2dx__SettingLayer__) */
