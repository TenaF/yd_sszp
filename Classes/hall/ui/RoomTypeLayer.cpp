//
//  LogoScene.cpp
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#include <list>
#include "sdk/sdk.h"
#include "sdk/message/CMD_LogonServer.h"
#include "sdk/util/Util.h"
#include "sdk/ui/WebView.h"
#include "sdk/util/QFileUtil.h"
#include "hall/data/DataManager.h"
#include "RoomTypeLayer.h"
#include "MainLayer.h"
#include "SettingLayer.h"
#include "LoadingLayer.h"
#include "GiveLayer.h"
#include "SaveTakeLayer.h"
#define TAG "RoomTypeLayer"

enum UI_ROOM_TYPE {
	MATCH = 26,				//
	FREE = 27,				//
	BACK = 17,				//

	CUSTOMER_BTN = 16,        //
	TRUMPET_BTN = 67,
	SETTING_BTN = 70,
	REFRESH_BTN = 4139,
	BUY_BTN = 14,
	RECHARGE_BTN = 4361,
	SAVE_TAKE_BTN = 7457250,
	GIVE_BTN = 4363,
	NICKNAME_TEXT = 7,
	SCORE_TEXT = 13,
	HEAD_IMG = 3,
	LAYER_BOTTOM = 65,
	LAYER_OPTION = 15,
	LAYER_USER_INFO = 1,

	NOTICE = 20,
	NOTICE_SVIEW = 22,
	NOTICE_PANEL = 139,
	NOTICE_LABEL = 23,
	CONTENT_SCROLL_VIEW = 24,

	ROOM_GAME_ID = 7457318,

};

RoomTypeLayer::RoomTypeLayer(int kindID) {
	m_kindID = kindID;
	m_loadingParent = nullptr;
}

RoomTypeLayer::~RoomTypeLayer() {
	this->removeAllChildrenWithCleanup(true);
	pRoot = nullptr;
	m_loadingParent = nullptr;
}

void RoomTypeLayer::initView() {

	pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("room_type_list.json");
	CCSize visibleSize = Director::getInstance()->getVisibleSize();
	pRoot->setAnchorPoint(CCPoint(0.5f, 0.5f));
	pRoot->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(pRoot);

	Button * pBack = static_cast<Button *>(pRoot->getChildByTag(UI_ROOM_TYPE::BACK));
	pBack->setTouchEnabled(true);
	pBack->addTouchEventListener(this, toucheventselector(RoomTypeLayer::touchButton));

	Widget * m_pBottom = dynamic_cast<Widget*>(pRoot->getChildByTag(LAYER_BOTTOM));
	
	//支付
	Button * m_pRechange = dynamic_cast<Button*>(m_pBottom->getChildByTag(RECHARGE_BTN));
	m_pRechange->addTouchEventListener(this, toucheventselector(RoomTypeLayer::touchButton));

	//�
	Button * pGive = static_cast<Button *>(m_pBottom->getChildByTag(GIVE_BTN));
	pGive->addTouchEventListener(this, toucheventselector(RoomTypeLayer::touchButton));


	//存取
	Button * m_pSaveTake = dynamic_cast<Button*>(m_pBottom->getChildByTag(SAVE_TAKE_BTN));
	m_pSaveTake->addTouchEventListener(this, toucheventselector(RoomTypeLayer::touchButton));

	Text * pGameID = static_cast<Text *>(pRoot->getChildByTag(ROOM_GAME_ID));
	pGameID->setText(StringUtils::toString(UserInfo::instance()->getGameId()));

	Widget * m_pOption = dynamic_cast<Widget*>(pRoot->getChildByTag(LAYER_OPTION));

	//����
	Button * m_pSetting = dynamic_cast<Button*>(m_pOption->getChildByTag(SETTING_BTN));
	m_pSetting->addTouchEventListener(this, toucheventselector(RoomTypeLayer::touchButton));

	Widget * m_pUserInfo = dynamic_cast<Widget*>(pRoot->getChildByTag(LAYER_USER_INFO));

	//ˢ��
	Button * m_pRefresh = dynamic_cast<Button*>(m_pUserInfo->getChildByTag(REFRESH_BTN));
	m_pRefresh->setVisible(false);
	m_pRefresh->addTouchEventListener(this, toucheventselector(RoomTypeLayer::touchButton));

	//����
	Button * m_pBuy = dynamic_cast<Button*>(m_pUserInfo->getChildByTag(BUY_BTN));
	m_pBuy->addTouchEventListener(this, toucheventselector(RoomTypeLayer::touchButton));

	Text * pNickname = static_cast<Text *>(m_pUserInfo->getChildByTag(NICKNAME_TEXT));
	pNickname->setText(UserInfo::instance()->getNickName());

	Text * pUserScore = static_cast<Text *>(m_pUserInfo->getChildByTag(SCORE_TEXT));
	pUserScore->setText(StringUtils::toString(UserInfo::instance()->getScore()));

	ImageView * pHead = static_cast<ImageView *>(m_pUserInfo->getChildByTag(HEAD_IMG));
	if (pHead) {
		string headUrl = StringUtils::format("head_%d.png", UserInfo::instance()->getFaceId());
		if (!FileUtils::getInstance()->isFileExist(headUrl)) {
			if (UserInfo::instance()->getSex() == 1)
				headUrl = "head_0.png";        //默认图片
			else
				headUrl = "head_50.png";
		}

		pHead->loadTexture(headUrl);
		pHead->setScale(1.7);
		pHead->setTouchEnabled(true);
		pHead->addTouchEventListener(this, toucheventselector(MainLayer::touchHeadImage));
	}

	pView = static_cast<ui::ScrollView*>(pRoot->getChildByTag(CONTENT_SCROLL_VIEW));

}

