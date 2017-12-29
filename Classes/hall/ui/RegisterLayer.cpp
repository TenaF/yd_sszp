#include "RegisterLayer.h"
#include "LoginLayer.h"
#include "sdk/ui/PromptLayer.h"
#include "hall/data/DataManager.h"
#include "MainLayer.h"
#include "sdk/util/QFileUtil.h"

enum UI_REGISTER_BUTTON
{
	REGISTER				=	448 ,			//
	BACK					=	467 ,
	CLOSE					=	7457194 ,			//
	REGISTER_ACCOUNT		=	460 ,
	REGISTER_NICK			=	464 ,
	REGISTER_PASSWORD		=	462 ,
	REGISTER_CONFIRM		=	230 ,
	REGISTER_MALE			=	7457168 ,
	REGISTER_FAMALE			=	7457170 ,
	REGISTER_PHONE			=	7457382 ,
	//新增
	REGISTER_BANK_PASSWORD  =   7459779,
	REGISTER_BANK_CONFIRM   =   7459778,
	REGISTER_CARD           =   7459781,
};

RegisterLayer * RegisterLayer::s_pInstance = nullptr;

RegisterLayer::RegisterLayer(void)
{
	wFaceID 			= 0;
	checkValue 			= 1;
	pRoot 				= nullptr;
	pRegisterAccounts 	= nullptr;
	pRegisterPass 		= nullptr;
	pRegisterNick 		= nullptr;
	pRegisterConfirm 	= nullptr;
	showClose = false;
}

RegisterLayer::~RegisterLayer(void)
{
	this->removeAllChildrenWithCleanup(true);
}

