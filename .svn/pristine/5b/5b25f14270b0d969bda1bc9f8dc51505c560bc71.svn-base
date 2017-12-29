#include "RegisterLayer.h"
#include "LoginLayer.h"
#include "hall/data/DataManager.h"
#include "sdk/util/QFileUtil.h"
#include "sdk/ui/PromptLayer.h"
#include "hall/ui/LoadingLayer.h"
#include "hall/ui/LoadingScene.h"
#define TAG "LoginLayer"

enum UI_LOGIN_BUTTON
{
	LOGIN				=	441 ,			//游客登录
	REGISTER			=	439 ,			//游客转正
	QUICK_LOGIN = 4446,						//快速登陆
	CLOSE				=	7457196 ,		//关闭
	TEXT_SERVERIP = 4453,
	TEXT_SERVERPORT = 4454,
	TEXT_ACCOUNTS = 436,
	TEXT_PASSWORD		=	438 ,
	DRAG_BUTTON			=	3522 ,			//下拉菜单
	LIST_VIEW_ACCOUNT 	= 	3528 ,
	LABLE_ACCOUNT   	=  	253 ,
	PLANT_LIST			=	3523 ,
	FORGET_PASS			=  	3519 ,
	IPCONNECT_CHECK		=   4458 ,
	DISTRIBUTE_PORT_LABEL = 4459 ,
	DISTRIBUTE_PORT_CHECK = 4456 ,
	RESET_BTN           =   4000,
};

LoginLayer * LoginLayer::s_pInstance = nullptr;

LoginLayer::LoginLayer(void)
{
	pRoot = nullptr;
	accountBox = nullptr;
	passwordBox = nullptr;
	fromLogin = false;
	bIpConnect = true;
}

LoginLayer::~LoginLayer(void)
{
	this->removeAllChildrenWithCleanup(true);
	pRoot = nullptr;
	accountBox = nullptr;
	passwordBox = nullptr;
}

