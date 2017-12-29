//
//  MainLayer.cpp
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#include "MainLayer.h"
#include "sdk/sdk.h"
#include "RoomTypeLayer.h"
#include "hall/data/DataManager.h"
#include "sdk/ui/PromptLayer.h"
#include "LoadingLayer.h"
#include "sdk/ui/UpgradeLayer.h"
#include "SettingLayer.h"
#include "sdk/ui/WebView.h"
#include "sdk/util/QFileUtil.h"
#include "LoginLayer.h"
#include "UpdateHeadLayer.h"
#include "GiveLayer.h"
#include "SaveTakeLayer.h"
#include "ConfirmLayer.h"
#include "ConnectGameScene.h"
#include "hall/ui/LoadingScene.h"

#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

//UI button
enum BUTTON_TAG {

	CUSTOMER_BTN = 16,        //
	TRUMPET_BTN = 67,
	SETTING_BTN = 70,
	PACKAGE_BTN = 4365,
	SHOP_BTN = 69,
	RECHARGE_BTN = 4361,
	GIVE_BTN = 4363,
	LAYER_BOTTOM = 65,
	LAYER_OPTION = 15,

	NOTICE_SVIEW = 22,
	NOTICE_PANEL = 139,
	NOTICE_LABEL = 23,

	//有用
	LAYER_USER_INFO = 1,
	REFRESH_BTN = 4139,
	HEAD_IMG = 3,
	NICKNAME_TEXT = 7,
	SCORE_TEXT = 13,
	SCORE_BANK_TEXT = 7459900,
	HALL_GAME_ID = 7459901,

	SAVE_TAKE_BTN = 7459903,
	BUY_BTN = 7459905,

	NOTICE = 7460000,

	SWITCH_BTN = 7459898,
	HALL_CLOSE = 17,

	CONTENT_SCROLL_VIEW = 7459906,
	CONTENT_CHECK       =  5000,
};

#define TAG "MainLayer"

#define LOADING_ARMATURE 101
#define BASE_TAG 500
#define DOWNLOAD_FILE ""

//static GameClientManager * assetManager = NULL;

MainLayer::MainLayer() {
	_pathToSave = "";
	_showDownloadInfo = NULL;
	menuStat = false;
	m_loadingParent = nullptr;
	beginTouchX = 0;
	beginTouchY = 0;
	m_nPageCount = 0;
	//pLayer = nullptr;
}

MainLayer::~MainLayer() {
//	CC_SAFE_DELETE(assetManager);
}

