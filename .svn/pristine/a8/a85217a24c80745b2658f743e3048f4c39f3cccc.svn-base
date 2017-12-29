//
//  LogoScene.cpp
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#include "sdk/sdk.h"
#include "PromptLayer.h"

enum UI_PROMPT {
	CLOSE = 98,			//关闭
	ALTER_TEXT = 23,
	PROMPT_PANEL = 364,
	PROMPT_IMG = 649,

	PROMPT_LAYER = 77777,
};

#define TAG "PromptLayer"

PromptLayer * PromptLayer::s_pInstance = nullptr;

BtnCallFunc PromptLayer::m_btnCallback = nullptr;

PromptLayer::PromptLayer() {
	message = "";
}

PromptLayer::~PromptLayer() {
	
// 	if (this == s_pInstance)
// 	{
// 		s_pInstance = nullptr;
// 	}
	
}

bool PromptLayer::init() {
	if (!LayerColor::init()) {
		return false;
	}

	pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("prompt/prompt.json");
	CCSize visibleSize = Director::getInstance()->getVisibleSize();
	pRoot->setAnchorPoint(CCPoint(0.5f,0.5f));
	pRoot->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	this->addChild(pRoot, 100, 100);
	
// 	Widget * pPanel = dynamic_cast<Widget*>(pRoot->getChildByTag(PROMPT_PANEL));
// 	ImageView * pImg = dynamic_cast<ImageView*>(pPanel->getChildByTag(PROMPT_IMG));
// 	Text * pText = dynamic_cast<Text*>(pPanel->getChildByTag(ALTER_TEXT));
// 	pText->setFontSize(30);
// 	pText->setText(message.c_str());
// 
// 	CCSize textSize = pText->getContentSize();
// 	CCSize imgSize = pImg->getContentSize();
// 	float scaleX = (textSize.width + 80) / imgSize.width;
// 	pImg->setScaleX(scaleX);

	scheduleOnce(schedule_selector(PromptLayer::onSchedule), 1.5f);//3
	return true;
}

void PromptLayer::onEnter() {
	LayerColor::onEnter();
}

void PromptLayer::onExit() {
	LayerColor::onExit();
}

void PromptLayer::onSchedule(float f) {
	hideModal();
}

