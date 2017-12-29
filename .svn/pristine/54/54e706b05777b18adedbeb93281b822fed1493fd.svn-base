//
//  LoginLayer.h
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#ifndef __hello2dx__ModifyNicknameLayer__
#define __hello2dx__ModifyNicknameLayer__

#include "sdk/sdk.h"

class ModifyNicknameLayer : public LayerColor , public EditBoxDelegate
{
	private:
		static ModifyNicknameLayer * s_pInstance;
	public :
		Widget * pRoot;
		EditBox * nickBox;
	public:

		static void createObject();

		ModifyNicknameLayer();
		~ModifyNicknameLayer();

		//void onSchedule(float tick);

		virtual bool init();
		virtual void onEnter();
		virtual void onExit();

		//
		virtual void editBoxEditingDidBegin(EditBox * editBox);
		virtual void editBoxEditingDidEnd(EditBox * editBox);
		virtual void editBoxTextChanged(EditBox * editBox);
		virtual void editBoxReturn(EditBox * editBox);

		//通知接口
		void notifySuccess(Ref * obj);
		//通知接口
		void notifyFail(Ref * obj);

		void onSchedule(float f);
		void hideModal();
	
		void touchButton(Ref * obj ,TouchEventType eventType);

		CREATE_FUNC(ModifyNicknameLayer);
};

#endif /* defined(__hello2dx__ModifyNicknameLayer__) */
