//
//  LoadingFishScene.cpp
//  hello2dx
//
//  Created by on 15-11-7.
//
//

#include "sdk/sdk.h"
#include "LoadingFishScene.h"
#include "hall/data/DataManager.h"
#include "sdk/ui/PromptLayer.h"

#define TAG "LoadingFishScene"

enum UI_LOADING
{
	TAG_TABLE_BACKGROUND,
};

LoadingFishScene * LoadingFishScene::s_pInstance = nullptr;

LoadingFishScene::LoadingFishScene(){
	//message = nullptr;
}

LoadingFishScene::~LoadingFishScene(){
	if (s_pInstance)
	{
		s_pInstance->removeAllChildren();
		s_pInstance = nullptr;
	}
}


bool LoadingFishScene::init()
{
	NYInfo(TAG , "init()");
	if(!LayerColor::init())
	{
		return false;
	}

	CCSize visibleSize = Director::getInstance()->getVisibleSize();
    
    m_Layout = Layout::create();
    this->addChild(m_Layout);

	auto loadingbg = ImageView::create("loadinggame/bg.png");
	loadingbg->setPosition(Vec2(VisibleRect::center().x, VisibleRect::center().y/* + 10*/));
	m_Layout->addChild(loadingbg);
	loadingbg->setTag(TAG_TABLE_BACKGROUND);

	progessbj = ImageView::create("loadinggame/blue.png");
	progessbj->setPosition(Vec2((VisibleRect::getWidth())/2, (VisibleRect::getHeight())/2));
	progessbj->setZOrder(1);
	m_Layout->addChild(progessbj);

	m_pProgress = LoadingBar::create("loadinggame/green.png");
	m_pProgress->setPercent(50.0f);
	m_pProgress->setPosition(Vec2((VisibleRect::getWidth())/2, (VisibleRect::getHeight())/2));
	m_Layout->addChild(m_pProgress);
	m_pProgress->setZOrder(2);

	m_spBlock = ImageView::create("loadinggame/block.png");
//	m_spBlock->setPosition(Vec2((VisibleRect::getWidth()-progessbj->getSize().width)/2, (VisibleRect::getHeight())/2+45));
	m_spBlock->setPosition(Vec2((VisibleRect::getWidth())/2, (VisibleRect::getHeight())/2+45));
	m_spBlock->setZOrder(2);
	m_Layout->addChild(m_spBlock);
//	m_spBlock->runAction(Sequence::create(CCMoveTo::create(10,Vec2((VisibleRect::getWidth()+progessbj->getSize().width)/2, (VisibleRect::getHeight())/2+45)),NULL));

	m_LoadState = ImageView::create("loadinggame/loading0.png");
	m_LoadState->setZOrder(2);
	m_LoadState->setPosition(Vec2((VisibleRect::getWidth())/2, (VisibleRect::getHeight()-m_LoadState->getSize().height)/2-70));
	m_Layout->addChild(m_LoadState);

//	schedule(schedule_selector(LoadingFishScene::updateProgess), 0.2f);
	schedule(schedule_selector(LoadingFishScene::logintip), 30.0f);
	m_LoadCount = 0;

	return true;
}


void LoadingFishScene::onEnter(){
//	NotificationCenter::getInstance()->addObserver(this , callfuncO_selector(LoadingFishScene::normalclose) , GAME_LOGIN_FINISH , nullptr);
	LayerColor::onEnter();
}

void LoadingFishScene::onExit(){
//	NotificationCenter::getInstance()->removeObserver(this , GAME_LOGIN_FINISH);
	this->removeAllChildrenWithCleanup(true);
	NYInfo(TAG , "onExit()");
	LayerColor::onExit();

}

//
void LoadingFishScene::createObject(){
	NYInfo(TAG , "createObject()");
	if(!s_pInstance){
		s_pInstance = LoadingFishScene::create();
	}

	//s_pInstance->message = pStr;
	CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
	pScene->addChild(s_pInstance, 10);

}

//
void LoadingFishScene::hideModal(){
	NYInfo(TAG , "hideModal()");
	if(s_pInstance){
		s_pInstance->removeFromParent();
		s_pInstance = nullptr;
	}
}

void LoadingFishScene::updateProgess(float dt)
{
	m_LoadCount++;
	
	if (m_LoadCount<=100)
	{
		m_pProgress->setPercent(m_LoadCount);
	}

	switch ((m_LoadCount/2)%4)
	{
	case 0:
		m_LoadState->loadTexture("loadinggame/loading0.png");
		break;
	case 1:
		m_LoadState->loadTexture("loadinggame/loading1.png");
		break;
	case 2:
		m_LoadState->loadTexture("loadinggame/loading2.png");
		break;
	case 3:
		m_LoadState->loadTexture("loadinggame/loading3.png");
		break;
	default:
		m_LoadState->loadTexture("loadinggame/loading0.png");
		break;
	}
}

void LoadingFishScene::close(float dt)
{
	unschedule(schedule_selector(LoadingFishScene::close));
	hideModal();
}

void LoadingFishScene::logintip(float dt)
{
	unschedule(schedule_selector(LoadingFishScene::logintip));
	PromptLayer::createObject("服务器长时间未响应，请退出游戏重新登录");
	schedule(schedule_selector(LoadingFishScene::close), 4.0f);
}

void LoadingFishScene::normalclose(Ref * obj)
{
	m_pProgress->setPercent(100);
	m_spBlock->setPosition(Vec2((VisibleRect::getWidth()+progessbj->getSize().width)/2, (VisibleRect::getHeight())/2+45));
	schedule(schedule_selector(LoadingFishScene::close), 1.0f);
}

LoadingFishScene* LoadingFishScene::instance() {
	if (!s_pInstance) {
		createObject();
	}
	return s_pInstance;
}

void LoadingFishScene::setProgress(int nPer)
{
	if (nPer<=100)
	{
		m_pProgress->setPercent(nPer);
		m_pProgress->draw();
		m_spBlock->setPosition(Vec2((VisibleRect::getWidth()-progessbj->getSize().width)/2+progessbj->getSize().width*nPer/100, (VisibleRect::getHeight())/2+45));
		m_spBlock->draw();
	}

	m_LoadCount++;

	switch ((m_LoadCount)%4)
	{
	case 0:
		m_LoadState->loadTexture("loadinggame/loading0.png");
		break;
	case 1:
		m_LoadState->loadTexture("loadinggame/loading1.png");
		break;
	case 2:
		m_LoadState->loadTexture("loadinggame/loading2.png");
		break;
	case 3:
		m_LoadState->loadTexture("loadinggame/loading3.png");
		break;
	default:
		m_LoadState->loadTexture("loadinggame/loading0.png");
		break;
	}
}