void RoomTypeLayer::onEnter() {
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomTypeLayer::notifySuccess), LOGON_LOGON_SUCCESS, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomTypeLayer::notifyServerList), LOGON_SERVER_LIST, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomTypeLayer::notifySuccess), LOGON_FACE_INFO, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomTypeLayer::notifySuccess), LOGON_REFRESH_USER_INFO, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomTypeLayer::notifySuccess), LOGON_USER_INSURE_SUCCESS, nullptr);

	BaseScene::onEnter();

	initRoomList();

	list<tagGameServer *> serverList = DataManager::getInstance()->getCurGameServerList();
	if (serverList.empty() || serverList.size() == 0) {
		SendMsg::sendServerList();
	}

}

void RoomTypeLayer::onExit() {

	BaseScene::onExit();
	NotificationCenter::getInstance()->removeObserver(this, LOGON_LOGON_SUCCESS);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_SERVER_LIST);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_FACE_INFO);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_REFRESH_USER_INFO);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_USER_INSURE_SUCCESS);
	
}

void RoomTypeLayer::touchButton(Ref * obj ,TouchEventType eventType) {
	Button * pButton = (Button *) obj;
	if (eventType == TouchEventType::TOUCH_EVENT_ENDED) {
		int tag = pButton->getTag();
		switch(pButton->getTag()) {
			case UI_ROOM_TYPE::MATCH : {
				string params = DataManager::getInstance()->getParamsFromGameList(m_kindID);
				string serverInfo = "";
				list<tagGameServer *> serverList = DataManager::getInstance()->getCurGameServerList();
				if (serverList.size() > 0) {
					list<tagGameServer *>::iterator it = serverList.begin();
					for (; it != serverList.end(); it++) {
						tagGameServer * pServer = *it;
						if (pServer->wKindID == m_kindID) {

								string strIP = Util::getStringWithTChar(pServer->szServerAddr);
								string strServerName = Util::getStringWithTChar(pServer->szServerName);
								string strOnlineCount = StringUtils::toString(pServer->dwOnLineCount);
								string strFullCount = StringUtils::toString(pServer->dwFullCount);
								string serverDetail = StringUtils::format("%s-%s-%s-%d-%s-%d-%d", strServerName.c_str(), strOnlineCount.c_str(),strFullCount.c_str(), 0, strIP.c_str(), pServer->wServerPort, pServer->wServerID);

								if(serverInfo.empty())
								{
									serverInfo = StringUtils::format("%s" , serverDetail.c_str());
								}else{
									serverInfo = StringUtils::format("%s#%s" ,serverInfo.c_str() , serverDetail.c_str());
								}
						}
					}
				}

				Util::openApp(StringUtils::format("%s|%s|%d|%s|%s|%d|%d", params.c_str(), serverInfo.c_str(), 0, UserInfo::instance()->getAccount().c_str(),UserInfo::instance()->getPassword().c_str(),UserInfo::instance()->getUserId(),UserInfo::instance()->getScore()));

			}
				break;

			case UI_ROOM_TYPE::FREE : {

				string params = DataManager::getInstance()->getParamsFromGameList(m_kindID);
				string serverInfo = "";
				list<tagGameServer *> serverList = DataManager::getInstance()->getCurGameServerList();
				if (serverList.size() > 0) {
					list<tagGameServer *>::iterator it = serverList.begin();
					for (; it != serverList.end(); it++) {
						tagGameServer * pServer = *it;
						if (pServer->wKindID == m_kindID) {

								string strIP = Util::getStringWithTChar(pServer->szServerAddr);
								string strServerName = Util::getStringWithTChar(pServer->szServerName);
								string strOnlineCount = StringUtils::toString(pServer->dwOnLineCount);
								string strFullCount = StringUtils::toString(pServer->dwFullCount);

								string serverDetail = StringUtils::format("%s-%s-%s-%d-%s-%d-%d", strServerName.c_str(), strOnlineCount.c_str(),strFullCount.c_str(), 0, strIP.c_str(), pServer->wServerPort, pServer->wServerID);
								if(serverInfo.empty())
								{
									serverInfo = StringUtils::format("%s" , serverDetail.c_str());
								}else{
									serverInfo = StringUtils::format("%s#%s" ,serverInfo.c_str() , serverDetail.c_str());
								}
						}
					}
				}
				Util::openApp(StringUtils::format("%s|%s|%d|%s|%s|%d|%d", params.c_str(), serverInfo.c_str(), 0, UserInfo::instance()->getAccount().c_str(),UserInfo::instance()->getPassword().c_str(),UserInfo::instance()->getUserId(),UserInfo::instance()->getScore()));

			}
				break;

			case UI_ROOM_TYPE::BACK : {
				Director::getInstance()->popScene();
			}
				break;

			case UI_ROOM_TYPE::SETTING_BTN :        //设置
				SettingLayer::createObject();
				break;

			case UI_ROOM_TYPE::REFRESH_BTN :        //刷新
				SendMsg::getUserScore();
				break;

			case UI_ROOM_TYPE::BUY_BTN :        		//购买
			{
				string userScore = StringUtils::toString(UserInfo::instance()->getScore());
				Util::iPay(userScore,UserInfo::instance()->getAccount());
			}
				break;
			
			case UI_ROOM_TYPE::RECHARGE_BTN :        			//兑换
			{
				string userScore = StringUtils::toString(UserInfo::instance()->getScore());
				Util::iPay(userScore,UserInfo::instance()->getAccount());
			}
				break;

			case UI_ROOM_TYPE::GIVE_BTN :        			//赠送
			{
				GiveLayer::createObject();
			}
				break;

			case UI_ROOM_TYPE::SAVE_TAKE_BTN :        			//存取
				{
					SaveTakeLayer::createObject();
				}
				break;
			default :
				break;
		}
	}
}

