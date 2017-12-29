//
//  LogoScene.cpp
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#include "sdk/sdk.h"
#include "LoadingScene.h"
#include "hall/data/DataManager.h"

#define TAG "LoadingScene"
 
enum UI_LOADING
{
	CLOSE			 =	 98  ,			//关闭
	ALTER_TEXT       =   23 ,
	LOADING_ARMATURE_1 = 1212,
};

LoadingScene * LoadingScene::s_pInstance = nullptr;

LoadingScene::LoadingScene(){
}

LoadingScene::~LoadingScene(){
// 	if (s_pInstance)
// 	{
// 		s_pInstance->removeAllChildren();
// 		s_pInstance = nullptr;
// 	}
}


bool LoadingScene::init()
{
	if(!LayerColor::init())
	{
		return false;
	}


	CCSize visibleSize = Director::getInstance()->getVisibleSize();

	
	setColor(ccc3(0, 0, 0));
	setOpacity(150);
	setContentSize(CCSizeMake(visibleSize.width, visibleSize.height));

	log("!!!!!!!!!!!Init Create loading ANI  !!!!!!!!!!!!");
	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/CattleAnimation/CattleAnimation0.png", "animation/CattleAnimation/CattleAnimation0.plist", "animation/CattleAnimation/CattleAnimation.ExportJson");
	cocostudio::Armature * armature = cocostudio::Armature::create("CattleAnimation");
	armature->getAnimation()->play("CattleAndGold");
	armature->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
	armature->setZOrder(2);
	armature->setTag(UI_LOADING::LOADING_ARMATURE_1);
	this->addChild(armature);


	//模态触摸处理
	auto callback = [](Touch *, Event *)
	{
		return true;
	};
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = callback;
	listener->setSwallowTouches(true);//吞噬触摸
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);//用这种 同步NODE，移除NODE ，监听器自动移除



	return true;
}


void LoadingScene::onEnter(){

	LayerColor::onEnter();
	//log("!!!!!!!!!!!onEnter Create loading ANI  !!!!!!!!!!!!");
// 	std::string fullPath = FileUtils::getInstance()->fullPathForFilename("animation/CattleAnimation/CattleAnimation0.plist");
// 	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(fullPath);
	//cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/CattleAnimation/CattleAnimation0.png", "animation/CattleAnimation/CattleAnimation0.plist", "animation/CattleAnimation/CattleAnimation.ExportJson");
}

void LoadingScene::onExit(){

	LayerColor::onExit();
	cocostudio::ArmatureDataManager::getInstance()->removeArmatureFileInfo("animation/CattleAnimation/CattleAnimation.ExportJson");
// 	std::string fullPath = FileUtils::getInstance()->fullPathForFilename("animation/CattleAnimation/CattleAnimation0.plist");
// 	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(fullPath);
}


void LoadingScene::createObject(const std::string& strMsg/* = ""*/){
	if(!s_pInstance){
		s_pInstance = LoadingScene::create();
	}

	CCSize visibleSize = Director::getInstance()->getVisibleSize();
	CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
	if (pScene)
	{
        if(0 == s_pInstance->getParent())
            pScene->addChild(s_pInstance, 1000);
        
		if (0 != s_pInstance->getParent() && pScene != (CCScene *)s_pInstance->getParent())
        {
            s_pInstance->removeFromParentAndCleanup(true);
			s_pInstance = nullptr;
			s_pInstance = LoadingScene::create();
			s_pInstance->setName(LOADING_SCENE);
            pScene->addChild(s_pInstance, 1000);
        }
		
        if (strMsg.length() > 0)
		{
			auto labelMsg = Label::create();
			labelMsg->setString(strMsg.c_str());
			labelMsg->setSystemFontName("微软雅黑");
			labelMsg->setSystemFontSize(32);
            labelMsg->enableOutline(ccColor4B::BLUE,0.5f);
			labelMsg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + Vec2(-50, 0));
			labelMsg->setTag(UI_LOADING::ALTER_TEXT);
			s_pInstance->addChild(labelMsg);

			auto arm = s_pInstance->getChildByTag(UI_LOADING::LOADING_ARMATURE_1);
			if (arm)
			{
				int offx = labelMsg->getContentSize().width / 2 + 20 +110;
				arm->setScale(0.75f);
				arm->setPosition(labelMsg->getPosition() + Vec2(offx, 0));
			}
				
		}
	
	}
	else{
		if (s_pInstance) {
			s_pInstance->removeFromParent();
			s_pInstance = nullptr;
		}
	}


}


void LoadingScene::hideModal(){
	if(s_pInstance){
		s_pInstance->removeFromParent();
		s_pInstance = nullptr;
	}
}