void MainLayer::initView() {


	CCSize visibleSize = Director::getInstance()->getVisibleSize();
	pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("hall.json");
	pRoot->setAnchorPoint(CCPoint(0.5f, 0.5f));
	pRoot->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(pRoot);

//	Widget * m_pBottom = dynamic_cast<Widget*>(pRoot->getChildByTag(LAYER_BOTTOM));
	////背包
	//Button * m_pPackage = dynamic_cast<Button*>(m_pBottom->getChildByTag(PACKAGE_BTN));
	//m_pPackage->setVisible(false);
	//m_pPackage->addTouchEventListener(this, toucheventselector(MainLayer::touchButton));

	//滚动公告
	/*Widget * pNotice = static_cast<Widget *>(pRoot->getChildByTag(NOTICE));
	 pNotice->setVisible(false);*/

	//商城
	/*Button * m_pShop = dynamic_cast<Button*>(m_pBottom->getChildByTag(SHOP_BTN));
	 m_pShop->setVisible(false);
	 m_pShop->addTouchEventListener(this, toucheventselector(MainLayer::touchButton));*/

	//充值
	Button * m_pRechange = dynamic_cast<Button*>(pRoot->getChildByTag(BUY_BTN));
	m_pRechange->loadTextures("hall/btn_recharge_d.png","","hall/btn_recharge_n.png");
	bool isUpLoaded = UserDefault::getInstance()->getBoolForKey("uploaded");
	m_pRechange->setBright(isUpLoaded);
	m_pRechange->addTouchEventListener(this, toucheventselector(MainLayer::touchButton));

	//存取款
	Button * pSaveTake = static_cast<Button *>(pRoot->getChildByTag(SAVE_TAKE_BTN));
	pSaveTake->addTouchEventListener(this, toucheventselector(MainLayer::touchButton));

	//赠送
//	Button * pGive = static_cast<Button *>(m_pBottom->getChildByTag(GIVE_BTN));
//	pGive->addTouchEventListener(this, toucheventselector(MainLayer::touchButton));

//	Widget * m_pOption = dynamic_cast<Widget*>(pRoot->getChildByTag(LAYER_OPTION));
	//大喇叭
	/*Button * m_pTrumpet = dynamic_cast<Button*>(m_pOption->getChildByTag(TRUMPET_BTN));
	 m_pTrumpet->addTouchEventListener(this, toucheventselector(MainLayer::touchButton));
	 m_pTrumpet->setVisible(false);*/

	//客服
	/*Button * pHuanle = static_cast<Button *>(m_pOption->getChildByTag(CUSTOMER_BTN));
	 pHuanle->addTouchEventListener(this, toucheventselector(MainLayer::touchButton));
	 pHuanle->setVisible(false);*/

	//设置
//	Button * m_pSetting = dynamic_cast<Button*>(m_pOption->getChildByTag(SETTING_BTN));
//	m_pSetting->addTouchEventListener(this, toucheventselector(MainLayer::touchButton));

	Widget * m_pUserInfo = dynamic_cast<Widget*>(pRoot->getChildByTag(LAYER_USER_INFO));

	//刷新
	Button * m_pRefresh = dynamic_cast<Button*>(m_pUserInfo->getChildByTag(REFRESH_BTN));
	m_pRefresh->addTouchEventListener(this, toucheventselector(MainLayer::touchButton));

	//关闭
	Button * m_pClose = dynamic_cast<Button*>(pRoot->getChildByTag(HALL_CLOSE));
	m_pClose->addTouchEventListener(this, toucheventselector(MainLayer::touchButton));

	//切换账号
	Button * pSwitch = dynamic_cast<Button*>(pRoot->getChildByTag(SWITCH_BTN));
	pSwitch->addTouchEventListener(this, toucheventselector(MainLayer::touchButton));

	//onSchedule(1);
	PageView* pTmpView = static_cast<ui::PageView*>(pRoot->getChildByTag(CONTENT_SCROLL_VIEW));
	pView = MyPageView::create();
	pView->setTouchEnabled(true);
	pView->addEventListenerPageView(this, pagevieweventselector(MainLayer::pageViewEvent));
	pView->setPosition(pTmpView->getPosition());
	pView->setContentSize(pTmpView->getContentSize());
	pView->setSize(pTmpView->getSize());
	pView->setActionTag(pTmpView->getTag()-2000);
	pView->setAnchorPoint(pTmpView->getAnchorPoint());
	pRoot->addChild(pView);
	pView->setListener(this);
	pTmpView->removeFromParent();

	CheckBox * pCurPageCheckBox = dynamic_cast<CheckBox*>(pRoot->getChildByTag(CONTENT_CHECK));
	m_strCurSelType = "0";
	//pView->is
	//获得已安装的apk
	char* pChar = Util::getAppList();
	std::string appstr = "";
	if (pChar != NULL) {
		appstr = pChar;
	}

	std::vector < std::string > vec;
	Util::split(appstr, "|", &vec);

	rapidjson::Document _doc;
	bool bRet = false;
	ssize_t size = 0;

	unsigned char*ch = cocos2d::CCFileUtils::getInstance()->getFileData("game_list.json", "r", &size);
	std::string contentStr = std::string((const char*) ch, size);

	CCLog(" game list :: %d %s", size, contentStr.c_str());
	
	_doc.Parse<0>(contentStr.c_str());
	if (_doc.HasParseError()) {
		CCLOG("GetParseError%s\n", _doc.GetParseError());
	}

	rapidjson::Value& pArray = _doc["games"];
	if (!pArray.IsArray())
		return;

	int nPosX = 50;
	int nPosY = 345;

	Layout* pPageTmp;
	for (int i = 0; i < pArray.Capacity(); ++i) {
		int nIndex = i%9;
		if (nIndex==0)
		{
			nPosX = 50;
			nPosY = 345;

			pPageTmp = Layout::create();
			pView->addPage(pPageTmp);

			CheckBox* pTmp = CheckBox::create("commonresource/page_no_select.png",
                                          "commonresource/page_no_select.png",
                                          "commonresource/page_select.png",
                                          "commonresource/page_select.png",
                                          "commonresource/page_select.png");
			pTmp->setPosition(pCurPageCheckBox->getPosition()+Vec2(m_nPageCount*40,0));
			pTmp->setTag(pCurPageCheckBox->getTag()+m_nPageCount+1);
			pTmp->setTouchEnabled(false);
			if (i==0)
			{
				pTmp->setSelectedState(true);
			}
			else
			{
				pTmp->setSelectedState(false);
			}
			pRoot->addChild(pTmp);
			m_nPageCount++;
		}
		else
		{
			if (nIndex%3==0)
			{
				nPosX = 50;
				nPosY -= 170;
			}
			else
			{
				nPosX += 200;
			}
		}
		const rapidjson::Value& valueEnt = pArray[i];
		if (valueEnt.HasMember("name") && valueEnt.HasMember("click_up") && valueEnt.HasMember("click_down") && valueEnt.HasMember("download")) {
			const rapidjson::Value& vName = valueEnt["name"];
			std::string name = vName.GetString();

			const rapidjson::Value& vclick_up = valueEnt["click_up"];
			std::string click_up = vclick_up.GetString();

			const rapidjson::Value& vclick_down = valueEnt["click_down"];
			std::string click_down = vclick_down.GetString();

			const rapidjson::Value& vdownload = valueEnt["download"];
			std::string download = vdownload.GetString();

			const rapidjson::Value& vpackage = valueEnt["package"];
			std::string package = vpackage.GetString();

			const rapidjson::Value& vKindid = valueEnt["kindid"];
			std::string kindid = vKindid.GetString();

			const rapidjson::Value& vVersion = valueEnt["version"];
			std::string version = vVersion.GetString();

			open_map.insert(map<int, std::string>::value_type(BASE_TAG + i, package + "|" + name + "|" + kindid));
			bool isInstall = false;
			for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
				string strName = *it;
				if (strName == name) {
					isInstall = true;
					break;
				}
			}

			if (isInstall) {

				auto settingMenuItem = Sprite::create(click_up);
				settingMenuItem->setAnchorPoint(CCPoint::ZERO);
				settingMenuItem->setPosition(Vec2(nPosX, nPosY));
				settingMenuItem->setTag(BASE_TAG + i);
				pPageTmp->addChild(settingMenuItem);

				DataManager::getInstance()->initConfigGameList(std::atoi(kindid.c_str()), package + "|" + name);

				auto buttonListener = EventListenerTouchOneByOne::create();
				buttonListener->setSwallowTouches(false);

				buttonListener->onTouchBegan = [&](Touch * touch ,Event * event) {

					Vec2 beginVec = touch->getLocation();
					beginTouchX = beginVec.x;
					beginTouchY = beginVec.y;

					Sprite * pBtn = static_cast<Sprite *>(event->getCurrentTarget());
					CCPoint pt = pBtn->convertToNodeSpace(beginVec);

					int w = pBtn->getContentSize().width;
					int h = pBtn->getContentSize().height;

					CCRect rc(0,0,w,h);        //

					if(rc.containsPoint(pt))
					{
						pBtn->setScale(1.01f);
					}
					return true;
				};

				buttonListener->onTouchMoved = [&](Touch * touch ,Event * event) {
				};

				//游戏升级
				initDownloadDir();

				_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener, settingMenuItem);
				
			} else {
				auto settingMenuItem = Sprite::create(click_up);
				settingMenuItem->setAnchorPoint(CCPoint::ZERO);
				settingMenuItem->setPosition(Vec2(nPosX, nPosY));
				settingMenuItem->setTag(BASE_TAG + i);
				pPageTmp->addChild(settingMenuItem);

				download_map.insert(map<int, std::string>::value_type(settingMenuItem->getTag(), download + "|" + version));

				auto buttonListener = EventListenerTouchOneByOne::create();
				buttonListener->setSwallowTouches(false);

				buttonListener->onTouchBegan = [&](Touch * touch ,Event * event) {

					Vec2 beginVec = touch->getLocation();
					beginTouchX = beginVec.x;
					beginTouchY = beginVec.y;

					Sprite * pBtn = static_cast<Sprite *>(event->getCurrentTarget());
					CCPoint pt = pBtn->convertToNodeSpace(beginVec);

					int w = pBtn->getContentSize().width;
					int h = pBtn->getContentSize().height;

					CCRect rc(0,0,w,h);        //

						if(rc.containsPoint(pt))
						{
							pBtn->setScale(1.01f);
						}
						return true;
					};

				buttonListener->onTouchMoved = [&](Touch * touch ,Event * event) { };

				buttonListener->onTouchEnded = [&](Touch * touch ,Event * event) {
					Sprite * pBtn = static_cast<Sprite *>(event->getCurrentTarget());
					CCLog(" onTouchEnded tag %d",pBtn->getTag());

					//OpenGL坐标系位置
					Vec2 endVec = touch->getLocation();
					//触发点相对MenuItem的坐标
					CCPoint pt = pBtn->convertToNodeSpace(endVec);
					CCLog("  relative position x:%f  y:%f",pt.x,pt.y);

					int w = pBtn->getContentSize().width;
					int h = pBtn->getContentSize().height;

					CCRect rc(0,0,w,h);//

					if(abs(endVec.x - beginTouchX) < 10 && rc.containsPoint(pt))
					{
						CCLog(" click download %d",pBtn->getTag());
						pBtn->setScale(1.0f);
						downloadFile((Object*)pBtn);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
						if (UserDefault::getInstance()->getBoolForKey("uploaded"))
						{
							//删除之前复制的资源
							for (auto it = open_map.begin(); it != open_map.end(); ++it)
							{
								std::string openAppParam = it->second;
								std::vector<std::string> vec;
								Util::split(openAppParam, "|", &vec);
								string package = vec.front();

								std::string writePath = FileUtils::getInstance()->getWritablePath();
								writePath += package;
								for (auto it = writePath.begin(); it != writePath.end(); ++it)
								{
									if ((*it) == '/')
										(*it) = '\\';
								}

								std::string cmdline = StringUtils::format("del %s\\*.* /q", writePath.c_str());
								system(cmdline.c_str());
								system("y");

								cmdline.clear();
								cmdline = StringUtils::format("rd /s %s /q", writePath.c_str());
								system(cmdline.c_str());
								system("y");
							}

							//复制当前子游戏的资源
							std::string openAppParam = open_map[pBtn->getTag()];
							std::vector < std::string > vec;
							Util::split(openAppParam, "|", &vec);

							string path = vec.front();
							std::string cmdline = "";
							std::string searchPath = "";
							std::string writePath = "";

							vector<std::string> vec_t = FileUtils::getInstance()->getSearchPaths();
							searchPath = vec_t.at(0);
							writePath = FileUtils::getInstance()->getWritablePath();

							searchPath += "GameRes\\";
							searchPath += path;
							
							writePath += path;
							writePath += '\\';

							for (auto it = searchPath.begin(); it != searchPath.end(); ++it)
							{
								if ((*it) == '/')
									(*it) = '\\';
							}

							for (auto it = writePath.begin(); it != writePath.end(); ++it)
							{
								if ((*it) == '/')
									(*it) = '\\';
							}

							cmdline = StringUtils::format("xcopy %s %s /s /e /y", searchPath.c_str(), writePath.c_str());
							system(cmdline.c_str());
						}
#endif

					}
				};
				_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener, settingMenuItem);
			}
		}
	}

	pCurPageCheckBox->setVisible(false);
	if (m_nPageCount==1)
	{
		CheckBox * pTmpCheckBox = dynamic_cast<CheckBox*>(pRoot->getChildByTag(CONTENT_CHECK+1));
		pTmpCheckBox->setVisible(false);
	}

	//上面的MENU
	m_TabGameType[0] = CCMenuItemImage::create("hall/tab_0_n.png", "hall/tab_0_s.png", "hall/tab_0_s.png", this,menu_selector(MainLayer::MenuSelect));
	m_TabGameType[0]->setTag(6000);
	m_TabGameType[1] = CCMenuItemImage::create("hall/tab_1_n.png", "hall/tab_1_s.png", "hall/tab_1_s.png", this,menu_selector(MainLayer::MenuSelect));
	m_TabGameType[1]->setTag(6001);
	m_TabGameType[2] = CCMenuItemImage::create("hall/tab_2_n.png", "hall/tab_2_s.png", "hall/tab_2_s.png", this,menu_selector(MainLayer::MenuSelect));
	m_TabGameType[2]->setTag(6002);
	m_TabGameType[3] = CCMenuItemImage::create("hall/tab_3_n.png", "hall/tab_3_s.png", "hall/tab_3_s.png", this,menu_selector(MainLayer::MenuSelect));
	m_TabGameType[3]->setTag(6003);
	CCMenu* pMenu = CCMenu::create(m_TabGameType[0], m_TabGameType[1], m_TabGameType[2],m_TabGameType[3],NULL);
	pMenu->alignItemsHorizontallyWithPadding(0);
	pMenu->setPosition(ccp(850, 680));
	m_TabGameType[0]->selected();
	m_TabGameType[1]->unselected();
	m_TabGameType[2]->unselected();
	m_TabGameType[3]->unselected();
	pRoot->addChild(pMenu);


}

