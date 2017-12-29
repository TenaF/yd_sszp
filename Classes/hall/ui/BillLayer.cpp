#include "MainLayer.h"
#include "BillLayer.h"
#include "sdk/util/Util.h"
#include "hall/data/DataManager.h"
#include "hall/ui/GiveLayer.h"
#include "hall/ui/SaveTakeLayer.h"

BillLayer * BillLayer::s_pInstance = nullptr;

enum UI_BILL{
	BACK				=  297 ,		//后退
	USER_INFO			=  4428 ,		//用户信息
	SCROLL_VIEW			=	299 ,
	PANEL_BILL			=	298 ,
	LABEL_TYPE			=	300 ,
	LABEL_IN_OUT		=	302 ,
	LABEL_LEFT			=	303 ,
	LABEL_MEMO			=	304 ,
};



BillLayer::BillLayer(void)
{
	m_nRecordCount = 0;
	m_BillRecord.clear();
}

BillLayer::~BillLayer(void)
{
	m_BillRecord.clear();
}

//
void BillLayer::notifyBill(Ref * obj)
{
	MSG_GR_QUERY_GameBillInfo_Data * pInfo = (MSG_GR_QUERY_GameBillInfo_Data *)obj;
	ui::ScrollView * pParent = static_cast<ui::ScrollView *>(pRoot->getChildByTag(UI_BILL::SCROLL_VIEW));
	MSG_GR_QUERY_GameBillInfo_Data billRecord;
	memset(&billRecord, 0, sizeof(MSG_GR_QUERY_GameBillInfo_Data));

	memcpy(&billRecord, pInfo, sizeof(MSG_GR_QUERY_GameBillInfo_Data));
	m_BillRecord.insert(map<int, MSG_GR_QUERY_GameBillInfo_Data>::value_type(m_nRecordCount, billRecord));
	m_nRecordCount++;
	if (m_nRecordCount==pInfo->dwRecordCount)
	{
		for (int t=0;t<m_nRecordCount;t++)
		{
			createBill(&m_BillRecord[t],  pParent, t);
		}
	}
//	createBill(pInfo ,  pParent);
}

//创建
void BillLayer::createBill(MSG_GR_QUERY_GameBillInfo_Data * pInfo , ui::ScrollView * pParent, int nRecordIndex){
	
	int scrollHeight = pParent->getInnerContainerSize().height;		//滚动层高帿

	ui::Layout * pPanel = static_cast<ui::Layout *>(pParent->getChildByTag(UI_BILL::PANEL_BILL));
	pPanel->setVisible(false);

	int panelHeight = pPanel->getContentSize().height;
	int totalHeight =  m_nRecordCount * panelHeight;	

	scrollHeight = scrollHeight > totalHeight ? scrollHeight : totalHeight;
	pParent->setInnerContainerSize(CCSize(pParent->getContentSize().width , scrollHeight));

	int i = nRecordIndex;
	int positionY = scrollHeight - (i+1) * panelHeight + panelHeight/2;

	Text * pType = static_cast<Text *>(pPanel->getChildByTag(UI_BILL::LABEL_TYPE));	//类型
	//std::basic_string<unsigned short> str = pInfo->BillData.szBillType;
	string tType = "";
	switch (pInfo->BillData.wOperateType)
	{
	case 1:
		tType = U2F("存款");
		break;
	case 2:
		tType = U2F("取款");
		break;
	case 3:
		tType = U2F("转赠");
		break;
	case 4:
		tType = U2F("受赠");
		break;
	case 5:
		tType = U2F("房间赛奖励");
		break;
	case 6:
		tType = U2F("商城");
		break;
	case 7:
		tType = U2F("道具");
		break;
	case 8:
		tType = U2F("VIP登陆奖励");
		break;
	case 9:
		tType = U2F("报名费用");
		break;
	case 10:
		tType = U2F("退赛费用");
		break;
	case 11:
		tType = U2F("淘汰赛奖励");
		break;
	default:
		tType = U2F("未知");
		break;
	}
	Label * pNewType = Label::create(tType , "Arial" , 20);
	pNewType->setContentSize(pType->getContentSize());
	pNewType->setPosition(Vec2(pType->getPosition().x,positionY));
	pParent->addChild(pNewType);

	Text * pInOut = static_cast<Text *>(pPanel->getChildByTag(UI_BILL::LABEL_IN_OUT));	//收入支出
	string strInOut = StringUtils::toString(pInfo->BillData.lInCount);
	Label * pNewInOut = Label::create(strInOut, "" , 20);
	pNewInOut->setContentSize(pInOut->getContentSize());
	pNewInOut->setPosition(Vec2(pInOut->getPosition().x,positionY));
	pParent->addChild(pNewInOut);

	Text * pLeft = static_cast<Text *>(pPanel->getChildByTag(UI_BILL::LABEL_MEMO));	//剩余
	Label * pNewLeft = Label::create(StringUtils::toString(pInfo->BillData.lOutCount) , "" , 20);
	pNewLeft->setContentSize(pLeft->getContentSize());
	pNewLeft->setPosition(Vec2(pLeft->getPosition().x,positionY));
	pParent->addChild(pNewLeft);

	Text * pMemo = static_cast<Text *>(pPanel->getChildByTag(UI_BILL::LABEL_LEFT));	//注释
	//std::basic_string<unsigned short> str1 = pInfo->BillData.szDescribe;
	string tMemo = Util::getStringWithTChar(pInfo->BillData.szBillTime);;
	Label * pNewMemo = Label::create(tMemo , "" , 20);
	pNewMemo->setContentSize(pMemo->getContentSize());
	pNewMemo->setPosition(Vec2(pMemo->getPosition().x,positionY));
	pParent->addChild(pNewMemo);
	
}