bool RegisterLayer::init()
{
	if(!LayerColor::init())
	{
		return false;
	}
	
	CCSize visibleSize = Director::getInstance()->getVisibleSize();

	setTouchEnabled(true);
	this->setSwallowsTouches(false);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	setColor(ccc3(0,0,0));
	setOpacity(150);
	setContentSize(CCSizeMake(visibleSize.width,visibleSize.height));

	pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("register.json");
	pRoot->setAnchorPoint(CCPoint(0.5f,0.5f));
	pRoot->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	this->addChild(pRoot);

	/*ImageView * m_pVisitorTitle = dynamic_cast<ImageView*>(pRoot->getChildByTag(IMAGE_VISITOR_TITLE));
	CC_ASSERT(m_pVisitorTitle);
	m_pVisitorTitle->setVisible(false);*/
	//
	Button * m_pVisitorRegister = dynamic_cast<Button*>(pRoot->getChildByTag(REGISTER));
	CC_ASSERT(m_pVisitorRegister);
	m_pVisitorRegister->setTouchEnabled(true);
	m_pVisitorRegister->addTouchEventListener(this,toucheventselector(RegisterLayer::touchButtonEvent));

	//
	/*
	Button * m_pClose = dynamic_cast<Button*>(pRoot->getChildByTag(CLOSE));
	CC_ASSERT(m_pClose);
	m_pClose->setTouchEnabled(true);
//	m_pClose->setVisible(false);
	m_pClose->addTouchEventListener(this,toucheventselector(RegisterLayer::touchButtonEvent));
	*/

	Button * m_pBack = dynamic_cast<Button*>(pRoot->getChildByTag(BACK));
	m_pBack->setTouchEnabled(true);
	m_pBack->addTouchEventListener(this,toucheventselector(RegisterLayer::touchButtonEvent));

	//
	TextField * pAccount = dynamic_cast<TextField*>(pRoot->getChildByTag(REGISTER_ACCOUNT));
	CC_ASSERT(pAccount);
	
	pRegisterAccounts = EditBox::create(pAccount->getContentSize(),Scale9Sprite::create("commonresource/edit_white.png"));
	pRegisterAccounts->setPosition(pAccount->getPosition());
	pRegisterAccounts->setAnchorPoint(CCPoint::ZERO);
	pRegisterAccounts->setFontSize(30);
	pRegisterAccounts->setFontColor(Color3B::BLACK);
	pRegisterAccounts->setZOrder(pAccount->getZOrder());
	pRegisterAccounts->setPlaceHolder(U2F("6~20个字符").c_str());
	pRegisterAccounts->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
	pRegisterAccounts->setInputMode(EditBox::InputMode::SINGLE_LINE);
	pRegisterAccounts->setDelegate(this);
	pRegisterAccounts->setTouchEnabled(true);
	pRegisterAccounts->setMaxLength(20);

	pAccount->removeFromParent();
	pRegisterAccounts->setTag(REGISTER_ACCOUNT);
	pRoot->addChild(pRegisterAccounts);


	//
	TextField * pNick = dynamic_cast<TextField*>(pRoot->getChildByTag(REGISTER_NICK));
	CC_ASSERT(pNick);
	
	pRegisterNick = EditBox::create(pNick->getContentSize(),Scale9Sprite::create("commonresource/edit_white.png"));
	pRegisterNick->setPosition(pNick->getPosition());
	pRegisterNick->setAnchorPoint(CCPoint::ZERO);
	pRegisterNick->setFontSize(30);
	pRegisterNick->setFontColor(Color3B::BLACK);
	pRegisterNick->setZOrder(pNick->getZOrder());
	pRegisterNick->setPlaceHolder(U2F("6~20个字符").c_str());
	pRegisterNick->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
	pRegisterNick->setInputMode(EditBox::InputMode::SINGLE_LINE);
	pRegisterNick->setDelegate(this);
	pRegisterNick->setTouchEnabled(true);
	pRegisterNick->setMaxLength(20);

	pNick->removeFromParent();
	pRegisterNick->setTag(REGISTER_NICK);
	pRoot->addChild(pRegisterNick);
	 
	//
	TextField * pPassword = dynamic_cast<TextField*>(pRoot->getChildByTag(REGISTER_PASSWORD));
	CC_ASSERT(pPassword);
	
	pRegisterPass = EditBox::create(pPassword->getContentSize(),Scale9Sprite::create("commonresource/edit_white.png"));
	pRegisterPass->setPosition(pPassword->getPosition());
	pRegisterPass->setAnchorPoint(CCPoint::ZERO);
	pRegisterPass->setFontSize(30);
	pRegisterPass->setFontColor(Color3B::BLACK);
	pRegisterPass->setZOrder(pPassword->getZOrder());
	pRegisterPass->setPlaceHolder(U2F("6~20个字符").c_str());
	pRegisterPass->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
	pRegisterPass->setInputMode(EditBox::InputMode::SINGLE_LINE);
	pRegisterPass->setInputFlag(EditBox::InputFlag::PASSWORD);
	pRegisterPass->setDelegate(this);
	pRegisterPass->setTouchEnabled(true);
	pRegisterPass->setMaxLength(20);

	pPassword->removeFromParent();
	pRegisterPass->setTag(REGISTER_PASSWORD);
	pRoot->addChild(pRegisterPass);

	//
	TextField * pConfirmPassword = dynamic_cast<TextField*>(pRoot->getChildByTag(REGISTER_CONFIRM));
	CC_ASSERT(pConfirmPassword);
	
	m_pRegisterConfirmPass = EditBox::create(pConfirmPassword->getContentSize(),Scale9Sprite::create("commonresource/edit_white.png"));
	m_pRegisterConfirmPass->setPosition(pConfirmPassword->getPosition());
	m_pRegisterConfirmPass->setAnchorPoint(CCPoint::ZERO);
	m_pRegisterConfirmPass->setFontSize(30);
	m_pRegisterConfirmPass->setFontColor(Color3B::BLACK);
	m_pRegisterConfirmPass->setZOrder(pConfirmPassword->getZOrder());
	m_pRegisterConfirmPass->setPlaceHolder(U2F("6~20个字符").c_str());
	m_pRegisterConfirmPass->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
	m_pRegisterConfirmPass->setInputMode(EditBox::InputMode::SINGLE_LINE);
	m_pRegisterConfirmPass->setInputFlag(EditBox::InputFlag::PASSWORD);
	m_pRegisterConfirmPass->setDelegate(this);
	m_pRegisterConfirmPass->setTouchEnabled(true);
	m_pRegisterConfirmPass->setMaxLength(20);

	pConfirmPassword->removeFromParent();
	m_pRegisterConfirmPass->setTag(REGISTER_CONFIRM);
	pRoot->addChild(m_pRegisterConfirmPass);

	//
	TextField * pRegisterBankPass = dynamic_cast<TextField*>(pRoot->getChildByTag(REGISTER_BANK_PASSWORD));
	CC_ASSERT(pRegisterBankPass);
	
	m_pRegisterBankPass = EditBox::create(pRegisterBankPass->getContentSize(),Scale9Sprite::create("commonresource/edit_white.png"));
	m_pRegisterBankPass->setPosition(pRegisterBankPass->getPosition());
	m_pRegisterBankPass->setAnchorPoint(CCPoint::ZERO);
	m_pRegisterBankPass->setFontSize(30);
	m_pRegisterBankPass->setFontColor(Color3B::BLACK);
	m_pRegisterBankPass->setZOrder(pRegisterBankPass->getZOrder());
	//m_pRegisterBankPass->setPlaceHolder(U2F("6~20个字符").c_str());
	m_pRegisterBankPass->setPlaceHolder(U2F("不用填").c_str());
	m_pRegisterBankPass->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
	m_pRegisterBankPass->setInputMode(EditBox::InputMode::SINGLE_LINE);
	m_pRegisterBankPass->setInputFlag(EditBox::InputFlag::PASSWORD);
	m_pRegisterBankPass->setDelegate(this);
	m_pRegisterBankPass->setTouchEnabled(true);
	m_pRegisterBankPass->setMaxLength(20);

	pRegisterBankPass->removeFromParent();
	m_pRegisterBankPass->setTag(REGISTER_BANK_PASSWORD);
	pRoot->addChild(m_pRegisterBankPass);

	//
	TextField * pRegisterBankConfirmPass = dynamic_cast<TextField*>(pRoot->getChildByTag(REGISTER_BANK_CONFIRM));
	CC_ASSERT(pRegisterBankConfirmPass);
	
	m_pRegisterBankConfirmPass = EditBox::create(pRegisterBankConfirmPass->getContentSize(),Scale9Sprite::create("commonresource/edit_white.png"));
	m_pRegisterBankConfirmPass->setPosition(pRegisterBankConfirmPass->getPosition());
	m_pRegisterBankConfirmPass->setAnchorPoint(CCPoint::ZERO);
	m_pRegisterBankConfirmPass->setFontSize(30);
	m_pRegisterBankConfirmPass->setFontColor(Color3B::BLACK);
	m_pRegisterBankConfirmPass->setZOrder(pRegisterBankConfirmPass->getZOrder());
	//m_pRegisterBankConfirmPass->setPlaceHolder(U2F("6~20个字符").c_str());
	m_pRegisterBankConfirmPass->setPlaceHolder(U2F("不用填").c_str());
	m_pRegisterBankConfirmPass->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
	m_pRegisterBankConfirmPass->setInputMode(EditBox::InputMode::SINGLE_LINE);
	m_pRegisterBankConfirmPass->setInputFlag(EditBox::InputFlag::PASSWORD);
	m_pRegisterBankConfirmPass->setDelegate(this);
	m_pRegisterBankConfirmPass->setTouchEnabled(true);
	m_pRegisterBankConfirmPass->setMaxLength(20);

	pRegisterBankConfirmPass->removeFromParent();
	m_pRegisterBankConfirmPass->setTag(REGISTER_BANK_CONFIRM);
	pRoot->addChild(m_pRegisterBankConfirmPass);

	////
	TextField * pConfirm = dynamic_cast<TextField*>(pRoot->getChildByTag(REGISTER_PHONE));
	CC_ASSERT(pConfirm);

	pRegisterConfirm = EditBox::create(pConfirm->getContentSize(),Scale9Sprite::create("commonresource/edit_white.png"));
	pRegisterConfirm->setPosition(pConfirm->getPosition());
	pRegisterConfirm->setAnchorPoint(CCPoint::ZERO);
	pRegisterConfirm->setFontSize(30);
	pRegisterConfirm->setFontColor(Color3B::BLACK);
	pRegisterConfirm->setZOrder(pConfirm->getZOrder());
	//pRegisterConfirm->setPlaceHolder(U2F("找回密码的有效凭证").c_str());
	pRegisterConfirm->setPlaceHolder(U2F("不用填").c_str());
	pRegisterConfirm->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
	pRegisterConfirm->setInputMode(EditBox::InputMode::NUMERIC);
	pRegisterConfirm->setDelegate(this);
	pRegisterConfirm->setTouchEnabled(true);
	pRegisterConfirm->setMaxLength(11);

	pConfirm->removeFromParent();
	pRegisterNick->setTag(REGISTER_PHONE);
	pRoot->addChild(pRegisterConfirm);

	////
	TextField * pRegisterCard = dynamic_cast<TextField*>(pRoot->getChildByTag(REGISTER_CARD));
	CC_ASSERT(pRegisterCard);

	m_pRegisterCard = EditBox::create(pRegisterCard->getContentSize(),Scale9Sprite::create("commonresource/edit_white.png"));
	m_pRegisterCard->setPosition(pRegisterCard->getPosition());
	m_pRegisterCard->setAnchorPoint(CCPoint::ZERO);
	m_pRegisterCard->setFontSize(30);
	m_pRegisterCard->setFontColor(Color3B::BLACK);
	m_pRegisterCard->setZOrder(pRegisterCard->getZOrder());
	//m_pRegisterCard->setPlaceHolder(U2F("有效证件").c_str());
	m_pRegisterCard->setPlaceHolder(U2F("不用填").c_str());
	m_pRegisterCard->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
	m_pRegisterCard->setInputMode(EditBox::InputMode::NUMERIC);
	m_pRegisterCard->setDelegate(this);
	m_pRegisterCard->setTouchEnabled(true);
	m_pRegisterCard->setMaxLength(18);

	pRegisterCard->removeFromParent();
	m_pRegisterCard->setTag(REGISTER_CARD);
	pRoot->addChild(m_pRegisterCard);
	
	//
	CheckBox * pMale = dynamic_cast<CheckBox*>(pRoot->getChildByTag(REGISTER_MALE));
	CC_ASSERT(pMale);
	pMale->setTouchEnabled(true);
	pMale->setSelectedState(true);
	pMale->addEventListenerCheckBox(this,checkboxselectedeventselector(RegisterLayer::checkSelectEvent));

	CheckBox * pFamale = dynamic_cast<CheckBox*>(pRoot->getChildByTag(REGISTER_FAMALE));
	CC_ASSERT(pFamale);
	pFamale->setTouchEnabled(true);
	pFamale->addEventListenerCheckBox(this,checkboxselectedeventselector(RegisterLayer::checkSelectEvent));

	return true;
}

