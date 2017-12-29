//
//  LogoScene.cpp
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#include "sdk/sdk.h"
#include "LoadingLayer.h"
#include "hall/data/DataManager.h"

#define TAG "LoadingLayer"

enum UI_LOADING
{
	CLOSE			 =	 98  ,			//关闭
	ALTER_TEXT       =   23 ,
	LOADING_ARMATURE_1 = 1212,
};

LoadingLayer * LoadingLayer::s_pInstance = nullptr;

LoadingLayer::LoadingLayer(){
	//message = nullptr;
}

LoadingLayer::~LoadingLayer(){
	this->removeAllChildrenWithCleanup(true);
}


bool LoadingLayer::init()
{
	NYInfo(TAG , "init()");
	if(!LayerColor::init())
	{
		return false;
	}

	//pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("alter/alter.json");
	//this->addChild(pRoot,4,100);

	return true;
}


void LoadingLayer::onEnter(){

	LayerColor::onEnter();

	CCSize visibleSize = Director::getInstance()->getVisibleSize();

	//遮盖层
	setTouchEnabled(true);
	this->setSwallowsTouches(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	setColor(ccc3(0,0,0));
	//setOpacity(150);
	setContentSize(CCSizeMake(visibleSize.width,visibleSize.height));

	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/loading/dh.ExportJson");
	cocostudio::Armature * armature = cocostudio::Armature::create("dh");
	armature->getAnimation()->play("Animation1");
	armature->setPosition(ccp(visibleSize.width/2,visibleSize.height/2));
	armature->setZOrder(2);
	armature->setScale(0.4f);
	armature->setTag(LOADING_ARMATURE_1);
	this->addChild(armature);

	//CCSequence * popupActions = CCSequence::create(CCScaleTo::create(0.0,0.0),CCScaleTo::create(0.06,1.05),CCScaleTo::create(0.08,0.95),CCScaleTo::create(0.08,1.0),NULL);
	//this->runAction(popupActions);

}

void LoadingLayer::onExit(){

	this->removeAllChildrenWithCleanup(true);
	NYInfo(TAG , "onExit()");
	LayerColor::onExit();

}

//
void LoadingLayer::createObject(){
	NYInfo(TAG , "createObject()");
	if(!s_pInstance){
		s_pInstance = LoadingLayer::create();
	}

	CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
	pScene->addChild(s_pInstance);

}

//
void LoadingLayer::hideModal(){
	NYInfo(TAG , "hideModal()");
	if(s_pInstance){
		s_pInstance->removeFromParent();
		s_pInstance = nullptr;
	}
}