void MainLayer::onEnter() {

	BaseScene::onEnter();
	//SoundManager::instance()->playBgMusic();
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainLayer::notifySuccess), LOGON_LOGON_SUCCESS, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainLayer::notifyFail), LOGON_LOGON_FAILURE, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainLayer::notifySuccess), LOGON_FACE_INFO, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainLayer::notifyMessage), LOGON_OPERATE_FAILURE, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainLayer::notifySuccess), LOGON_REFRESH_USER_INFO, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainLayer::notifySuccess), LOGON_USER_INSURE_SUCCESS, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainLayer::notifySuccess), LOGON_TRANSFER_SUCCESS, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainLayer::connectNetSucceed), CONNECT_NETWORK_FINISH, nullptr);
	
	//schedule(schedule_selector(MainLayer::scheduleNotice), 0.2f);
	//LoginLayer::createObject(pRoot);
	if (UserInfo::instance()->getUserId() == 0) {

		LoginLayer::createObject(pRoot);
	} else {
		onSchedule(1);
	}

	//notifySuccess(nullptr);
//	SendMsg::refreshUserScore();
//	int sign = Util::getSignal();
//	if(sign <= 0)
//	{
//		PromptLayer::createObject("当前网络不可用!");
//	}

}

void MainLayer::onExit() {

	NotificationCenter::getInstance()->removeObserver(this, LOGON_LOGON_SUCCESS);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_LOGON_FAILURE);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_FACE_INFO);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_OPERATE_FAILURE);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_REFRESH_USER_INFO);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_USER_INSURE_SUCCESS);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_TRANSFER_SUCCESS);
	NotificationCenter::getInstance()->removeObserver(this, CONNECT_NETWORK_FINISH);
	
//	this->unschedule(schedule_selector(MainLayer::scheduleNotice));

	BaseScene::onExit();
}



