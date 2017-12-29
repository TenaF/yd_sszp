//
//  LoginScene.h
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#ifndef __hello2dx__PromptLayer__
#define __hello2dx__PromptLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace ui;
typedef std::function<void()> BtnCallFunc;


class PromptLayer: public LayerColor {
	private:
		static PromptLayer * s_pInstance;
		Widget * pRoot;
		std::string message;
		static BtnCallFunc m_btnCallback;
	public:
		enum class PromptType{
			TYPE_PROMT_CONFIRM,				//只有确定按钮 确定按钮执行回调事件 点击空白处移除弹框不执行回调
			TYPE_PROMT_FORCE_CONFIRM,		//只有确定按钮 强制执行回调事件
			TYPE_PROMT_CONFIRM_WITH_CANCLE1,//确定和取消按钮 取消和点击空白移除弹框不执行回调  
			TYPE_PROMT_CONFIRM_WITH_CANCLE2,//放弃和继续按钮 取消和点击空白移除弹框不执行回调  
		};
		static void promptCommon(const char* text, float font_size = 34, const BtnCallFunc& btnCallback = nullptr, PromptType type = PromptType::TYPE_PROMT_CONFIRM);

		static void popRewardPanel(DWORD coin, DWORD medal);

		static void createObject(const char * pAlte);
    
        static void hideModalObject();
    
		PromptLayer();
		~PromptLayer();

		//void onSchedule(float tick);

		virtual bool init();
		virtual void onEnter();
		virtual void onExit();

		void onSchedule(float f);
		void hideModal();

		void touchButton(Ref* obj, TouchEventType eventType);
		void refresh(std::string szMessage);
		CREATE_FUNC(PromptLayer);
};

#endif /* defined(__hello2dx__PromptLayer__) */
