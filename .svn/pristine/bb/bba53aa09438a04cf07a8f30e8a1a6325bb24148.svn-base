#ifndef __BillLayer_H__
#define __BillLayer_H__

#include "sdk/sdk.h"

class BillLayer : public LayerColor , public TextFieldDelegate
{
public:
		static BillLayer * s_pInstance;
private :
	Widget * pRoot;
	int i ;
	CCMenuItemImage* m_TabBankFun[3];
	int m_nRecordCount;
	std::map<int,MSG_GR_QUERY_GameBillInfo_Data> m_BillRecord;
public:
	BillLayer(void);
	~BillLayer(void);

	virtual bool init();
	void onEnter();
	void onExit();

	void touchButton(Ref * obj ,TouchEventType eventType);
	//创建账单
	void createBill(MSG_GR_QUERY_GameBillInfo_Data * pInfo , ui::ScrollView * pParent, int nRecordIndex);
	//观察者通知
	void notifyBill(Ref * obj);

	static void createObject();
	void hideModal();
	void MenuSelect(Ref* obj);

	CREATE_FUNC(BillLayer);
};

#endif

