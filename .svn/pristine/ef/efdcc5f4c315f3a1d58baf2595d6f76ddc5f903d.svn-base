#include "hall/data/DataManager.h"
#include "sdk/util/QFileUtil.h"
#include "GiveLayer.h"
#include "sdk/ui/PromptLayer.h"
#include "hall/ui/MainLayer.h"
#include "hall/ui/SaveTakeLayer.h"
#include "hall/ui/BillLayer.h"

enum UI_GIVE {
	GIVE_CONFIRM_BTN = 448,			//
	GIVE_CANEL_BTN = 7457194,			//
	GIVE_SCORE_LABEL = 464,			//
	GIVE_GAME_ID_BOX = 460,
	GIVE_AMOUNT_BOX = 7457198,
	GIVE_PASSWORD_BOX = 462,			//
	GIVE_CLOSE = 7457194,

	GIVE_BANK_SCORE_LABEL = 7461528,
	GIVE_FORGET_PASS	=  	7461531 ,

	GIVE_REFRESH_BTN = 7461598,
};

#define TAG "GiveLayer"

GiveLayer * GiveLayer::s_pInstance = nullptr;

GiveLayer::GiveLayer(void) {
	pRoot = nullptr;
	nicknameBox = nullptr;
	amountBox = nullptr;
	passwordBox = nullptr;
	//mainLayer = nullptr;
	
}

GiveLayer::~GiveLayer(void) {
	pRoot = nullptr;
	nicknameBox = nullptr;
	amountBox = nullptr;
	passwordBox = nullptr;
	//mainLayer = nullptr;
}

bool GiveLayer::init() {
	if (!LayerColor::init()) {
		return false;
	}

	CCSize visibleSize = Director::getInstance()->getWinSize();

	//遮盖层
	/*
	setTouchEnabled(true);
	this->setSwallowsTouches(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	setColor(ccc3(0, 0, 0));
	setOpacity(150);
	setContentSize(CCSizeMake(visibleSize.width, visibleSize.height));
	*/

	return true;
}

