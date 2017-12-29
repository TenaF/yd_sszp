//
//  LoginScene.h
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#ifndef __hello2dx__UpdatePasswordLayer__
#define __hello2dx__UpdatePasswordLayer__

#include "sdk/sdk.h"

class UpdatePasswordLayer : public LayerColor,public EditBoxDelegate
{
	private:
		static UpdatePasswordLayer * s_pInstance;
	public :
		Widget * pRoot;
		EditBox * passwordBox;
		EditBox * pOldEdit;
		EditBox * confirmBox;
	public:

		static void createObject();

		UpdatePasswordLayer();
		~UpdatePasswordLayer();

		//void onSchedule(float tick);

		virtual bool init();
		virtual void onEnter();
		virtual void onExit();

		virtual void editBoxEditingDidBegin(EditBox * editBox);
		virtual void editBoxEditingDidEnd(EditBox * editBox);
		virtual void editBoxTextChanged(EditBox * editBox);
		virtual void editBoxReturn(EditBox * editBox);

		void hideModal();
	
		void touchButton(Ref * obj ,TouchEventType eventType);
		//回调通知：失败
		void notifyFail(Ref * obj);
		//回调通知：修改成功
		void notifySuccess(Ref * obj);

		CREATE_FUNC(UpdatePasswordLayer);
};

#endif /* defined(__hello2dx__UpdatePasswordLayer__) */
