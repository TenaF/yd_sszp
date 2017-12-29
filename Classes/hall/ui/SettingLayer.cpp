//
//  LogoScene.cpp
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#include "sdk/sdk.h"
#include "SettingLayer.h"
#include "hall/data/DataManager.h"
#include "UpdatePasswordLayer.h"
#include "LoginLayer.h"


enum UI_SETTING
{
	SETTING_CLOSE				=	7457192  ,			//
	SETTING_MODIFY_PASS			=	7457183 ,
	SETTING_SWITCH				=	7457182 ,
	
};

SettingLayer * SettingLayer::s_pInstance = nullptr;

SettingLayer::SettingLayer(){

}

SettingLayer::~SettingLayer(){
	this->removeAllChildrenWithCleanup(true);
}


bool SettingLayer::init()
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

	pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("setting.json");
	pRoot->setAnchorPoint(CCPoint(0.5f,0.5f));
	pRoot->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	this->addChild(pRoot);

	//�ر�
	Button * pBack = static_cast<Button *>(pRoot->getChildByTag(SETTING_CLOSE));
	pBack->addTouchEventListener(this,toucheventselector(SettingLayer::touchButton));

	//ת��
	Button * pModifyPass = static_cast<Button *>(pRoot->getChildByTag(SETTING_MODIFY_PASS));
	pModifyPass->addTouchEventListener(this,toucheventselector(SettingLayer::touchButton));

	//�л�
	Button * pSwitch = static_cast<Button *>(pRoot->getChildByTag(SETTING_SWITCH));
	pSwitch->addTouchEventListener(this,toucheventselector(SettingLayer::touchButton));

	return true;
}


void SettingLayer::onEnter(){

	LayerColor::onEnter();

}

void SettingLayer::onExit(){
	LayerColor::onExit();
}

//
void SettingLayer::createObject(){

	if(!s_pInstance){
		s_pInstance = SettingLayer::create();
	}

	CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
	pScene->addChild(s_pInstance);

}

//
void SettingLayer::hideModal(){

	if(s_pInstance){
		s_pInstance->removeFromParent();
		s_pInstance = nullptr;
	}

}


//
void SettingLayer::touchButton(Ref * obj ,TouchEventType eventType)
{
	Button * pButton = (Button *)obj;
	if(eventType==TouchEventType::TOUCH_EVENT_ENDED)
	{
				switch(pButton->getTag()){
				case SETTING_MODIFY_PASS:
					{
						UpdatePasswordLayer::createObject();
						hideModal();
					}
					break;

				case SETTING_SWITCH:
					{
						LoginLayer::createObject(nullptr);
						hideModal();
					}
					break;

				case SETTING_CLOSE:
					{
						hideModal();
					}
					break;
				}
	}
}


//
void SettingLayer::sliderChangeEvent(Ref * pSender , SliderEventType type){
	if(type == SLIDER_PERCENTCHANGED){
		Slider * pSlider = (Slider *)pSender;
		int percent = pSlider->getPercent();
		CCLog("  valude = %d",percent);
	}
}

