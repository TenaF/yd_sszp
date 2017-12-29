#ifndef __SaveTakeLayer_H__
#define __SaveTakeLayer_H__

#include "sdk/sdk.h"
//#include "hall/ui/MainLayer.h"

class SaveTakeLayer: public LayerColor, public EditBoxDelegate {
	public:
		static SaveTakeLayer * s_pInstance;

	private:
		Widget * pRoot;
		EditBox * amountBox;
		EditBox * m_PassBox;
		CCMenuItemImage* m_TabBankFun[3];

	public:

		SaveTakeLayer(void);
		~SaveTakeLayer(void);

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
		void notifyRefresh(Ref * obj);
		void hideModal();
		void touchButtonEvent(Ref * obj ,TouchEventType eventType);

		void MenuSelect(Ref* obj);

		CREATE_FUNC(SaveTakeLayer);

};

#endif

