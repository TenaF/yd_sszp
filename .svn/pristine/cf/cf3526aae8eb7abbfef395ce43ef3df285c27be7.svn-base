//
//  LogoScene.cpp
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#include "sdk/sdk.h"
#include "ConnectGameScene.h"
#include <list>
#include "sdk/message/CMD_LogonServer.h"
#include "hall/data/DataManager.h"
#include "sdk/util/Util.h"
#include "hall/data/DataManager.h"
#include "LoadingScene.h"
#include "sdk/ui/PromptLayer.h"

enum UI_SERVER_LIST
{
	//可用
	BTN_BACK = 63, //
	PAGE_VIEW = 7463638,
	CHECK_BOX = 7463639,

	TAG_MENU_BASE = 11000,
	TAG_MENU_BUTTON_BASE = 10000,
	TAG_MENU_LAB_BASE = 20000,
	LABEL_SCORE = 1009,
};

#define TAG  "ConnectGameScene"

ConnectGameScene::ConnectGameScene()
{
	pScollView = nullptr;
}

ConnectGameScene::~ConnectGameScene()
{
	this->removeAllChildrenWithCleanup(true);

	pScollView = nullptr;
}

void ConnectGameScene::initView()
{
	pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("room_list/room_list.json");
	CCSize visibleSize = Director::getInstance()->getVisibleSize();
	pRoot->setAnchorPoint(CCPoint(0.5f , 0.5f));
	pRoot->setPosition(Vec2(visibleSize.width / 2 , visibleSize.height / 2));
	this->addChild(pRoot);

	PageView* pTmpView = static_cast<ui::PageView*>(pRoot->getChildByTag(PAGE_VIEW));
	pScollView = MyPageView::create();
	pScollView->setTouchEnabled(true);
//	pScollView->addEventListener(CC_CALLBACK_2(ConnectGameScene::pageViewLEvent, this));
	pScollView->addEventListenerPageView(this, pagevieweventselector(ConnectGameScene::pageViewEvent));
	pScollView->setPosition(pTmpView->getPosition());
	pScollView->setContentSize(pTmpView->getContentSize());
	pScollView->setSize(pTmpView->getSize());
	pScollView->setActionTag(pTmpView->getTag()-10);
	pScollView->setAnchorPoint(pTmpView->getAnchorPoint());
	pRoot->addChild(pScollView);
	pScollView->setListener(this);
	pTmpView->removeFromParent();

	Button * pBack = static_cast<Button *>(pRoot->getChildByTag(BTN_BACK));
	pBack->setTouchEnabled(true);
	pBack->addTouchEventListener(this , toucheventselector(ConnectGameScene::touchButton));

	initRoomList(DataManager::getInstance()->getCurrentKindID());
}

void ConnectGameScene::onEnter()
{
	SceneBase::onEnter();
	NotificationCenter::getInstance()->addObserver(this , callfuncO_selector(ConnectGameScene::notifySuccess) , LOGON_LOGON_SUCCESS , nullptr);
	NotificationCenter::getInstance()->addObserver(this , callfuncO_selector(ConnectGameScene::notifyFail) , LOGON_LOGON_FAILURE , nullptr);
	NotificationCenter::getInstance()->addObserver(this , callfuncO_selector(ConnectGameScene::notifyServerList) , LOGON_SERVER_LIST , nullptr);

	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(ConnectGameScene::notifyFail),GAME_LOGON_FAILURE,nullptr);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(ConnectGameScene::notifyFail),GAME_REQUEST_FAILURE,nullptr);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(ConnectGameScene::notifySuccess),GAME_LOGIN_FINISH,nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ConnectGameScene::notifyWaitDistribute), GAME_WAIT_DISTRIBUTE, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ConnectGameScene::notifySuccess), GAME_SITDOWN_FINISH, nullptr);
	if (UserInfo::instance()->getIsChangeTable())
	{
		UserInfo::instance()->setIsChangeTable(false);
	}
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
	//log(Director::getInstance()->getTextureCache()->getCachedTextureInfo().c_str());
}