bool BillLayer::init()
{
	if (!LayerColor::init()) {
		return false;
	}

	m_nRecordCount = 0;
	DWORD dwUserID= UserInfo::instance()->getUserId();

	pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("bill.json");
	CCSize visibleSize = Director::getInstance()->getVisibleSize();
	pRoot->setAnchorPoint(CCPoint(0.5f,0.5f));
	pRoot->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	this->addChild(pRoot);

	//后退
	Button * pBack = static_cast<Button *>(pRoot->getChildByTag(BACK));
	pBack->addTouchEventListener(this,toucheventselector(BillLayer::touchButton));

	//查看
//	Button * pUserInfo = static_cast<Button *>(pRoot->getChildByTag(USER_INFO));
//	pUserInfo->addTouchEventListener(this,toucheventselector(BillLayer::touchButton));

	//
	//Layout * pSearchPanel = static_cast<Layout *>(pRoot->getChildByTag(PANEL_SEARCH));
	//TextField * pDate = static_cast<TextField *>(pSearchPanel->getChildByTag(LABEL_DATE));
	//pDate->setVisible(false);

	struct tm *tm;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time_t timep;
	time(&timep);
	tm=localtime(&timep);
	
#else
	//S now;
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval,NULL);
	//Time::gettimeofdayCocos2d(tm , NULL);
	tm = localtime(&nowTimeval.tv_sec);
#endif

	int year = tm->tm_year + 1900 ;
	int month = tm->tm_mon + 1;
	int day = tm->tm_mday;

	Label * pLable = Label::create(StringUtils::toString(year).append("-").append(StringUtils::toString(month)).append("-").append(StringUtils::toString(day)) ,"",20);
	pLable->setTag(4);
	//pLable->setContentSize(pDate->getContentSize());
	//Vec2 pos = pDate->getPosition();
	//pLable->setPosition(Vec2(pos.x,pos.y-pDate->getContentSize().height));
	//pLable->setPosition(pDate->getPosition());
	//pSearchPanel->addChild(pLable);

	//
	//TextFieldTTF * pSearch = static_cast<TextFieldTTF *>(pRoot->getChildByTag(TEXT_SEARCH));

	ui::ScrollView * pParent = static_cast<ui::ScrollView *>(pRoot->getChildByTag(UI_BILL::SCROLL_VIEW));
	ui::Layout * pPanel = static_cast<ui::Layout *>(pParent->getChildByTag(UI_BILL::PANEL_BILL));
	pPanel->setVisible(false);

	//上面的MENU
	m_TabBankFun[0] = CCMenuItemImage::create("commonresource/tab_save_n.png", "commonresource/tab_save_s.png", "commonresource/tab_save_s.png", this,menu_selector(BillLayer::MenuSelect));
	m_TabBankFun[0]->setTag(6000);
	m_TabBankFun[1] = CCMenuItemImage::create("commonresource/tab_give_n.png", "commonresource/tab_give_s.png", "commonresource/tab_give_s.png", this,menu_selector(BillLayer::MenuSelect));
	m_TabBankFun[1]->setTag(6001);
	m_TabBankFun[2] = CCMenuItemImage::create("commonresource/tab_bill_n.png", "commonresource/tab_bill_s.png", "commonresource/tab_bill_s.png", this,menu_selector(BillLayer::MenuSelect));
	m_TabBankFun[2]->setTag(6002);
	CCMenu* pMenu = CCMenu::create(m_TabBankFun[0], m_TabBankFun[1], m_TabBankFun[2],NULL);
	pMenu->alignItemsHorizontallyWithPadding(2);
	pMenu->setPosition(ccp(860, 700));
	m_TabBankFun[0]->unselected();
	m_TabBankFun[1]->unselected();
	m_TabBankFun[2]->selected();
	pRoot->addChild(pMenu);
	
	return true;
}


void BillLayer::onEnter(){

	LayerColor::onEnter();
	//
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(BillLayer::notifyBill),LOGON_BILL_INFO,nullptr);
}

void BillLayer::onExit(){
	NotificationCenter::getInstance()->removeObserver(this,LOGON_BILL_INFO);
	LayerColor::onExit();
}


//button event
void BillLayer::touchButton(Ref * obj ,TouchEventType eventType){

	Button * pButton = (Button *)obj;
	switch(eventType){
		case TouchEventType::TOUCH_EVENT_ENDED:
			{
				switch(pButton->getTag()){
					case BACK:				//
						{
							hideModal();
						}
						break;
				
				}
			}
			break;
	}
}

//创建对象
void BillLayer::createObject() {

	if (!s_pInstance) {
		s_pInstance = BillLayer::create();

		CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
		pScene->addChild(s_pInstance);
	}
	SendMsg::sendQueryBill();
}

void BillLayer::hideModal() {
	if (s_pInstance) {
		s_pInstance->removeFromParent();
		s_pInstance = nullptr;
	}
}

void BillLayer::MenuSelect(Ref* obj)
{
	switch(((CCMenuItemImage*)obj)->getTag())
	{
	case 6000:
		{
			hideModal();
			SaveTakeLayer::createObject();
		}
		break;
	case 6001:
		{
			hideModal();
			GiveLayer::createObject();
		}
		break;
	default:
		{
			m_TabBankFun[0]->unselected();
			m_TabBankFun[1]->unselected();
			m_TabBankFun[2]->selected();
		}
		break;
	}
}