void MainLayer::connectNetSucceed(Ref * obj)
{
	auto inter = (Integer*)obj;
	if (inter)
	{
		int logonType = inter->getValue();
		//原来的登录流程
		//通用请求RC4key可以传主副命令
		//SendMsg::SendGetRc4KeyWithSubCmdID(MDM_GP_GET_KEY, logonType, 0);//在这里向服务器发送
		//原来的登录流程
		UserInfo::instance()->httpRequest();
		UserInfo::instance()->SendType = 0;
		UserInfo::instance()->mainCmd = MDM_GP_GET_KEY;
		UserInfo::instance()->subCmd = logonType;
	}

}


//回调通知的接口
void MainLayer::notifySuccess(Ref * object) {
	LoadingScene::hideModal();
	//MainController::getInstance()->connectHallFinish();
	log("MainLayer::notifySuccess %s", "receive msg");///"收到用户的更新消息"
	updateUserInfo();
	auto myscheduler = Director::getInstance()->getScheduler();
	if (!myscheduler->isScheduled("CheckConnectRoomInfo", MainController::getInstance()));
	    //MainController::getInstance()->checkConnectRoomInfo();
}

//回调通知的接口
void MainLayer::notifyFail(Ref * object) {

	LoadingScene::hideModal();
	//MainController::getInstance()->connectHallFinish();
	CMD_MB_LogonFailure * pFailure = (CMD_MB_LogonFailure *) object;

	string szError = Util::getStringWithTChar(pFailure->szDescribeString);
	NYInfo(TAG, " 登录失败回调 ");
	PromptLayer::createObject((char *) szError.c_str());

}

//弹出信息
void MainLayer::notifyMessage(Ref * object) {

//	CMD_MB_LogonFailure * pFailure = (CMD_MB_LogonFailure *) object;
//
//	char * szError = (char *) object;
//	PromptLayer::createObject(szError);

}

void MainLayer::onSchedule(float t) {

	if (UserInfo::instance()->getUserId() > 0) {
		this->unschedule(schedule_selector(MainLayer::onSchedule));
		updateUserInfo();
	}
}

void MainLayer::updateUserInfo() {
	//if (UserInfo::instance()->getUserId() > 0)
	//{
	Widget * m_pUserInfo = dynamic_cast<Widget*>(pRoot->getChildByTag(LAYER_USER_INFO));
	Text * pNickname = static_cast<Text *>(m_pUserInfo->getChildByTag(NICKNAME_TEXT));
	pNickname->setText(UserInfo::instance()->getNickName());

	Text * pUserScore = static_cast<Text *>(m_pUserInfo->getChildByTag(SCORE_TEXT));
//	pUserScore->setText("游戏币:"+StringUtils::toString(UserInfo::instance()->getScore()));
	pUserScore->setText(StringUtils::format("%s%lld",U2F("游戏币:").c_str(),UserInfo::instance()->getScore()));

	Text * pBankScore = static_cast<Text *>(m_pUserInfo->getChildByTag(SCORE_BANK_TEXT));
//	pBankScore->setText("保险柜:"+StringUtils::toString(UserInfo::instance()->getInsureScore()));
	pBankScore->setText(StringUtils::format("%s%lld",U2F("保险柜:").c_str(),UserInfo::instance()->getInsureScore()));

	Text * pGameID = static_cast<Text *>(m_pUserInfo->getChildByTag(HALL_GAME_ID));
	pGameID->setText("ID:"+StringUtils::toString(UserInfo::instance()->getGameId()));

	ImageView * pHead = static_cast<ImageView *>(m_pUserInfo->getChildByTag(HEAD_IMG));
	if (pHead) {
		string headUrl = StringUtils::format("head/head_%d.png", UserInfo::instance()->getFaceId());
		if (!FileUtils::getInstance()->isFileExist(headUrl)) {
			if (UserInfo::instance()->getSex() == 1)
				headUrl = "head/head_0.png";        //默认图片
			else
				headUrl = "head/head_50.png";
		}

		pHead->loadTexture(headUrl);
		//pHead->setScale(1.7f);
		pHead->setTouchEnabled(true);
		pHead->addTouchEventListener(this, toucheventselector(MainLayer::touchHeadImage));
	}
	//}

	//启动消息
	Text * pStartMessage = static_cast<Text *>(pRoot->getChildByTag(NOTICE));
	pStartMessage->setColor(ccc3(24,176,201));
	pStartMessage->setText(UserInfo::instance()->getStartMessage());
}

//滚动公告定时器
void MainLayer::scheduleNotice(float t) {
	Widget * pNotice = static_cast<Widget *>(pRoot->getChildByTag(NOTICE));
	pNotice->setTouchEnabled(false);
	ui::ScrollView * pScoll = dynamic_cast<ui::ScrollView *>(pNotice->getChildByTag(NOTICE_SVIEW));
	pScoll->setTouchEnabled(false);

	Widget * pWidget = dynamic_cast<Widget *>(pScoll->getChildByTag(NOTICE_PANEL));
	pWidget->setTouchEnabled(false);
	float width1 = pWidget->getContentSize().width;
	float x = pWidget->getPositionX();
	if (x + width1 > 0) {
		pWidget->setPositionX(x - 10);
	} else {
		string strMessage = "";        //DataManager::getInstance()->getMessage();
		CCLog(U2F("公告消息：%s").c_str(), strMessage.c_str());
		if (!strMessage.empty()) {
			pNotice->setVisible(true);
			setNotice(strMessage);

		} else {
			pNotice->setVisible(false);
		}
		pWidget->setPositionX(width1);

		//SendMsg::GetMatchResult();

	}
}

void MainLayer::setNotice(string strNotice) {
	Widget * pNotice = static_cast<Widget *>(pRoot->getChildByTag(NOTICE));
	ui::ScrollView * pScoll = dynamic_cast<ui::ScrollView *>(pNotice->getChildByTag(NOTICE_SVIEW));
	CCSize scrollSize = pScoll->getContentSize();

	Widget * pWidget = dynamic_cast<Widget *>(pScoll->getChildByTag(NOTICE_PANEL));
	Text * pLabel = static_cast<Text *>(pWidget->getChildByTag(NOTICE_LABEL));

	if (pLabel) {
		pLabel->setTouchEnabled(false);
		pLabel->setText(strNotice);
		CCSize labelSize = pLabel->getContentSize();
		pScoll->setInnerContainerSize(CCSize(labelSize.width + scrollSize.width, scrollSize.height));
		pLabel->setColor(Color3B::YELLOW);
		pWidget->setContentSize(labelSize);
	}
}