void RegisterLayer::checkSelectEvent(Ref * pSender , CheckBoxEventType eventType){

	CheckBox * pCheckBox = (CheckBox *)pSender;
	switch (eventType)
	{
		case CheckBoxEventType::CHECKBOX_STATE_EVENT_SELECTED:
			{
				if(pCheckBox->getTag() == REGISTER_FAMALE){
					checkValue = 0;
					wFaceID = 50;
					CheckBox * pMale = dynamic_cast<CheckBox*>(pRoot->getChildByTag(REGISTER_MALE));
					pMale->setSelectedState(false);
				}else{
					checkValue = 1;
					wFaceID = 0;
					CheckBox * pFamale = dynamic_cast<CheckBox*>(pRoot->getChildByTag(REGISTER_FAMALE));
					pFamale->setSelectedState(false);
				}
			}
		default:
			break;
	}
}

void RegisterLayer::onEnter(){

	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(RegisterLayer::notifySuccess),LOGON_LOGON_SUCCESS,nullptr);

	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(RegisterLayer::notifyFail),LOGON_LOGON_FAILURE,nullptr);

	LayerColor::onEnter();

	//CCSequence * popupActions = CCSequence::create(CCScaleTo::create(0.0,0.0),CCScaleTo::create(0.06,1.05),CCScaleTo::create(0.08,0.95),CCScaleTo::create(0.08,1.0),NULL);
	//this->runAction(popupActions);

}

