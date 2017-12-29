//
//  LogoScene.cpp
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#include "sdk/sdk.h"
#include "UpdatePasswordLayer.h"
#include "hall/data/DataManager.h"
#include "sdk/ui/PromptLayer.h"
#include "sdk/util/QFileUtil.h"

enum UI_REGISTER
{
	CLOSE				=	152  ,			//
	CONFIRM				=   151 ,			//

	OLD_PASSWORD		=	143 ,
	NEW_PASSWORD		=	149 ,
	CONFIRM_PASSWORD	=	150 ,
};

UpdatePasswordLayer * UpdatePasswordLayer::s_pInstance = nullptr;

UpdatePasswordLayer::UpdatePasswordLayer(){
		passwordBox = nullptr;
		pOldEdit = nullptr;
		confirmBox = nullptr;

		//
}

UpdatePasswordLayer::~UpdatePasswordLayer(){
	this->removeAllChildrenWithCleanup(true);
	passwordBox = nullptr;
	pOldEdit = nullptr;
	confirmBox = nullptr;
}


bool UpdatePasswordLayer::init()
{
	if(!LayerColor::init())
	{
		return false;
	}

	CCSize visibleSize = Director::getInstance()->getVisibleSize();
	setTouchEnabled(true);
	this->setSwallowsTouches(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	setColor(ccc3(0,0,0));
	setOpacity(150);
	setContentSize(CCSizeMake(visibleSize.width,visibleSize.height));

	pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("modify_password.json");
	pRoot->setAnchorPoint(CCPoint(0.5f,0.5f));
	pRoot->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	this->addChild(pRoot);

	//
	Button * pConfirm = dynamic_cast<Button*>(pRoot->getChildByTag(CONFIRM));
	CC_ASSERT(pConfirm);
	pConfirm->setTouchEnabled(true);
	pConfirm->addTouchEventListener(this,toucheventselector(UpdatePasswordLayer::touchButton));


	//
	Button * m_pClose = dynamic_cast<Button*>(pRoot->getChildByTag(CLOSE));
	CC_ASSERT(m_pClose);
	m_pClose->setTouchEnabled(true);
	m_pClose->addTouchEventListener(this,toucheventselector(UpdatePasswordLayer::touchButton));

	//
	TextField * pOld = static_cast<TextField*>(pRoot->getChildByTag(OLD_PASSWORD));
	CC_ASSERT(pOld);

	pOld->setVisible(false);
	pOldEdit = EditBox::create(pOld->getContentSize(),Scale9Sprite::create());
	pOldEdit->setPosition(pOld->getPosition());
	pOldEdit->setFontSize(30);
	pOldEdit->setZOrder(pOld->getZOrder());
	pOldEdit->setPlaceHolder(pOld->getPlaceHolder().c_str());
	pOldEdit->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
	pOldEdit->setInputMode(EditBox::InputMode::SINGLE_LINE);
	pOldEdit->setInputFlag(EditBox::InputFlag::PASSWORD);
	pOldEdit->setAnchorPoint(CCPoint::ZERO);
	pOldEdit->setDelegate(this);
	pOldEdit->setTouchEnabled(true);
	pOldEdit->setVisible(true);
	pOldEdit->setMaxLength(15);
	pRoot->addChild(pOldEdit);
	pOld->removeFromParent();
	pOldEdit->setTag(OLD_PASSWORD);


	//
	TextField * pPassword = dynamic_cast<TextField*>(pRoot->getChildByTag(NEW_PASSWORD));
	CC_ASSERT(pPassword);

	passwordBox = EditBox::create(pPassword->getContentSize(),Scale9Sprite::create());
	passwordBox->setPosition(pPassword->getPosition());
	passwordBox->setFontSize(30);
	passwordBox->setZOrder(pPassword->getZOrder());
	passwordBox->setAnchorPoint(CCPoint::ZERO);
	passwordBox->setPlaceHolder(pPassword->getPlaceHolder().c_str());
	passwordBox->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
	passwordBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
	passwordBox->setDelegate(this);
	passwordBox->setTouchEnabled(true);
	passwordBox->setVisible(true);
	passwordBox->setInputFlag(EditBox::InputFlag::PASSWORD);
	passwordBox->setMaxLength(15);
	pRoot->addChild(passwordBox);
	pPassword->removeFromParent();
	passwordBox->setTag(NEW_PASSWORD);


	//
	TextField * pConfirePassword = dynamic_cast<TextField*>(pRoot->getChildByTag(CONFIRM_PASSWORD));
	CC_ASSERT(pConfirePassword);

	confirmBox = EditBox::create(pConfirePassword->getContentSize(),Scale9Sprite::create());
	confirmBox->setPosition(pConfirePassword->getPosition());
	confirmBox->setFontSize(30);
	confirmBox->setZOrder(pConfirePassword->getZOrder());
	confirmBox->setPlaceHolder(pConfirePassword->getPlaceHolder().c_str());
	confirmBox->setAnchorPoint(CCPoint::ZERO);
	confirmBox->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
	confirmBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
	confirmBox->setDelegate(this);
	confirmBox->setTouchEnabled(true);
	confirmBox->setVisible(true);
	confirmBox->setInputFlag(EditBox::InputFlag::PASSWORD);
	pRoot->addChild(confirmBox);
	pConfirePassword->removeFromParent();
	confirmBox->setTag(CONFIRM_PASSWORD);

	return true;
}


void UpdatePasswordLayer::onEnter(){
	//

	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(UpdatePasswordLayer::notifySuccess),LOGON_OPERATE_SUCCESS,nullptr);
	//NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(UpdatePasswordLayer::notifyFail), LOGON_OPERATE_FAILURE,nullptr);

	LayerColor::onEnter();

	pOldEdit->setText("");
	passwordBox->setText("");
	confirmBox->setText("");

}