void GiveLayer::onEnter() {

//	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GiveLayer::notifySuccess), LOGON_USER_INSURE_SUCCESS, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GiveLayer::notifySuccess), LOGON_TRANSFER_SUCCESS, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GiveLayer::notifyFail), LOGON_USER_INSURE_FAILURE, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GiveLayer::notifyRefreshSuccess), LOGON_USER_INSURE_SUCCESS, nullptr);

	LayerColor::onEnter();

	//加载UI
	pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("give.json");
	CCSize visibleSize = Director::getInstance()->getVisibleSize();
	pRoot->setAnchorPoint(CCPoint(0.5f,0.5f));
	pRoot->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	this->addChild(pRoot);

	//确认
	Button * pConfirm = static_cast<Button*>(pRoot->getChildByTag(GIVE_CONFIRM_BTN));
	pConfirm->addTouchEventListener(this, toucheventselector(GiveLayer::touchButtonEvent));

	//取消
	//Button * pCanel = static_cast<Button*>(pRoot->getChildByTag(GIVE_CANEL_BTN));
	//pCanel->addTouchEventListener(this,toucheventselector(GiveLayer::touchButtonEvent));

	//刷新
	Button * pRefresh = static_cast<Button*>(pRoot->getChildByTag(GIVE_REFRESH_BTN));
	pRefresh->addTouchEventListener(this, toucheventselector(GiveLayer::touchButtonEvent));

	//关闭
	Button * pClose = static_cast<Button*>(pRoot->getChildByTag(GIVE_CLOSE));
	pClose->addTouchEventListener(this, toucheventselector(GiveLayer::touchButtonEvent));

	/*
	Text * pPrompt = static_cast<Text*>(pRoot->getChildByTag(GIVE_PROMPT));
	pPrompt->setText(StringUtils::format("%d", DataManager::getInstance()->getTransferRatio()).c_str());
	*/

	Text * pScore = static_cast<Text*>(pRoot->getChildByTag(GIVE_SCORE_LABEL));
	pScore->setText(StringUtils::toString(UserInfo::instance()->getScore()));

	Text * pBankScore = static_cast<Text*>(pRoot->getChildByTag(GIVE_BANK_SCORE_LABEL));
	pBankScore->setText(StringUtils::toString(UserInfo::instance()->getInsureScore()));

	//游戏ID
	TextField * pGameID = static_cast<TextField*>(pRoot->getChildByTag(GIVE_GAME_ID_BOX));
	pGameID->setVisible(false);

	nicknameBox = EditBox::create(pGameID->getContentSize(), Scale9Sprite::create("commonresource/edit_black.png"));
	nicknameBox->setPosition(pGameID->getPosition());
	nicknameBox->setAnchorPoint(CCPoint::ZERO);
	nicknameBox->setFont("", 30);
	nicknameBox->setZOrder(pGameID->getZOrder());
	nicknameBox->setPlaceHolder(U2F("对方GameID").c_str());
	nicknameBox->setReturnType(EditBox::KeyboardReturnType::DONE);
	nicknameBox->setInputMode(EditBox::InputMode::ANY);
	nicknameBox->setDelegate(this);
	nicknameBox->setTouchEnabled(true);
	nicknameBox->setVisible(true);
	nicknameBox->setMaxLength(15);

	pRoot->addChild(nicknameBox);
	pGameID->removeFromParent();
	nicknameBox->setTag(GIVE_GAME_ID_BOX);

	//数量
	TextField * pAmount = dynamic_cast<TextField*>(pRoot->getChildByTag(GIVE_AMOUNT_BOX));
	CC_ASSERT(pAmount);

	amountBox = EditBox::create(pAmount->getContentSize(), Scale9Sprite::create("commonresource/edit_black.png"));
	amountBox->setPosition(pAmount->getPosition());
	amountBox->setFont("", 30);
	amountBox->setZOrder(pAmount->getZOrder());
	amountBox->setPlaceHolder(U2F("赠送数量").c_str());
	amountBox->setReturnType(EditBox::KeyboardReturnType::DONE);
	amountBox->setInputMode(EditBox::InputMode::NUMERIC);
	amountBox->setDelegate(this);
	amountBox->setAnchorPoint(CCPoint::ZERO);
	amountBox->setTouchEnabled(true);
	amountBox->setVisible(true);
	amountBox->setMaxLength(15);

	pRoot->addChild(amountBox);
	pAmount->removeFromParent();
	amountBox->setTag(GIVE_AMOUNT_BOX);

	////密码
	TextField * pPassword = dynamic_cast<TextField*>(pRoot->getChildByTag(GIVE_PASSWORD_BOX));
	CC_ASSERT(pPassword);

	passwordBox = EditBox::create(pPassword->getContentSize(), Scale9Sprite::create("commonresource/edit_black.png"));
	passwordBox->setPosition(pPassword->getPosition());
	passwordBox->setAnchorPoint(CCPoint::ZERO);
	passwordBox->setFont("", 30);
	passwordBox->setZOrder(pPassword->getZOrder());
	passwordBox->setPlaceHolder(U2F("银行密码").c_str());
	passwordBox->setReturnType(EditBox::KeyboardReturnType::DONE);
	passwordBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
	passwordBox->setDelegate(this);
	passwordBox->setTouchEnabled(true);
	passwordBox->setVisible(true);
	passwordBox->setInputFlag(EditBox::InputFlag::PASSWORD);
	passwordBox->setMaxLength(20);

	pRoot->addChild(passwordBox);
	pPassword->removeFromParent();
	passwordBox->setTag(GIVE_PASSWORD_BOX);

	//
	CheckBox * pForgetPass = dynamic_cast<CheckBox*>(pRoot->getChildByTag(GIVE_FORGET_PASS));
	pForgetPass->setTouchEnabled(true);
	pForgetPass->setSelectedState(false);

	//清空数据
	string lastBankPass = QFileUtil::getBankPass();
	if(!lastBankPass.empty())
	{
		passwordBox->setText(lastBankPass.c_str());
		pForgetPass->setSelectedState(true);
	}

	//上面的MENU
	m_TabBankFun[0] = CCMenuItemImage::create("commonresource/tab_save_n.png", "commonresource/tab_save_s.png", "commonresource/tab_save_s.png", this,menu_selector(GiveLayer::MenuSelect));
	m_TabBankFun[0]->setTag(6000);
	m_TabBankFun[1] = CCMenuItemImage::create("commonresource/tab_give_n.png", "commonresource/tab_give_s.png", "commonresource/tab_give_s.png", this,menu_selector(GiveLayer::MenuSelect));
	m_TabBankFun[1]->setTag(6001);
	m_TabBankFun[2] = CCMenuItemImage::create("commonresource/tab_bill_n.png", "commonresource/tab_bill_s.png", "commonresource/tab_bill_s.png", this,menu_selector(GiveLayer::MenuSelect));
	m_TabBankFun[2]->setTag(6002);
	CCMenu* pMenu = CCMenu::create(m_TabBankFun[0], m_TabBankFun[1], m_TabBankFun[2],NULL);
	pMenu->alignItemsHorizontallyWithPadding(2);
	pMenu->setPosition(ccp(860, 700));
	m_TabBankFun[0]->unselected();
	m_TabBankFun[1]->selected();
	m_TabBankFun[2]->unselected();
	pRoot->addChild(pMenu);
	
	//CCSequence * popupActions = CCSequence::create(CCScaleTo::create(0.0,0.0),CCScaleTo::create(0.06,1.05),CCScaleTo::create(0.08,0.95),CCScaleTo::create(0.08,1.0),NULL);
	//this->runAction(popupActions);
}