void RegisterLayer::onExit(){

	LayerColor::onExit();

	NotificationCenter::getInstance()->removeObserver(this,LOGON_LOGON_SUCCESS);

	NotificationCenter::getInstance()->removeObserver(this,LOGON_LOGON_FAILURE);
}


void RegisterLayer::touchButtonEvent(Ref * obj ,TouchEventType eventType){

	Button * pButton = (Button *)obj;
	
	switch(eventType){
		case TouchEventType::TOUCH_EVENT_ENDED:
			{
				switch(pButton->getTag()){
					case REGISTER:
						{
							string accounts = pRegisterAccounts->getText();		//
							accounts = Util::trim(accounts);
							string nickname = pRegisterNick->getText();			//
							nickname = Util::trim(nickname);
							string password = pRegisterPass->getText();			//
							password = Util::trim(password);
							string confirm 	= pRegisterConfirm->getText();		//
							confirm = Util::trim(confirm);
							string strConfirmPass = m_pRegisterConfirmPass->getText();
							strConfirmPass = Util::trim(strConfirmPass);
							string strBankPass = m_pRegisterBankPass->getText();
							strBankPass = Util::trim(strBankPass);
							string strBankConfirmPass = m_pRegisterBankConfirmPass->getText();
							strBankConfirmPass = Util::trim(strBankConfirmPass);
							string strCard = m_pRegisterCard->getText();
							strCard = Util::trim(strCard);

							if(accounts.empty()){
								PromptLayer::createObject("用户名不能为空");
								return;
							}else{
								if(accounts.length() < 6)
								{
									PromptLayer::createObject("用户名长度在6~20位");
									return;
								}
								string reg = "(^[\u4E00-\u9FFF]+$)|(^[a-zA-Z0-9_]+$)";
								if(!Util::checkRegex(accounts , reg)){
									PromptLayer::createObject("用户名非法");
									return;
								}
							}

							if(nickname.empty()){
								PromptLayer::createObject("昵称不能为空");
								return;
							}else{
								if(nickname.length() < 6)
								{
									PromptLayer::createObject("昵称名长度在6~20位");
									return;
								}
								string reg = "(^[\u4E00-\u9FFF]+$)|(^[a-zA-Z0-9_]+$)";
								if(!Util::checkRegex(nickname , reg)){
									PromptLayer::createObject("昵称非法字符");
									return;
								}
							}

							if(password.empty()){
								PromptLayer::createObject("密码不能为空");
								return;
							}else{
								if(password.length() < 6)
								{
									PromptLayer::createObject("密码名长度在6~20位");
									return;
								}
								string reg = "^[0-9a-z]+$";
								if(!Util::checkRegex(password , reg)){
									PromptLayer::createObject("密码中有非法字符");
									return;
								}
								if (password!=strConfirmPass)
								{
									PromptLayer::createObject("您输入的两次密码不一致");
									return;
								}
							}

// 							if(strBankPass.empty()){
// 								PromptLayer::createObject("保险柜密码不能为空");
// 								return;
// 							}else{
// 								if(strBankPass.length() < 6)
// 								{
// 									PromptLayer::createObject("保险柜密码名长度在6~20位");
// 									return;
// 								}
// 								string reg = "^[0-9a-z]+$";
// 								if(!Util::checkRegex(strBankPass , reg)){
// 									PromptLayer::createObject("密码中有非法字符");
// 									return;
// 								}
// 								if (strBankPass!=strBankConfirmPass)
// 								{
// 									PromptLayer::createObject("您输入的两次保险柜密码不一致");
// 									return;
// 								}
// 							}

// 							if(confirm.empty()){
// 								PromptLayer::createObject("手机号不能为空！");
// 								return;
// 							}else{
// 								string reg = "^[0-9]+$";
// 								if(!Util::checkRegex(confirm , reg)){
// 									PromptLayer::createObject("手机号中有非法字符");
// 									return;
// 								}
// 							}

// 							if(strCard.empty()){
// 								PromptLayer::createObject("身份证号不能为空！");
// 								return;
// 							}else{
// 								string reg = "^[0-9]+$";
// 								if(!Util::checkRegex(confirm , reg)){
// 									PromptLayer::createObject("身份证号中有非法字符");
// 									return;
// 								}
// 								if (strCard.length()!=15&&strCard.length()!=18)
// 								{
// 									PromptLayer::createObject("无效身份证号");
// 									return;
// 								}
// 							}

							//保存登录密码
							UserInfo::instance()->setTempPassword(password);
							UserInfo::instance()->setAccount(accounts);

							UserInfo::instance()->nickTemp = nickname;
							//UserInfo::instance()->pswTempConfirm = confirm;
							UserInfo::instance()->checkValue = checkValue;
							UserInfo::instance()->wFaceID = wFaceID;
							UserInfo::instance()->strBankPass = "888888";
							UserInfo::instance()->strCard = "";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
							UserInfo::instance()->setLoginType(LOGIN_TYPE_MB_ANDROID_ACCOUNT);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
							UserInfo::instance()->setLoginType(LOGIN_TYPE_MB_APPLE_ACCOUNT);
#endif
							UserInfo::instance()->setTempPassword(password);
							UserInfo::instance()->setAccount(accounts);


							//schedule(schedule_selector(RegisterLayer::outTime), 10.0f);
							MainController::getInstance()->serverRequest(SUB_GP_REGISTER_GETKEY, m_bIpConnect);

							//SendMsg::registerAccounts(accounts.c_str(),Util::strToMd5Str(password).c_str(),nickname.c_str(), checkValue ,wFaceID, Util::strToMd5Str(strBankPass).c_str(), strCard.c_str());
						}
						break;

					case CLOSE:
						/*
						{
							hideModal();
						}
						break;
						*/

					case BACK:
						{
//							Button * m_pClose = dynamic_cast<Button*>(pRoot->getChildByTag(CLOSE));
							if(showClose)
							{
								LoginLayer::createObject(nullptr);
							}
							else
							{
								LoginLayer::createObject(CCDirector::sharedDirector()->getRunningScene());
							}

							hideModal();
						}
						break;
				}
			}
			break;
	}
}
void RegisterLayer::setIpConnect(bool isIp)
{
	m_bIpConnect = isIp;
}