void ConnectGameScene::onExit()
{
	NotificationCenter::getInstance()->removeObserver(this , LOGON_LOGON_SUCCESS);
	NotificationCenter::getInstance()->removeObserver(this , LOGON_LOGON_FAILURE);
	NotificationCenter::getInstance()->removeObserver(this , LOGON_SERVER_LIST);

	NotificationCenter::getInstance()->removeObserver(this , GAME_LOGON_FAILURE);
	NotificationCenter::getInstance()->removeObserver(this , GAME_REQUEST_FAILURE);
	NotificationCenter::getInstance()->removeObserver(this, GAME_LOGIN_FINISH);
	NotificationCenter::getInstance()->removeObserver(this, GAME_WAIT_DISTRIBUTE);
	NotificationCenter::getInstance()->removeObserver(this, GAME_SITDOWN_FINISH);
	SceneBase::onExit();
}

//创建房间
void ConnectGameScene::createRoomButton(Vec2 pos , void * pServer , Layout * _parent)
{
	auto pButtonItem = Sprite::create("room_list/room_info/pageinfo.png");
	pButtonItem->setTag(TAG_MENU_LAB_BASE + ((tagGameServer *) pServer)->wServerID);
    pButtonItem->setPosition(pos);
    _parent->addChild(pButtonItem);
    pButtonItem->setVisible(true);
	pButtonItem->setZOrder(4);


	tagGameServer * pGameSvr = (tagGameServer *) pServer;
	string serverName = Util::getStringWithTChar(pGameSvr->szServerName);
	LabelTTF * pRoomName = LabelTTF::create(U2F(serverName).c_str() , "" , 30);
	pRoomName->setAnchorPoint(Vec2(0,0.5));
	pRoomName->setPosition(Vec2(30, 50));
	pRoomName->setHorizontalAlignment(TextHAlignment::LEFT);
	pRoomName->setVerticalAlignment(TextVAlignment::CENTER);
	pRoomName->setColor(Color3B(1,77,106));
	pButtonItem->addChild(pRoomName);

	if (pGameSvr->dwOnLineCount<pGameSvr->dwFullCount/3)
	{
		LabelTTF * pRoomState = LabelTTF::create(U2F("闲") , "" , 30);
		pRoomState->setAnchorPoint(Vec2(0,0.5));
		pRoomState->setPosition(Vec2(424, 50));
		pRoomState->setColor(Color3B(22,255,66));
		pButtonItem->addChild(pRoomState);
	}
	else if (pGameSvr->dwOnLineCount>=pGameSvr->dwFullCount/3&&pGameSvr->dwOnLineCount<pGameSvr->dwFullCount*2/3)
	{
		LabelTTF * pRoomState = LabelTTF::create(U2F("挤") , "" , 30);
		pRoomState->setAnchorPoint(Vec2(0,0.5));
		pRoomState->setPosition(Vec2(424, 50));
		pRoomState->setColor(Color3B(255,241,22));
		pButtonItem->addChild(pRoomState);
	}
	else
	{
		LabelTTF * pRoomState = LabelTTF::create(U2F("忙"), "" , 30);
		pRoomState->setAnchorPoint(Vec2(0,0.5));
		pRoomState->setPosition(Vec2(424, 50));
		pRoomState->setColor(Color3B(219,0,0));
		pButtonItem->addChild(pRoomState);
	}

	string strEnterLimit = StringUtils::format("%lld", pGameSvr->lMinServerScore)+U2F("以上进入");
	LabelTTF * pEnterLimit = LabelTTF::create(strEnterLimit , "" , 30);
	pEnterLimit->setAnchorPoint(Vec2(0,0.5));
	pEnterLimit->setPosition(Vec2(532, 50));
	pEnterLimit->setHorizontalAlignment(TextHAlignment::LEFT);
	pEnterLimit->setVerticalAlignment(TextVAlignment::CENTER);
	pEnterLimit->setColor(Color3B(255,255,255));
	pButtonItem->addChild(pEnterLimit);

	auto pBtnView = Button::create("room_list/room_info/btn_enter_n.png", "room_list/room_info/btn_enter_d.png");
	pBtnView->setAnchorPoint(Vec2(0,0.5));
	pBtnView->setTag(TAG_MENU_BUTTON_BASE + ((tagGameServer *) pServer)->wServerID);
	Vec2 vcOrc(pButtonItem->getContentSize().width/2,pButtonItem->getContentSize().height/2);
	
	pBtnView->addTouchEventListener(CC_CALLBACK_2(ConnectGameScene::memuCallback, this));
	pBtnView->setPosition(pos-vcOrc+Vec2(756, 50));
    pBtnView->setTouchEnabled(true);
	pBtnView->setZOrder(5);
	_parent->addChild(pBtnView);

//	_parent->addChild(pMenu);
}