//button event
void MainLayer::touchButton(Ref * obj ,TouchEventType eventType) {

	Button * pButton = (Button *) obj;

	if (eventType == TouchEventType::TOUCH_EVENT_ENDED) {
		switch(pButton->getTag()) {

			case SETTING_BTN :        //设置
			{
				SettingLayer::createObject();
			}
				break;
			case REFRESH_BTN :        //刷新
			{
										  UserInfo::instance()->httpRequest();
										  UserInfo::instance()->SendType = 0;
										  UserInfo::instance()->mainCmd = MDM_GP_GET_KEY;
										  UserInfo::instance()->subCmd = SUB_GP_GETKEY_QUERYSCORE;
			}
				break;
			case BUY_BTN :        		//购买
			{
				/*string userScore = StringUtils::toString(UserInfo::instance()->getScore());
				Util::iPay(userScore, UserInfo::instance()->getAccount());*/
				//上传前资源测试开关
				bool bright = pButton->isBright();
				UserDefault::getInstance()->setBoolForKey("uploaded", !bright);
				pButton->setBright(!bright);
			}
				break;
				//case PACKAGE_BTN :        		//背包
				//{
				//	/*BaseScene * pLayer = new PackageLayer();
				//	Director::getInstance()->pushScene(BaseScene::createScene(pLayer));
				//	pLayer->release();*/
				//}
				//	break;
				//case SHOP_BTN :        			//商城
				//{
				//	/*BaseScene * pLayer = new ExchangePhysicalLayer();
				//	Director::getInstance()->pushScene(BaseScene::createScene(pLayer));
				//	pLayer->release();*/
				//}
				//	break;
			case RECHARGE_BTN :        			//兑换
			{
//				BaseScene * pLayer = new PayModeLayer();
//				Director::getInstance()->pushScene(BaseScene::createScene(pLayer));
//				pLayer->release();

				string userScore = StringUtils::toString(UserInfo::instance()->getScore());
				Util::iPay(userScore, UserInfo::instance()->getAccount());
			}
				break;

			case SAVE_TAKE_BTN :       				//存取款
			{
				SaveTakeLayer::createObject();
			}
				break;

			case GIVE_BTN :        			//赠送
			{
				GiveLayer::createObject();
			}
				break;

			case HALL_CLOSE :        			//关闭
			{
				//修改密码
				if (SettingLayer::s_pInstance) {
					SettingLayer::hideModal();
				}

				//大喇叭
				/*if (TrumpetLayer::s_pInstance) {
				 TrumpetLayer::hideModal();
				 }*/

				BaseScene::onBackPressed();
			}
				break;
			case SWITCH_BTN:
			{
				LoginLayer::createObject(pRoot);
			}
			break;

		}
	}
}

void MainLayer::touchHeadImage(Ref * obj ,TouchEventType eventType) {

	if (eventType == TouchEventType::TOUCH_EVENT_ENDED) {
		/*BaseScene * pLayer = new UserInfoLayer();
		 Director::getInstance()->pushScene(BaseScene::createScene(pLayer));
		 pLayer->release();*/
		UpdateHeadLayer::createObject();
	}
}

//下载操作
void MainLayer::downloadFile(Ref * obj) {

	//if (menuStat == false) {

	Sprite * pButton = (Sprite *) obj;
	//std::string zipUrl = download_map[pButton->getTag()];
	std::string openAppParam = download_map[pButton->getTag()];
	std::vector < std::string > vec;
	Util::split(openAppParam, "|", &vec);

	std::string zipUrl = vec[0];
	std::string versionUrl = vec[1];
	CCLog("   download url : %s  version url:%s", zipUrl.c_str(), versionUrl.c_str());
	//assetManager = getAssetManager(versionUrl, zipUrl);

	initLoad(obj);        //显示下载

	initDownloadDir();        //初始化下载路径

	size_t index = zipUrl.rfind("/");
	string url = zipUrl.substr(index + 1);
	index = url.find(".");
	std::string name = url.substr(0, index);
	name.append(".apk");
	saveApk = _pathToSave + name;
	CCLog(" save path == %s", saveApk.c_str());

	MainLayer::upgrade (nullptr);

	//menuStat = true;
	//}
}

//游戏版本升级
void MainLayer::onUpgrade(int tag) {

	//if (menuStat == false) {

	Sprite * obj = (Sprite *) pView->getChildByTag(tag);
	std::string openAppParam = download_map[tag];
	std::vector < std::string > vec;
	Util::split(openAppParam, "|", &vec);

	std::string zipUrl = vec[0];
	std::string versionUrl = vec[1];
	CCLog("   upgrade : %s  %s ", zipUrl.c_str(), versionUrl.c_str());
	//assetManager = getAssetManager(versionUrl, zipUrl);

	initLoad(obj);

	initDownloadDir();        //初始化下载路径

	size_t index = zipUrl.rfind("/");
	string url = zipUrl.substr(index + 1);
	index = url.find(".");
	std::string name = url.substr(0, index);
	name.append(".apk");
	saveApk = _pathToSave + name;
	CCLog(" save path == %s", saveApk.c_str());

	MainLayer::upgrade (nullptr);

	//menuStat = true;
	//}
}

//打开已安装apk
void MainLayer::openApp(Ref * obj) {

	MenuItemImage * pButton = (MenuItemImage *) obj;
	std::string openAppParam = open_map[pButton->getTag()];
	std::vector < std::string > vec;
	Util::split(openAppParam, "|", &vec);
	
	string kindID = vec.back();
	CCLog(" 游戏类型 ： %s ", kindID.c_str());

	string params = DataManager::getInstance()->getParamsFromGameList(std::atoi(kindID.c_str()));
	string serverInfo = "";
	list<tagGameServer *> serverList = DataManager::getInstance()->getCurGameServerList();
	if (serverList.size() > 0) {
		list<tagGameServer *>::iterator it = serverList.begin();
		for (; it != serverList.end(); it++) {
			tagGameServer * pServer = *it;
			if (pServer->wKindID == std::atoi(kindID.c_str())) {

				string strIP = Util::getStringWithTChar(pServer->szServerAddr);
				string strServerName = Util::getStringWithTChar(pServer->szServerName);
				string strOnlineCount = StringUtils::toString(pServer->dwOnLineCount);
				string strFullCount = StringUtils::toString(pServer->dwFullCount);
				string serverDetail = StringUtils::format("%s-%s-%s-%lld-%s-%d-%d", strServerName.c_str(), strOnlineCount.c_str(), strFullCount.c_str(), pServer->lMinServerScore,
						strIP.c_str(), pServer->wServerPort, pServer->wServerID);

				if (serverInfo.empty()) {
					serverInfo = StringUtils::format("%s", serverDetail.c_str());
				} else {
					serverInfo = StringUtils::format("%s#%s", serverInfo.c_str(), serverDetail.c_str());
				}
			}
		}
	}

	Util::openApp(
			StringUtils::format("%s|%s|%d|%s|%s|%d|%lld", params.c_str(), serverInfo.c_str(), 0, UserInfo::instance()->getAccount().c_str(),
					UserInfo::instance()->getPassword().c_str(), UserInfo::instance()->getUserId(), UserInfo::instance()->getScore()));

}