void PromptLayer::promptCommon(const char* text, float font_size/* = 34*/, const BtnCallFunc& btnCallback/* = nullptr*/, PromptType type/* = TYPE_PROMT_CONFIRM*/)
{
	std::string strText = text;
	if (strText.empty())return;

	CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
	if (pScene->getChildByName(TAG))return;
	auto winSize = Director::getInstance()->getWinSize();
	auto layout = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("prompt_layer.json");
	layout->setSize(CCSize(VisibleRect::getWidth(), VisibleRect::getHeight()));
	layout->setPosition(Vec2::ZERO);
	layout->setTouchEnabled(true);
	layout->setName(TAG);
	pScene->addChild(layout, 1000);
	auto itemImg = (Widget*)layout->getChildByName("tips_bg");
	itemImg->setScale(0);
	itemImg->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.15f, 1.25f), ScaleTo::create(0.1f, 1.0f)));
	itemImg->setTouchEnabled(true);

	//确定按钮事件
	auto callback = [=](Ref* target, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			//auto parent = (Node*)target;
// 			itemImg->runAction(ScaleTo::create(0.1f, 0));
// 			layout->runAction(CCSequence::createWithTwoActions(DelayTime::create(0.1f), RemoveSelf::create(true)));
			if (btnCallback)
				btnCallback();
			//layout->removeFromParentAndCleanup(true);
			layout->runAction(CCSequence::createWithTwoActions(DelayTime::create((float)1/60.f), RemoveSelf::create(true)));
		}
	};

	//取消按钮事件
	auto Cancle_callback = [=](Ref* target, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
// 			itemImg->runAction(ScaleTo::create(0.1f, 0));
// 			layout->runAction(CCSequence::createWithTwoActions(DelayTime::create(0.1f), RemoveSelf::create(true)));
			//layout->removeFromParentAndCleanup(true);
			layout->runAction(CCSequence::createWithTwoActions(DelayTime::create((float)1 / 60.f), RemoveSelf::create(true)));
		}
	};


	
	auto tipsText = dynamic_cast<Text*>(itemImg->getChildByName("label_tips"));
	tipsText->setString(text);
	tipsText->setFontSize(font_size);
	auto label_len = Label::create(text, tipsText->getFontName(), font_size)->getContentSize().width;
	if (label_len > tipsText->getContentSize().width)
		tipsText->setTextHorizontalAlignment(TextHAlignment::LEFT);
	else
		tipsText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	tipsText->setTextVerticalAlignment(TextVAlignment::CENTER);
	tipsText->enableOutline(Color4B(Color3B(153, 51, 0)));

	auto pConfirm_Btn = dynamic_cast<Button*>(itemImg->getChildByName("confirm_Btn"));
	auto pCancle_Btn = dynamic_cast<Button*>(itemImg->getChildByName("cancle_Btn"));
	if (pConfirm_Btn && pCancle_Btn)
	{
		switch (type)
		{
			
		case PromptType::TYPE_PROMT_CONFIRM:
		case PromptType::TYPE_PROMT_FORCE_CONFIRM:
		{
			//如果只有一个确定按钮，则点面板和确定执行一样的回调取消面板及执行回调事件
			pCancle_Btn->setVisible(false);
			pCancle_Btn->setEnabled(false);
			pConfirm_Btn->setPositionX(itemImg->getContentSize().width / 2);
			pConfirm_Btn->addTouchEventListener(callback);
			if (type == PromptType::TYPE_PROMT_FORCE_CONFIRM)
				layout->addTouchEventListener(callback);
			else
				layout->addTouchEventListener(Cancle_callback);
		}
			break;
		case PromptType::TYPE_PROMT_CONFIRM_WITH_CANCLE1:
		case PromptType::TYPE_PROMT_CONFIRM_WITH_CANCLE2:
		{
			//如果有一个确定和取消按钮，则点面板和取消执行移除面板，确定则执行移除面板及回调事件
			pCancle_Btn->addTouchEventListener(Cancle_callback);
			layout->addTouchEventListener(Cancle_callback);
			pConfirm_Btn->addTouchEventListener(callback);
			if (type == PromptType::TYPE_PROMT_CONFIRM_WITH_CANCLE2)
			{
				pCancle_Btn->loadTextures("commonresource/give_up_tbn.png", "commonresource/give_up_tbn2.png");
				pConfirm_Btn->loadTextures("commonresource/go_on_btn.png","commonresource/go_on_btn2.png");
			}
		}
			break;
		default:
			break;
		}
	}
	

}
//弹出信息
void PromptLayer::createObject(const char * pStr) {
	
	CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
	if (pScene->getChildByTag(PROMPT_LAYER) != nullptr)
	{
        PromptLayer::hideModalObject();
	}

	s_pInstance = nullptr;

	if (!s_pInstance){
		s_pInstance = PromptLayer::create();
		s_pInstance->setTag(PROMPT_LAYER);
		pScene->addChild(s_pInstance,1000);//1000
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	s_pInstance->message = U2F(pStr).c_str();
#else
	s_pInstance->message = pStr;
#endif
	s_pInstance->refresh(s_pInstance->message);

}
void PromptLayer::hideModalObject()
{
	CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
	if (nullptr != pScene->getChildByTag(PROMPT_LAYER))
	{
		if (s_pInstance && s_pInstance->getParent())
			s_pInstance->hideModal();
	}
}

void PromptLayer::hideModal() {

	if (s_pInstance && s_pInstance->getParent()) {
		s_pInstance->removeFromParent();
		s_pInstance = nullptr;
		m_btnCallback = nullptr;
	}
}

void PromptLayer::touchButton(Ref * obj ,TouchEventType eventType) {
	Button * pButton = (Button *) obj;
	switch(eventType) {
		case TouchEventType::TOUCH_EVENT_BEGAN : {
			switch(pButton->getTag()) {

				case CLOSE : {
					hideModal();
				}
					break;
			}
		}
			break;
	}
}

void PromptLayer::refresh(string szMessage)
{
	Widget * pPanel = dynamic_cast<Widget*>(pRoot->getChildByTag(PROMPT_PANEL));
	ImageView * pImg = dynamic_cast<ImageView*>(pPanel->getChildByTag(PROMPT_IMG));
	Text * pText = dynamic_cast<Text*>(pPanel->getChildByTag(ALTER_TEXT));
	pText->setFontSize(30);
	pText->setText(szMessage.c_str());

	CCSize textSize = pText->getContentSize();
	CCSize imgSize = pImg->getContentSize();
	float scaleX = (textSize.width + 80) / imgSize.width;
	pImg->setScaleX(scaleX);
}