//房间触发事件
void ConnectGameScene::memuCallback(Ref* target, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
    {
        int menuTag = static_cast<Node*>(target)->getTag();
		int serverID = menuTag - TAG_MENU_BUTTON_BASE;
		tagGameServer * pGameServer = DataManager::getInstance()->getGameServer(serverID);
		UserInfo::instance()->setCurrentServerID(serverID);
		LoadingScene::createObject();
		if (!NetworkManager::getInstance()->isConnected(NetworkManager::FLAG_GAME))
		{
			string ip = Util::getStringWithTChar(pGameServer->szServerAddr);
			//游戏内联网
			//游戏房间改用大厅端口号和ip网关防御用[zhangyang 2016/03/19]
			UserInfo::instance()->gamePort = pGameServer->wServerPort;

			//游戏房间改用大厅端口号和ip网关防御用[zhangyang 2016/03/19]
			if (UserInfo::instance()->getDistributePort())
			{
				NYInfo(TAG, "startGameConnect  %d:%s,%d", 1, Util::getIpByHostName((char*)(UserInfo::instance()->getServerIp().c_str())), UserInfo::instance()->getServerPort());
				MainController::getInstance()->startConnect(NetworkManager::FLAG_GAME, Util::getIpByHostName((char*)(UserInfo::instance()->getServerIp().c_str())), UserInfo::instance()->getServerPort());
			}
			else
			{
				NYInfo(TAG, "startGameConnect  %d:%s,%d", 0, Util::getIpByHostName((char*)ip.c_str()), pGameServer->wServerPort);
				MainController::getInstance()->startConnect(NetworkManager::FLAG_GAME, Util::getIpByHostName((char*)ip.c_str()), pGameServer->wServerPort);
			}
		}
		this->schedule(schedule_selector(ConnectGameScene::scheduleLogin), 0.5f);
	}
}

//登录成功后回调
void ConnectGameScene::notifySuccess(Ref * obj)
{
	LoadingScene::hideModal();
	pScollView->setTouchEnabled(true);
	unschedule(schedule_selector(ConnectGameScene::outTime));
}

//失败通知
void ConnectGameScene::notifyFail(Ref * obj)
{
	char * pError = (char*)obj;
	NYInfo(TAG , " 登录失败回调 ");
	PromptLayer::createObject(pError);
	LoadingScene::hideModal();
	pScollView->setTouchEnabled(true);
	unschedule(schedule_selector(ConnectGameScene::outTime));
}

//玩家游戏房间登陆成功等待分配--防作弊有
void ConnectGameScene::notifyWaitDistribute(Ref * obj)
{
	LoadingScene::createObject(U2F("正在分配座椅，请耐心等待").c_str());
	//schedule(schedule_selector(ConnectGameScene::waitDistributeOutTime), 20.0f);
}
void ConnectGameScene::notifyServerList(Ref * obj)
{
	CCLog("初始化房间  房间类型：%d 房间个数:%d",DataManager::getInstance()->getCurrentKindID() ,DataManager::getInstance()->getCurGameServerList().size());
	//Util::hiddenDialog(true);
	initRoomList(DataManager::getInstance()->getCurrentKindID());
}

