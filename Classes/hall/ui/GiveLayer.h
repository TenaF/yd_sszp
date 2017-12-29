#ifndef __GiveLayer_H__
#define __GiveLayer_H__

#include "sdk/sdk.h"
//#include "hall/ui/MainLayer.h"

class GiveLayer: public LayerColor, public EditBoxDelegate {
	public:
		static GiveLayer * s_pInstance;

	private:
		Widget * pRoot;
		EditBox * nicknameBox;
		EditBox * amountBox;
		EditBox * passwordBox;

		CCMenuItemImage* m_TabBankFun[3];

	public:

		GiveLayer(void);
		~GiveLayer(void);

		static void createObject();

		virtual bool init();
		virtual void onEnter();
		virtual void onExit();

		virtual void editBoxEditingDidBegin(EditBox * editBox);
		virtual void editBoxEditingDidEnd(EditBox * editBox);
		virtual void editBoxTextChanged(EditBox * editBox);
		virtual void editBoxReturn(EditBox * editBox);

		//通知接口
		void notifySuccess(Ref * obj);
		//通知接口
		void notifyFail(Ref * obj);
		void notifyRefreshSuccess(Ref * obj);

		void MenuSelect(Ref* obj);

		void hideModal();
		void touchButtonEvent(Ref * obj ,TouchEventType eventType);

		CREATE_FUNC(GiveLayer);

};

#endif

