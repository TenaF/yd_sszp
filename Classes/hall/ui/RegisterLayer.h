#ifndef __RegisterLayer_H__
#define __RegisterLayer_H__

#include "sdk/sdk.h"


class RegisterLayer : public LayerColor, public EditBoxDelegate
{
private:
	static RegisterLayer * s_pInstance;

	Widget * pRoot;
	EditBox * pRegisterAccounts;
	EditBox * pRegisterPass;
	EditBox * pRegisterNick;
	EditBox * pRegisterConfirm;
	EditBox * m_pRegisterConfirmPass;
	EditBox * m_pRegisterBankPass;
	EditBox * m_pRegisterBankConfirmPass;
	EditBox * m_pRegisterCard;
	int checkValue ;
	int wFaceID ;
	bool showClose;
	bool m_bIpConnect;
public:
	RegisterLayer(void);
	~RegisterLayer(void);

	static void createObject(bool showClose, bool bIpConnect);

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	virtual void editBoxEditingDidBegin(EditBox * editBox);
	virtual void editBoxEditingDidEnd(EditBox * editBox);
	virtual void editBoxTextChanged(EditBox * editBox);
	virtual void editBoxReturn(EditBox * editBox);

	void checkSelectEvent(Ref * pSender , CheckBoxEventType eventType);
	void touchButtonEvent(Ref * obj ,TouchEventType eventType);
	void hideModal();

	void notifySuccess(Ref * obj);
	void notifyFail(Ref * obj);

	void showCloseBtn(bool isShow);
	void setIpConnect(bool isIp); 

	CREATE_FUNC(RegisterLayer);
};

#endif