void RoomTypeLayer::initRoomList() {

//	Button * match_button = static_cast<Button *>(pView->getChildByTag(UI_ROOM_TYPE::MATCH));
//	match_button->setVisible(false);
//	match_button->addTouchEventListener(this, toucheventselector(RoomTypeLayer::touchButton));

//	Button * konckout_button = static_cast<Button *>(pView->getChildByTag(UI_ROOM_TYPE::MATCH));
//	konckout_button->addTouchEventListener(this, toucheventselector(RoomTypeLayer::touchButton));

	Button * free_button = static_cast<Button *>(pView->getChildByTag(UI_ROOM_TYPE::FREE));
	free_button->addTouchEventListener(this, toucheventselector(RoomTypeLayer::touchButton));

	bool isDisMatch = false;		//
	bool isDisKonckout = false;        //
	bool isDisFree = false;
	bool isUse = false; //

	list<tagGameServer *> serverList = DataManager::getInstance()->getCurGameServerList();
	if (serverList.size() > 0) {
		list<tagGameServer *>::iterator it = serverList.begin();
		for (; it != serverList.end(); it++) {
			tagGameServer * pServer = *it;
			if (pServer->wKindID == m_kindID) {
				isUse = true;
			}
		}
	}

	if(isUse)
	{
		//match_button->setTouchEnabled(true);

		free_button->setTouchEnabled(true);
	}else{
		//match_button->setTouchEnabled(false);
		free_button->setTouchEnabled(false);
	}

}

