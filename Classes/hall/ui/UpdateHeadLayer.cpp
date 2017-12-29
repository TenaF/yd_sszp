//
//  LogoScene.cpp
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#include "sdk/sdk.h"
#include "hall/data/DataManager.h"
#include "sdk/util/Util.h"
#include "sdk/ui/PromptLayer.h"

#define TAG "PromptLayer"
#include "UpdateHeadLayer.h"

enum UI_REGISTER 
{

	CLOSE 					= 291, //
	CONFIRM 				= 292, //
	IMAGE_HEAD 				= 297,
	IMAGE_HEAD_BEGIN 		= 300,
	IMAGE_HEAD_LAST 		= 360,

	IMAGE_BACKGROUP_BEGIN 	= 400,
	IMAGE_BACKGROUP_END 	= 460,
	PANEL_HIDDEN 			= 500, //

};

UpdateHeadLayer * UpdateHeadLayer::s_pInstance = nullptr;

UpdateHeadLayer::UpdateHeadLayer()
{

	//
}

UpdateHeadLayer::~UpdateHeadLayer()
{
	s_pInstance->removeAllChildren();
}

bool UpdateHeadLayer::init()
{
	if (!LayerColor::init()) {
		return false;
	}

	CCSize visibleSize = Director::getInstance()->getVisibleSize();
	setTouchEnabled(true);
	this->setSwallowsTouches(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	setColor(ccc3(0,0,0));
	setOpacity(150);
	setContentSize(CCSizeMake(visibleSize.width,visibleSize.height));

	pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("modify_head.json");
	pRoot->setAnchorPoint(CCPoint(0.5f,0.5f));
	pRoot->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	this->addChild(pRoot);

	//
	Button * pConfirm = dynamic_cast<Button*>(pRoot->getChildByTag(CONFIRM));
	CC_ASSERT(pConfirm);
	pConfirm->setTouchEnabled(true);
	pConfirm->addTouchEventListener(this,toucheventselector(UpdateHeadLayer::touchButton));

	//
	Button * m_pClose = dynamic_cast<Button*>(pRoot->getChildByTag(CLOSE));
	CC_ASSERT(m_pClose);
	m_pClose->setTouchEnabled(true);
	m_pClose->addTouchEventListener(this,toucheventselector(UpdateHeadLayer::touchButton));

	//
	cocos2d::Vector<Node *> aChild = pRoot->getChildren();
	for (Node * pNode : aChild) {
		if (pNode->getTag() >= IMAGE_HEAD_BEGIN && pNode->getTag() <= IMAGE_HEAD_LAST) {
			((Button *) pNode)->addTouchEventListener(this,toucheventselector(UpdateHeadLayer::touchButton));
		}
	}

	return true;
}

void UpdateHeadLayer::onEnter() {

	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(UpdateHeadLayer::notifySuccess),LOGON_FACE_INFO, nullptr);
	//修改失败
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(UpdateHeadLayer::notifyFail),LOGON_OPERATE_FAILURE, nullptr);

	selectFace = UserInfo::instance()->getFaceId();
	//
	ImageView * pImageHead = static_cast<ImageView *>(pRoot->getChildByTag(IMAGE_HEAD));
	if (pImageHead) {
		string headUrl = StringUtils::format("head/head_%d.png",UserInfo::instance()->getFaceId());
		if(!FileUtils::getInstance()->isFileExist(headUrl)){
						if(UserInfo::instance()->getSex() == 1)
							headUrl = "head/head_0.png";		//默认图片
						else
							headUrl = "head/head_50.png";
		}
		pImageHead->loadTexture(headUrl);
	}

	LayerColor::onEnter();

}

void UpdateHeadLayer::onExit() {
	LayerColor::onExit();

	NotificationCenter::getInstance()->removeObserver(this,LOGON_FACE_INFO);
	NotificationCenter::getInstance()->removeObserver(this,LOGON_OPERATE_FAILURE);
}

//
void UpdateHeadLayer::createObject() {

	if (!s_pInstance) {
		s_pInstance = UpdateHeadLayer::create();
	}

	CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
	pScene->addChild(s_pInstance);

}

//
void UpdateHeadLayer::hideModal() {

	if (s_pInstance) {

		s_pInstance->removeFromParent();
		s_pInstance = nullptr;
	}

}

//
void UpdateHeadLayer::touchButton(Ref * obj, TouchEventType eventType) {
	Button * pButton = (Button *) obj;

	switch (eventType) {
		case TouchEventType::TOUCH_EVENT_ENDED: {
			if (pButton->getTag() >= IMAGE_HEAD_BEGIN && pButton->getTag() <= IMAGE_HEAD_LAST) {

				//
				WORD wFaceID = 0;
				/*Button * lastImage = static_cast<Button*>(pRoot->getChildByTag(selectFace + 400));
				if (lastImage){
					lastImage->loadTexture("genggaitouxiang/head/yyy1.png");
				}else{

				}*/

				wFaceID = pButton->getTag() - 300;
				selectFace = wFaceID;

				ImageView * pDis = static_cast<ImageView *>(pRoot->getChildByTag(IMAGE_HEAD));
				pDis->loadTexture(StringUtils::format("head/head_%d.png",wFaceID));

				break;
			}

			switch (pButton->getTag()) {

				case CONFIRM: //
				{
					SendMsg::sendUpdateHeadInfo(selectFace);
				}
				break;

				case CLOSE: {
					hideModal();
				}
				break;
			}
		}
		break;
	}
}

//修改失败
void UpdateHeadLayer::notifyFail(Ref * obj) {

//	NYInfo(TAG , " 接收到修改头像失败信息 ");
//	CMD_GP_OperateFailure * pFail = (CMD_GP_OperateFailure *) obj;
//
//	//std::basic_string<unsigned short> strDes = pFail->szDescribeString;
//	string error = Util::getStringWithTChar(pFail->szDescribeString);
//	PromptLayer::createObject((char *) error.c_str());

}

//修改成功
void UpdateHeadLayer::notifySuccess(Ref * obj) {
	//
	NYInfo(TAG , " 接收到修改头像成功信息 ");
	CMD_GP_UserFaceInfo * pFace = (CMD_GP_UserFaceInfo*) obj;
	UserInfo::instance()->setFaceId(pFace->wFaceID);

	ImageView * pImageHead = static_cast<ImageView *>(pRoot->getChildByTag(IMAGE_HEAD));
	if (pImageHead) 
	{
		string headUrl = StringUtils::format("head/head_%d.png",UserInfo::instance()->getFaceId());
		if(!FileUtils::getInstance()->isFileExist(headUrl))
		{
			if(UserInfo::instance()->getSex() == 1)
				headUrl = "head/head_0.png";		//默认图片
			else
				headUrl = "head/head_50.png";
		}
		pImageHead->loadTexture(headUrl);
	}

	hideModal();

}

