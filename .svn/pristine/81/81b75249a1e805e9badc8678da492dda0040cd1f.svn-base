//
//  InputPassword.cpp
//  hello2dx
//
//  Created by on 15-03-11.
//
//

#include "InputPassword_fruit.h"
#include "sdk/ui/PromptLayer.h"

namespace fruit
{
	#define TAG "InputPassword"

	enum
	{
		TAG_BG,
		TAG_LABEL_ENTERROOM,
		TAG_LABEL_NEWROOM,
		TAG_PASSWORD_EDIT,
		TAG_CANCEL_BTN,
		TAG_NEWROOM_BTN,
		TAG_ENTERROOM_BTN,
	};

	InputPassword::InputPassword(InputPasswordListener* listener)
	{
		m_Listener = listener;
		
		auto visible = Director::getInstance()->getVisibleSize();

		this->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
		this->setContentSize(visible);
		this->setPosition(Vec2(0,0));
		this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		this->setBackGroundColor(Color3B::BLACK);
		this->setBackGroundColorOpacity(100);

		m_pBg = ImageView::create(SETPATH("fruit/table/inputpassword/bg.png"));
		m_pBg->setTag(TAG_BG);
		m_pBg->setPosition(Vec2(visible/2));
		addChild(m_pBg);

		auto newroomlabel = ImageView::create(SETPATH("fruit/table/inputpassword/label_newroom.png"));
		newroomlabel->setTag(TAG_LABEL_NEWROOM);
		newroomlabel->setPosition(Vec2(350,363));
		m_pBg->addChild(newroomlabel);

		auto enterroomlabel = ImageView::create(SETPATH("fruit/table/inputpassword/label_enterroom.png"));
		enterroomlabel->setTag(TAG_LABEL_ENTERROOM);
		enterroomlabel->setPosition(Vec2(280,363));
		m_pBg->addChild(enterroomlabel);

		auto passwordBox = EditBox::create(CCSize(504, 81), Scale9Sprite::create(SETPATH("fruit/table/inputpassword/edit_password.png")));
		passwordBox->setPosition(Vec2(160,220));
		passwordBox->setFont("",22);
		passwordBox->setFontColor(Color3B::BLACK);
		passwordBox->setZOrder(2);
		passwordBox->setAnchorPoint(CCPoint::ZERO);
		passwordBox->setPlaceHolder(U2F("请输入房间密码").c_str());
		passwordBox->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
		passwordBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
		passwordBox->setDelegate(this);
		passwordBox->setTouchEnabled(true);
		passwordBox->setVisible(true);
		passwordBox->setInputFlag(EditBox::InputFlag::PASSWORD);
		passwordBox->setMaxLength(6);
		m_pBg->addChild(passwordBox);
		passwordBox->setTag(TAG_PASSWORD_EDIT);

		// btn
		auto btn_cancel = Button::create(SETPATH("fruit/table/inputpassword/btn_cancel_n.png"), SETPATH("fruit/table/inputpassword/btn_cancel_d.png"), SETPATH("fruit/table/inputpassword/btn_cancel_d.png"));
		btn_cancel->setTag(TAG_CANCEL_BTN);
		btn_cancel->setTouchEnabled(true);
		btn_cancel->setEnabled(true);
		btn_cancel->addTouchEventListener(CC_CALLBACK_2(InputPassword::onTouch, this));
		btn_cancel->setPosition(Vec2(267, 106));
		m_pBg->addChild(btn_cancel);

		// enter btn
		auto btn_enter = Button::create(SETPATH("fruit/table/inputpassword/btn_enter_n.png"), SETPATH("fruit/table/inputpassword/btn_enter_d.png"), SETPATH("fruit/table/inputpassword/btn_enter_d.png"));
		btn_enter->setTag(TAG_ENTERROOM_BTN);
		btn_enter->setTouchEnabled(true);
		btn_enter->setEnabled(true);
		btn_enter->addTouchEventListener(CC_CALLBACK_2(InputPassword::onTouch, this));
		btn_enter->setPosition(Vec2(560, 106));
		m_pBg->addChild(btn_enter);

		//new btn
		auto btn_new = Button::create(SETPATH("fruit/table/inputpassword/btn_new_n.png"), SETPATH("fruit/table/inputpassword/btn_new_d.png"), SETPATH("fruit/table/inputpassword/btn_new_d.png"));
		btn_new->setTag(TAG_NEWROOM_BTN);
		btn_new->setTouchEnabled(true);
		btn_new->setEnabled(true);
		btn_new->addTouchEventListener(CC_CALLBACK_2(InputPassword::onTouch, this));
		btn_new->setPosition(Vec2(560, 106));
		m_pBg->addChild(btn_new);
	}

	void InputPassword::onTouch(Ref* target, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			int tag = static_cast<Node*>(target)->getTag();
			if (tag==TAG_ENTERROOM_BTN || tag==TAG_NEWROOM_BTN)
			{
				EditBox* pPassword = (EditBox*)(m_pBg->getChildByTag(TAG_PASSWORD_EDIT));
				if (pPassword&&m_Listener)
				{
					string strPassword = pPassword->getText();
					if(strPassword.empty())
					{
						PromptLayer::createObject("密码不能为空");
						return;
					}
					if (strPassword.length()>6)
					{
						PromptLayer::createObject("密码长度不能超过6位");
						return;
					}
					m_Listener->ExecSitDownOperate(strPassword);
				}
				this->setVisible(false);
			}
			if (tag==TAG_CANCEL_BTN)
			{
				this->setVisible(false);
			}
			this->getEventDispatcher()->removeEventListener(touchlist);
			touchlist = nullptr;
		}
	}

	void InputPassword::showInputPasswordWindow(int nSign)
	{

		touchlist = EventListenerTouchOneByOne::create();
		touchlist->setSwallowTouches(true);
		touchlist->onTouchBegan = [=](Touch *t, Event* e){
			return true;
		};
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchlist, this);

		Button* NewRoomBtn = (Button*)(m_pBg->getChildByTag(TAG_NEWROOM_BTN));
		Button* EnterRoomBtn = (Button*)(m_pBg->getChildByTag(TAG_ENTERROOM_BTN));
		ImageView* LabelEnterRoom = (ImageView*)(m_pBg->getChildByTag(TAG_LABEL_ENTERROOM));
		ImageView* LabelNewRoom = (ImageView*)(m_pBg->getChildByTag(TAG_LABEL_NEWROOM));

		if (nSign==1)
		{
			NewRoomBtn->setVisible(true);
			LabelNewRoom->setVisible(true);
			EnterRoomBtn->setVisible(false);
			LabelEnterRoom->setVisible(false);
		}
		if (nSign==2)
		{
			NewRoomBtn->setVisible(false);
			LabelNewRoom->setVisible(false);
			EnterRoomBtn->setVisible(true);
			LabelEnterRoom->setVisible(true);
		}
		this->setVisible(true);
	}

	//虚函数重载
	void InputPassword::editBoxEditingDidBegin(EditBox * editBox){

	}

	void InputPassword::editBoxEditingDidEnd(EditBox * editBox){

	}

	void InputPassword::editBoxTextChanged(EditBox * editBox){

	}

	void InputPassword::editBoxReturn(EditBox * editBox){

	}
}