//初始化下载信息
void MainLayer::initLoad(Ref * pNode) {

	Sprite * pMenuImage = (Sprite*) pNode;
	CCSize pz = pMenuImage->getContentSize();

	auto pLayer = Layer::create();
	pLayer->setPosition(pMenuImage->getPosition());
	pLayer->setAnchorPoint(Vec2(pz.width / 2, pz.height / 2));
	pLayer->setContentSize(pz);
	GLubyte parentOpacity = 100;

	_showDownloadInfo = Label::createWithSystemFont("", "", 35);
	_showDownloadInfo->setAnchorPoint(CCPoint(0.5f, 0.5f));
	_showDownloadInfo->setPosition(Vec2(pz.width / 2 - 20, pz.height / 2));
	//_showDownloadInfo->setContentSize(CCSize(80, 40));
	_showDownloadInfo->setColor(Color3B::RED);
	_showDownloadInfo->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);

	Sprite * sprite = Sprite::create("hall/hall_hidden.png");

	sprite->setOpacity(parentOpacity);
	sprite->setCascadeOpacityEnabled(true);
	sprite->setAnchorPoint(CCPoint::ZERO);
	sprite->setPosition(CCPoint::ZERO);

	ct = ProgressTimer::create(sprite);
	ct->setPosition(CCPoint::ZERO);
	ct->setAnchorPoint(CCPoint::ZERO);
	ct->setPercentage(0);
	ct->setType(kCCProgressTimerTypeRadial);
	ct->setReverseProgress(false);

	pLayer->setTouchEnabled(false);
	pLayer->addChild(ct);
	pLayer->addChild(_showDownloadInfo);
	pView->addChild(pLayer, 2);

}

void MainLayer::onError(AssetsManager::ErrorCode errorCode) {
	if (errorCode == AssetsManager::ErrorCode::NO_NEW_VERSION) {
		//_showDownloadInfo->setString("已是最新版本");

	} else if (errorCode == AssetsManager::ErrorCode::NETWORK) {
		_showDownloadInfo->setString("网络连接错误,请重试");
		PromptLayer::createObject("网络中断,请检查网络状况");

	} else if (errorCode == AssetsManager::ErrorCode::CREATE_FILE) {
		_showDownloadInfo->setString("创建文件错误");
		PromptLayer::createObject("创建文件错误,请重试");
	}

	scheduleOnce(schedule_selector(MainLayer::onRefreshPage), 0.5f);
}

void MainLayer::onProgress(int percent) {
	if (percent < 0) {
		return;
	}
	char progress[20];
	snprintf(progress, 20, "%d%%", percent);

	_showDownloadInfo->setString(progress);
	ct->setPercentage(percent);
}

void MainLayer::onSuccess() {

	_showDownloadInfo->setString("安装中..");
	Util::upgrade(saveApk);
	scheduleOnce(schedule_selector(MainLayer::onRefreshPage), 0.1f);

	menuStat = false;
}
//
//GameClientManager* MainLayer::getAssetManager(std::string versionURL ,std::string downloadURL) {
//
//	if (!assetManager) {
//		assetManager = new GameClientManager(downloadURL.c_str(), versionURL.c_str(), _pathToSave.c_str());
//		assetManager->setDelegate(this);
//		assetManager->setConnectionTimeout(10);
//	} else {
//		assetManager->setVersionFileUrl(versionURL.c_str());
//		assetManager->setPackageUrl(downloadURL.c_str());
//	}
//
//	return assetManager;
//}