bool LoginLayer::init()
{
	NYInfo(TAG , " init ");
	if(!LayerColor::init())
	{
		return false;
	}

	JsonUtils::getInstance()->loadJson(_LEVEL_DES_, false);
	JsonUtils::getInstance()->saveJson(_LEVEL_DES_);
	
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(LoginLayer::onTouchBegan,this);
	listener->onTouchEnded = CC_CALLBACK_2(LoginLayer::onTouchEnded,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	CCSize winSize = Director::getInstance()->getVisibleSize();
	
	//遮盖层
	setTouchEnabled(true);
	this->setSwallowsTouches(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	setColor(ccc3(0,0,0));
	setOpacity(150);
	setContentSize(CCSizeMake(1280,800));

	pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("login.json");
	CCSize visibleSize = Director::getInstance()->getVisibleSize();
	pRoot->setAnchorPoint(CCPoint(0.5f,0.5f));
	pRoot->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	this->addChild(pRoot);

	//注册
	Button * m_pVisitorRegister = static_cast<Button*>(pRoot->getChildByTag(REGISTER));
	m_pVisitorRegister->addTouchEventListener(this, toucheventselector(LoginLayer::touchButtonEvent));

	//快速登陆
	auto quickLoginBtn = static_cast<Button*>(pRoot->getChildByTag(QUICK_LOGIN));
	quickLoginBtn->addTouchEventListener(this, toucheventselector(LoginLayer::touchButtonEvent));

	//登录
	Button * m_pVistorLogin = static_cast<Button*>(pRoot->getChildByTag(LOGIN));
	m_pVistorLogin->addTouchEventListener(this,toucheventselector(LoginLayer::touchButtonEvent));






	//关闭
	/*
	Button * m_pAccountLogin = static_cast<Button*>(pRoot->getChildByTag(CLOSE));
	m_pAccountLogin->setVisible(false);
	m_pAccountLogin->addTouchEventListener(this,toucheventselector(LoginLayer::touchButtonEvent));
	*/
	//重置按钮
	Button * pResetBtn = static_cast<Button*>(pRoot->getChildByTag(RESET_BTN));
	pResetBtn->addTouchEventListener(this,toucheventselector(LoginLayer::touchButtonEvent));
	pResetBtn->setZOrder(3);

	CheckBox * pForgetPass = dynamic_cast<CheckBox*>(pRoot->getChildByTag(FORGET_PASS));
	pForgetPass->setTouchEnabled(true);
	pForgetPass->setSelectedState(true);

	//是否分发端口字
	auto distribute_port_label = (Widget*)(pRoot->getChildByTag(DISTRIBUTE_PORT_LABEL));
	distribute_port_cheack = dynamic_cast<CheckBox*>(pRoot->getChildByTag(DISTRIBUTE_PORT_CHECK));
	distribute_port_cheack->setTouchEnabled(true);
	distribute_port_cheack->addEventListener([=](Ref* ref, CheckBox::EventType type){
		RefreshSerInfo();
	});

	pIpConnect = dynamic_cast<CheckBox*>(pRoot->getChildByTag(IPCONNECT_CHECK));
	//pIpConnect->setPositionX(pForgetPass->getPositionX()-300);
	pIpConnect->setTouchEnabled(true);

	auto checkcallback = [=](Ref* ref, CheckBox::EventType type){
		bIpConnect = pIpConnect->getSelectedState();
		if (serverIpBox && portBox)
		{
			serverIpBox->setVisible(bIpConnect);
			serverIpBox->setTouchEnabled(bIpConnect);
			portBox->setVisible(bIpConnect);
			portBox->setTouchEnabled(bIpConnect);
			distribute_port_label->setVisible(bIpConnect);
			distribute_port_cheack->setVisible(bIpConnect);
			distribute_port_cheack->setTouchEnabled(bIpConnect);
		}
	};
	pIpConnect->addEventListener(checkcallback);


	
	//忘记密码
	bool checkboxState = pForgetPass->getSelectedState();

	//下拉用户名
	Button * pDragButton = static_cast<Button*>(pRoot->getChildByTag(DRAG_BUTTON));
	CC_ASSERT(pDragButton);
	pDragButton->setTouchEnabled(true);
	pDragButton->addTouchEventListener(this,toucheventselector(LoginLayer::touchButtonEvent));
	pDragButton->setZOrder(3);
	



	auto * pTextServerIp= static_cast<TextField*>(pRoot->getChildByTag(TEXT_SERVERIP));

	pTextServerIp->setVisible(false);

	serverIpBox = EditBox::create(pTextServerIp->getContentSize(), Scale9Sprite::create("commonresource/edit_white.png"));
	serverIpBox->setPosition(pTextServerIp->getPosition());
	serverIpBox->setAnchorPoint(CCPoint::ZERO);
	serverIpBox->setFontSize(30);
	serverIpBox->setFontColor(Color3B::BLACK);
	serverIpBox->setZOrder(2);
	serverIpBox->setPlaceHolder(U2F("请输入ip地址：").c_str());
	serverIpBox->setReturnType(EditBox::KeyboardReturnType::DONE);
	serverIpBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
	serverIpBox->setDelegate(this);
	serverIpBox->setTouchEnabled(true);
	serverIpBox->setVisible(true);
	serverIpBox->setMaxLength(15);

	pRoot->addChild(serverIpBox);
	//pAccounts->removeFromParent();
	serverIpBox->setTag(TEXT_SERVERIP);
	auto porttext = static_cast<TextField*>(pRoot->getChildByTag(TEXT_SERVERPORT));

	porttext->setVisible(false);
	portBox = EditBox::create(porttext->getContentSize(), Scale9Sprite::create("commonresource/edit_white.png"));
	portBox->setPosition(porttext->getPosition());
	portBox->setAnchorPoint(CCPoint::ZERO);
	portBox->setFontSize(30);
	portBox->setFontColor(Color3B::BLACK);
	portBox->setZOrder(2);
	portBox->setPlaceHolder(U2F("请输入端口：").c_str());
	portBox->setReturnType(EditBox::KeyboardReturnType::DONE);
	portBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
	portBox->setDelegate(this);
	portBox->setTouchEnabled(true);
	portBox->setVisible(true);
	portBox->setMaxLength(15);

	pRoot->addChild(portBox);
	//pAccounts->removeFromParent();
	portBox->setTag(TEXT_SERVERPORT);

	if (UserInfo::instance()->getServerIp() != "NONE")
	{
		serverIpBox->setText(UserInfo::instance()->getServerIp().c_str());
		portBox->setText(cocos2d::Value(UserInfo::instance()->getServerPort()).asString().c_str());
		distribute_port_cheack->setSelectedState(UserInfo::instance()->getDistributePort());
		RefreshSerInfo();
	}


	//账号
	TextField * pAccounts = static_cast<TextField*>(pRoot->getChildByTag(TEXT_ACCOUNTS));

	pAccounts->setVisible(false);
	accountBox = EditBox::create(pAccounts->getContentSize(),Scale9Sprite::create("commonresource/edit_white.png"));
	accountBox->setPosition(pAccounts->getPosition());
	accountBox->setAnchorPoint(CCPoint::ZERO);
	accountBox->setFontSize(30);
	accountBox->setFontColor(Color3B::BLACK);
	accountBox->setZOrder(2);
	accountBox->setPlaceHolder(pAccounts->getPlaceHolder().c_str());
	accountBox->setReturnType(EditBox::KeyboardReturnType::DONE);
	accountBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
	accountBox->setDelegate(this);
	accountBox->setTouchEnabled(true);
	accountBox->setVisible(true);
	accountBox->setMaxLength(15);

	pRoot->addChild(accountBox);
	pAccounts->removeFromParent();
	accountBox->setTag(TEXT_ACCOUNTS);

	//密码
	TextField * pPassword = dynamic_cast<TextField*>(pRoot->getChildByTag(TEXT_PASSWORD));

	passwordBox = EditBox::create(pPassword->getContentSize(),Scale9Sprite::create("commonresource/edit_white.png"));
	passwordBox->setPosition(pPassword->getPosition());
	passwordBox->setAnchorPoint(CCPoint::ZERO);
	passwordBox->setFontColor(Color3B::BLACK);
	passwordBox->setFontSize(30);
	passwordBox->setZOrder(2);
	passwordBox->setPlaceHolder(pPassword->getPlaceHolder().c_str());
	passwordBox->setReturnType(EditBox::KeyboardReturnType::DONE);
	passwordBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
	passwordBox->setDelegate(this);
	passwordBox->setTouchEnabled(true);
	passwordBox->setVisible(true);
	passwordBox->setInputFlag(EditBox::InputFlag::PASSWORD);
	passwordBox->setMaxLength(15);

	pRoot->addChild(passwordBox);
	pPassword->removeFromParent();
	passwordBox->setTag(TEXT_PASSWORD);

	Widget * pListLayout = static_cast<Widget*>(pRoot->getChildByTag(PLANT_LIST));
	pListLayout->setZOrder(4);
	ListView * pListView = static_cast<ListView*>(pListLayout->getChildByTag(LIST_VIEW_ACCOUNT));


	//默认取消ip直连
	pIpConnect->setSelectedState(false);
	checkcallback(pIpConnect, pIpConnect->getSelectedState() ? CheckBox::EventType::SELECTED : CheckBox::EventType::UNSELECTED);

	return true;
}

void LoginLayer::onEnter(){

	//注册为监听者
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LoginLayer::notifySuccess),LOGON_LOGON_SUCCESS,nullptr);
	//注册为监听者
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoginLayer::notifyFail), LOGON_LOGON_FAILURE, nullptr);

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoginLayer::connectFail), CONNECT_SERVER_FAIL, nullptr);
	

	LayerColor::onEnter();

	//清空数据
	EditBox * pPassword = dynamic_cast<EditBox*>(pRoot->getChildByTag(TEXT_PASSWORD));
	pPassword->setText("");
	EditBox * pAccounts = dynamic_cast<EditBox*>(pRoot->getChildByTag(TEXT_ACCOUNTS));
	pAccounts->setText("");

	
	string lastLoginAccount = QFileUtil::getLoginAccounts();
	if(!lastLoginAccount.empty())
	{
		pAccounts->setText(lastLoginAccount.c_str());
	}

	Widget * pListLayout = static_cast<Widget*>(pRoot->getChildByTag(PLANT_LIST));
	CC_ASSERT(pListLayout);
	pListLayout->setVisible(false);
	pListLayout->setTouchEnabled(false);

	//登录记录
	ui::ListView * pListView = static_cast<ListView*>(pListLayout->getChildByTag(LIST_VIEW_ACCOUNT));
	CC_ASSERT(pListView);
	pListView->setTouchEnabled(true);
	int width = pListView->getInnerContainerSize().width;

	pListView->removeAllChildrenWithCleanup(true);

	Button * pButton = Button::create("commonresource/edit_black.png","");
	pButton->setScale9Enabled(true);
	pButton->setSize(CCSize(width,40));
	pListView->setItemModel(pButton);

	int cnt = 1;
	cocos2d::ValueMap vec = QFileUtil::getLoginFile();
	for(auto &x : vec){
		cnt++;
	}
	
	for(int i = 0;i<cnt-1;i++){
		pListView->pushBackDefaultItem();
	}

	cnt = 0;
	for(auto &x : vec){
		string vkey = x.first;
		Button * pBtn = (Button *)(pListView->getItem(cnt));
		pBtn->setTag(cnt);
		pBtn->setTitleFontSize(25);
		pBtn->addTouchEventListener(this,toucheventselector(LoginLayer::listSelectEvent));
		pBtn->setTitleText(vkey);

		if(!lastLoginAccount.empty() && lastLoginAccount == x.first)
		{
			string vPwd = x.second.asString();
			pPassword->setText(vPwd.c_str());
		}
		cnt++;
	}

	//CCSequence * popupActions = CCSequence::create(CCScaleTo::create(0.0,0.0),CCScaleTo::create(0.06,1.05),CCScaleTo::create(0.08,0.95),CCScaleTo::create(0.08,1.0),NULL);
	//this->runAction(popupActions);
}

