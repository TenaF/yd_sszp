//
//  LoginScene.h
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#ifndef __hello2dx__UpdateHeadLayer__
#define __hello2dx__UpdateHeadLayer__

#include "sdk/sdk.h"

class UpdateHeadLayer : public LayerColor
{
	private:
		static UpdateHeadLayer * s_pInstance;
	public :
		Widget * pRoot;
		int selectFace;
		//Layer * pSelectPanel;
		//cocostudio::Armature * armature ;
	public:

		static void createObject();

		UpdateHeadLayer();
		~UpdateHeadLayer();

		virtual bool init();
		virtual void onEnter();
		virtual void onExit();
		//隐藏
		void hideModal();
		//button触发事件
		void touchButton(Ref * obj ,TouchEventType eventType);
		//更新头像
		void updateHead(float tick);
		//监听通知
		void notifySuccess(Ref * obj);
		//监听通知:修改失败
		void notifyFail(Ref * obj);

		CREATE_FUNC(UpdateHeadLayer);
		
};

#endif 