void MainLayer::initDownloadDir() {

	_pathToSave = FileUtils::getInstance()->getWritablePath();
	_pathToSave += DOWNLOAD_FILE;

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	DIR *pDir = NULL;
	pDir = opendir(_pathToSave.c_str());
	if (!pDir) {
		mkdir(_pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
#else
	if ((GetFileAttributesA(_pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(_pathToSave.c_str(), 0);
	}
#endif

	NYInfo(TAG, "创建文件路径 = %s", _pathToSave.c_str());
}

void MainLayer::upgrade(Ref * pSender) {

//	NYInfo(TAG, "  升级中... ");
//	assetManager->setStoragePath(_pathToSave.c_str());
//	assetManager->setSearchPath();
//	assetManager->update();
}

void MainLayer::onRefreshPage(float f) {
	BaseScene * pLayer = new MainLayer();
	Director::getInstance()->replaceScene(BaseScene::createScene(pLayer));
	pLayer->release();
}

void MainLayer::pageViewEvent(Ref* pSender, PageViewEventType type)
{
	if (type == PageViewEventType::PAGEVIEW_EVENT_TURNING)
	{
		int nIndex = pView->getCurPageIndex();
		for (int i=0; i<m_nPageCount;i++)
		{
			CheckBox * pCurPageCheckBox = dynamic_cast<CheckBox*>(pRoot->getChildByTag(CONTENT_CHECK+i+1));
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
}

void MainLayer::MenuSelect(Ref* obj)
{
	switch(((CCMenuItemImage*)obj)->getTag())
	{
	case 6000:
		m_TabGameType[0]->selected();
		m_TabGameType[1]->unselected();
		m_TabGameType[2]->unselected();
		m_TabGameType[3]->unselected();
		m_strCurSelType = "0";
		break;
	case 6001:
		m_TabGameType[0]->unselected();
		m_TabGameType[1]->selected();
		m_TabGameType[2]->unselected();
		m_TabGameType[3]->unselected();
		m_strCurSelType = "1";
		break;
	case 6002:
		m_TabGameType[0]->unselected();
		m_TabGameType[1]->unselected();
		m_TabGameType[2]->selected();
		m_TabGameType[3]->unselected();
		m_strCurSelType = "2";
		break;
	case 6003:
		m_TabGameType[0]->unselected();
		m_TabGameType[1]->unselected();
		m_TabGameType[2]->unselected();
		m_TabGameType[3]->selected();
		m_strCurSelType = "3";
		break;
	default:
		m_TabGameType[0]->selected();
		m_TabGameType[1]->unselected();
		m_TabGameType[2]->unselected();
		m_TabGameType[3]->unselected();
		m_strCurSelType = "0";
		break;
	}

	//重新建立页
	for (int i=0; i<m_nPageCount;i++)
	{
		CheckBox * pCurPageCheckBox = dynamic_cast<CheckBox*>(pRoot->getChildByTag(CONTENT_CHECK+i+1));
		if (pCurPageCheckBox!=nullptr)
		{
			pCurPageCheckBox->removeFromParent();
		}
	}
	
	pView->removeAllPages();
	m_nPageCount = 0;
	open_map.clear();

	CheckBox * pCurPageCheckBox = dynamic_cast<CheckBox*>(pRoot->getChildByTag(CONTENT_CHECK));
	//pView->is
	//获得已安装的apk
	char* pChar = Util::getAppList();
	std::string appstr = "";
	if (pChar != NULL) {
		appstr = pChar;
	}

	std::vector < std::string > vec;
	Util::split(appstr, "|", &vec);

	rapidjson::Document _doc;
	bool bRet = false;
	ssize_t size = 0;

	unsigned char*ch = cocos2d::CCFileUtils::getInstance()->getFileData("game_list.json", "r", &size);
	std::string contentStr = std::string((const char*) ch, size);

	CCLog(" game list :: %d %s", size, contentStr.c_str());
	
	_doc.Parse<0>(contentStr.c_str());
	if (_doc.HasParseError()) {
		CCLOG("GetParseError%s\n", _doc.GetParseError());
	}

	rapidjson::Value& pArray = _doc["games"];
	if (!pArray.IsArray())
		return;

	int nPosX = 50;
	int nPosY = 345;

	Layout* pPageTmp;
	int k=0;
	for (int i = 0; i < pArray.Capacity(); ++i) {
		const rapidjson::Value& valueEnt = pArray[i];
		const rapidjson::Value& vGameType = valueEnt["gametype"];
		std::string strGameType = vGameType.GetString();
		if (m_strCurSelType!="0"&&m_strCurSelType!=strGameType)
		{
			continue;
		}
		int nIndex = k%9;
		if (nIndex==0)
		{
			nPosX = 50;
			nPosY = 345;

			pPageTmp = Layout::create();
			pView->addPage(pPageTmp);

			CheckBox* pTmp = CheckBox::create("commonresource/page_no_select.png",
                                          "commonresource/page_no_select.png",
                                          "commonresource/page_select.png",
                                          "commonresource/page_select.png",
                                          "commonresource/page_select.png");
			pTmp->setPosition(pCurPageCheckBox->getPosition()+Vec2(m_nPageCount*40,0));
			pTmp->setTag(pCurPageCheckBox->getTag()+m_nPageCount+1);
			pTmp->setTouchEnabled(false);
			if (k==0)
			{
				pTmp->setSelectedState(true);
			}
			else
			{
				pTmp->setSelectedState(false);
			}
			pRoot->addChild(pTmp);
			m_nPageCount++;
		}
		else
		{
			if (nIndex%3==0)
			{
				nPosX = 50;
				nPosY -= 170;
			}
			else
			{
				nPosX += 200;
			}
		}
		k++;
		if (valueEnt.HasMember("name") && valueEnt.HasMember("click_up") && valueEnt.HasMember("click_down") && valueEnt.HasMember("download")) {
			const rapidjson::Value& vName = valueEnt["name"];
			std::string name = vName.GetString();

			const rapidjson::Value& vclick_up = valueEnt["click_up"];
			std::string click_up = vclick_up.GetString();

			const rapidjson::Value& vclick_down = valueEnt["click_down"];
			std::string click_down = vclick_down.GetString();

			const rapidjson::Value& vdownload = valueEnt["download"];
			std::string download = vdownload.GetString();

			const rapidjson::Value& vpackage = valueEnt["package"];
			std::string package = vpackage.GetString();

			const rapidjson::Value& vKindid = valueEnt["kindid"];
			std::string kindid = vKindid.GetString();

			const rapidjson::Value& vVersion = valueEnt["version"];
			std::string version = vVersion.GetString();

			open_map.insert(map<int, std::string>::value_type(BASE_TAG + i, package + "|" + name + "|" + kindid));
			bool isInstall = false;
			for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
				string strName = *it;
				if (strName == name) {
					isInstall = true;
					break;
				}
			}

			if (isInstall) {

				auto settingMenuItem = Sprite::create(click_up);
				settingMenuItem->setAnchorPoint(CCPoint::ZERO);
				settingMenuItem->setPosition(Vec2(nPosX, nPosY));
				settingMenuItem->setTag(BASE_TAG + i);
				pPageTmp->addChild(settingMenuItem);

//				open_map.insert(map<int, std::string>::value_type(settingMenuItem->getTag(), package + "|" + name + "|" + kindid));
				DataManager::getInstance()->initConfigGameList(std::atoi(kindid.c_str()), package + "|" + name);

				auto buttonListener = EventListenerTouchOneByOne::create();
				buttonListener->setSwallowTouches(false);

				buttonListener->onTouchBegan = [&](Touch * touch ,Event * event) {

					Vec2 beginVec = touch->getLocation();
					beginTouchX = beginVec.x;
					beginTouchY = beginVec.y;

					Sprite * pBtn = static_cast<Sprite *>(event->getCurrentTarget());
					CCPoint pt = pBtn->convertToNodeSpace(beginVec);

					int w = pBtn->getContentSize().width;
					int h = pBtn->getContentSize().height;

					CCRect rc(0,0,w,h);        //

					//CCLog("   get position minx:%f miny:%f maxx:%f  maxy:%f",rc.getMinX(),rc.getMinY(),rc.getMaxX(),rc.getMaxY());

					if(rc.containsPoint(pt))
					{
						pBtn->setScale(1.01f);
					}
					return true;
				};

				buttonListener->onTouchMoved = [&](Touch * touch ,Event * event) {
				};

				//游戏升级
				initDownloadDir();
//				assetManager = getAssetManager(version, download);
//
//				if (assetManager->checkUpdate() && Util::getSignal() > 0) {
//
//					download_map.insert(map<int, std::string>::value_type(settingMenuItem->getTag(), download + "|" + version));
//					CCLog(" check new version .....%s ", version.c_str());
//					buttonListener->onTouchEnded = [&](Touch * touch ,Event * event) {
//						Sprite * pBtn = static_cast<Sprite *>(event->getCurrentTarget());
//
//						//OpenGL坐标系位置
//							Vec2 endVec = touch->getLocation();
//
//							//触发点相对MenuItem的坐标
//							CCPoint pt = pBtn->convertToNodeSpace(endVec);
//
//							int w = pBtn->getContentSize().width;
//							int h = pBtn->getContentSize().height;
//
//							CCRect rc(0,0,w,h);//
//
//							if(abs(endVec.x - beginTouchX) < 10 && rc.containsPoint(pt))
//							{
//								CCLog(" upgrade game ==  %d",pBtn->getTag());
//								pBtn->setScale(1.0f);
//								ConfirmLayer::createObject(this , pBtn->getTag());
//								//downloadFile((Object*)pBtn);
//							}
//						};
//				} else {
//
//					CCLog(" check instanll  .....%s ", version.c_str());
//					buttonListener->onTouchEnded = [&](Touch * touch ,Event * event) {
//
//						Sprite * pBtn = static_cast<Sprite *>(event->getCurrentTarget());
//						CCLog("isInstall onTouchEnded tag %d",pBtn->getTag());
//
//						//OpenGL坐标系位置
//						Vec2 endVec = touch->getLocation();
//						//触发点相对MenuItem的坐标
//						CCPoint pt = pBtn->convertToNodeSpace(endVec);
//
//						int w = pBtn->getContentSize().width;
//						int h = pBtn->getContentSize().height;
//
//						CCRect rc(0,0,w,h);//
//
//						if(abs(endVec.x - beginTouchX) < 10 && rc.containsPoint(pt))
//						{
//							CCLog(" click openApp %d",pBtn->getTag());
//							pBtn->setScale(1.0f);
//							openApp((Object*)pBtn);
//						}
//					};
//				}
				_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener, settingMenuItem);

			} else {
				auto settingMenuItem = Sprite::create(click_up);
				settingMenuItem->setAnchorPoint(CCPoint::ZERO);
				settingMenuItem->setPosition(Vec2(nPosX, nPosY));
				settingMenuItem->setTag(BASE_TAG + i);
				pPageTmp->addChild(settingMenuItem);

				download_map.insert(map<int, std::string>::value_type(settingMenuItem->getTag(), download + "|" + version));

				auto buttonListener = EventListenerTouchOneByOne::create();
				buttonListener->setSwallowTouches(false);

				buttonListener->onTouchBegan = [&](Touch * touch ,Event * event) {

					Vec2 beginVec = touch->getLocation();
					beginTouchX = beginVec.x;
					beginTouchY = beginVec.y;

					Sprite * pBtn = static_cast<Sprite *>(event->getCurrentTarget());
					CCPoint pt = pBtn->convertToNodeSpace(beginVec);

					int w = pBtn->getContentSize().width;
					int h = pBtn->getContentSize().height;

					CCRect rc(0,0,w,h);        //

						if(rc.containsPoint(pt))
						{
							pBtn->setScale(1.01f);
						}
						return true;
					};

				buttonListener->onTouchMoved = [&](Touch * touch ,Event * event) { };

				buttonListener->onTouchEnded = [&](Touch * touch ,Event * event) {
					Sprite * pBtn = static_cast<Sprite *>(event->getCurrentTarget());
					CCLog(" onTouchEnded tag %d",pBtn->getTag());

					//OpenGL坐标系位置
					Vec2 endVec = touch->getLocation();
					//触发点相对MenuItem的坐标
					CCPoint pt = pBtn->convertToNodeSpace(endVec);
					CCLog("  relative position x:%f  y:%f",pt.x,pt.y);

					int w = pBtn->getContentSize().width;
					int h = pBtn->getContentSize().height;

					CCRect rc(0,0,w,h);//

					if(abs(endVec.x - beginTouchX) < 10 && rc.containsPoint(pt))
					{
						CCLog(" click download %d",pBtn->getTag());
						pBtn->setScale(1.0f);
						downloadFile((Object*)pBtn);
					}
				};
				_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonListener, settingMenuItem);
			}
		}
	}

	int nIndex = pView->getCurPageIndex();
	if (nIndex >= m_nPageCount)
	{
		nIndex = 0;
	}
	for (int i=0; i<m_nPageCount;i++)
	{
		CheckBox * pCurPageCheckBox = dynamic_cast<CheckBox*>(pRoot->getChildByTag(CONTENT_CHECK+i+1));
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
		if (m_nPageCount==1)
		{
			pCurPageCheckBox->setVisible(false);
		}
	}
}

void MainLayer::touchCheck(Vec2 Pos)
{
	int nCurePage = pView->getCurPageIndex();
	Layout* pLayout = pView->getPage(nCurePage);

	//该层响应
	//获得已安装的apk
	char* pChar = Util::getAppList();
	std::string appstr = "";
	if (pChar != NULL) {
		appstr = pChar;
	}

	std::vector < std::string > vec;
	Util::split(appstr, "|", &vec);

	rapidjson::Document _doc;
	bool bRet = false;
	ssize_t size = 0;

	unsigned char*ch = cocos2d::CCFileUtils::getInstance()->getFileData("game_list.json", "r", &size);
	std::string contentStr = std::string((const char*) ch, size);

	CCLog(" game list :: %d %s", size, contentStr.c_str());
	
	_doc.Parse<0>(contentStr.c_str());
	if (_doc.HasParseError()) {
		CCLOG("GetParseError%s\n", _doc.GetParseError());
	}

	rapidjson::Value& pArray = _doc["games"];
	if (!pArray.IsArray())
		return;

	int k=0;
	for (int i = 0; i < pArray.Capacity(); ++i) {
		const rapidjson::Value& valueEnt = pArray[i];
		const rapidjson::Value& vGameType = valueEnt["gametype"];
		std::string strGameType = vGameType.GetString();
		if (m_strCurSelType!="0"&&m_strCurSelType!=strGameType)
		{
			continue;
		}
		if (k/9==nCurePage)
		{
			auto pBtn = pLayout->getChildByTag(BASE_TAG + i);

			//触发点相对MenuItem的坐标
			CCPoint pt = pBtn->convertToNodeSpace(Pos);

			int w = pBtn->getContentSize().width;
			int h = pBtn->getContentSize().height;

			CCRect rc(0,0,w,h);//

			if(rc.containsPoint(pt))
			{
				CCLog(" click openApp %d",pBtn->getTag());
				pBtn->runAction(CCSequence::create(CCScaleTo::create(0.06,1.05f),CCScaleTo::create(0.08,1.00f),CCScaleTo::create(0.08,1.00f),NULL));
				clickGame((Object*)pBtn);
			}
		}
		
		k++;
	}
}

void MainLayer::clickGame(Ref * obj)
{
    Sprite * pButton = (Sprite *) obj;
 
	std::string openAppParam = open_map[pButton->getTag()];
	std::vector < std::string > vec;
	Util::split(openAppParam, "|", &vec);
	
	string strkindID = vec.back();
	int kindid = std::atoi(strkindID.c_str());

    DataManager::getInstance()->setCurrentKindID(kindid);
    SceneBase * pLayer = new ConnectGameScene();
	pLayer->setName("ConnectGameScene");
    Director::getInstance()->pushScene(SceneBase::createScene(pLayer));
    pLayer->release();
}