void GiveLayer::onExit() {

//	NotificationCenter::getInstance()->removeObserver(this, LOGON_USER_INSURE_SUCCESS);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_TRANSFER_SUCCESS);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_USER_INSURE_FAILURE);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_USER_INSURE_SUCCESS);

	removeAllChildrenWithCleanup(true);
	LayerColor::onExit();

	
}

void GiveLayer::touchButtonEvent(Ref * obj ,TouchEventType eventType) {

	Button * pButton = (Button *) obj;
	switch(eventType) {
		case TouchEventType::TOUCH_EVENT_BEGAN : {
			switch(pButton->getTag()) {

				case GIVE_CONFIRM_BTN :        //赠送
				{

					string nick = nicknameBox->getText();
					if (nick.empty()) {
						PromptLayer::createObject(U2F("赠送对方的ID不能为空").c_str());
						return;
					}

					if (cocos2d::Value(nick).asInt() == UserInfo::instance()->getGameId()) {
						PromptLayer::createObject(U2F("不能填写自己的游戏ID").c_str());
						return;
					}

					/*Util::trim(nick);
					if (nick == UserInfo::instance()->getNickName()) {
					PromptLayer::createObject(U2F("不能填写自己的昵称").c_str());
					return;
					}*/


					string strAmount = amountBox->getText();
					if (strAmount.empty()) {
						PromptLayer::createObject(U2F("赠送数量不能为空").c_str());
						return;
					}

					string psw = passwordBox->getText();
					if (psw.empty()) {
					PromptLayer::createObject(U2F("登录密码不能为空").c_str());
					return;
					}

					cocos2d::Value val = cocos2d::Value(strAmount);

					//SendMsg::sendTransferMoney(F2U(nick).c_str(), val.asDouble(),Util::strToMd5Str(psw).c_str());

					UserInfo::instance()->pswTemp = psw;//密码
					UserInfo::instance()->nickTemp = nick;//GameID
					//数量
					//SCORE val = std::stoll(strAmount);
					UserInfo::instance()->setAmountTemp(val.asDouble());
					//如果绑定了微信 就验证码
					if (UserInfo::instance()->getbWeChatBind())
					{
						// 						//请求验证码 并且弹出验证界面
						// 						UserInfo::instance()->httpRequest();
						// 						UserInfo::instance()->SendType = 0;
						// 						UserInfo::instance()->mainCmd = MDM_GP_GET_KEY;
						// 						UserInfo::instance()->subCmd = SUB_GP_GET_KEY;
						// 
						// 						Verifaction::createObjectAndShowOnTopScene(2);
						PromptLayer::createObject("验证码已发送，请在打开微信，在公众号中查看！");
					}
					UserInfo::instance()->httpRequest();
					UserInfo::instance()->SendType = 0;
					UserInfo::instance()->mainCmd = MDM_GP_GET_KEY;
					UserInfo::instance()->subCmd = SUB_GP_GETKEY_TRANSFERSCORE;
					passwordBox->setText("");
				}
					break;

				case GIVE_CLOSE : {
					hideModal();
				}
					break;
				case GIVE_REFRESH_BTN :        //刷新
					{
												   UserInfo::instance()->httpRequest();
												   UserInfo::instance()->SendType = 0;
												   UserInfo::instance()->mainCmd = MDM_GP_GET_KEY;
												   UserInfo::instance()->subCmd = SUB_GP_GETKEY_QUERYSCORE;
					}
						break;

			}
		}
			break;
	}
}

//创建对象
void GiveLayer::createObject() {

	if (!s_pInstance) {
		s_pInstance = GiveLayer::create();

		CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
		pScene->addChild(s_pInstance);
	}


}

void GiveLayer::hideModal() {
	if (s_pInstance) {
		s_pInstance->removeFromParent();
		s_pInstance = nullptr;
	}
}

//通知接口
void GiveLayer::notifySuccess(Ref * obj) {

	hideModal();
	PromptLayer::createObject("转账完成");

}

void GiveLayer::notifyRefreshSuccess(Ref * obj)
{
	PromptLayer::createObject("刷新完成");
}

//通知接口
void GiveLayer::notifyFail(Ref * obj) {

	char * pFail = (char *) obj;
	PromptLayer::createObject(pFail);
}

void GiveLayer::editBoxEditingDidBegin(EditBox * editBox) {
}

void GiveLayer::editBoxEditingDidEnd(EditBox * editBox) {
}

void GiveLayer::editBoxTextChanged(EditBox * editBox) {
}

void GiveLayer::editBoxReturn(EditBox * editBox) {
}

void GiveLayer::MenuSelect(Ref* obj)
{
	switch(((CCMenuItemImage*)obj)->getTag())
	{
	case 6000:
		{
			hideModal();
			SaveTakeLayer::createObject();
		}
		break;
	case 6002:
		{
			hideModal();
			BillLayer::createObject();
		}
		break;
	default:
		{
			m_TabBankFun[0]->unselected();
			m_TabBankFun[1]->selected();
			m_TabBankFun[2]->unselected();
		}
		break;
	}
}