void RegisterLayer::createObject(bool showClose, bool bIpConnect){

	if(!s_pInstance){
		s_pInstance = RegisterLayer::create();
	}

	CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
	pScene->addChild(s_pInstance);
	s_pInstance->showCloseBtn(showClose);
	s_pInstance->setIpConnect(bIpConnect);

}


void RegisterLayer::hideModal(){

	if(s_pInstance&&s_pInstance->getParent()){
		s_pInstance->removeFromParent();
		s_pInstance = nullptr;
	}

}

void RegisterLayer::notifySuccess(Ref * obj){

		BaseScene * pLayer = new MainLayer();
		Director::getInstance()->replaceScene(BaseScene::createScene(pLayer));
		pLayer->release();

		if(UserInfo::instance()->getAccount() != UserInfo::instance()->getMachineID())
		  QFileUtil::saveLoginFile(UserInfo::instance()->getAccount().c_str(),UserInfo::instance()->getPassword().c_str());

		hideModal();
}

void RegisterLayer::notifyFail(Ref * obj){

	CMD_MB_LogonFailure * pFailure = (CMD_MB_LogonFailure *)obj;
	string szError = Util::getStringWithTChar(pFailure->szDescribeString);
	PromptLayer::createObject((char *)szError.c_str());
	UserInfo::instance()->setAccount("");
	//MainController::getInstance()->startConnect(NetworkManager::FLAG_HALL, Util::getIpByHostName(LOGIN_SERVER_IP), LOGIN_SERVER_PORT);

}

void RegisterLayer::showCloseBtn(bool isShow)
{
//	Button * m_pClose = dynamic_cast<Button*>(pRoot->getChildByTag(CLOSE));
//	m_pClose->setVisible(isShow);
	showClose = isShow;
}

void RegisterLayer::editBoxEditingDidBegin(EditBox * editBox){}

void RegisterLayer::editBoxEditingDidEnd(EditBox * editBox){}

void RegisterLayer::editBoxTextChanged(EditBox * editBox){}

void RegisterLayer::editBoxReturn(EditBox * editBox){}

