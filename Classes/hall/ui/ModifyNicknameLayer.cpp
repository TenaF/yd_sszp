//
//  LogoScene.cpp
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#include "sdk/sdk.h"
#include "ModifyNicknameLayer.h"
#include "hall/data/DataManager.h"
#include "hall/data/DataManager.h"
#include "sdk/ui/PromptLayer.h"

enum UI_MODIFY_NICKNAME
{
	CLOSE			=	7457190 ,			//关闭
	CONFIRM			=   7457187 ,
	TEXT_NICK		=	7457188 ,
};

ModifyNicknameLayer * ModifyNicknameLayer::s_pInstance = nullptr;

ModifyNicknameLayer::ModifyNicknameLayer(){
}

ModifyNicknameLayer::~ModifyNicknameLayer(){
	this->removeAllChildrenWithCleanup(true);
}


bool ModifyNicknameLayer::init()
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

	pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("modify_nickname.json");
	pRoot->setAnchorPoint(CCPoint(0.5f,0.5f));
	pRoot->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	this->addChild(pRoot);

	//关闭
	Button * pClose = dynamic_cast<Button*>(pRoot->getChildByTag(UI_MODIFY_NICKNAME::CLOSE));
	pClose->addTouchEventListener(this,toucheventselector(ModifyNicknameLayer::touchButton));

	//提交按钮
	Button * pConfirm = dynamic_cast<Button*>(pRoot->getChildByTag(UI_MODIFY_NICKNAME::CONFIRM));
	CC_ASSERT(pConfirm);
	pConfirm->addTouchEventListener(this,toucheventselector(ModifyNicknameLayer::touchButton));
	
	TextField * pText = dynamic_cast<TextField*>(pRoot->getChildByTag(UI_MODIFY_NICKNAME::TEXT_NICK));
	CC_ASSERT(pText);

	nickBox = EditBox::create(pText->getContentSize(),Scale9Sprite::create());
	nickBox->setPosition(pText->getPosition());
	//nickBox->setFontSize(20);
	nickBox->setZOrder(pText->getZOrder());
	nickBox->setPlaceHolder(pText->getPlaceHolder().c_str());
	nickBox->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
	nickBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
	nickBox->setDelegate(this);
	nickBox->setTouchEnabled(true);
	nickBox->setVisible(true);
	nickBox->setMaxLength(15);
	this->addChild(nickBox);

	pText->removeFromParent();
	nickBox->setTag(UI_MODIFY_NICKNAME::TEXT_NICK);
	
	return true;
}


void ModifyNicknameLayer::onEnter(){

	//
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(ModifyNicknameLayer::notifySuccess),LOGON_LOGON_SUCCESS,nullptr);
	//失败通知
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(ModifyNicknameLayer::notifyFail),LOGON_LOGON_FAILURE,nullptr);

	LayerColor::onEnter();

}

void ModifyNicknameLayer::onExit(){

	LayerColor::onExit();

	NotificationCenter::getInstance()->removeObserver(this ,LOGON_LOGON_SUCCESS);

	NotificationCenter::getInstance()->removeObserver(this ,LOGON_LOGON_FAILURE);

}


//
void ModifyNicknameLayer::createObject(){

	if(!s_pInstance){
		s_pInstance = ModifyNicknameLayer::create();
	}

	CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
	pScene->addChild(s_pInstance);

}

//
void ModifyNicknameLayer::hideModal(){

	if(s_pInstance){
		s_pInstance->removeFromParent();
		s_pInstance = nullptr;
	}
}


//
void ModifyNicknameLayer::touchButton(Ref * obj ,TouchEventType eventType)
{
	Button * pButton = (Button *)obj;
	switch(eventType){
		case TouchEventType::TOUCH_EVENT_ENDED:
			{
				switch(pButton->getTag()){

				case UI_MODIFY_NICKNAME::CLOSE:
						{
							hideModal();
						}
						break;

				case UI_MODIFY_NICKNAME::CONFIRM:
						{

							string nickname = nickBox->getText();
							nickname = Util::trim(nickname);

							if(nickname.empty()){
								PromptLayer::createObject("请填写新的昵称");
								break;
							}

							string defaultNick = StringUtils::format("游客%d",UserInfo::instance()->getGameId());
							if(nickname == defaultNick)
							{
								PromptLayer::createObject("请填写新的昵称");
								break;
							}else{
								string reg = "(^[\u2E80-\u9FFF]+$)|(^[0-9a-zA-Z\\_]+$)";
								if(!Util::checkRegex(nickname , reg)){
									PromptLayer::createObject("昵称有非法字符");
									return;
								}
							}

							//SendMsg::sendUpdateNickname(nickname.c_str());
						}
						break;
				}
			}
			break;
	}
}

//通知接口
void ModifyNicknameLayer::notifySuccess(Ref * obj)
{
	hideModal();
	PromptLayer::createObject("修改完成");
}

//通知接口
void ModifyNicknameLayer::notifyFail(Ref * obj)
{

	hideModal();
	PromptLayer::createObject("修改失败");

}


void ModifyNicknameLayer::editBoxEditingDidBegin(EditBox * editBox){
}


void ModifyNicknameLayer::editBoxEditingDidEnd(EditBox * editBox){
	CCLog("   did end ");
}


void ModifyNicknameLayer::editBoxTextChanged(EditBox * editBox){
	CCLog("   did change ");
}


void ModifyNicknameLayer::editBoxReturn(EditBox * editBox){
	CCLog("   did return  ");
}

