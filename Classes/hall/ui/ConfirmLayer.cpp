/*
 * ConfirmLayer.cpp
 *
 *  Created on: 2014-11-5
 *      Author: win7
 */

#include "ConfirmLayer.h"
#include "MainLayer.h"

ConfirmLayer * ConfirmLayer::instance = nullptr;

ConfirmLayer::ConfirmLayer() {
	parent = nullptr;
}

ConfirmLayer::~ConfirmLayer() {
	//removeAllChildrenWithCleanup(true);
	parent = nullptr;
}

bool ConfirmLayer::init() {
	if (!LayerColor::init())
		return false;

	setContentSize(CCSize(544, 292));

	return true;
}

void ConfirmLayer::onEnter() {
	LayerColor::onEnter();

	CCSize contentSize = CCSize(544, 292);

	ImageView * pImage = ImageView::create("exit/bj.png");
	pImage->setAnchorPoint(Vec2::ZERO);
	pImage->setPosition(Vec2::ZERO);
	pImage->setScale(0.8f);
	this->addChild(pImage);

//	Label * pLabel = Label::create(U2F("确定要退出大厅吗？").c_str(), "", 30);
	auto pLabel = CCLabelTTF::create(U2F("游戏有版本更新需要升级吗？").c_str(), "微软雅黑", 30);
	pLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	pLabel->setPosition(Vec2(contentSize.width / 2, 200));
	this->addChild(pLabel);

	Button * pCanel = Button::create("exit/gfgfgf.png", "exit/gfgfgf1.png");
	pCanel->setTouchEnabled(true);
	pCanel->setScale(1.2f);
	pCanel->setTag(EXIT_CANEL_BTN);
	pCanel->addTouchEventListener(this, toucheventselector(ConfirmLayer::touchButton));
	pCanel->setPosition(Vec2(contentSize.width / 2 - 50, 50));
	pCanel->setAnchorPoint(Vec2(1, 0));

	Button * pConfirm = Button::create("exit/gfggf.png", "exit/gfggf1.png");
	pConfirm->setTouchEnabled(true);
	pConfirm->setScale(1.2f);
	pConfirm->setTag(EXIT_CONFIRM_BTN);
	pConfirm->addTouchEventListener(this, toucheventselector(ConfirmLayer::touchButton));
	pConfirm->setPosition(Vec2(contentSize.width / 2 + 50, 50));
	pConfirm->setAnchorPoint(Vec2(0, 0));

	Button * pClose = Button::create("exit/close1.png", "exit/close2.png");
	pClose->setTouchEnabled(true);
	pClose->setScale(1.2f);
	pClose->setTag(EXIT_CLOSE);
	pClose->addTouchEventListener(this, toucheventselector(ConfirmLayer::touchButton));
	pClose->setPosition(Vec2(541, 288));
	pClose->setAnchorPoint(Vec2(1, 1));

	this->addChild(pCanel);
	this->addChild(pConfirm);
	this->addChild(pClose);

	CCSize visibleSize = Director::getInstance()->getWinSize();
	this->setPosition(Vec2(visibleSize.width / 2 - contentSize.width / 2, visibleSize.height / 2 - contentSize.height / 2));
	//CCActionInterval * moveBy = CCMoveBy::create(0.1f, Vec2(visibleSize.width / 2 - contentSize.width / 2, visibleSize.height / 2 - contentSize.height / 2));
	//CCActionInterval * scaleTo = CCScaleTo::create(0.1f, 1);
	//CCFiniteTimeAction * seq = CCSequence::create(moveBy, scaleTo, NULL);

	//runAction(seq);

}

void ConfirmLayer::createObject(Object* obj , int tag) {

	if (!instance) {
		instance = ConfirmLayer::create();
		instance->parent = obj;
		instance->buttonTag = tag;
		CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
		pScene->addChild(instance, 10);
	}

}

void ConfirmLayer::hideModal() {
	if (instance) {
		instance->removeFromParent();
		instance = nullptr;
	}
}

void ConfirmLayer::onExit() {
	removeAllChildrenWithCleanup(true);
	LayerColor::onExit();
}

void ConfirmLayer::touchButton(Ref * obj ,TouchEventType type) {
	if (type == TouchEventType::TOUCH_EVENT_ENDED) {
		Button * pButton = (Button*) obj;
		switch(pButton->getTag()) {
			case EXIT_CLOSE : {
				hideModal();
			}
				break;

			case EXIT_CANEL_BTN : {
				hideModal();
			}
				break;

			case EXIT_CONFIRM_BTN : {
				((MainLayer*)parent)->onUpgrade(buttonTag);
				hideModal();
			}
				break;
		}

	}
}

