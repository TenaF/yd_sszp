#include "hall/data/DataManager.h"
#include "sdk/util/QFileUtil.h"
#include "SaveTakeLayer.h"
#include "sdk/ui/PromptLayer.h"
#include "hall/ui/MainLayer.h"
#include "hall/ui/GiveLayer.h"
#include "hall/ui/BillLayer.h"

enum UI_GIVE {
	TAKE_BTN = 7457236,			//
	SAVE_BTN = 7457234,			//
	CLOSE_BTN = 7457240,			//

	SCORE_LABEL = 7457243,
	INSURE_LABEL = 7457241,
	AMOUNT_TEXT = 7457246,

	BANKPASS_TEXT = 7461499,
	FORGET_PASS	=  	7461500 ,

	REFRESH_BTN = 7461597,
};

#define TAG "SaveTakeLayer"

SaveTakeLayer * SaveTakeLayer::s_pInstance = nullptr;

SaveTakeLayer::SaveTakeLayer(void) {
	pRoot = nullptr;
	amountBox = nullptr;
}

SaveTakeLayer::~SaveTakeLayer(void) {
	pRoot = nullptr;
	amountBox = nullptr;
	//mainLayer = nullptr;
}

bool SaveTakeLayer::init() {
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

void SaveTakeLayer::onEnter() {

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SaveTakeLayer::notifySuccess), LOGON_TRANSFER_SUCCESS, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SaveTakeLayer::notifySuccess), LOGON_USER_INSURE_SUCCESS, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SaveTakeLayer::notifyRefresh), LOGON_REFRESH_USER_INFO, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SaveTakeLayer::notifyFail), LOGON_USER_INSURE_FAILURE, nullptr);

	LayerColor::onEnter();

	//加载UI
	pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("save_take.json");
	CCSize visibleSize = Director::getInstance()->getVisibleSize();
	pRoot->setAnchorPoint(CCPoint(0.5f,0.5f));
	pRoot->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	this->addChild(pRoot);

	//取款
	Button * pTake = static_cast<Button*>(pRoot->getChildByTag(TAKE_BTN));
	pTake->addTouchEventListener(this, toucheventselector(SaveTakeLayer::touchButtonEvent));

	//存款
	Button * pSave = static_cast<Button*>(pRoot->getChildByTag(SAVE_BTN));
	pSave->addTouchEventListener(this,toucheventselector(SaveTakeLayer::touchButtonEvent));

	//刷新
	Button * pRefresh = static_cast<Button*>(pRoot->getChildByTag(REFRESH_BTN));
	pRefresh->addTouchEventListener(this, toucheventselector(SaveTakeLayer::touchButtonEvent));

	//关闭
	Button * pClose = static_cast<Button*>(pRoot->getChildByTag(CLOSE_BTN));
	pClose->addTouchEventListener(this, toucheventselector(SaveTakeLayer::touchButtonEvent));

	Text * pScore = static_cast<Text*>(pRoot->getChildByTag(SCORE_LABEL));
	pScore->setText(StringUtils::toString(UserInfo::instance()->getScore()));

	Text * pInsure = static_cast<Text*>(pRoot->getChildByTag(INSURE_LABEL));
	pInsure->setText(StringUtils::toString(UserInfo::instance()->getInsureScore()));

	//数量
	TextField * pAmount = dynamic_cast<TextField*>(pRoot->getChildByTag(AMOUNT_TEXT));
	CC_ASSERT(pAmount);

	amountBox = EditBox::create(pAmount->getContentSize(), Scale9Sprite::create("commonresource/edit_black.png"));
	amountBox->setPosition(pAmount->getPosition());
	//amountBox->setAnchorPoint(CCPoint::ZERO);
	amountBox->setFont("", 30);
	amountBox->setZOrder(pAmount->getZOrder());
	amountBox->setPlaceHolder(U2F("存取数量").c_str());
	amountBox->setReturnType(EditBox::KeyboardReturnType::DONE);
	amountBox->setInputMode(EditBox::InputMode::NUMERIC);
	amountBox->setDelegate(this);
	amountBox->setAnchorPoint(CCPoint::ZERO);
	amountBox->setTouchEnabled(true);
	amountBox->setVisible(true);
	amountBox->setMaxLength(15);

	pRoot->addChild(amountBox);
	pAmount->removeFromParent();
	amountBox->setTag(AMOUNT_TEXT);

	//
	TextField * pBankPass = dynamic_cast<TextField*>(pRoot->getChildByTag(BANKPASS_TEXT));
	CC_ASSERT(pBankPass);
	
	m_PassBox = EditBox::create(pBankPass->getContentSize(),Scale9Sprite::create("commonresource/edit_black.png"));
	m_PassBox->setPosition(pBankPass->getPosition());
	m_PassBox->setAnchorPoint(CCPoint::ZERO);
	m_PassBox->setFontSize(30);
	m_PassBox->setZOrder(pBankPass->getZOrder());
	m_PassBox->setPlaceHolder(U2F("密码").c_str());
	m_PassBox->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
	m_PassBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
	m_PassBox->setInputFlag(EditBox::InputFlag::PASSWORD);
	m_PassBox->setDelegate(this);
	m_PassBox->setTouchEnabled(true);
	m_PassBox->setMaxLength(20);

	pBankPass->removeFromParent();
	m_PassBox->setTag(BANKPASS_TEXT);
	pRoot->addChild(m_PassBox);

	//
	CheckBox * pForgetPass = dynamic_cast<CheckBox*>(pRoot->getChildByTag(FORGET_PASS));
	pForgetPass->setTouchEnabled(true);
	pForgetPass->setSelectedState(false);

	//清空数据
	string lastBankPass = QFileUtil::getBankPass();
	if(!lastBankPass.empty())
	{
		m_PassBox->setText(lastBankPass.c_str());
		pForgetPass->setSelectedState(true);
	}

	//上面的MENU
	m_TabBankFun[0] = CCMenuItemImage::create("commonresource/tab_save_n.png", "commonresource/tab_save_s.png", "commonresource/tab_save_s.png", this,menu_selector(SaveTakeLayer::MenuSelect));
	m_TabBankFun[0]->setTag(6000);
	m_TabBankFun[1] = CCMenuItemImage::create("commonresource/tab_give_n.png", "commonresource/tab_give_s.png", "commonresource/tab_give_s.png", this,menu_selector(SaveTakeLayer::MenuSelect));
	m_TabBankFun[1]->setTag(6001);
	m_TabBankFun[2] = CCMenuItemImage::create("commonresource/tab_bill_n.png", "commonresource/tab_bill_s.png", "commonresource/tab_bill_s.png", this,menu_selector(SaveTakeLayer::MenuSelect));
	m_TabBankFun[2]->setTag(6002);
	CCMenu* pMenu = CCMenu::create(m_TabBankFun[0], m_TabBankFun[1], m_TabBankFun[2],NULL);
	pMenu->alignItemsHorizontallyWithPadding(2);
	pMenu->setPosition(ccp(860, 700));
	m_TabBankFun[0]->selected();
	m_TabBankFun[1]->unselected();
	m_TabBankFun[2]->unselected();
	pRoot->addChild(pMenu);
}