void RoomTypeLayer::notifySuccess(Ref * obj) {

	NYInfo(TAG , " 接收到头像修改通知 ");

	Widget * m_pUserInfo = dynamic_cast<Widget*>(pRoot->getChildByTag(LAYER_USER_INFO));
	Text * pNickname = static_cast<Text *>(m_pUserInfo->getChildByTag(NICKNAME_TEXT));
	pNickname->setText(UserInfo::instance()->getNickName());

	Text * pUserScore = static_cast<Text *>(m_pUserInfo->getChildByTag(SCORE_TEXT));
	pUserScore->setText(StringUtils::toString(UserInfo::instance()->getScore()));

	ImageView * pHead = static_cast<ImageView *>(m_pUserInfo->getChildByTag(HEAD_IMG));
	if (pHead) {
		string headUrl = StringUtils::format("head_%d.png", UserInfo::instance()->getFaceId());
		if (!FileUtils::getInstance()->isFileExist(headUrl)) {
			if (UserInfo::instance()->getSex() == 1)
				headUrl = "head_0.png";        //默认图片
			else
				headUrl = "head_50.png";
		}

		pHead->loadTexture(headUrl);
		//pHead->setScale(1.7);
		pHead->setTouchEnabled(true);
		pHead->addTouchEventListener(this, toucheventselector(MainLayer::touchHeadImage));
	}
}

void RoomTypeLayer::notifyServerList(Ref * obj) {
	//LoadingLayer::hideModal();
	initRoomList();
}

void RoomTypeLayer::showLoading(Node * _parent) {

	hiddenLoading();
	if (_parent) {
		CCSize pos = _parent->getContentSize();
		//Layer * pLayer = Layer::create();
		Layer * pLayer = LayerColor::create();
		pLayer->setTouchEnabled(true);
		pLayer->setSwallowsTouches(true);
		pLayer->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
		pLayer->setColor(ccc3(0, 0, 0));
		pLayer->setOpacity(100);
		pLayer->setZOrder(20);
		pLayer->setTag(2222222);
		pLayer->setContentSize(CCSizeMake(pos.width, pos.height));
		pLayer->setPosition(Director::getInstance()->getVisibleOrigin());

		//背景
		Layer * pBJ = LayerColor::create();
		pBJ->setTouchEnabled(true);
		pBJ->setSwallowsTouches(true);
		pBJ->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
		pBJ->setColor(ccc3(0, 0, 0));
		pBJ->setOpacity(150);
		pBJ->setZOrder(2);
		pBJ->setContentSize(CCSizeMake(100, 100));
		pBJ->setPosition(ccp(pos.width / 2 - 50, pos.height / 2 - 50));
		pLayer->addChild(pBJ);

		cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/loading/dh.ExportJson");
		cocostudio::Armature * armature = cocostudio::Armature::create("dh");
		armature->getAnimation()->play("Animation1");
		armature->setPosition(ccp(pos.width / 2, pos.height / 2));
		armature->setZOrder(2);
		armature->setScale(0.3f);
		//armature->setTag(2222222);
		pLayer->addChild(armature);
		_parent->addChild(pLayer);
		m_loadingParent = _parent;
	}
	
}

void RoomTypeLayer::hiddenLoading() {
	if (m_loadingParent) {
		auto armature = m_loadingParent->getChildByTag(2222222);
		armature->removeFromParentAndCleanup(true);
		m_loadingParent = nullptr;
	}
}

void RoomTypeLayer::onHiddenLoading(float f) {
	hiddenLoading();
}

