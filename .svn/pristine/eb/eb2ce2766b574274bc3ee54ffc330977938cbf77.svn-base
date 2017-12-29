#ifndef __LoginLayer_H__
#define __LoginLayer_H__

#include "sdk/sdk.h"


class LoginLayer : public LayerColor ,public EditBoxDelegate
{
private:
	static LoginLayer * s_pInstance;

	Widget * pRoot;
	EditBox * accountBox; 
	EditBox * serverIpBox;
	CheckBox * distribute_port_cheack;
	EditBox * portBox;
	EditBox * passwordBox;
	CheckBox * pIpConnect;
	bool fromLogin ;		
	bool bIpConnect;

public:
	LoginLayer(void);
	~LoginLayer(void);

	static void createObject(Node * pScene);

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	virtual void editBoxEditingDidBegin(EditBox * editBox);
	virtual void editBoxEditingDidEnd(EditBox * editBox);
	virtual void editBoxTextChanged(EditBox * editBox);
	virtual void editBoxReturn(EditBox * editBox);

	bool onTouchBegan(Touch * touch , Event * event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	//通知接口
	void notifySuccess(Ref * obj);
	//通知接口
	void notifyFail(Ref * obj);
	void connectFail(Ref * obj);
	void checkIpSerInfoEditbox(EditBox * editBox);

	void hideModal();
	void touchButtonEvent(Ref * obj ,TouchEventType eventType);
	void listSelectEvent(Ref * pSender , TouchEventType type);
	void setShowCloseBtn();

	CREATE_FUNC(LoginLayer);

	void RefreshSerInfo();


private :
	inline void hideDragList();
};

#endif