void SaveTakeLayer::onExit() {

	NotificationCenter::getInstance()->removeObserver(this, LOGON_TRANSFER_SUCCESS);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_USER_INSURE_SUCCESS);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_USER_INSURE_FAILURE);
	NotificationCenter::getInstance()->removeObserver(this, LOGON_REFRESH_USER_INFO);

	removeAllChildrenWithCleanup(true);
	LayerColor::onExit();

	
}

void SaveTakeLayer::touchButtonEvent(Ref * obj ,TouchEventType eventType) {

	Button * pButton = (Button *) obj;
	switch(eventType) {
		case TouchEventType::TOUCH_EVENT_BEGAN : {
			switch(pButton->getTag()) {

				case TAKE_BTN :        //取款
				{

					string strAmount = amountBox->getText();
					if (strAmount.empty()) {
						PromptLayer::createObject(U2F("取款数量不能为空").c_str());
						return;
					}

					string psw = m_PassBox->getText();
					if(psw.empty()){
						PromptLayer::createObject(U2F("密码不能为空").c_str());
						return;
					}

					CheckBox * pForgetPass = dynamic_cast<CheckBox*>(pRoot->getChildByTag(FORGET_PASS));
					bool checkboxState = pForgetPass->getSelectedState();
					if(checkboxState)
					{
						//保存登录记录
						QFileUtil::saveBankPass(psw.c_str());
					}
					else
					{
						QFileUtil::saveBankPass("");
					}

					cocos2d::Value val = cocos2d::Value(strAmount);

					if (val.asDouble()<=0)
					{
						PromptLayer::createObject(U2F("取款数量必须大于0").c_str());
						return;
					}

					//SendMsg::sendTakeMoney(val.asDouble(), Util::strToMd5Str(psw).c_str());


					UserInfo::instance()->pswTemp = psw;
					UserInfo::instance()->setAmountTemp(val.asDouble());


					//如果绑定了微信 就验证码
					if (UserInfo::instance()->getbWeChatBind())
					{
						// 						//请求验证码 并且弹出验证界面
						// 						UserInfo::instance()->httpRequest();
						// 						UserInfo::instance()->SendType = 0;
						// 						UserInfo::instance()->mainCmd = MDM_GP_GET_KEY; 
						// 						UserInfo::instance()->subCmd = SUB_GP_GET_KEY; 
						// 						Verifaction::createObjectAndShowOnTopScene(1);
						PromptLayer::createObject("验证码已发送，请在打开微信，在公众号中查看！");
					}

					UserInfo::instance()->httpRequest();
					UserInfo::instance()->SendType = 0;
					UserInfo::instance()->mainCmd = MDM_GP_GET_KEY;
					UserInfo::instance()->subCmd = SUB_GP_GETKEY_TAKESCORE;


					m_PassBox->setText("");
				}
					break;

				case SAVE_BTN :        //存款
					{

						string strAmount = amountBox->getText();
						if (strAmount.empty()) {
							PromptLayer::createObject(U2F("存款数量不能为空").c_str());
							return;
						}

						cocos2d::Value val = cocos2d::Value(strAmount);

						if (val.asDouble()<=0)
						{
							PromptLayer::createObject(U2F("存款数量必须大于0").c_str());
							return;
						}
						//SendMsg::sendSaveMoney(val.asDouble());
						UserInfo::instance()->httpRequest();
						UserInfo::instance()->SendType = 0;
						UserInfo::instance()->mainCmd = MDM_GP_GET_KEY;
						UserInfo::instance()->subCmd = SUB_GP_GETKEY_SAVESCORE;


						UserInfo::instance()->setAmountTemp(val.asDouble());
						amountBox->setText("0");
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

				case CLOSE_BTN : {
					hideModal();
				}
					break;

			}
		}
			break;
	}
}

//创建对象
void SaveTakeLayer::createObject() {

	if (!s_pInstance) {
		s_pInstance = SaveTakeLayer::create();

		CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
		pScene->addChild(s_pInstance);
	}


}

void SaveTakeLayer::hideModal() {
	if (s_pInstance) {
		s_pInstance->removeFromParent();
		s_pInstance = nullptr;
	}
}

//通知接口
void SaveTakeLayer::notifySuccess(Ref * obj) {

	Text * pScore = static_cast<Text*>(pRoot->getChildByTag(SCORE_LABEL));
	pScore->setText(StringUtils::toString(UserInfo::instance()->getScore()));

	Text * pInsure = static_cast<Text*>(pRoot->getChildByTag(INSURE_LABEL));
	pInsure->setText(StringUtils::toString(UserInfo::instance()->getInsureScore()));

	amountBox->setText("0");
	
	PromptLayer::createObject("操作完成");

}

//刷新
void SaveTakeLayer::notifyRefresh(Ref * obj)
{

	Text * pScore = static_cast<Text*>(pRoot->getChildByTag(SCORE_LABEL));
	pScore->setText(StringUtils::toString(UserInfo::instance()->getScore()));

	Text * pInsure = static_cast<Text*>(pRoot->getChildByTag(INSURE_LABEL));
	pInsure->setText(StringUtils::toString(UserInfo::instance()->getInsureScore()));

}

//通知接口
void SaveTakeLayer::notifyFail(Ref * obj) {

	char * pFail = (char *) obj;
	PromptLayer::createObject(pFail);
}

void SaveTakeLayer::editBoxEditingDidBegin(EditBox * editBox) {
}

void SaveTakeLayer::editBoxEditingDidEnd(EditBox * editBox) {
}

void SaveTakeLayer::editBoxTextChanged(EditBox * editBox) {
}

void SaveTakeLayer::editBoxReturn(EditBox * editBox) {
}

void SaveTakeLayer::MenuSelect(Ref* obj)
{
	switch(((CCMenuItemImage*)obj)->getTag())
	{
	case 6001:
		{
			hideModal();
			GiveLayer::createObject();
			break;
		}
	case 6002:
		{
			hideModal();
			BillLayer::createObject();
			break;
		}
	default:
		{
			m_TabBankFun[0]->selected();
			m_TabBankFun[1]->unselected();
			m_TabBankFun[2]->unselected();
			break;
		}
	}
}