void ConnectGameScene::initRoomList(int roomType)
{
	CheckBox * pCurPageCheckBox = dynamic_cast<CheckBox*>(pRoot->getChildByTag(CHECK_BOX));
	pCurPageCheckBox->setVisible(false);

	int nPosX = 600;
	int nPosY = 410;

	m_nPageCount = 0;
	m_nCurPage = 0;
	Layout* pPageTmp;
	int nIndex = 0;
	list<tagGameServer *> listServer = DataManager::getInstance()->getCurGameServerList();
	list<tagGameServer *>::iterator it = listServer.begin();
	for (; it != listServer.end() ; it++)
	{
		tagGameServer * pServer = *it;
//         if(DataManager::getInstance()->getCurrentKindID() != pServer->wKindID)
// 		{
// 			continue;
// 		}
		int nLeft = nIndex%3;
		if (nLeft==0)
		{
			nPosX = 600;
			nPosY = 410;

			pPageTmp = Layout::create();
			pScollView->addPage(pPageTmp);
			pPageTmp->setTouchEnabled(true);
//			pPageTmp->setSwallowsTouches(true);
			pPageTmp->setEnabled(true);
			pPageTmp->setContentSize(CCSize(1200,500));

			CheckBox* pTmp = CheckBox::create("room_list/room_info/page_n.png",
                                          "room_list/room_info/page_n.png",
                                          "room_list/room_info/page_s.png",
                                          "room_list/room_info/page_s.png",
                                          "room_list/room_info/page_s.png");
			pTmp->setPosition(pCurPageCheckBox->getPosition()+Vec2(m_nPageCount*40,0));
			pTmp->setTag(pCurPageCheckBox->getTag()+m_nPageCount+1);
			if (m_nPageCount==0)
			{
				pTmp->setSelectedState(true);
			}
			else
			{
				pTmp->setSelectedState(false);
			}
			pTmp->setTouchEnabled(false);
			pRoot->addChild(pTmp);
			m_nPageCount++;
		}
		else
		{
			nPosY -= 140;
		}
		createRoomButton(Vec2(nPosX,nPosY) , *it , pPageTmp);
		nIndex++;
	}

	if (m_nPageCount==1)
	{
		CheckBox * pTmpCheckBox = dynamic_cast<CheckBox*>(pRoot->getChildByTag(CHECK_BOX+1));
		pTmpCheckBox->setVisible(false);
	}
}

void ConnectGameScene::touchButton(Ref * obj , TouchEventType eventType)
{
	Button * pButton = (Button *) obj;
	if (eventType == TouchEventType::TOUCH_EVENT_ENDED)
	{
		switch (pButton->getTag())
		{
		case BTN_BACK:
			{
				SceneBase::onBackPressed();
			}
				break;

		}
	}
}

//进入游戏
void ConnectGameScene::scheduleLogin(float t){

	if(NetworkManager::getInstance()->isConnected(NetworkManager::FLAG_GAME)){

		this->unschedule(schedule_selector(ConnectGameScene::scheduleLogin));

				UserInfo::instance()->SendType = 1;
		UserInfo::instance()->httpRequest();

	}
}

void ConnectGameScene::pageViewEvent(Ref* pSender, PageViewEventType type)
{
	if (type == PageViewEventType::PAGEVIEW_EVENT_TURNING)
	{
		int nIndex = pScollView->getCurPageIndex();
		for (int i=0; i<m_nPageCount;i++)
		{
			CheckBox * pCurPageCheckBox = dynamic_cast<CheckBox*>(pRoot->getChildByTag(CHECK_BOX+i+1));
			if (pCurPageCheckBox!=nullptr)
			{
				if (i==nIndex)
				{
					pCurPageCheckBox->setSelectedState(true);
				}
				else
				{
					pCurPageCheckBox->setSelectedState(false);
				}
			}
		}
	}

	return;
}

void ConnectGameScene::pageViewLEvent(Ref* pSender, PageView::EventType type)
{
}