void UpdatePasswordLayer::onExit(){
	LayerColor::onExit();
	NotificationCenter::getInstance()->removeObserver(this,LOGON_OPERATE_SUCCESS);
	//NotificationCenter::getInstance()->removeObserver(this,LOGON_OPERATE_FAILURE);
}

//
void UpdatePasswordLayer::createObject(){

	if(!s_pInstance){
		s_pInstance = UpdatePasswordLayer::create();
	}

	CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
	pScene->addChild(s_pInstance);

}

//
void UpdatePasswordLayer::hideModal(){

	if(s_pInstance){
		s_pInstance->removeFromParent();
		s_pInstance = nullptr;
	}
}


//
void UpdatePasswordLayer::touchButton(Ref * obj ,TouchEventType eventType)
{
	Button * pButton = (Button *)obj;
	switch(eventType){
		case TouchEventType::TOUCH_EVENT_ENDED:
			{
				switch(pButton->getTag()){

					case CONFIRM: //
						{
							string psw = passwordBox->getText();
							if(psw.empty()){
								char * pError = "密码不能为空";
								PromptLayer::createObject((char *)pError);
								return;
							}

							string cpsw = confirmBox->getText();
							if(cpsw.empty()){
								char * pError = "确认密码不能为空";
								PromptLayer::createObject(pError);
								return;
							}

							if(psw != cpsw){
								char * pError = "确认密码和密码不相同";
								PromptLayer::createObject((char *)pError);
								return;
							}

							if(Util::strToMd5Str(psw) == UserInfo::instance()->getPassword()){
								char * pError = "新密码不能和原密码相同";
								PromptLayer::createObject((char *)pError);
								return;
							}

							UserInfo::instance()->setTempPassword(psw);
							SendMsg::sendUpdatePassword(Util::strToMd5Str(pOldEdit->getText()).c_str(),Util::strToMd5Str(psw).c_str());
							
						}
						break;

					case CLOSE:
						{
							hideModal();
						}
						break;
				}
			}
			break;
	}
}



void UpdatePasswordLayer::editBoxEditingDidEnd(EditBox * editBox)
{
}


void UpdatePasswordLayer::editBoxTextChanged(EditBox * editBox)
{
}


void UpdatePasswordLayer::editBoxReturn(EditBox * editBox)
{
}

void UpdatePasswordLayer::editBoxEditingDidBegin(EditBox * editBox)
{
}

//修改失败
//void UpdatePasswordLayer::notifyFail(Ref * obj)
//{
//
//		CMD_GP_OperateFailure * pFail = (CMD_GP_OperateFailure *)obj;
//		string error = Util::getStringWithTChar(pFail->szDescribeString);
//		PromptLayer::createObject((char *)error.c_str());
//		CCLog(" update password fail %" ,error.c_str());
//
//}

//修改成功
void UpdatePasswordLayer::notifySuccess(Ref * obj)
{
	UserInfo::instance()->setPassword(UserInfo::instance()->getTempPassword());
	UserInfo::instance()->setTempPassword("");

	QFileUtil::saveLastLogin(UserInfo::instance()->getAccount().c_str(),UserInfo::instance()->getPassword().c_str());

	if(UserInfo::instance()->getAccount() != UserInfo::instance()->getMachineID())
	{
		CCLog("  设置密码 %s",UserInfo::instance()->getPassword().c_str());
		QFileUtil::saveLoginFile(UserInfo::instance()->getAccount().c_str(),UserInfo::instance()->getPassword().c_str());
	}

	PromptLayer::createObject(U2F("修改成功").c_str());
		
	s_pInstance->hideModal();
	
}