void LoginLayer::onExit(){

	NotificationCenter::getInstance()->removeObserver(this, CONNECT_SERVER_FAIL);

	NotificationCenter::getInstance()->removeObserver(this, LOGON_LOGON_SUCCESS);
	//删除为监听者
	NotificationCenter::getInstance()->removeObserver(this, LOGON_LOGON_FAILURE);
	LayerColor::onExit();


}

void LoginLayer::RefreshSerInfo()
{
	MainController::getInstance()->restartLoginSer();
	SerInfo serinfo;
	strcpy(serinfo.IP, serverIpBox->getText());
	serinfo.nPort = cocos2d::Value(portBox->getText()).asInt();
	serinfo.bDistributePort = distribute_port_cheack->getSelectedState();
	MainController::getInstance()->pushSerInfo(serinfo);
}


void LoginLayer::touchButtonEvent(Ref * obj ,TouchEventType eventType){

	Button * pButton = (Button *)obj;
	switch(eventType){
		case TouchEventType::TOUCH_EVENT_ENDED:
			{
				hideDragList();

				switch(pButton->getTag()){
					case LOGIN:
						{
								  if (bIpConnect)
								  {
// 									  auto ip = UserInfo::instance()->getServerIp();
// 									  if (ip == "NONE")
// 									  {
// 										  PromptLayer::createObject(U2F("服务器地址不能为空！").c_str());
// 										  return;
// 									  }
// 
// 									  auto port = UserInfo::instance()->getServerPort();
// 									  if (port <= 0 || port > 65535)
// 									  {
// 										  PromptLayer::createObject(U2F("端口非法！").c_str());
// 										  return;
// 									  }
									  checkIpSerInfoEditbox(serverIpBox);
									  checkIpSerInfoEditbox(portBox);
								  }

							//登录消息
							string strAccount = accountBox->getText();
							if(strAccount.empty()){
								PromptLayer::createObject(U2F("账号不能为空").c_str());
								return;
							}

							string psw = passwordBox->getText();
							if(psw.empty()){
								PromptLayer::createObject(U2F("密码不能为空").c_str());
								return;
							}
							LoadingScene::createObject(U2F("loading...").c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
							UserInfo::instance()->setLoginType(LOGIN_TYPE_MB_ANDROID_ACCOUNT);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
							UserInfo::instance()->setLoginType(LOGIN_TYPE_MB_APPLE_ACCOUNT);
#endif


							UserInfo::instance()->setTempPassword(psw);
							UserInfo::instance()->setAccount(strAccount);

							MainController::getInstance()->serverRequest(SUB_GP_LOGIN_GETKEY,bIpConnect);

						}
						break;
					case REGISTER:
					{
									 if (fromLogin)
										 RegisterLayer::createObject(false, bIpConnect);
									 else
										 RegisterLayer::createObject(true, bIpConnect);

									 hideModal();
					}
						break;
					case QUICK_LOGIN://快速登陆
						{
							
									  if (bIpConnect)
									  {
// 										  auto ip = UserInfo::instance()->getServerIp();
// 										  if (ip == "NONE")
// 										  {
// 											  PromptLayer::createObject(U2F("服务器地址不能为空！").c_str());
// 											  return;
// 										  }
										  checkIpSerInfoEditbox(serverIpBox);
										  checkIpSerInfoEditbox(portBox);
									  }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
									 UserInfo::instance()->setLoginType(LOGIN_TYPE_MB_ANDROID_QUICK);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
									 UserInfo::instance()->setLoginType(LOGIN_TYPE_MB_APPLE_QUICK);
#endif

									 MainController::getInstance()->serverRequest(SUB_GP_GETKEY_QUICKLOGON, bIpConnect);

						}
						break;

					case DRAG_BUTTON:		//下拉菜单
						{
							//显示下拉菜单
							Widget * pList = static_cast<Widget*>(pRoot->getChildByTag(PLANT_LIST));
							if(pList->isVisible())
							{
								pList->setVisible(false);

								//密码输入框不可编辑（事件级别高影响下拉菜单的选择）
								EditBox * pPassword = dynamic_cast<EditBox*>(pRoot->getChildByTag(TEXT_PASSWORD));
								pPassword->setEnabled(true);
							}
							else
							{
								pList->setVisible(true);

								//密码输入框不可编辑（事件级别高影响下拉菜单的选择）
								EditBox * pPassword = dynamic_cast<EditBox*>(pRoot->getChildByTag(TEXT_PASSWORD));
								pPassword->setEnabled(false);
							}
						}
						break;
					
					case FORGET_PASS:		//忘记密码
						{
//							hideModal();
						}
						break;

					case CLOSE:
						{
							hideModal();
						}
						break;
					case RESET_BTN:
						{
							EditBox * pPassword = dynamic_cast<EditBox*>(pRoot->getChildByTag(TEXT_PASSWORD));
							pPassword->setText("");
						}
						break;
					
				}
			}
			break;
	}
}


//滚动层选择事件
void LoginLayer::listSelectEvent(Ref * pSender , TouchEventType type){

	//ListView * listView = static_cast<ListView *>(pSender);
	Button * pBtn = static_cast<Button *>(pSender);

	if(type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		cocos2d::ValueMap vec = QFileUtil::getLoginFile();
		//int curIndx = listView->getCurSelectedIndex();
		int curIndx = pBtn->getTag();
		int cnt = 0;
		for(auto &x : vec){
			if(cnt == curIndx){

				EditBox * pAccounts = static_cast<EditBox*>(pRoot->getChildByTag(TEXT_ACCOUNTS));
				string key = x.first;
				pAccounts->setText(key.c_str());

				//密码可编辑
				EditBox * pPassword = dynamic_cast<EditBox*>(pRoot->getChildByTag(TEXT_PASSWORD));
				pPassword->setEnabled(true);
				string al = ((cocos2d::Value)(x.second)).asString();
				pPassword->setText(al.c_str());

				//隐藏下拉菜单
				Widget * pListLayout = static_cast<Widget*>(pRoot->getChildByTag(PLANT_LIST));
				pListLayout->setVisible(false);

				//m_loginType = 1;	//用保持信息登录
				break;
			}
			cnt++;
		}
	}
}


//创建对象
void LoginLayer::createObject(Node * pScene){

	if(!s_pInstance){
		s_pInstance = LoginLayer::create();
	}

	if(pScene == nullptr){

		pScene = CCDirector::sharedDirector()->getRunningScene();
		s_pInstance->setShowCloseBtn();
		
	}else{
		s_pInstance->fromLogin = true;
	}

	pScene->addChild(s_pInstance,10);

}

//
void LoginLayer::hideModal(){
	if(s_pInstance){
		s_pInstance->removeFromParent();
		s_pInstance = nullptr;
	}

}

//通知接口
void LoginLayer::notifySuccess(Ref * obj)
{
// 	LoadingScene::hideModal();
// 	MainController::getInstance()->connectHallFinish();
	CheckBox * pForgetPass = dynamic_cast<CheckBox*>(pRoot->getChildByTag(FORGET_PASS));
	bool checkboxState = pForgetPass->getSelectedState();
	if(checkboxState)
	{
		//保存登录记录
		if(UserInfo::instance()->getAccount() != UserInfo::instance()->getMachineID())
			QFileUtil::saveLoginFile(UserInfo::instance()->getAccount().c_str(),UserInfo::instance()->getPassword().c_str());
	}

	hideModal();
	LoadingScene::hideModal();

// 	if (bIpConnect)
// 	{
// 		SerInfo serInfo = MainController::getInstance()->getSerInfo(0);
// 		string ip = (char*)serInfo.IP;
// 		//NYInfo(TAG,"========ip======%s", ip.c_str());
// 		serverIpBox->setText(ip.c_str());
// 	}
}
void LoginLayer::connectFail(Ref * obj)
{
	LoadingScene::hideModal();
	PromptLayer::createObject(U2F("连接失败！").c_str());
}
//通知接口
void LoginLayer::notifyFail(Ref * obj)
{
// 	LoadingScene::hideModal();
// 	MainController::getInstance()->connectHallFinish();
		CMD_GP_OperateFailure * pFail = (CMD_GP_OperateFailure *)obj;

		string szError = Util::getStringWithTChar(pFail->szDescribeString);
		PromptLayer::createObject((char *)szError.c_str());
		UserInfo::instance()->setAccount("");
		//MainController::getInstance()->startConnect(NetworkManager::FLAG_HALL, Util::getIpByHostName(LOGIN_SERVER_IP), LOGIN_SERVER_PORT);
		LoadingScene::hideModal();
		hideModal();
// 		if (bIpConnect)
// 		{
// 			SerInfo serInfo = MainController::getInstance()->getSerInfo(0);
// 			string ip = (char*)serInfo.IP;
// 			//NYInfo(TAG,"========ip======%s", ip.c_str());
// 			serverIpBox->setText(ip.c_str());
// 		}
}

void LoginLayer::checkIpSerInfoEditbox(EditBox * editBox)
{
	if (nullptr == editBox)
	{
		PromptLayer::createObject("editBox == null~!");
		return;
	}
	std::string text = editBox->getText();
	if (text.empty())
		return;

	if (editBox->getTag() == TEXT_SERVERIP)
	{
		int count = 0;

		for (auto c : text)
		{
			if (c == '.')count++;
		}
		auto vec = Util::splitString(Util::trim(text), ".");
		for (auto s : vec)
		{
			auto n = (unsigned int)atoi(s.c_str());
			if (n > 255 || n < 0)
			{
				count = -1;
				break;
			}
		}

		if (count != 3 || (unsigned int)vec.size() != 4)
		{
			PromptLayer::createObject("ip地址非法！");
			return;
		}
	}
	else if (editBox->getTag() == TEXT_SERVERPORT)
	{
		auto port = cocos2d::Value(text).asInt();
		if (port <= 0 || port > 65535)
		{
			PromptLayer::createObject("端口非法！");
			return;
		}
	}

}

void LoginLayer::editBoxEditingDidBegin(EditBox * editBox){
	hideDragList();
}

void LoginLayer::editBoxEditingDidEnd(EditBox * editBox){
	//m_loginType = 0;
	if (bIpConnect)
	{
		//MainController::getInstance()->restartLoginSer();
		checkIpSerInfoEditbox(editBox);
		RefreshSerInfo();
// 		SerInfo serinfo;
// 		strcpy(serinfo.IP, serverIpBox->getText());
// 		serinfo.nPort = cocos2d::Value(portBox->getText()).asInt();
// 		serinfo.bDistributePort = (bool)0;
// 		MainController::getInstance()->pushSerInfo(serinfo);
	}
}


void LoginLayer::editBoxTextChanged(EditBox * editBox){
	//m_loginType = 0;
}


void LoginLayer::editBoxReturn(EditBox * editBox){
}

//隐藏下拉菜单
void LoginLayer::hideDragList(){
	Widget * pListView = static_cast<Widget*>(pRoot->getChildByTag(PLANT_LIST));
	pListView->setVisible(false);

	//设置密码可编辑
	EditBox * pPasswordEB = dynamic_cast<EditBox*>(pRoot->getChildByTag(TEXT_PASSWORD));
	pPasswordEB->setEnabled(true);
}

bool LoginLayer::onTouchBegan(Touch * touch , Event * event)
{
	return true;
}

void LoginLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	CCLog("  onTouchend ");
	hideDragList();
}

void LoginLayer::setShowCloseBtn()
{
	/*
	Button * m_pAccountLogin = static_cast<Button*>(pRoot->getChildByTag(CLOSE));
	m_pAccountLogin->setVisible(true);
	*/
}