void ConnectGameScene::touchCheck(Vec2 Pos)
{
	m_nCurPage = pScollView->getCurPageIndex();
	Layout* pLayout = pScollView->getPage(m_nCurPage);

	int nServerIndex = 0;
	list<tagGameServer *> listServer = DataManager::getInstance()->getCurGameServerList();
	list<tagGameServer *>::iterator it = listServer.begin();
	for (; it != listServer.end() ; it++)
	{
		tagGameServer * pServer = *it;
//         if(DataManager::getInstance()->getCurrentKindID() != pServer->wKindID)
// 		{
// 			continue;
// 		}
		if (nServerIndex/3==m_nCurPage)
		{
			auto pBtn = pLayout->getChildByTag(TAG_MENU_BUTTON_BASE + ((tagGameServer *) *it)->wServerID);

			//触发点相对MenuItem的坐标
			CCPoint pt = pBtn->convertToNodeSpace(Pos);

			int w = pBtn->getContentSize().width;
			int h = pBtn->getContentSize().height;

			CCRect rc(0,0,w,h);//

			if(rc.containsPoint(pt))
			{
				pBtn->runAction(CCSequence::create(CCScaleTo::create(0.06,1.05f),CCScaleTo::create(0.08,1.00f),CCScaleTo::create(0.08,1.00f),NULL));
				int menuTag = pBtn->getTag();
				int serverID = menuTag - TAG_MENU_BUTTON_BASE;
				tagGameServer * pGameServer = DataManager::getInstance()->getGameServer(serverID);
				UserInfo::instance()->setCurrentServerID(serverID);
				string serverName = Util::getStringWithTChar(pGameServer->szServerName);
				UserInfo::instance()->setServerName(serverName);

				LoadingScene::createObject();
				pScollView->setTouchEnabled(false);
				schedule(schedule_selector(ConnectGameScene::outTime), 20.0f);
				if (!NetworkManager::getInstance()->isConnected(NetworkManager::FLAG_GAME))
				{
					string ip = Util::getStringWithTChar(pGameServer->szServerAddr);
					//游戏内联网
					//游戏房间改用大厅端口号和ip网关防御用[zhangyang 2016/03/19]
					UserInfo::instance()->gamePort = pGameServer->wServerPort;

					//游戏房间改用大厅端口号和ip网关防御用[zhangyang 2016/03/19]
					if (UserInfo::instance()->getDistributePort())
					{
						NYInfo(TAG,"startGameConnect  %d:%s,%d", 1, Util::getIpByHostName((char*)(UserInfo::instance()->getServerIp().c_str())), UserInfo::instance()->getServerPort());
						MainController::getInstance()->startConnect(NetworkManager::FLAG_GAME, Util::getIpByHostName((char*)(UserInfo::instance()->getServerIp().c_str())), UserInfo::instance()->getServerPort());
					}
					else
					{
						NYInfo(TAG,"startGameConnect  %d:%s,%d", 0, Util::getIpByHostName((char*)ip.c_str()), pGameServer->wServerPort);
						MainController::getInstance()->startConnect(NetworkManager::FLAG_GAME, Util::getIpByHostName((char*)ip.c_str()), pGameServer->wServerPort);
					}
				}
				this->schedule(schedule_selector(ConnectGameScene::scheduleLogin), 0.5f);

			}
		}

		nServerIndex++;
	}
}

void ConnectGameScene::waitDistributeOutTime(float dt)
{
	unschedule(schedule_selector(ConnectGameScene::waitDistributeOutTime));

	PromptLayer::createObject("正在尝试重新分配座椅，请稍等···");
	schedule(schedule_selector(ConnectGameScene::autoClose), 3.0f);
}

void ConnectGameScene::outTime(float dt)
{
	unschedule(schedule_selector(ConnectGameScene::outTime));

	UserInfo::instance()->setCurrentServerID(0);
	NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_GAME);
	MainController::getInstance()->setGameState(false);

	PromptLayer::createObject("连接服务器超时，请检查网络连接");
	schedule(schedule_selector(ConnectGameScene::autoClose), 3.0f);//15757834996 cT6RvdYq
}

void ConnectGameScene::autoClose(float dt)
{
	unschedule(schedule_selector(ConnectGameScene::autoClose));
	LoadingScene::hideModal();
	pScollView->setTouchEnabled(true);
}
