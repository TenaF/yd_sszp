#include "MainScene_sszp.h"
#include "game/sszp/data/PokerData_sszp.h"
#include "game/sszp/message/PokerMessageController_sszp.h"
#include "game/sszp/message/SendGameMsg_sszp.h"
#include "game/sszp/ui/widget/helpLayer_sszp.h"
#include "game/sszp/ui/widget/OutGameLayer_sszp.h"
#include "game/sszp/ui/widget/SettingLayer_sszp.h"
#include "game/sszp/data/SoundManager_sszp.h"
#include "hall/manager/SoundManager_hall.h"

namespace sszp
{
	MainScene_sszp::MainScene_sszp()
	{
		isSound = false;
		isSound_music = false;
		BankerCount = 0;
		isAllowBank = true;
		isContinueJetton = false;
		isUpJetton = true;
		wCurrentJettonScore = 0;
		my_AllJettonScore = 0;
		my_AllJettonResultScore = 0;
		lAllLimitScore = 0;
		lUserSingleLimitScore = 0;
		lAreaLimitScore = 0;
		initNumber = 0;
		continue_number = 0;
		usableL = nullptr;
		scoreL = nullptr;
		isBanerMove = false;
		isMenu = false;
		isPlayerList = true;
		timeLabelTop = nullptr;
		nameL = nullptr;
		beanL = nullptr;
		over_layout = nullptr;
		wCurrentBankerState = CurrentBanker_sszp_None;
		memset(&wCurrentGameEnd, 0, sizeof(CMD_S_GameEnd));
		memset(&itemp_myJettonLabel, 0, sizeof(itemp_myJettonLabel));
		memset(&itemp_totalTtf, 0, sizeof(itemp_totalTtf));
	}


	MainScene_sszp::~MainScene_sszp()
	{
	}
	void MainScene_sszp::onEnter()
	{
		SceneBase::onEnter();
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainScene_sszp::closeSocket_noifyFial), GAME_DISCONNECT, nullptr);
		SoundManager::instance()->playBgMusic(SETPATH("sszp/sound/BetBk.mp3").c_str());
	}
	void MainScene_sszp::onExit()
	{
		this->unscheduleUpdate();
		NotificationCenter::getInstance()->removeObserver(this, GAME_DISCONNECT);
		SceneBase::onExit();
		SoundManager::instance()->stopBgMusic();
	}
	void MainScene_sszp::closeSocket_noifyFial(Object *obj)
	{
		PromptLayer::createObject(U2F("与服务器断开连接，3秒后自动退出游戏").c_str());
		NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_GAME);

		this->runAction(Sequence::create(DelayTime::create(3), CallFuncN::create([=](Node *node){
			UserInfo::instance()->setCurrentServerID(0);
			MainController::getInstance()->setGameState(false);
			PokerMessageController::instance()->onQuitGame();
		}), NULL));
	}
	bool MainScene_sszp::init()
	{
		if (!Layer::init())
		{
			return false;
		}
		pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(SETPATH("sszp/sszp_1.json").c_str());
		pRoot->addTouchEventListener(this, toucheventselector(MainScene_sszp::onTouch));
		pRoot->setAnchorPoint(CCPoint(0.0f, 0.0f));
		pRoot->setPosition(Vec2(0, 0));
		this->addChild(pRoot);

		over_layout = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(SETPATH("sszp/sszp_over.json").c_str());
		over_layout->addTouchEventListener(this, toucheventselector(MainScene_sszp::onTouch));
		over_layout->setVisible(false);
		pRoot->addChild(over_layout, 100);

		//加载所有高亮图片为精灵，并且至灰
		BgPanel = pRoot->getChildByName<ImageView *>("bg");

		//圆心
		center_point_null_picture = BgPanel->getChildByName<Widget*>("center_point_null_picture");
		center_point_null_picture->setAnchorPoint(ccp(0.5f, 0.5f));

		//转盘上面一圈16个动物
		for (int i = 0; i < 16; i++)
		{
			block[i] = center_point_null_picture->getChildByName<ImageView *>(__String::createWithFormat("Sprite%d", i)->getCString());
		}

		//设置金币以及下注按钮
		B_CurMoney[0] = BgPanel->getChildByName<Button *>("cur1");
		B_CurMoney[0]->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onJessonTouch, this));
		B_CurMoney[1] = BgPanel->getChildByName<Button *>("cur10");
		B_CurMoney[1]->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onJessonTouch, this));
		B_CurMoney[2] = BgPanel->getChildByName<Button *>("cur50");
		B_CurMoney[2]->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onJessonTouch, this));
		B_CurMoney[3] = BgPanel->getChildByName<Button *>("cur100");
		B_CurMoney[3]->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onJessonTouch, this));
		B_CurMoney[4] = BgPanel->getChildByName<Button *>("cur500");
		B_CurMoney[4]->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onJessonTouch, this));
		B_CurMoney[5] = BgPanel->getChildByName<Button *>("cur1000");
		B_CurMoney[5]->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onJessonTouch, this));
		for (int i = 0; i < 6; i++)
		{
			CurMoneyLabel[i] = B_CurMoney[i]->getChildByName<TextBMFont *>("cur1_Text");
		}

		//自己的信息
		nameL = BgPanel->getChildByName<Text *>("nameL");
		nameL->setString(UserInfo::instance()->getNickName().c_str());
		usableL = BgPanel->getChildByName<Text *>("usableL");
		usableL->setString(__String::createWithFormat("%lld", UserInfo::instance()->getScore())->getCString());
		scoreL = BgPanel->getChildByName<Text *>("scoreL");
		scoreL->setString("0");

		//自己的头像框
		auto head_frame = BgPanel->getChildByName<ImageView *>("head_frame");

		ClippingNode* clippingNode = ClippingNode::create();
		clippingNode->setContentSize(head_frame->getContentSize());
		clippingNode->setPosition(head_frame->getPosition());
		BgPanel->addChild(clippingNode, head_frame->getLocalZOrder());

		Sprite* sencil = Sprite::create(SETPATH("sszp/sszp/mainScene/bg_Theheadframe_2.png"));
		clippingNode->setStencil(sencil);
		clippingNode->addChild(sencil);

		//用户头像
		string headUrl = StringUtils::format("user_info/bighead_%d.png", UserInfo::instance()->getFaceId());
		Sprite* _playerHeadSprite = nullptr;

		//如果faceId不在我们头像的区间，初始化faceId
		if (!FileUtils::getInstance()->isFileExist(headUrl))
		{
			_playerHeadSprite = Sprite::create("user_info/bighead_0.png");
		}
		else
		{
			_playerHeadSprite = Sprite::create(headUrl);
		}
		_playerHeadSprite->setScale(0.6);
		clippingNode->setAlphaThreshold(0);
		clippingNode->addChild(_playerHeadSprite);


		//庄家信息
		bankerUserL = BgPanel->getChildByName<Text *>("bankerUserL");  //庄家昵称
		bankerScoreL = BgPanel->getChildByName<Text *>("bankerScoreL");//庄家金币
		bankerTimeL = BgPanel->getChildByName<Text *>("bankerTimeL");  //庄家成绩
		auto head_BankerFrame = BgPanel->getChildByName<ImageView *>("head_BankerFrame");
		head_BankerFrame->loadTexture("user_info/bighead_0.png");

		//金币加亮
		const BlendFunc blendFunc = { GL_ONE, GL_ONE };//去黑色发亮处理
		jettonLabelVec.reserve(22);
		ImageView* frame_light[4] = { 0 };
		frame_light[0] = BgPanel->getChildByName<ImageView *>("frame_light_0");//庄家头像上的发亮
		frame_light[1] = BgPanel->getChildByName<ImageView *>("frame_light");//自己头像上的发亮
		frame_light[2] = BgPanel->getChildByName<ImageView *>("gold_light_0");//庄家金币上的发亮
		frame_light[3] = BgPanel->getChildByName<ImageView *>("gold_light");//自己金币上的发亮
		for (int i = 0; i < 4; i++)
		{
			Sprite *itemp_GoldLight = (Sprite *)frame_light[i]->getVirtualRenderer();
			itemp_GoldLight->setBlendFunc(blendFunc);
		}

		//顶上各个时间段图片
		label_bet = BgPanel->getChildByName<ImageView *>("label_bet");
		label_bet->setVisible(false);

		//顶上倒计时时间
		timeLabelTop = TextBMFont::create();
		timeLabelTop->setFntFile(SETPATH("sszp/sszp/fnt/count_down.fnt").c_str());
		timeLabelTop->setPosition(label_bet->getPosition() + Vec2(0, 44));
		BgPanel->addChild(timeLabelTop);

		//旋转圆圈
		circle = ProressCirCle::create();
		circle->setPosition(label_bet->getPosition() + Vec2(0, 47));
		circle->setBackGroundSprite(SETPATH("sszp/sszp_png/img_circle2.png"));
		circle->setProgressTimer(SETPATH("sszp/sszp_png/img_bg.png"));
		BgPanel->addChild(circle);

		ImageView *rotate_bg = ImageView::create(SETPATH("sszp/sszp_png/img_circle1.png"));
		circle->addChild(rotate_bg);
		circle->setWearOff(false);
		circle->setRoateTail(rotate_bg);

		//开始下注图片
		show_img = BgPanel->getChildByName<ImageView *>("show_img");
		show_img->setVisible(false);

		//下注结束
		imageViewBetEnd = ImageView::create(SETPATH("sszp/sszp_png/BetEnd.png"));
		imageViewBetEnd->setPosition(Vec2(684, 440));
		imageViewBetEnd->setVisible(false);
		BgPanel->addChild(imageViewBetEnd, 8);

		//添加容器以及里面的内容
		for (int i = 1; i < 9; i++)
		{
			auto jettonV = BgPanel->getChildByName(__String::createWithFormat("Panel_jesson_%d", i)->getCString());

			//总金额
			TextBMFont * ttf = TextBMFont::create();
			ttf->setFntFile(SETPATH("sszp/sszp/fnt/bet1_number.fnt"));
			ttf->setVisible(false);
			ttf->setPosition(jettonV->getPosition() + Vec2(0, -60));
			ttf->setTag(i);
			ttf->setString("0");
			BgPanel->addChild(ttf, 5);
			jettonLabelVec.pushBack(ttf);

			//自己下注后金额
			myJettonLabel[i - 1] = TextBMFont::create();
			myJettonLabel[i - 1]->setFntFile(SETPATH("sszp/sszp/fnt/bet_number.fnt"));
			myJettonLabel[i - 1]->setVisible(false);
			myJettonLabel[i - 1]->setPosition(jettonV->getPosition() + Vec2(0, 60));
			myJettonScore[i - 1] = 0;
			BgPanel->addChild(myJettonLabel[i - 1], 6);

			//下注后的标签
			blockLight[i - 1] = Scale9Sprite::create(SETPATH("sszp/sszp/mainScene/bet_number_1.png"));
			blockLight[i - 1]->setPosition(myJettonLabel[i - 1]->getPosition());
			blockLight[i - 1]->setVisible(false);
			BgPanel->addChild(blockLight[i - 1], 5);

			if (i == 5)
			{
				ttf->setPosition(jettonV->getPosition() + Vec2(24, -60));
				myJettonLabel[4]->setPosition(jettonV->getPosition() + Vec2(24, 60));
				blockLight[4]->setPosition(myJettonLabel[4]->getPosition());
			}
			if (i == 7)
			{
				ttf->setPosition(jettonV->getPosition() + Vec2(7, -60));
				myJettonLabel[6]->setPosition(jettonV->getPosition() + Vec2(7, 60));
				blockLight[6]->setPosition(myJettonLabel[6]->getPosition());
			}
		}

		//重复下注按钮
		repeatJ_btn = BgPanel->getChildByName<Button *>("repeatJ_btn");
		repeatJ_btn->setTag(ButtonType_sszp_repeatJ);
		repeatJ_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onTouch, this));

		//自动跟注按钮
		automaticJ_btn = BgPanel->getChildByName<Button *>("automaticJ_btn");
		automaticJ_btn->setTag(ButtonType_sszp_automaticJ);
		automaticJ_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onTouch, this));

		//坐庄中提示板
		jessonPanelBlack = BgPanel->getChildByName<ImageView *>("jessonPanelBlack");
		jessonPanelBlack->setVisible(false);

		const BlendFunc blendFunc_1 = { GL_ONE, GL_ONE };//去黑色发亮处理
		for (int i = 0; i < 8; i++)
		{
			myJettonLabelPanel[i] = BgPanel->getChildByName<ImageView*>(__String::createWithFormat("Panel_jesson_%d", i + 1)->getCString());
			Sprite *itemp_light = (Sprite *)myJettonLabelPanel[i]->getVirtualRenderer();
			itemp_light->setBlendFunc(blendFunc_1); //旋转圆圈加高亮
			myJettonLabelPanel[i]->setOpacity(0);
			myJettonLabelPanel[i]->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onTouch, this));
		}
		//点击重复下注显示出来的信息版
		upper_plane = static_cast<Widget *>(BgPanel->getChildByName("upper_plane"));
		upper_plane->setVisible(false);

		//点击重复下注显示确定
		auto upper_sure = static_cast<Widget *>(upper_plane->getChildByName("upper_sure"));
		upper_sure->setTag(ButtonType_sszp_repeatJSure);
		upper_sure->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onTouch, this));

		//点击重复下注显示关闭
		auto upper_cloer = static_cast<Widget *>(upper_plane->getChildByName("upper_cloer"));
		upper_cloer->setTag(ButtonType_sszp_repeatJClose);
		upper_cloer->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onTouch, this));

		//自动跟注
		btn_plane = static_cast<Widget *>(BgPanel->getChildByName("btn_plane"));  //自动下注按钮移动plane
		btn_plane->setVisible(false);

		auto btn_10 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_10"));
		btn_10->setTag(ButtonType_sszp_automaticJ_10);
		btn_10->addEventListenerCheckBox(this, checkboxselectedeventselector(MainScene_sszp::selectedEventautomatic));

		auto btn_20 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_20"));
		btn_20->setTag(ButtonType_sszp_automaticJ_20);
		btn_20->addEventListenerCheckBox(this, checkboxselectedeventselector(MainScene_sszp::selectedEventautomatic));

		auto btn_50 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_50"));
		btn_50->setTag(ButtonType_sszp_automaticJ_Infinite);
		btn_50->addEventListenerCheckBox(this, checkboxselectedeventselector(MainScene_sszp::selectedEventautomatic));

		//剩余跟注显示fnt字体
		automaticJ_label = static_cast<TextBMFont *>(BgPanel->getChildByName("automaticJ_label")); 
		automaticJ_label->setVisible(false);

		//路单
		list_planeL = static_cast<ImageView *>(BgPanel->getChildByName("list_planeL"));
		list_planeL->setTag(12);
		list_planeL->setTag(ButtonType_sszp_listplantL_move);
		list_planeL->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onTouch, this));

		auto light_Frame_coin = static_cast<ImageView *>(list_planeL->getChildByName("light_Frame_coin"));
		Sprite* itemp_FrameLight = (Sprite*)light_Frame_coin->getVirtualRenderer();
		itemp_FrameLight->setBlendFunc(blendFunc); //旋转圆圈加高亮

		banker_list = static_cast<ListView *>(list_planeL->getChildByName("banker_list"));
		banker_list->setVisible(true);
		ohter_list = static_cast<ListView *>(list_planeL->getChildByName("ohter_list"));
		ohter_list->setVisible(false);

		//我要上庄按钮
		banker_Btn = static_cast<Button *>(list_planeL->getChildByName("banker_btn"));
		banker_Btn->setTag(ButtonType_sszp_banker);
		banker_Btn->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onTouch, this));

		//侧面按钮点击拉近
		auto banker_move = static_cast<Button *>(list_planeL->getChildByName("banker_move"));
		banker_move->setTag(ButtonType_sszp_banker_move);
		banker_move->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onTouch, this));

		//路单以及上面的显示框
		record_list = static_cast<ListView *>(BgPanel->getChildByName("record_list"));

		//设置帮助退出按钮
		auto menuPanel = static_cast<Widget *>(BgPanel->getChildByName("menuPanel"));

		auto menu_btn = static_cast<Button *>(BgPanel->getChildByName("menu_btn"));
		menu_btn->setTag(ButtonType_sszp_menu);
		menu_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onTouch, this));

		auto setting_btn = static_cast<Button *>(menuPanel->getChildByName("setting_btn"));//设置音效按钮
		setting_btn->setTag(ButtonType_sszp_setting);
		setting_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onTouch, this));

		auto setting_music = static_cast<Button *>(menuPanel->getChildByName("setting_btn_0"));//设置音乐按钮
		setting_music->setTag(ButtonType_sszp_settingMusic);
		setting_music->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onTouch, this));

		//根据原来的音效设置声音
		isSound_music = HallSoundManager::instance()->getBgMusicConfig();
		if (isSound_music)
		{
			SoundManager::instance()->playBgMusic(SETPATH("sszp/sound/BetBk.mp3").c_str());
			setting_music->loadTextures(SETPATH("sszp/sszp/menu/menu_music.png"), SETPATH("sszp/sszp/menu/menu_music_2.png"));
		}
		else
		{
			SoundManager::instance()->stopBgMusic();
			setting_music->loadTextures(SETPATH("sszp/sszp/menu/menu_music3.png"), SETPATH("sszp/sszp/menu/menu_music3_2.png"));
		}

		isSound = HallSoundManager::instance()->getEffectConfig();
		if (isSound)
		{
			setting_btn->loadTextures(SETPATH("sszp/sszp/menu/btn_set.png"), SETPATH("sszp/sszp/menu/btn_set_2.png"));
		}
		else
		{
			setting_btn->loadTextures(SETPATH("sszp/sszp/menu/menu_sound3.png"), SETPATH("sszp/sszp/menu/menu_sound3_2.png"));
		}
		auto help_btn = static_cast<Button *>(menuPanel->getChildByName("help_btn"));//帮助按钮
		help_btn->setTag(ButtonType_sszp_help);
		help_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onTouch, this));

		auto out_btn = static_cast<Button *>(menuPanel->getChildByName("out_btn"));//退出按钮
		out_btn->setTag(ButtonType_sszp_out);
		out_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_sszp::onTouch, this));

		auto Image_289_0 = static_cast<ImageView *>(menuPanel->getChildByName("Image_289_0"));//退出按钮蒙版
		Image_289_0->setVisible(false);

		//旋转的圆圈
		S_circleLight = Sprite::create(SETPATH("sszp/sszp_png/brc_quan.png"));
		S_circleLight->setVisible(false);
		BgPanel->addChild(S_circleLight, 9);
		S_circleLight->setBlendFunc(blendFunc); //旋转圆圈加高亮
		S_circleLight->runAction(RepeatForever::create(RotateBy::create(2.0f, 360)));

		//下面走马灯
		auto light_panel = static_cast<ImageView *>(BgPanel->getChildByName("light_panel"));
		auto dark_panel = static_cast<ImageView *>(BgPanel->getChildByName("dark_panel"));
		light_panel->setVisible(true);
		dark_panel->setVisible(false);
		Sequence *seq = Sequence::create(DelayTime::create(0.7),
			CallFunc::create([=]{
			light_panel->setVisible(false);
			dark_panel->setVisible(true);
		}),
			DelayTime::create(0.7),
			CallFunc::create([=]{
			light_panel->setVisible(true);
			dark_panel->setVisible(false);
		}), NULL);
		light_panel->runAction(RepeatForever::create(seq));

		//指针
		point_Animal = static_cast<ImageView *>(BgPanel->getChildByName("point"));

		return true;
	}


	MainScene_sszp * MainScene_sszp::instance = nullptr;
	MainScene_sszp * MainScene_sszp::createScene()
	{
		if (!instance)
		{
			instance = MainScene_sszp::create();
		}
		return instance;
	}
	//开始转动转盘
	void MainScene_sszp::startMoveBlock(CMD_S_GameEnd& gameEnd)
	{
		//下注已满，提前开牌
		LONGLONG earlyStartScore = 0;
		for (int i = 0; i < AREA_COUNT; i++)
		{
			earlyStartScore += itemp_totalTtf[i];
		}
		if (earlyStartScore == lAreaLimitScore*AREA_COUNT)
		{
			PromptLayer::createObject(U2F("下注已满，提前开牌").c_str());
		}

		//开奖结构体临时存储下
		wCurrentGameEnd = gameEnd;

		//开奖结果
		initNumber = gameEnd.cbTableCard;

		//如果是庄家
		if (wCurrentBanker == UserInfo::instance()->getSeatId())
		{
			automaticJ_label->setString(__String::createWithFormat(U2F("您现在是庄家...(已坐庄%d轮)").c_str(), gameEnd.nBankerTime)->getCString());
		}

		//旋转动画，延迟十一秒后调用结束画面
		point_Animal->runAction(EaseSineOut::create(RotateBy::create(10.0f, -360 * 5 - (22.5 * (17 - initNumber)))));
		center_point_null_picture->runAction(EaseSineOut::create(RotateBy::create(10.0f, 1440)));
		schedule(schedule_selector(MainScene_sszp::moveEndAnimalPanel), 0.0f, 0, 10.5f);
		schedule(schedule_selector(MainScene_sszp::moveSlowLight), 0.0f, 0, 11.0f);
	}
	//中奖的动物加上黄色蒙版 渐闪 和筹码渐闪
	void MainScene_sszp::moveEndAnimalPanel(float dt)
	{
		auto m_tmBlock = static_cast<ImageView *>(center_point_null_picture->getChildByName(__String::createWithFormat("Sprite%d", initNumber - 1)->getCString()));

		//开奖动物黄色的圆圈，小的动物和大的动物圆圈不一样，以及做一些动作处理
		Sprite *m_sYellowPanel = NULL;
		if (changeEndNumber(initNumber) > 4)
		{
			m_sYellowPanel = Sprite::create(SETPATH("sszp/sszp_png/effect_SSK_02.png"));
		}
		else
		{
			m_sYellowPanel = Sprite::create(SETPATH("sszp/sszp_png/effect_SSK_01.png"));
		}
		m_sYellowPanel->setName("m_sYellowPanel");
		center_point_null_picture->addChild(m_sYellowPanel, 1);
		m_sYellowPanel->setPosition(m_tmBlock->getPosition());
		Sequence *seq = Sequence::createWithTwoActions(FadeOut::create(0.7), FadeIn::create(1.0));
		m_sYellowPanel->runAction(RepeatForever::create(seq));

		//获取世界坐标
		Vec2 m_vPoint = m_tmBlock->getWorldPosition();
		Vec2 pos = BgPanel->convertToNodeSpace(m_vPoint);

		//动物生成出来
		int m_itempNumber = changeEndNumber(initNumber) + 1;
		Sprite *m_sendAnimal = Sprite::create(SETPATH(__String::createWithFormat("sszp/sszp/animal/endAnimal_%d.png", m_itempNumber)->getCString()).c_str());
		m_sendAnimal->setName("m_sendAnimal");
		BgPanel->addChild(m_sendAnimal, 6);
		m_sendAnimal->setPosition(pos);
		m_sendAnimal->setScale(0.3);
		
		//动物过来到中心的runaction
		auto m_itempPoint = static_cast<ImageView *>(BgPanel->getChildByName("ponintHead"));
		m_sendAnimal->runAction(
			Spawn::create(
			RotateBy::create(0.5f, 360),
			EaseSineOut::create(MoveTo::create(3.0f, m_itempPoint->getPosition() + Vec2(0, 2))),
			ScaleTo::create(3.0f, 1.0f),
			NULL));

		//指针消失
		point_Animal->runAction(FadeOut::create(4.0f));
		//-----筹码渐闪
		Sequence *seq2 = Sequence::createWithTwoActions(FadeOut::create(0.7), FadeIn::create(1.0));
		myJettonLabelPanel[changeEndNumber(initNumber)]->runAction(RepeatForever::create(seq2));
	}

	//开奖一系列动画
	void MainScene_sszp::moveSlowLight(float dt)
	{
		//庄家和闲家开奖后声音不同
		bool itemp_bBanker = !strcmp(bankerUserL->getString().c_str(), UserInfo::instance()->getNickName().c_str());
		if (itemp_bBanker)
		{
			if (wCurrentGameEnd.lBankerScore > 0)
			{
				SoundManager::instance()->playEffect(SETPATH("sszp/sound/CHEER3.mp3").c_str());
			}
			else
			{
				SoundManager::instance()->playEffect(SETPATH("sszp/sound/end_lost.mp3").c_str());
			}
		}
		else
		{
			if (wCurrentGameEnd.lUserScore > 0)
			{
				SoundManager::instance()->playEffect(SETPATH("sszp/sound/CHEER3.mp3").c_str());
			}
			else
			{
				SoundManager::instance()->playEffect(SETPATH("sszp/sound/end_lost.mp3").c_str());
			}
		}
		auto childs = BgPanel->getChildren();
		this->runAction(
			Sequence::create(
			DelayTime::create(5.5f),

			//结算界面
			CallFunc::create([=]{
			overLayoutFunc(wCurrentGameEnd.lUserScore, wCurrentGameEnd);
		}),
			DelayTime::create(6.0),

			//结算界面消失
			CallFunc::create([=]{
			over_layout->setVisible(false);

		}),
			DelayTime::create(1.0),

			//散落到局面的金币飞出界面外
			CallFunc::create([=]{
			for (auto itor = childs.begin(); itor != childs.end(); itor++)
			{
				auto node = (*itor);
				if (node && node->getName() == "jettonSprite1"){
					int rand_angel = rand() % 7;
					Vec2 itemp_angel = randAngel(rand_angel);
					node->runAction(
						EaseSineOut::create(MoveTo::create(0.5f, itemp_angel))
						);
				}
			}
		}),
			NULL));
	}

	//开奖后具体是哪个动物
	int MainScene_sszp::changeEndNumber(int itemp_endNumber)
	{
		int index = 0;
		switch (itemp_endNumber)
		{
		case 1:
			index = 0;
			break;
		case 2:
			index = 5;
			break;
		case 3:
			index = 7;
			break;
		case 4:
			index = 6;
			break;
		case 5:
			index = 1;
			break;
		case 6:
			index = 7;
			break;
		case 7:
			index = 6;
			break;
		case 8:
			index = 4;
			break;
		case 9:
			index = 3;
			break;
		case 10:
			index = 6;
			break;
		case 11:
			index = 4;
			break;
		case 12:
			index = 5;
			break;
		case 13:
			index = 2;
			break;
		case 14:
			index = 4;
			break;
		case 15:
			index = 7;
			break;
		case 16:
			index = 5;
			break;
		}
		return index;
	}

	//筹码选中事件
	void MainScene_sszp::onJessonTouch(Ref* target, Widget::TouchEventType type)
	{
		Button *btn = (Button *)target;
		if (Widget::TouchEventType::BEGAN == type)
		{
			int itemp_tag = btn->getTag();
			if (isBanerMove == true)
			{
				if (itemp_tag != ButtonType_sszp_listplantL_move&&itemp_tag != ButtonType_sszp_banker &&itemp_tag != ButtonType_sszp_b &&itemp_tag != ButtonType_sszp_p &&itemp_tag != ButtonType_sszp_banker_move)
				{
					setListState(isBanerMove);
				}
			}
		}
		else if (Widget::TouchEventType::ENDED == type)
		{
			if (wCurrentTimeType == TimeType_sszp_PlaceJetton)
			{
				if (std::atoll(usableL->getString().c_str()) < ButtonNameChangeScore(btn->getName()))
				{
					PromptLayer::createObject(U2F("金币不足无法下注").c_str());

					return;
				}

				if (wCurrentJettonScore != 0)
				{
					//原来筹码下降--状态变为原来状态
					auto btns = (Button *)BgPanel->getChildByName(CurrentButtonName(wCurrentJettonScore));
					btns->setBrightStyle(BrightStyle::NORMAL);
					JessonUpOrDown(false, wCurrentJettonScore);

					//新筹码上升
					wCurrentJettonScore = ButtonNameChangeScore(btn->getName());
					JessonUpOrDown(true, wCurrentJettonScore);
				}
			}
			else
			{
				PromptLayer::createObject(U2F("请稍候，未到下注时间").c_str());
			}
		}
	}

	void MainScene_sszp::onTouch(Ref* target, Widget::TouchEventType type)
	{
		Button *btn = (Button *)target;
		auto itemp_tag = btn->getTag();
		if (Widget::TouchEventType::BEGAN == type)
		{
			//设置按钮缩进
			if (itemp_tag == ButtonType_sszp_out || itemp_tag == ButtonType_sszp_help || itemp_tag == ButtonType_sszp_settingMusic || itemp_tag == ButtonType_sszp_setting)
			{
				menuButtonMask(true, itemp_tag);
			}
			//庄家界面缩进
			if (isBanerMove == true)
			{
				if (itemp_tag != ButtonType_sszp_listplantL_move&&itemp_tag != ButtonType_sszp_banker &&itemp_tag != ButtonType_sszp_b &&itemp_tag != ButtonType_sszp_p &&itemp_tag != ButtonType_sszp_banker_move)
				{
					setListState(isBanerMove);
				}
			}
		}
		else if (Widget::TouchEventType::MOVED == type)
		{
			//设置按钮上面选中状态消失
			if (itemp_tag == ButtonType_sszp_out || itemp_tag == ButtonType_sszp_help || itemp_tag == ButtonType_sszp_settingMusic || itemp_tag == ButtonType_sszp_setting)
			{
				auto menuPanel = static_cast<Widget *>(BgPanel->getChildByName("menuPanel"));
				Vec2 pos = menuPanel->convertToNodeSpace(btn->getTouchMovePos());
				bool m_bIstrue = btn->getBoundingBox().containsPoint(pos);
				if (!m_bIstrue)
				{
					menuButtonMask(false);
				}
			}
		}
		else if (Widget::TouchEventType::CANCELED == type)
		{
			if (itemp_tag == ButtonType_sszp_out || itemp_tag == ButtonType_sszp_help || itemp_tag == ButtonType_sszp_settingMusic || itemp_tag == ButtonType_sszp_setting)
			{
				//设置按钮上面选中状态消失
				menuButtonMask(false);
			}
		}
		else if (Widget::TouchEventType::ENDED == type)
		{
			if (isMenu)
			{
				setMenuState(isMenu);
			}

			switch (itemp_tag)
			{
			case ButtonType_sszp_continue:
			{
											 over_layout->setVisible(false);
			}
				break;
			case ButtonType_sszp_banker_move:
			{
												setListState(isBanerMove);
			}
				break;
			case ButtonType_sszp_banker:
			{
										   if (!isAllowBank)
										   {
											   PromptLayer::createObject(U2F("该房间已限制玩家上庄").c_str());
											   return;
										   }
										   if (std::atoll(usableL->getString().c_str()) < lApplyBankerCondition)
										   {
											   PromptLayer::createObject(__String::createWithFormat(U2F("金币不足%lld，无法申请上庄").c_str(), lApplyBankerCondition)->getCString());
											   return;
										   }
										   switch (wCurrentBankerState)
										   {
										   case CurrentBanker_sszp_Already:		//当前已经是庄，我要下装
										   {
																			   SendGameMsg::sendApplyBanker(false);
										   }
											   break;
										   case CurrentBanker_sszp_Ready:		//当前是准备上庄，申请上庄
										   {
																			 SendGameMsg::sendApplyBanker(false);
										   }
											   break;
										   case CurrentBanker_sszp_None:		//没有状态，取消上庄
										   {
																			SendGameMsg::sendApplyBanker(true);
										   }
											   break;
										   default:
											   break;
										   }
										   banker_Btn->setTouchEnabled(false);
			}
				break;

			case ButtonType_sszp_automaticJ:
			{
											   //检测当前是否押注，当前无押注会返回需要押注的提示
											   bool itemp_isLastJesson = checkAutoMatic();
											   if (!itemp_isLastJesson)
											   {
												   return;
											   }

											   //取消掉所有状态
											   hideContinueJetton();

											   float h = 0.0f;
											   if (btn_plane->isVisible())
											   {
												   automaticJ_btn->loadTextures(SETPATH("sszp/sszp/mainScene/automaticJ_btn_n.png"), SETPATH("sszp/sszp/mainScene/automaticJ_btn_s.png"));
												   h = -btn_plane->getContentSize().height + 195;
											   }
											   else
											   {
												   automaticJ_btn->loadTextures(SETPATH("sszp/sszp_png/btn_qxgz_default.png"), SETPATH("sszp/sszp_png/btn_qxgz_pressed.png"));
												   h = btn_plane->getContentSize().height - 195;
											   }
											   if (btn_plane->getNumberOfRunningActions() == 0)
												   btn_plane->runAction(Sequence::create(MoveTo::create(0.1f, Vec2(btn_plane->getPositionX(), btn_plane->getPositionY() + h)), CallFuncN::create([=](Node *node){
												   if (h > 0)
												   {
													   btn_plane->setVisible(true);
												   }
												   else
												   {
													   btn_plane->setVisible(false);
												   }
											   }), NULL));
			}

				break;
			case ButtonType_sszp_repeatJ:
			{
											upper_plane->setVisible(true);
			}
				break;
			case ButtonType_sszp_repeatJClose:
			{
												 upper_plane->setVisible(false);
			}
				break;
			case ButtonType_sszp_repeatJSure:
			{
												//重复下注
												repaetButtonNoifyFial();
			}
				break;

			case 2001:
			case 2002:
			case 2003:
			case 2004:
			case 2005:
			case 2006:
			case 2007:
			case 2008:
			{
						 //点击下注
						 JettonArea(btn);
			}
				break;
			case ButtonType_sszp_menu:
			{
										 setMenuState(isMenu);
			}
				break;
			case ButtonType_sszp_out:
			{
										menuButtonMask(false);
										bool istrue = false;
										if (wCurrentTimeType == TimeType_sszp_PlaceJetton && my_AllJettonScore != 0)
										{
											istrue = true;
										}
										if (!strcmp(bankerUserL->getString().c_str(), UserInfo::instance()->getNickName().c_str()))
										{
											istrue = true;
										}if (istrue)
										{
											OutGameLayer::createObject(istrue);
										}
										else
										{
											SendMsg::sendStandUpReq(NetworkManager::FLAG_GAME, UserInfo::instance()->getTableId(), UserInfo::instance()->getSeatId(), 1, 0);
										}
			}
				break;
			case ButtonType_sszp_setting:
			{
											menuButtonMask(false);
											isSound = HallSoundManager::instance()->getEffectConfig();
											HallSoundManager::instance()->setEffectConfig(!isSound);
											auto menuPanel = static_cast<Widget *>(BgPanel->getChildByName("menuPanel"));
											auto setting_btn = static_cast<Button *>(menuPanel->getChildByName("setting_btn"));
											if (isSound)
											{
												setting_btn->loadTextures(SETPATH("sszp/sszp/menu/menu_sound3.png"), SETPATH("sszp/sszp/menu/menu_sound3_2.png"));
											}
											else
											{
												setting_btn->loadTextures(SETPATH("sszp/sszp/menu/btn_set.png"), SETPATH("sszp/sszp/menu/btn_set_2.png"));
											}
			}
				break;
			case ButtonType_sszp_settingMusic:
			{
												 menuButtonMask(false);
												 isSound_music = HallSoundManager::instance()->getBgMusicConfig();
												 HallSoundManager::instance()->setBgMusicConfig(!isSound_music);
												 auto menuPanel = static_cast<Widget *>(BgPanel->getChildByName("menuPanel"));
												 auto setting_music = static_cast<Button *>(menuPanel->getChildByName("setting_btn_0"));
												 if (isSound_music)
												 {
													 SoundManager::instance()->stopBgMusic();
													 setting_music->loadTextures(SETPATH("sszp/sszp/menu/menu_music3.png"), SETPATH("sszp/sszp/menu/menu_music3_2.png"));
												 }
												 else
												 {
													 SoundManager::instance()->playBgMusic(SETPATH("sszp/sound/BetBk.mp3").c_str());
													 setting_music->loadTextures(SETPATH("sszp/sszp/menu/menu_music.png"), SETPATH("sszp/sszp/menu/menu_music_2.png"));
												 }
			}
				break;
			case ButtonType_sszp_help:
			{
										 menuButtonMask(false);
										 helpLayer::createObject();
			}
				break;
			default:
				break;
			}
		}
	}

	//menu上的蒙版伸缩
	void MainScene_sszp::menuButtonMask(bool m_isTrue, int m_iTouchTag) 
	{
		auto menuPanel = static_cast<Widget *>(BgPanel->getChildByName("menuPanel"));
		auto Image_289_0 = static_cast<ImageView *>(menuPanel->getChildByName("Image_289_0"));//退出按钮蒙版
		if (m_isTrue)
		{
			auto m_itempButton = (ImageView *)menuPanel->getChildByTag(m_iTouchTag);
			Image_289_0->setPosition(m_itempButton->getPosition() + Vec2(-2, 0));
			Image_289_0->setVisible(true);
		}
		else
		{
			Image_289_0->setVisible(false);
		}
	}

	//自动跟注选中框事件
	void MainScene_sszp::selectedEventautomatic(cocos2d::Ref *pSender, CheckBoxEventType type)
	{
		CheckBox * itep_check = dynamic_cast<CheckBox *>(pSender);
		continueJetton_number = itep_check->getTag();
		auto btn_10 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_10"));
		auto btn_20 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_20"));
		auto btn_50 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_50"));
		switch (type)
		{
		case cocos2d::ui::CHECKBOX_STATE_EVENT_SELECTED:	//选中后处理
			if (checkAutoMatic())
			{
				showContinueJetton();
				switch (itep_check->getTag())
				{
				case ButtonType_sszp_automaticJ_10:
					btn_20->setSelectedState(false);
					btn_50->setSelectedState(false);
					break;
				case ButtonType_sszp_automaticJ_20:
					btn_10->setSelectedState(false);
					btn_50->setSelectedState(false);
					break;
				case ButtonType_sszp_automaticJ_Infinite:
					btn_10->setSelectedState(false);
					btn_20->setSelectedState(false);
					break;
				default:
					break;
				}
			}
			else
			{
				itep_check->setSelectedState(false);
			}
			break;
			
		case cocos2d::ui::CHECKBOX_STATE_EVENT_UNSELECTED:
			if (isContinueJetton)
			{
				switch (itep_check->getTag())
				{
				case ButtonType_sszp_automaticJ_10:
					btn_10->setSelectedState(true);
					break;
				case ButtonType_sszp_automaticJ_20:
					btn_20->setSelectedState(true);
					break;
				case ButtonType_sszp_automaticJ_Infinite:
					btn_50->setSelectedState(true);
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}
	}

	//自动跟注勾全部取消掉
	void MainScene_sszp::automaticJCancel() 
	{
		auto btn_10 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_10"));
		auto btn_20 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_20"));
		auto btn_50 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_50"));
		btn_10->setSelectedState(false);
		btn_20->setSelectedState(false);
		btn_50->setSelectedState(false);
	}

	//下注处理
	void MainScene_sszp::JettonArea(Button *btn)
	{
		if (!strcmp(bankerUserL->getString().c_str(), UserInfo::instance()->getNickName().c_str()))
		{
			PromptLayer::createObject(U2F("做庄中，无法下注").c_str());
			return;
		}
		if (wCurrentTimeType != TimeType_sszp_PlaceJetton)
		{
			PromptLayer::createObject(U2F("请稍候，未到下注时间").c_str());
			return;
		}
		if (isContinueJetton)
		{
			PromptLayer::createObject(U2F("自动跟注中,请取消跟注再试").c_str());
			return;
		}
		if (std::atoll(usableL->getString().c_str()) < tagChange[0])
		{
			PromptLayer::createObject(U2F("金币不足无法下注").c_str());
			return;
		}
		if (wCurrentJettonScore == 0)
		{
			PromptLayer::createObject(U2F("请选择筹码后下注").c_str());
			return;
		}
		if (my_AllJettonScore + wCurrentJettonScore > lAllLimitScore)
		{
			PromptLayer::createObject(U2F("已达个人下注上限无法下注").c_str());
			return;
		}
		if (itemp_myJettonLabel[btn->getTag() - 2001] + wCurrentJettonScore > lUserSingleLimitScore)
		{
			PromptLayer::createObject(U2F("已达区域个人下注上限，请选择其他区域下注").c_str());
			return;
		}
		if (itemp_totalTtf[btn->getTag() - 2001] + wCurrentJettonScore > lAreaLimitScore)
		{
			PromptLayer::createObject(U2F("该区域下注已满，请选择其他区域下注").c_str());
			return;
		}
		int jettonType = 0;
		jettonType = btn->getTag() - 1000;
		if (jettonType > 1000)
		{
			jettonType = btn->getTag() - 2000;
		}
		if (PlaceJettonLimit(jettonType))
		{
			SendGameMsg::sendPlace_Jetton(NetworkManager::FLAG_GAME, jettonType, wCurrentJettonScore);
		}
		else
		{
			PromptLayer::createObject(U2F("已达庄家赔付上限，无法下注").c_str());
		}
	}

	//押注限制
	bool MainScene_sszp::PlaceJettonLimit(int type)
	{
		if (!strcmp(bankerUserL->getString().c_str(), U2F("系统坐庄").c_str()))
		{
			return true;
		}
		int rateNumber = getCurrentRate(type);
		LONGLONG currentNumber = std::atoll(jettonLabelVec.at(type - 1)->getString().c_str()) *rateNumber;
		LONGLONG ohterNumber = 0;
		for (int i = 1; i < 9; i++)
		{
			if (i != type)
			{
				int ohter_rate = getCurrentRate(i);
				ohterNumber += std::atoll(jettonLabelVec.at(i - 1)->getString().c_str());
			}

		}
		if (std::atoll(bankerScoreL->getString().c_str()) + ohterNumber >= currentNumber + (wCurrentJettonScore * getCurrentRate(type)))
		{
			return true;
		}
		else
			return false;
	}

	void MainScene_sszp::clearPlayer(Object * player)
	{
		PokerPlayerInfo* playerInfo = (PokerPlayerInfo*)player;

		for (int i = 0; i < ohter_list->getItems().size(); i++)
		{
			if (!strcmp(ohter_list->getItem(i)->getName().c_str(), playerInfo->getNickName().c_str()))
			{
				ohter_list->removeItem(i);
				PokerData::instance()->removePlayerInfo(playerInfo->getInfo().dwUserID);
			}
		}
		//刷新在线人数
		int itemp_onlineNumber = ohter_list->getItems().size();
		auto online_number = static_cast<TextBMFont *>(BgPanel->getChildByName("online_number"));
		online_number->setString(__String::createWithFormat("%d", itemp_onlineNumber)->getCString());
	}

	//筹码赋值
	void MainScene_sszp::setJessonValue(const LONGLONG itemp_Chip[])
	{
		for (int i = 0; i < 6; i++)
		{
			tagChange[i] = itemp_Chip[i];

			if (itemp_Chip[i] < 10000)
			{
				CurMoneyLabel[i]->setString(__String::createWithFormat("%lld", itemp_Chip[i])->getCString());
			}
			else if (itemp_Chip[i] >= 10000)
			{
				CurMoneyLabel[i]->setString(__String::createWithFormat("%lld%s", (itemp_Chip[i]) / 10000, U2F("万").c_str())->getCString());
				if (itemp_Chip[i] >= 10000000)
				{
					Label *itemp_CurMoneyLabel = dynamic_cast<Label *>(CurMoneyLabel[i]->getVirtualRenderer());

					//字体间距缩进
					itemp_CurMoneyLabel->setAdditionalKerning(-4);		
				}
			}
		}
	}
	void MainScene_sszp::initGameScene(CMD_S_StatusFree& obj)
	{
		auto list_planeL = static_cast<Widget *>(BgPanel->getChildByName("list_planeL"));
		auto list_banker = static_cast<Text *>(list_planeL->getChildByName("list_banker"));
		auto list_score = static_cast<Text *>(list_planeL->getChildByName("list_score"));

		lAreaLimitScore = obj.lAreaLimitScore;				//玩家单个区域下注
		lUserSingleLimitScore = obj.lUserAlearLimitScore;	//个人区域下注限制,每个玩家在任意一个区域下注最高封顶数量
		lAllLimitScore = obj.lUserMaxScore;					//玩家总下注
		isAllowBank = obj.bAllowBank;						//是否准许上庄
		lApplyBankerCondition = obj.lApplyBankerCondition;	//上庄条件

		PokerPlayerInfo * player = PokerData::instance()->getPlayerInfoBySeatId(obj.wBankerUser);

		//筹码保存值
		LONGLONG itemp_chip[6] = { 0 };
		memcpy(&itemp_chip, obj.nChip, sizeof(itemp_chip));
		setJessonValue(itemp_chip);

		if (obj.wBankerUser == INVALID_CHAIR)
		{
			list_banker->setString(U2F("系统坐庄"));
			list_score->setString(__String::createWithFormat("%lld", obj.lBankerScore)->getCString());
			bankerUserL->setString(U2F("系统坐庄"));

			//庄家头像替换
			auto head_BankerFrame = static_cast<ImageView *>(BgPanel->getChildByName("head_BankerFrame"));
			head_BankerFrame->loadTexture("user_info/bighead_0.png");

			bankerScoreL->setString(__String::createWithFormat("%lld", obj.lBankerScore)->getCString());
			wCurrentBanker = INVALID_CHAIR;
		}
		else
		{
			if (player)
			{
				list_banker->setString(U2F(player->getNickName().c_str()));
				bankerUserL->setString(U2F(player->getNickName().c_str()));
				//玩家头像替换
				auto head_BankerFrame = static_cast<ImageView *>(BgPanel->getChildByName("head_BankerFrame"));
				int faceId = player->getFaceId();
				string headUrl = StringUtils::format("user_info/bighead_%d.png", faceId);
				if (!FileUtils::getInstance()->isFileExist(headUrl))//如果faceId不在我们头像的区间，初始化faceId
				{
					//如果faceId不在我们头像的区间，在手机头像随机一个
					headUrl = StringUtils::format("user_info/bighead_0.png");
				}
				head_BankerFrame->loadTexture(headUrl);
				upper_plane->setVisible(false);  //重复下注面板隐藏
				wCurrentBanker = player->getSeatId();
			}
			list_score->setString(__String::createWithFormat("%lld", obj.lBankerScore)->getCString());
			bankerScoreL->setString(__String::createWithFormat("%lld", obj.lBankerScore)->getCString());
		}

		//庄家成绩
		bankerTimeL->setString(__String::createWithFormat("%lld", obj.lBankerWinScore)->getCString());
		
		//上庄需要金币
		auto list_bankerScore = static_cast<Text *>(list_planeL->getChildByName("list_bankerScore"));
		list_bankerScore->setString(__String::createWithFormat("%s%lld", U2F("上庄需要 ").c_str(), lApplyBankerCondition)->getCString());

		//上庄界面是否显示出来
		if (!isAllowBank)
		{
			list_planeL->setVisible(false);
		}
		else
		{
			list_planeL->setVisible(true);
		}

		//如果是庄家
		if (wCurrentBanker == UserInfo::instance()->getSeatId())
		{
			wCurrentBankerState = CurrentBanker_sszp_Already;
			this->changeBankerButton();
			this->showUpeerBanker(true);
		}
	}

	void MainScene_sszp::initGameScene(CMD_S_StatusPlay& obj)
	{
		PokerPlayerInfo * player = PokerData::instance()->getPlayerInfoBySeatId(obj.wBankerUser);
		auto list_planeL = BgPanel->getChildByName<Widget *>("list_planeL");
		auto list_banker = list_planeL->getChildByName<Text *>("list_banker");
		auto list_score = list_planeL->getChildByName<Text *>("list_score");

		//筹码保存值
		LONGLONG itemp_chip[6] = { 0 };
		memcpy(&itemp_chip, obj.nChip, sizeof(itemp_chip));
		setJessonValue(itemp_chip);

		lAreaLimitScore = obj.lAreaLimitScore;				//区域限制(每个注区的最高封顶数量)
		lUserSingleLimitScore = obj.lUserAlearLimitScore;	//个人区域下注限制,每个玩家在任意一个区域下注最高封顶数量
		lAllLimitScore = obj.lUserMaxScore;					//玩家总下注
		isAllowBank = obj.bAllowBank;						//是否准许上庄
		lApplyBankerCondition = obj.lApplyBankerCondition;	//申请上庄条件

		//上庄面板是否显示出来
		if (!isAllowBank)
		{
			list_planeL->setVisible(false);
		}
		else
		{
			list_planeL->setVisible(true);
		}

		if (obj.wBankerUser == INVALID_CHAIR)
		{
			list_banker->setString(U2F("系统坐庄"));
			list_score->setString(__String::createWithFormat("%lld", obj.lBankerScore)->getCString());
			bankerUserL->setString(U2F("系统坐庄"));
			//庄家头像替换
			auto head_BankerFrame = static_cast<ImageView *>(BgPanel->getChildByName("head_BankerFrame"));
			head_BankerFrame->loadTexture("user_info/bighead_0.png");
			bankerScoreL->setString(__String::createWithFormat("%lld", obj.lBankerScore)->getCString());
			wCurrentBanker = INVALID_CHAIR;
		}
		else
		{
			if (player)
			{
				list_banker->setString(U2F(player->getNickName().c_str()));
				bankerUserL->setString(U2F(player->getNickName().c_str()));
				wCurrentBanker = player->getSeatId();
				//玩家头像替换
				auto head_BankerFrame = static_cast<ImageView *>(BgPanel->getChildByName("head_BankerFrame"));
				int faceId = player->getFaceId();
				string headUrl = StringUtils::format("user_info/bighead_%d.png", faceId);
				if (!FileUtils::getInstance()->isFileExist(headUrl))//如果faceId不在我们头像的区间，初始化faceId
				{
					//如果faceId不在我们头像的区间，在手机头像随机一个
					headUrl = StringUtils::format("user_info/bighead_0.png");
				}
				head_BankerFrame->loadTexture(headUrl);

			}
			list_score->setString(__String::createWithFormat("%lld", obj.lBankerScore)->getCString());
			bankerScoreL->setString(__String::createWithFormat("%lld", obj.lBankerScore)->getCString());
		}
		bankerTimeL->setString(__String::createWithFormat("%lld", obj.lBankerWinScore)->getCString());
		
		//上庄面板中的上庄需要的金币
		auto list_bankerScore = static_cast<Text *>(list_planeL->getChildByName("list_bankerScore"));
		list_bankerScore->setString(__String::createWithFormat("%s%lld", U2F("上庄需要 ").c_str(), lApplyBankerCondition)->getCString());

		if (wCurrentBanker == UserInfo::instance()->getSeatId())
		{
			wCurrentBankerState = CurrentBanker_sszp_Already;
			banker_Btn->setTouchEnabled(false);
			banker_Btn->loadTextureNormal(SETPATH("sszp/sszp_png/banker_Btn2_s.png").c_str());
			this->showUpeerBanker(true);
			automaticJ_label->setString(__String::createWithFormat(U2F("您现在是庄家...(已坐庄%d轮)").c_str(), obj.cbBankerTime)->getCString());
			BankerCount = obj.cbBankerTime;
			jessonPanelBlack->setVisible(true);
			automaticJ_label->setVisible(true);
			for (int i = 0; i < 6; i++)
			{
				B_CurMoney[i]->setTouchEnabled(false);
				CurMoneyLabel[i]->setColor(Color3B(133, 133, 133));
			}
		}
		for (int i = 0; i < 8; i++)
		{
			if (obj.lAllJettonScore[i + 1]>0)
			{
				jettonLabelVec.at(i)->setString(__String::createWithFormat("%lld", obj.lAllJettonScore[i + 1])->getCString());
				itemp_totalTtf[i] += obj.lAllJettonScore[i + 1];
				jettonLabelVec.at(i)->setVisible(true);
			}
			if (obj.lUserJettonScore[i + 1] > 0)
			{
				myJettonLabel[i]->setString(__String::createWithFormat("%lld", obj.lUserJettonScore[i + 1])->getCString());
				my_AllJettonScore += obj.lUserJettonScore[i + 1];
				myJettonScore[i] = obj.lUserJettonScore[i + 1];
				itemp_myJettonLabel[i] = obj.lUserJettonScore[i + 1];
				auto lScore = (Text *)upper_plane->getChildByName(__String::createWithFormat("my_upper_%d", i + 1)->getCString());
				lScore->setString(__String::createWithFormat("%lld", obj.lUserJettonScore[i + 1])->getCString());
				myJettonLabel[i]->setVisible(true);
			}
		}
		//断线重连后可用金币减少
		if (my_AllJettonScore != 0)
		{
			usableL->setString(__String::createWithFormat("%lld", std::atoll(usableL->getString().c_str()) - my_AllJettonScore)->getCString());
		}
		if (PokerData::instance()->getMatchRule() & 0x00000040)
		{
			if (wCurrentTimeType == TimeType_sszp_Start)
			{
				int record = obj.cbTableCard;
				this->addRecordList(record, false);
				CMD_S_GameEnd endData;
				memset(&endData, 0, sizeof(CMD_S_GameEnd));
				endData.lBankerScore = obj.lEndBankerScore;
				endData.lUserScore = obj.lEndUserScore;
				scoreL->setString(__String::createWithFormat("%lld", obj.lEndUserScore)->getCString());
			}
		}
	}

	void MainScene_sszp::changeBankerButton()
	{
		switch (wCurrentBankerState)
		{
		case CurrentBanker_sszp_Already:
		{
										   banker_Btn->loadTextureNormal(SETPATH("sszp/sszp_png/banker_Btn2.png"));
		}
			break;
		case CurrentBanker_sszp_Ready:
		{
										 banker_Btn->loadTextureNormal(SETPATH("sszp/sszp_png/banker_Btn3.png"));
		}
			break;
		case CurrentBanker_sszp_None:
		{
										banker_Btn->loadTextureNormal(SETPATH("sszp/sszp/banker/banker_Btn1.png"));
		}
			break;
		default:
			break;
		}
	}

	//申请上庄列表，目前废弃
	void MainScene_sszp::setOhterPlayer(Object * player)
	{
		PokerPlayerInfo* playerInfo = (PokerPlayerInfo*)player;
		int itemp_onlineNumber = ohter_list->getItems().size();
		for (int i = 0; i < itemp_onlineNumber; i++)
		{
			if (!strcmp(ohter_list->getItem(i)->getName().c_str(), playerInfo->getNickName().c_str()))
			{
				LabelTTF * ttf = (LabelTTF *)ohter_list->getItem(i)->getChildByName(playerInfo->getNickName().c_str())->getChildByName("scoreTTf");
				if (ttf)
				{
					ttf->setString(__String::createWithFormat("%lld", playerInfo->getScore())->getCString());
				}
				for (int j = 0; j < banker_list->getItems().size(); j++)
				{

					if (!strcmp(banker_list->getItem(j)->getName().c_str(), ohter_list->getItem(i)->getName().c_str()))
					{
						LabelTTF * ttf_bank = (LabelTTF *)banker_list->getItem(j)->getChildByName("scoreTTf");
						if (ttf_bank)
						{
							ttf_bank->setString(__String::createWithFormat("%lld", playerInfo->getScore())->getCString());
						}
						return;
					}
				}
				return;
			}
		}

		// 在线人数
		auto online_number = static_cast<TextBMFont *>(BgPanel->getChildByName("online_number"));
		online_number->setString(__String::createWithFormat("%d", itemp_onlineNumber + 1)->getCString());

		auto banker = Sprite::create(SETPATH("sszp/sszp_png/playerList.png"));
		banker->setName(playerInfo->getNickName().c_str());
		auto bg = Layout::create();
		bg->setContentSize(banker->getContentSize());
		bg->setTag(playerInfo->getSeatId());
		bg->setName(playerInfo->getNickName().c_str());

		banker->setAnchorPoint(Vec2::ZERO);
		bg->addChild(banker);

		int faceId = playerInfo->getFaceId();
		string headUrl = StringUtils::format("user_info/bighead_%d.png", faceId);
		if (!FileUtils::getInstance()->isFileExist(headUrl))//如果faceId不在我们头像的区间，初始化faceId
		{
			//如果faceId不在我们头像的区间，在手机头像随机一个
			faceId = 0;
		}
		auto feath = Sprite::create(__String::createWithFormat("user_info/bighead_%d.png", faceId)->getCString());
		feath->setPosition(5, 11);
		feath->setScale(0.6f);
		feath->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		banker->addChild(feath);

		auto nameTTf = LabelTTF::create();
		nameTTf->setString(playerInfo->getNickName().c_str());
		nameTTf->setPosition(Vec2(90, 58));
		nameTTf->setAnchorPoint(Vec2(0.0f, 0.5f));
		nameTTf->setColor((Color3B(243, 208, 119)));
		nameTTf->setFontSize(20);
		banker->addChild(nameTTf);

		auto scoreTTf = LabelTTF::create();
		scoreTTf->setString(__String::createWithFormat("%lld", playerInfo->getScore())->getCString());
		scoreTTf->setPosition(Vec2(115, 28));
		scoreTTf->setAnchorPoint(Vec2(0.0f, 0.5f));
		scoreTTf->setColor((Color3B(243, 208, 119)));
		scoreTTf->setFontSize(20);
		scoreTTf->setName("scoreTTf");
		banker->addChild(scoreTTf);

		auto gold = Sprite::create(SETPATH("sszp/sszp_png/gold.png"));
		gold->setScale(0.4f);
		gold->setPosition(Vec2(-15, 0) + scoreTTf->getPosition());
		banker->addChild(gold);

		if (playerInfo->getInfo().dwUserID != UserInfo::instance()->getUserId())
		{
			nameTTf->setColor(Color3B(243, 81, 0));
			scoreTTf->setColor(Color3B(243, 81, 0));
		}
		ohter_list->insertCustomItem(bg, ohter_list->getItems().size());

		ohter_list->setInnerContainerSize(CCSize(0, ohter_list->getItems().size()*bg->getContentSize().height));
		ohter_list->refreshView();
	}

	//刷新金币
	void MainScene_sszp::updateScore(CMD_GR_MobileUserScore& buf)
	{
		if (buf.dwUserID == UserInfo::instance()->getUserId())
		{
			usableL->setString(__String::createWithFormat("%lld", buf.UserScore.lScore)->getCString());
		}
		else
		{
			if (wCurrentBanker == buf.dwUserID)
			{
				PokerPlayerInfo * player = PokerData::instance()->getPlayerInfo(buf.dwUserID);
				if (player)
				{
					bankerScoreL->setString(__String::createWithFormat("%lld", player->getScore())->getCString());
				}
			}
		}
	}

	//倒计时
	void MainScene_sszp::createUpdate(int time)
	{
		circle->setWearOff(false);
		circle->Start(time, nullptr);
		timeLabelTop->setString(__String::createWithFormat("%d", time)->getCString());
		timeLabelTop->setVisible(true);
		schedule(schedule_selector(MainScene_sszp::timeUpdate), 1.0f);
	}
	void MainScene_sszp::timeUpdate(float dt)
	{
		int num = std::atoll(timeLabelTop->getString().c_str());
		if (num > 1)
		{
			num--;
			if (timeLabelTop)
			{
				timeLabelTop->setString(__String::createWithFormat("%d", num)->getCString());
				if (num <= 4 && wCurrentTimeType == TimeType_sszp_PlaceJetton)
				{
					//SoundManager::instance()->playEffect(SETPATH("sszp/sound/sszp_clock_ring.mp3").c_str());
				}
			}
		}
		else
		{
			removeTimeLabel();
		}
	}

	void MainScene_sszp::removeTimeLabel()
	{
		unschedule(schedule_selector(MainScene_sszp::timeUpdate));
		timeLabelTop->setString(__String::createWithFormat("%d", 0)->getCString());
	}

	void MainScene_sszp::PlaceJettonFunction(CMD_S_PlaceJetton& jetton)
	{
		SoundManager::instance()->playEffect(SETPATH("sszp/sound/Bet.mp3").c_str());//这里添加声音
		bool isMyself = (jetton.wChairID == UserInfo::instance()->getSeatId()) ? true : false;
		updateShowScore(isMyself, jetton.cbJettonArea, jetton.lJettonScore);
	}

	void MainScene_sszp::updateShowScore(bool isMySeatId, BYTE jettonArea, SCORE jettonScore)
	{
		if (isMySeatId)//如果是自己
		{
			usableL->setString(__String::createWithFormat("%lld", std::atoll(usableL->getString().c_str()) - jettonScore)->getCString());
			my_AllJettonScore += jettonScore;
			myJettonLabel[jettonArea - 1]->setVisible(true);

			//筹码显示数字的底盘根据金币长度进行增加
			itemp_myJettonLabel[jettonArea - 1] += jettonScore;
			myJettonLabel[jettonArea - 1]->setString(__String::createWithFormat("%lld", itemp_myJettonLabel[jettonArea - 1])->getCString());
			blockLight[jettonArea - 1]->setContentSize(CCSize(73, 26));
			blockLight[jettonArea - 1]->setVisible(true);
			if (itemp_myJettonLabel[jettonArea - 1] >= 10000 && itemp_myJettonLabel[jettonArea - 1] < 100000)
			{
				blockLight[jettonArea - 1]->setContentSize(CCSize(93, 26));
			}
			else if (itemp_myJettonLabel[jettonArea - 1] >= 100000 && itemp_myJettonLabel[jettonArea - 1] < 1000000)
			{
				blockLight[jettonArea - 1]->setContentSize(CCSize(109, 26));
			}
			else if (itemp_myJettonLabel[jettonArea - 1] >= 1000000 && itemp_myJettonLabel[jettonArea - 1] < 10000000)
			{
				blockLight[jettonArea - 1]->setContentSize(CCSize(128, 26));
			}
			else if (itemp_myJettonLabel[jettonArea - 1] >= 10000000 && itemp_myJettonLabel[jettonArea - 1] < 100000000)
			{
				blockLight[jettonArea - 1]->setContentSize(CCSize(143, 26));
			}
			else if (itemp_myJettonLabel[jettonArea - 1] >= 10000000)
			{
				blockLight[jettonArea - 1]->setContentSize(CCSize(160, 26));
			}

			//1、每次下注检查筹码显示 2、金币不足圆圈消失
			if (isContinueJetton == false && wCurrentTimeType == TimeType_sszp_PlaceJetton)
			{
				for (int i = 0; i < 6; i++)
				{
					if (i == getJettonType(jettonScore) - 1)
					{
						if (std::atoll(usableL->getString().c_str()) < getJettonNumber(i + 1))
						{
							wCurrentJettonScore = 0;
							B_CurMoney[i]->setTouchEnabled(false);
							B_CurMoney[i]->setBright(false);
							CurMoneyLabel[i]->setColor(Color3B(133, 133, 133));
							B_CurMoney[i]->stopAllActions();
							B_CurMoney[i]->runAction(MoveTo::create(0.1f, Vec2(B_CurMoney[i]->getPosition().x, jessonDownCount)));
							S_circleLight->setVisible(false);
							if (i != 0)//筹码自动滑落到下一档区域
							{
								for (int j = i; j > 0; j--)
								{
									if (std::atoll(usableL->getString().c_str()) >= getJettonNumber(j))
									{
										auto btnsv = (Button *)BgPanel->getChildByName(CurrentButtonName(getJettonNumber(j)));
										btnsv->setBrightStyle(BrightStyle::HIGHLIGHT);
										S_circleLight->setVisible(true);
										S_circleLight->setPosition(btnsv->getPosition().x, jessonUpCount);
										btnsv->stopAllActions();
										btnsv->runAction(MoveTo::create(0.1f, Vec2(btnsv->getPosition().x, jessonUpCount)));
										wCurrentJettonScore = getJettonNumber(j);
										break;
									}
								}
							}
						}
					}
					else
					{
						if (std::atoll(usableL->getString().c_str()) >= getJettonNumber(i + 1))
						{
							B_CurMoney[i]->setTouchEnabled(true);
							B_CurMoney[i]->setBright(true);
							CurMoneyLabel[i]->setColor(Color3B(255, 255, 255));
						}
						else
						{
							B_CurMoney[i]->setTouchEnabled(false);
							B_CurMoney[i]->setBright(false);
							CurMoneyLabel[i]->setColor(Color3B(133, 133, 133));
						}
					}
				}
			}

			//筹码效果
			PlaceJettonEffect(jettonArea, jettonScore, true);
		}
		else
		{
			PlaceJettonEffect(jettonArea, jettonScore, false);
		}
		//其他人下的注显示
		TextBMFont *ttf = jettonLabelVec.at(jettonArea - 1);
		ttf->setVisible(true);
		itemp_totalTtf[jettonArea - 1] += jettonScore;
		ttf->setString(__String::createWithFormat("%lld", itemp_totalTtf[jettonArea - 1])->getCString());
	}

	void MainScene_sszp::PlaceJettonEffect(int Area, int JessonNumber, bool isMySealt)
	{
		//筹码效果
		auto jettonSprite = Sprite::create(SETPATH("sszp/sszp/mainScene/bg_Shangzhuang_1.png"));
		jettonSprite->setName("jettonSprite1");
		int curType = getJettonType(JessonNumber) - 1;
		if (isMySealt)
		{
			jettonSprite->setPosition(B_CurMoney[curType]->getPosition());
		}
		else
		{
			int rand_angel = rand() % 8;
			Vec2 itemp_angel = randAngel(rand_angel);
			jettonSprite->setPosition(itemp_angel);
		}
		auto jettonPosition_up = BgPanel->getChildByName(__String::createWithFormat("jetton_Panel_%d", Area)->getCString());
		BgPanel->addChild(jettonSprite, 4);

		//筹码从一个地方飞入
		jettonSprite->runAction(Sequence::create(
			ScaleTo::create(0.2f, 1.0f),
			EaseSineOut::create(MoveTo::create(0.3f, randJettonPos(Area))),
			NULL));
	}

	//随机一个下注区域的坐标 
	Vec2 MainScene_sszp::randJettonPos(int jettonArea)
	{
		auto itempImage = (ImageView *)BgPanel->getChildByTag(jettonArea + 100);
		float rand_x = rand() % (int)(itempImage->getContentSize().width - (B_CurMoney[1]->getContentSize().width *0.3f) - 15.0f);
		float rand_y = rand() % (int)(itempImage->getContentSize().height - (B_CurMoney[1]->getContentSize().height *0.3f));
		return Vec2(itempImage->getPosition() - Vec2(itempImage->getContentSize() / 2) + Vec2(B_CurMoney[1]->getContentSize() *0.15f) + Vec2(rand_x + 8, rand_y));
	}

	//金币飞进飞出的几个地方
	Vec2 MainScene_sszp::randAngel(int jettonArea)
	{
		switch (jettonArea)
		{
		case 0:
			return Vec2(-50, 200);
		case 1:
			return Vec2(-50, 560);
		case 2:
			return Vec2(300, 800);
		case 3:
			return Vec2(600, 800);
		case 4:
			return Vec2(900, 800);
		case 5:
			return Vec2(1400, 600);
		case 6:
			return Vec2(1400, 300);
		default:
			return Vec2(1400, 300);
		}
	}

	int MainScene_sszp::ButtonNameChangeScore(string currentName)
	{
		for (int i = 0; i < 6; i++)
		{
			if (currentName == B_CurMoney[i]->getName())
			{
				return tagChange[i];
			}
		}
	}

	SCORE MainScene_sszp::getJettonNumber(int jType)
	{
		SCORE jScore = 0;
		switch (jType)
		{
		case 1:
			jScore = tagChange[0];
			break;
		case 2:
			jScore = tagChange[1];
			break;
		case 3:
			jScore = tagChange[2];
			break;
		case 4:
			jScore = tagChange[3];
			break;
		case 5:
			jScore = tagChange[4];
			break;
		case 6:
			jScore = tagChange[5];
			break;
		default:
			break;
		}
		return jScore;
	}

	int	MainScene_sszp::getJettonType(SCORE jScore)
	{
		int jType = 0;
		if (tagChange[0] == jScore)
		{
			jType = 1;
		}
		else if (tagChange[1] == jScore)
		{
			jType = 2;
		}
		else if (tagChange[2] == jScore)
		{
			jType = 3;
		}
		else if (tagChange[3] == jScore)
		{
			jType = 4;
		}
		else if (tagChange[4] == jScore)
		{
			jType = 5;
		}
		else if (tagChange[5] == jScore)
		{
			jType = 6;
		}
		return jType;
	}

	void MainScene_sszp::clearRecordList()
	{
		record_list->removeAllChildren();
	}

	void MainScene_sszp::addRecordList(int id, bool isInit)
	{
		int index = 0;
		if (isInit)
		{
			switch (id)
			{
			case 1:
				index = 0;
				break;
			case 2:
				index = 5;
				break;
			case 3:
				index = 7;
				break;
			case 4:
				index = 6;
				break;
			case 5:
				index = 1;
				break;
			case 6:
				index = 7;
				break;
			case 7:
				index = 6;
				break;
			case 8:
				index = 4;
				break;
			case 9:
				index = 3;
				break;
			case 10:
				index = 6;
				break;
			case 11:
				index = 4;
				break;
			case 12:
				index = 5;
				break;
			case 13:
				index = 2;
				break;
			case 14:
				index = 4;
				break;
			case 15:
				index = 7;
				break;
			case 16:
				index = 5;
				break;
			}
		}
		else
		{
			index = id;
		}
		if (record_list->getItems().size() == 24)
		{
			record_list->removeItem(0);
		}
		auto pItem = Sprite::create(SETPATH(__String::createWithFormat("sszp/sszp_png/big_light_%d.png", index)->getCString()));
		pItem->setPosition(Vec2(4, 0));
		pItem->setAnchorPoint(Vec2::ZERO);

		auto itemBg = Sprite::create(SETPATH("sszp/sszp_png/record_item.png"));
		itemBg->setAnchorPoint(Vec2::ZERO);
		itemBg->addChild(pItem);

		auto crrent = Sprite::create(SETPATH("sszp/sszp_png/record_crrent.png"));
		crrent->setPosition(Vec2(2, 0));
		crrent->setName("crrent");
		crrent->setAnchorPoint(Vec2::ZERO);

		auto bg = Layout::create();
		bg->setTag(index);
		bg->setContentSize(crrent->getContentSize() + CCSize(2, 0));
		bg->setAnchorPoint(Vec2::ZERO);
		bg->addChild(itemBg, 2);
		bg->addChild(crrent, 1);

		if (record_list->getItems().size() > 0)
		{
			if (record_list->getItem(record_list->getItems().size() - 1)->getChildByName("crrent"))
			{
				record_list->getItem(record_list->getItems().size() - 1)->getChildByName("crrent")->removeFromParent();
			}
		}
		record_list->pushBackCustomItem(bg);
		record_list->setInnerContainerSize(record_list->getInnerContainerSize() + CCSize(bg->getContentSize().width, 0));
		record_list->scrollToRight(0.1f, true);
	}

	void MainScene_sszp::subChangeBanker(CMD_S_ChangeBanker& user)
	{
		PokerPlayerInfo * player = PokerData::instance()->getPlayerInfoBySeatId(user.wBankerUser);
		auto list_planeL = static_cast<Widget *>(BgPanel->getChildByName("list_planeL"));
		auto list_banker = static_cast<Text *>(list_planeL->getChildByName("list_banker"));
		auto list_score = static_cast<Text *>(list_planeL->getChildByName("list_score"));
		if (wCurrentBanker == UserInfo::instance()->getSeatId())
		{
			automaticJ_label->setVisible(false);
			jessonPanelBlack->setVisible(false);
			this->showUpeerBanker(false);
		}
		if (user.wBankerUser == INVALID_CHAIR)
		{
			PromptLayer::createObject(U2F("轮换庄家，系统做庄中").c_str());
			bankerUserL->setString(U2F("系统坐庄"));

			//庄家头像替换
			auto head_BankerFrame = static_cast<ImageView *>(BgPanel->getChildByName("head_BankerFrame"));
			head_BankerFrame->loadTexture("user_info/bighead_0.png");

			list_banker->setString(U2F("系统坐庄"));
			list_score->setString(__String::createWithFormat("%lld", user.lBankerScore)->getCString());
			bankerScoreL->setString(__String::createWithFormat("%lld", user.lBankerScore)->getCString());
			bankerTimeL->setString("0");
			wCurrentBanker = user.wBankerUser;
			if (wCurrentBankerState == CurrentBanker_sszp_Already)
			{
				wCurrentBankerState = CurrentBanker_sszp_None;
				this->changeBankerButton();
				banker_Btn->setTouchEnabled(true);
				automaticJ_label->setVisible(false);
				jessonPanelBlack->setVisible(false);
			}
			return;
		}
		else
		{
			bankerTimeL->setString("0");
			if (player)
			{
				PromptLayer::createObject(U2F(__String::createWithFormat("轮换庄家，%s做庄中", player->getNickName().c_str())->getCString()).c_str());
				bankerUserL->setString(U2F(player->getNickName().c_str()));
				list_banker->setString(U2F(player->getNickName().c_str()));

				//玩家头像替换
				auto head_BankerFrame = static_cast<ImageView *>(BgPanel->getChildByName("head_BankerFrame"));
				int faceId = player->getFaceId();
				string headUrl = StringUtils::format("user_info/bighead_%d.png", faceId);
				if (!FileUtils::getInstance()->isFileExist(headUrl))//如果faceId不在我们头像的区间，初始化faceId
				{
					//如果faceId不在我们头像的区间，在手机头像随机一个
					headUrl = StringUtils::format("user_info/bighead_0.png");
				}
				head_BankerFrame->loadTexture(headUrl);

				//重复下注面板隐藏
				upper_plane->setVisible(false);  
				wCurrentBanker = player->getSeatId();
			}
			list_score->setString(__String::createWithFormat("%lld", user.lBankerScore)->getCString());
			bankerScoreL->setString(__String::createWithFormat("%lld", user.lBankerScore)->getCString());
		}

		if (wCurrentBanker == UserInfo::instance()->getSeatId())
		{
			wCurrentBankerState = CurrentBanker_sszp_Already;
			this->changeBankerButton();
			banker_Btn->setTouchEnabled(true);
			btn_plane->setVisible(false);
			if (btn_plane->getNumberOfRunningActions() == 0)
			{
				btn_plane->runAction(MoveTo::create(0.1f, Vec2(btn_plane->getPositionX(), btn_plane->getPositionY())));
			}
			automaticJ_label->setVisible(true);
			jessonPanelBlack->setVisible(true);
			automaticJ_label->setString(__String::createWithFormat(U2F("您现在是庄家...(已坐庄%d轮)").c_str(), 0)->getCString());
			BankerCount = 0;
			if (isContinueJetton)
			{
				isContinueJetton = false;

				// 自动跟注框勾选全部取消掉
				automaticJCancel();
				automaticJ_btn->loadTextures(SETPATH("sszp/sszp/mainScene/automaticJ_btn_n.png"), SETPATH("sszp/sszp/mainScene/automaticJ_btn_s.png"));
			}

			repeatJ_btn->setTouchEnabled(false);
			repeatJ_btn->setColor(Color3B(133, 133, 133));
			automaticJ_btn->setTouchEnabled(false);
			automaticJ_btn->setColor(Color3B(133, 133, 133));

			return;
		}
		if (player)
		{
			for (int i = 0; i < banker_list->getItems().size(); i++)
			{
				if (banker_list->getItem(i)->getTag() == UserInfo::instance()->getSeatId())
				{
					wCurrentBankerState = CurrentBanker_sszp_Ready;
					this->changeBankerButton();
					banker_Btn->setTouchEnabled(true);
					return;
				}
			}
		}
		wCurrentBankerState = CurrentBanker_sszp_None;
		this->changeBankerButton();
		banker_Btn->setTouchEnabled(true);
	}

	//如果是自己上庄后的界面显示
	void MainScene_sszp::showUpeerBanker(bool isBanker)
	{
		repeatJ_btn->setTouchEnabled(!isBanker);
		automaticJ_btn->setTouchEnabled(!isBanker);

		if (isBanker)
		{
			repeatJ_btn->setColor(Color3B(133, 133, 133));
			automaticJ_btn->loadTextures(SETPATH("sszp/sszp/mainScene/automaticJ_btn_n.png"), SETPATH("sszp/sszp/mainScene/automaticJ_btn_s.png"));
			automaticJ_btn->setColor(Color3B(133, 133, 133));
		}
		else
		{
			repeatJ_btn->setColor(Color3B(255, 255, 255));
			automaticJ_btn->setColor(Color3B(255, 255, 255));
			automaticJ_btn->loadTextures(SETPATH("sszp/sszp/mainScene/automaticJ_btn_n.png"), SETPATH("sszp/sszp/mainScene/automaticJ_btn_s.png"));
			repeatJ_btn->loadTextures(SETPATH("sszp/sszp/mainScene/repeatJ_btn_n.png"), "");
		}
	}

	void MainScene_sszp::removeBanker(WORD id)
	{
		for (int i = 0; i < banker_list->getItems().size(); i++)
		{
			if (banker_list->getItem(i)->getTag() == id)
			{
				if (banker_list->getChildByTag(id))
				{
					banker_list->removeItem(i);
				}
				if (banker_list->getItems().size()>0)
				{
					banker_list->setInnerContainerSize(CCSize(0, banker_list->getItems().size() * banker_list->getItem(0)->getContentSize().height));
				}
			}
		}
		if (UserInfo::instance()->getSeatId() == id)
		{
			wCurrentBankerState = CurrentBanker_sszp_Already;
			this->changeBankerButton();
			banker_Btn->setTouchEnabled(true);
		}
	}

	void MainScene_sszp::removeBanker(string name)
	{
		for (int i = 0; i < banker_list->getItems().size(); i++)
		{
			if (!strcmp(banker_list->getItem(i)->getName().c_str(), name.c_str()))
			{
				if (banker_list->getChildByName(name))
				{
					banker_list->removeItem(i);
				}
				if (banker_list->getItems().size()>0)
				{
					banker_list->setInnerContainerSize(CCSize(0, banker_list->getItems().size() * banker_list->getItem(0)->getContentSize().height));
				}
			}
		}
		if (!strcmp(name.c_str(), UserInfo::instance()->getNickName().c_str()))
		{
			wCurrentBankerState = CurrentBanker_sszp_None;
			this->changeBankerButton();
			banker_Btn->setTouchEnabled(true);
		}
	}

	//取消掉所有状态
	void MainScene_sszp::hideContinueJetton()
	{
		if (!isContinueJetton)
		{
			return;
		}
		isContinueJetton = false;
		continue_number = 0;
		continueJetton_number = 0;

		// 自动跟注框勾选全部取消掉
		automaticJCancel();

		if (wCurrentTimeType == TimeType_sszp_PlaceJetton)
		{
			//开启筹码
			isJessonGray(true);

			//当前如果没有选择筹码，默认选择最小的一个
			if (wCurrentJettonScore == 0)
			{
				openSmallJesson();
			}
		}
		automaticJ_label->setVisible(false);
		jessonPanelBlack->setVisible(false);
		automaticJ_btn->setTouchEnabled(true);
	}

	void MainScene_sszp::changeBankerButtonState(CHANGE_BANKER_BUTTON m_eState) // 申请上庄按钮改变图片或颜色
	{
		switch (m_eState)
		{
		case CHANGE_BANKER_BUTTON_SSZP_NORMAL:
			banker_Btn->setColor(Color3B(255, 255, 255));
			banker_Btn->loadTextureNormal(SETPATH("sszp/sszp/banker/banker_Btn1.png")); //申请上庄 正常的按钮
			break;
		case CHANGE_BANKER_BUTTON_SSZP_NORMALGRAY:
			banker_Btn->setColor(Color3B(133, 133, 133));
			banker_Btn->loadTextureNormal(SETPATH("sszp/sszp/banker/banker_Btn1.png"));; //申请上庄 灰色的按钮
			break;
		case CHANGE_BANKER_BUTTON_SSZP_APPLYUPNORMAL:
			banker_Btn->loadTextureNormal(SETPATH("sszp/sszp_png/banker_Btn2.png")); //申请下庄 正常的按钮
			break;
		case CHANGE_BANKER_BUTTON_SSZP_MINEUPGRAY:
			banker_Btn->loadTextureNormal(SETPATH("sszp/sszp_png/banker_Btn2_s.png")); //我要下庄 灰色的按钮
			break;
		case CHANGE_BANKER_BUTTON_SSZP_CANCELNORMAL:
			banker_Btn->loadTextureNormal(SETPATH("sszp/sszp_png/banker_Btn3.png")); //取消申请 正常的按钮
			break;
		default:
			break;
		}
	}

	void MainScene_sszp::changeVoiceMp3(CHANGE_VOICE_MP3 m_eState) // 申请上庄按钮改变图片或颜色
	{
		switch (m_eState)
		{
		case CHANGE_VOICE_MP3_SSZP_PLEASE_BET:
			SoundManager::instance()->playEffect(SETPATH("sszp/sound/pleaseBet.mp3").c_str());
			break;
		case CHANGE_VOICE_MP3_SSZP_BET_END:
			SoundManager::instance()->playEffect(SETPATH("sszp/sound/BetEnd.mp3").c_str());
			break;

		default:
			break;
		}
	}

	//开始下注，空闲时间，游戏开始时间的三个倒计时显示图片
	void MainScene_sszp::changeLabelTime(CHANGE_LABEL_PIC m_eState) // 倒计时旁边的图片
	{
		label_bet->setVisible(true);
		switch (m_eState)
		{
		case CHANGE_LABEL_PIC_SSZP_BEGIN:
			label_bet->loadTexture(SETPATH("sszp/sszp/mainScene/lebel_Bet.png"));
			break;
		case CHANGE_LABEL_PIC_SSZP_START:
			label_bet->loadTexture(SETPATH("sszp/sszp/mainScene/lebel_Thelottery.png"));
			break;
		case CHANGE_LABEL_PIC_SSZP_LEISURE:
			label_bet->loadTexture(SETPATH("sszp/sszp/mainScene/lebel_free.png"));
			break;

		default:
			break;
		}
	}

	void MainScene_sszp::JessonUpOrDown(bool itemp_bIsUpOrDown, int itemp_wCurrentJettonScore) //筹码升起还是降落
	{
		if (itemp_bIsUpOrDown)
		{
			auto btnsv = (Button *)BgPanel->getChildByName(CurrentButtonName(itemp_wCurrentJettonScore));
			btnsv->setBrightStyle(BrightStyle::HIGHLIGHT);
			S_circleLight->setVisible(true);
			S_circleLight->setPosition(Vec2(btnsv->getPosition().x, jessonUpCount));
			btnsv->stopAllActions();
			btnsv->runAction(MoveTo::create(0.1f, Vec2(btnsv->getPosition().x, jessonUpCount)));
		}
		else
		{
			auto btns = (Button *)BgPanel->getChildByName(CurrentButtonName(itemp_wCurrentJettonScore));
			btns->stopAllActions();
			S_circleLight->setVisible(false);
			btns->runAction(MoveTo::create(0.1f, Vec2(btns->getPosition().x, jessonDownCount)));
		}
	}

	void MainScene_sszp::isJessonGray(bool m_bIsGray) //申请上庄按钮改变状态，是否是庄家
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_bIsGray)
			{
				if (std::atoll(usableL->getString().c_str()) >= getJettonNumber(i + 1))
				{
					B_CurMoney[i]->setTouchEnabled(true);
					B_CurMoney[i]->setBright(true);
					B_CurMoney[i]->setBrightStyle(BrightStyle::NORMAL);
					CurMoneyLabel[i]->setColor(Color3B(255, 255, 255));
					if (wCurrentJettonScore != 0 && wCurrentJettonScore == getJettonNumber(i + 1))
					{
						S_circleLight->setVisible(true);
						auto btnsv = (Button *)BgPanel->getChildByName(CurrentButtonName(getJettonNumber(i + 1)));
						btnsv->setBrightStyle(BrightStyle::HIGHLIGHT);
						btnsv->stopAllActions();
						btnsv->runAction(MoveTo::create(0.1f, Vec2(btnsv->getPosition().x, jessonUpCount)));
					}
				}
				else
				{
					B_CurMoney[i]->setTouchEnabled(false);
					B_CurMoney[i]->setBright(false);
					CurMoneyLabel[i]->setColor(Color3B(133, 133, 133));
				}
			}
			else
			{
				B_CurMoney[i]->setTouchEnabled(false);
				CurMoneyLabel[i]->setColor(Color3B(133, 133, 133));
				B_CurMoney[i]->setBright(false);
			}
		}
	}

	void MainScene_sszp::openSmallJesson() //打开最小的一个筹码
	{
		if (std::atoll(usableL->getString().c_str()) >= getJettonNumber(1))
		{
			wCurrentJettonScore = getJettonNumber(1);
			auto btns = (Button *)BgPanel->getChildByName(CurrentButtonName(wCurrentJettonScore));
			S_circleLight->setVisible(true);
			S_circleLight->setPosition(Vec2(btns->getPosition().x, jessonUpCount));
			btns->setBrightStyle(BrightStyle::HIGHLIGHT);
			btns->stopAllActions();
			btns->runAction(MoveTo::create(0.1f, Vec2(btns->getPosition().x, jessonUpCount)));
		}
	}

	void MainScene_sszp::setLabelType(int time, TimeType_sszp type, SCORE bankerScore)
	{
		createUpdate(time);
		wCurrentTimeType = type;
		bool isBanker = !strcmp(bankerUserL->getString().c_str(), UserInfo::instance()->getNickName().c_str());
		switch (type)
		{
		case TimeType_sszp_PlaceJetton:   //下注时间
		{
											  //声音--请下注
											  changeVoiceMp3(CHANGE_VOICE_MP3_SSZP_PLEASE_BET);

											  //切换顶上开始下注图片
											  changeLabelTime(CHANGE_LABEL_PIC_SSZP_BEGIN);

											  //庄家金币刷新,初始值为500万
											  if (!strcmp(bankerUserL->getString().c_str(), U2F("系统坐庄").c_str()))
											  {
												  bankerScoreL->setString(__String::createWithFormat("%lld", bankerScore)->getCString());
												  auto list_score = static_cast<Text *>(list_planeL->getChildByName("list_score"));
												  list_score->setString(bankerScoreL->getString());
											  }

											  //如果自己坐庄,如果当前是上庄按钮消失，改成我要下庄
											  if (isBanker)
											  {
												  automaticJ_label->setString(__String::createWithFormat(U2F("您现在是庄家...(已坐庄%d轮)").c_str(), ++BankerCount)->getCString());
												  banker_Btn->setTouchEnabled(false);
												  changeBankerButtonState(CHANGE_BANKER_BUTTON_SSZP_MINEUPGRAY);
												  isJessonGray(false);
											  }
											  else
											  {
												  //是否自动跟注，上庄图片隐藏掉,按钮变为我要上庄按钮
												  if (isContinueJetton)
												  {
													  if (continueJetton())//是的话，自动跟注功能，按钮置灰
													  {
														  isJessonGray(false);
														  return;
													  }
												  }
												  else
												  {
													  automaticJ_label->setVisible(false);
													  jessonPanelBlack->setVisible(false);
													  if (wCurrentBankerState != CurrentBanker_sszp_Ready)
													  {
														  //申请上庄原来状态
														  changeBankerButtonState(CHANGE_BANKER_BUTTON_SSZP_NORMAL);
													  }
												  }

												  //开启所有筹码
												  isJessonGray(true);

												  //默认下筹码选择最小的一个吧
												  if (wCurrentJettonScore == 0)
												  {
													  openSmallJesson();
												  }
											  }
		}
			break;
		case TimeType_sszp_Start:
		{
									//声音-停止下注
									changeVoiceMp3(CHANGE_VOICE_MP3_SSZP_BET_END);

									//切换顶上开始下注图片
									changeLabelTime(CHANGE_LABEL_PIC_SSZP_START);

									my_AllJettonResultScore = my_AllJettonScore;
									my_AllJettonScore = 0;
									//所有筹码变灰，点击功能取消
									isJessonGray(false);
									if (wCurrentJettonScore != 0 && !isContinueJetton)
									{
										JessonUpOrDown(false, wCurrentJettonScore);
									}
									for (int i = 0; i < 8; i++)
									{
										myJettonScore[i] = std::atoll(myJettonLabel[i]->getString().c_str());
										auto lScore = (Text *)upper_plane->getChildByName(__String::createWithFormat("my_upper_%d", i + 1)->getCString());
										lScore->setString(__String::createWithFormat("%lld", myJettonScore[i])->getCString());
									}
		}
			break;
		case TimeType_sszp_Leisure:
		{
									  //空闲图片切换
									  changeLabelTime(CHANGE_LABEL_PIC_SSZP_LEISURE);

									  //指针回归原位
									  point_Animal->runAction(
										  Spawn::create(
										  FadeIn::create(1.0f),
										  RotateTo::create(3.0f, 0),
										  NULL));

									  //筹码渐变消失掉
									  if (initNumber != 0)
									  {
										  myJettonLabelPanel[changeEndNumber(initNumber)]->setOpacity(0);
										  myJettonLabelPanel[changeEndNumber(initNumber)]->stopAllActions();
									  }

									  //玩家是否在申请上庄列表中
									  banker_Btn->setTouchEnabled(true);
									  if (wCurrentBankerState != CurrentBanker_sszp_Ready)
									  {
										  if (!isBanker) //不是庄家按钮恢复原样
										  {
											  //申请上庄原来状态
											  changeBankerButtonState(CHANGE_BANKER_BUTTON_SSZP_NORMAL);
										  }
										  else //庄家按钮变为
										  {
											  changeBankerButtonState(CHANGE_BANKER_BUTTON_SSZP_APPLYUPNORMAL);
										  }
									  }

									  //筹码变灰
									  isJessonGray(false);

									  //自己金币的筹码框隐藏掉
									  for (int itemp_initLightNumber = 0; itemp_initLightNumber < 8; itemp_initLightNumber++)
									  {
										  blockLight[itemp_initLightNumber]->setVisible(false);
									  }

									  //桌面上的筹码数额显示隐藏掉
									  int i = 0;
									  for (auto ttf : jettonLabelVec)
									  {
										  ttf->setString("0");
										  ttf->setVisible(false);
										  myJettonLabel[i]->setString("0");
										  itemp_myJettonLabel[i] = { 0 };
										  itemp_totalTtf[i] = { 0 };
										  myJettonLabel[i]->setVisible(false);
										  i++;
									  }

									  //散落到牌桌上的金币
									  auto childs = BgPanel->getChildren();
									  for (auto itor = childs.begin(); itor != childs.end(); itor++)
									  {
										  auto node = (*itor);
										  if (node && node->getName() == "jettonSprite1")
										  {
											  node->removeFromParentAndCleanup(true);
										  }
									  }

									  //开奖中黄色的圆圈
									  auto m_spriteYellowPanel = center_point_null_picture->getChildByName("m_sYellowPanel");
									  if (m_spriteYellowPanel)
									  {
										  m_spriteYellowPanel->removeFromParentAndCleanup(true); 
									  }

									  //开奖画面中间的动物
									  auto m_sendAnimal = BgPanel->getChildByName("m_sendAnimal");
									  if (m_sendAnimal)
									  {
										  m_sendAnimal->removeFromParentAndCleanup(true);
									  }

									  //结算界面隐藏掉
									  if (over_layout)				
									  {
										  over_layout->setVisible(false);
									  }
		}
			break;
		default:
			break;
		}
	}

	//申请上庄
	void MainScene_sszp::userApplyBanker(int seatId)
	{
		if (UserInfo::instance()->getSeatId() == seatId)
		{
			wCurrentBankerState = CurrentBanker_sszp_Ready;
			banker_Btn->loadTextureNormal(SETPATH("sszp/sszp_png/banker_Btn3.png"));
			banker_Btn->setTouchEnabled(true);
		}
		PokerPlayerInfo * player = PokerData::instance()->getPlayerInfoBySeatId(seatId);

		if (player)
		{
			for (int i = 0; i < banker_list->getItems().size(); i++)
			{
				if (!strcmp(banker_list->getItem(i)->getName().c_str(), player->getNickName().c_str()))
				{
					LabelTTF * ttf = (LabelTTF *)banker_list->getItem(i)->getChildByName("scoreTTf");
					if (ttf)
					{
						ttf->setString(__String::createWithFormat("%lld", player->getScore())->getCString());
					}
					return;
				}
			}

			auto banker = Sprite::create(SETPATH("sszp/sszp_png/playerList.png"));
			auto bg = Layout::create();
			bg->setContentSize(banker->getContentSize());
			bg->setTag(seatId);
			bg->setName(player->getNickName().c_str());

			banker->setAnchorPoint(Vec2::ZERO);
			bg->addChild(banker);
			int faceId = player->getFaceId();
			string headUrl = StringUtils::format("user_info/bighead_%d.png", faceId);
			bool m_bIsExit = FileUtils::getInstance()->isFileExist(headUrl);
			if (!m_bIsExit)
			{
				faceId = 0;
			}
			auto feath = Sprite::create(__String::createWithFormat("user_info/bighead_%d.png", faceId)->getCString());
			feath->setPosition(95, 11);
			feath->setScale(0.42f);
			feath->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			banker->addChild(feath);

			auto m_sItempFrame = Sprite::create(SETPATH("sszp/sszp/mainScene/bg_Theheadframe_1.png"));
			m_sItempFrame->setPosition(feath->getPosition() + Vec2(-7, -7));
			m_sItempFrame->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			m_sItempFrame->setScale(0.8f);
			banker->addChild(m_sItempFrame, 1);

			auto nameTTf = LabelTTF::create();
			nameTTf->setString(player->getNickName().c_str());
			nameTTf->setPosition(Vec2(165, 58));
			nameTTf->setAnchorPoint(Vec2(0.0f, 0.5f));
			nameTTf->setColor(Color3B(243, 81, 0));
			nameTTf->setFontSize(20);
			bg->addChild(nameTTf);

			auto scoreTTf = LabelTTF::create();
			scoreTTf->setString(__String::createWithFormat("%lld", player->getScore())->getCString());
			scoreTTf->setName("scoreTTf");
			scoreTTf->setPosition(Vec2(190, 28));
			scoreTTf->setColor(Color3B(243, 81, 0));
			scoreTTf->setAnchorPoint(Vec2(0.0f, 0.5f));
			scoreTTf->setFontSize(20);
			bg->addChild(scoreTTf);

			auto gold = Sprite::create(SETPATH("sszp/sszp_png/gold.png"));
			gold->setScale(0.4f);
			gold->setPosition(Vec2(-15, 0) + scoreTTf->getPosition());
			banker->addChild(gold);

			if (UserInfo::instance()->getSeatId() == seatId)
			{
				scoreTTf->setColor(Color3B(243, 208, 119));
				nameTTf->setColor(Color3B(243, 208, 119));
			}
			banker_list->pushBackCustomItem(bg);
			banker_list->setInnerContainerSize(CCSize(0, banker_list->getItems().size() * banker->getContentSize().height));
		}
	}

	//复投消息处理,各个区域进行遍历处理
	void MainScene_sszp::userRepeatPlaceJetton(bool istrue, CMD_S_RepeatBet& pQueryStorage)
	{
		//--临时存储八个区域的值--jetton_repeatNumberVec
		LONGLONG jetton_repeatNumberVec[8];
		for (int j = 1; j <= AREA_COUNT; j++)
		{
			jetton_repeatNumberVec[j - 1] = pQueryStorage.lJettonScore[j];
			for (int area = 5; area >= 0; area--)
			{
				//遍历下注数额大于的筹码，减去该筹码的n倍数
				if (jetton_repeatNumberVec[j - 1] >= tagChange[area])
				{
					int repeatTimes = jetton_repeatNumberVec[j - 1] / tagChange[area];
					//发送repeatTimes倍数遍历的筹码
					for (int repreatJessonNumber = repeatTimes; repreatJessonNumber > 0; repreatJessonNumber--)
					{
						updateShowScore(istrue, j, tagChange[area]);
					}
					jetton_repeatNumberVec[j - 1] = jetton_repeatNumberVec[j - 1] - tagChange[area] * repeatTimes;
				}
			}
		}
	}

	//重复下注
	void MainScene_sszp::repaetButtonNoifyFial()
	{
		if (isContinueJetton)
		{
			PromptLayer::createObject(U2F("自动跟注中，无法下注").c_str());
			return;
		}
		if (wCurrentTimeType == TimeType_sszp_PlaceJetton)
		{
			int number = 0;
			for (int i = 0; i < 8; i++)
			{
				number += myJettonScore[i];
			}
			if (number == 0)
			{
				PromptLayer::createObject(U2F("上局无押注，无法重复下注").c_str());
				return;
			}

			if (std::atoll(usableL->getString().c_str()) < tagChange[0] || std::atoll(usableL->getString().c_str()) < number)
			{
				PromptLayer::createObject(U2F("金币不足无法下注").c_str());
				return;
			}
			//继续加注逻辑
			continueJetton();
		}
		else
		{
			PromptLayer::createObject(U2F("请稍候，未到下注时间").c_str());
		}
	}

	//赔率
	int MainScene_sszp::getCurrentRate(int type)
	{
		int rateNumber = 0;
		switch (type)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			rateNumber = 19;
			break;
		default:
			rateNumber = 3;
			break;
		}
		return rateNumber;
	}

	//继续加注
	bool MainScene_sszp::continueJetton()
	{
		for (int i = 1; i < 9; i++)
		{	
			//自己的下注金额+当前总注-当前下注类型的总金额 > 庄家总金币
			if ((myJettonScore[i - 1] * getCurrentRate(i) + my_AllJettonScore - std::atoll(jettonLabelVec.at(i - 1)->getString().c_str()) >std::atoll(bankerScoreL->getString().c_str()))
				&& strcmp(bankerUserL->getString().c_str(), U2F("系统坐庄").c_str()))
			{
				automaticJ_label->setVisible(false);
				PromptLayer::createObject(U2F("已达庄家赔付上限，无法下注").c_str());
				automaticJ_btn->loadTextures(SETPATH("sszp/sszp/mainScene/automaticJ_btn_n.png"), SETPATH("sszp/sszp/mainScene/automaticJ_btn_s.png"));
				if (isContinueJetton)
				{
					isContinueJetton = false;

					// 自动跟注框勾选全部取消掉
					automaticJCancel();
				}
				continueJetton_number = 0;
				return false;
			}
		}

		//判断重复下注金币是否充足
		int itemp_Score = 0;
		for (int i = 0; i < 8; i++)
		{
			itemp_Score += myJettonScore[i];
		}
		if (itemp_Score>std::atoll(usableL->getString().c_str()))
		{
			isContinueJetton = false;
			automaticJ_label->setVisible(false);
			jessonPanelBlack->setVisible(false);
			automaticJ_btn->loadTextures(SETPATH("sszp/sszp/mainScene/automaticJ_btn_n.png"), SETPATH("sszp/sszp/mainScene/automaticJ_btn_s.png"));

			// 自动跟注框勾选全部取消掉
			automaticJCancel();
			PromptLayer::createObject(U2F("金币不足无法下注").c_str());
			return false;
		}
		CMD_C_PlaceJettons repeatPlaceJetton;
		memset(&repeatPlaceJetton, 0, sizeof(CMD_C_PlaceJettons));

		for (int i = 1; i < AREA_COUNT + 1; i++)
		{
			repeatPlaceJetton.lJettonScore[i] = myJettonScore[i - 1];
		}
		NetworkManager::getInstance()->sendData(NetworkManager::FLAG_GAME, MDM_GF_GAME, SUB_C_REPEAT_JETTON, &repeatPlaceJetton, sizeof(CMD_C_PlaceJettons));//发送复投命令

		if (isContinueJetton)
		{
			if (continueJetton_number != ButtonType_sszp_automaticJ_Infinite)//如果是无限跟注，不递减次数,递增跟注次数
			{
				continueJetton_number--;
				if (continueJetton_number == 0)//如果跟注次数为0，停止自动跟注
				{
					btn_plane->setVisible(false);//自动跟注板隐藏
					isContinueJetton = false;
					automaticJ_label->setVisible(false);
					jessonPanelBlack->setVisible(false);
					//自动跟注框勾选全部取消掉
					automaticJCancel();
					automaticJ_btn->loadTextures(SETPATH("sszp/sszp/mainScene/automaticJ_btn_n.png"), SETPATH("sszp/sszp/mainScene/automaticJ_btn_s.png"));
					return false;
				}
				automaticJ_label->setString(__String::createWithFormat(U2F("自动跟注中...(剩余%d轮)").c_str(), continueJetton_number)->getCString());
			}
			else
			{
				continue_number += 1;
				automaticJ_label->setString(__String::createWithFormat(U2F("自动跟注中...(已进行%d轮)").c_str(), continue_number)->getCString());
			}
		}
		return true;
	}

	//返回第几个筹码
	int	MainScene_sszp::getJettonCurType(SCORE jScore)
	{
		int jType = 0;
		if (tagChange[0] == jScore)
		{
			jType = 1;
		}
		else if (tagChange[1] == jScore)
		{
			jType = 2;
		}
		else if (tagChange[2] == jScore)
		{
			jType = 3;
		}
		else if (tagChange[3] == jScore)
		{
			jType = 4;
		}
		else if (tagChange[4] == jScore)
		{
			jType = 5;
		}
		else if (tagChange[5] == jScore)
		{
			jType = 6;
		}
		return jType;
	}

	//根据筹码的数额返回筹码的名字
	string MainScene_sszp::CurrentButtonName(int currentScore)
	{
		return B_CurMoney[getJettonCurType(currentScore) - 1]->getName();
	}

	//自动下注的显示
	void MainScene_sszp::showContinueJetton()
	{
		if (continueJetton_number != ButtonType_sszp_automaticJ_Infinite)
		{
			//可以跟注
			automaticJ_label->setString(__String::createWithFormat(U2F("自动跟注中...(剩余%d轮)").c_str(), continueJetton_number)->getCString());
		}
		else
		{
			automaticJ_label->setString(U2F("自动跟注中...(已进行0轮)"));
			continue_number = 0;
		}
		isContinueJetton = true;
		jessonPanelBlack->setVisible(true);
		automaticJ_label->setVisible(true);
		//将所有下注按钮至灰，不许点
		isJessonGray(false);
		if (wCurrentTimeType == TimeType_sszp_PlaceJetton && wCurrentJettonScore != 0)
		{
			JessonUpOrDown(false, wCurrentJettonScore);
		}
	}

	//在非下注时间，点击自动跟注多少轮，会进行判断是否满足自动跟注条件
	bool MainScene_sszp::checkAutoMatic()
	{
		int number = 0;
		if (wCurrentTimeType == TimeType_sszp_PlaceJetton)//如果不是下注时间自动跟注，那么比较上一轮的押注数目
		{
			for (int i = 0; i < 8; i++)
			{
				number += std::atoll(myJettonLabel[i]->getString().c_str());
			}
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{
				number += myJettonScore[i];
			}
		}

		if (number == 0)
		{
			PromptLayer::createObject(U2F("当前无押注，无法自动跟注").c_str());
			return false;
		}

		if (std::atoll(usableL->getString().c_str()) < 1000 || std::atoll(usableL->getString().c_str()) < number)
		{
			PromptLayer::createObject(U2F("金币不足无法下注").c_str());
			return false;
		}

		return true;
	}

	//设置缩进
	void MainScene_sszp::setMenuState(bool orisMenu)
	{
		auto menuPanel = static_cast<Widget *>(BgPanel->getChildByName("menuPanel"));
		auto menu_btn = static_cast<Button *>(BgPanel->getChildByName("menu_btn"));
		if (orisMenu)
		{
			isMenu = false;
			menu_btn->runAction(MoveTo::create(0.2f, menu_btn->getPosition() - Vec2(0, menuPanel->getContentSize().height)));
			menuPanel->runAction(MoveBy::create(0.2f, Vec2(0, 340)));
		}
		else
		{
			isMenu = true;
			menu_btn->runAction(MoveTo::create(0.2f, menu_btn->getPosition() + Vec2(0, menuPanel->getContentSize().height)));
			menuPanel->runAction(MoveBy::create(0.2f, Vec2(0, -340)));
		}
	}

	//庄家面板缩进
	void MainScene_sszp::setListState(bool b_itempBanerMove)
	{
		auto list_planeL = static_cast<Widget *>(BgPanel->getChildByName("list_planeL"));
		Button* btn = static_cast<Button *>(list_planeL->getChildByName("banker_move"));
		ImageView* img = static_cast<ImageView *>(btn->getChildByName("gray_img"));
		if (b_itempBanerMove)
		{
			isBanerMove = false;
			list_planeL->runAction(MoveTo::create(0.2f, list_planeL->getPosition() + Vec2(-(list_planeL->getContentSize().width + 51 - btn->getContentSize().width), 0)));
		}
		else
		{
			isBanerMove = true;
			list_planeL->runAction(MoveTo::create(0.2f, list_planeL->getPosition() + Vec2(list_planeL->getContentSize().width + 51 - btn->getContentSize().width, 0)));
		}
	}

	//结算界面
	void MainScene_sszp::overLayoutFunc(int type, CMD_S_GameEnd& endData)
	{
		//路单更新
		addRecordList(initNumber, true);

		//交叉闪烁的光效
		over_layout->setVisible(true);
		auto light1 = static_cast<ImageView *>(over_layout->getChildByName("imgo_light_1"));
		light1->runAction(RepeatForever::create(RotateBy::create(1, 60)));
		auto light2 = static_cast<ImageView *>(over_layout->getChildByName("imgo_light_2"));
		light2->runAction(RepeatForever::create(RotateBy::create(1, -60)));

		//结算面板上庄家金币
		auto banker_score = static_cast<TextBMFont *>(Helper::seekWidgetByName(over_layout, "banker_score"));
		if (endData.lBankerScore > 10000 || endData.lBankerScore < -10000)
		{
			banker_score->setString(__String::createWithFormat("%.1f%s", double(endData.lBankerScore) / 10000, U2F("万").c_str())->getCString());
		}
		else
		{
			banker_score->setString(__String::createWithFormat("%lld", endData.lBankerScore)->getCString());
		}
		//结算面板上玩家金币
		auto my_score = static_cast<TextBMFont *>(Helper::seekWidgetByName(over_layout, "my_score"));
		if (endData.lUserScore > 10000 || endData.lUserScore < -10000)
		{
			my_score->setString(__String::createWithFormat("%.1f%s", double(endData.lUserScore) / 10000, U2F("万").c_str())->getCString());
		}
		else
		{
			my_score->setString(__String::createWithFormat("%lld", endData.lUserScore)->getCString());
		}

		//刷新庄家金币
		bankerScoreL->setString(__String::createWithFormat("%lld", std::atoll(bankerScoreL->getString().c_str()) + endData.lBankerScore)->getCString());

		//庄家成绩
		bankerTimeL->setString(__String::createWithFormat("%lld", endData.lBankerTotallScore)->getCString());

		//申请上庄上的庄家金币
		auto list_score = static_cast<Text *>(list_planeL->getChildByName("list_score"));
		list_score->setString(__String::createWithFormat("%lld", std::atoll(bankerScoreL->getString().c_str()))->getCString());

		//玩家金币
		bool itemp_bBanker = !strcmp(bankerUserL->getString().c_str(), UserInfo::instance()->getNickName().c_str());
		if (itemp_bBanker)
		{
			usableL->setString(bankerScoreL->getString());
		}
		else
		{
			usableL->setString(__String::createWithFormat("%lld", std::atoll(usableL->getString().c_str()) + wCurrentGameEnd.lUserScore)->getCString());
		}

		//玩家成绩
		scoreL->setString(__String::createWithFormat("%lld", std::atoll(scoreL->getString().c_str()) + (wCurrentGameEnd.lUserScore - my_AllJettonResultScore))->getCString());

		///---特效
		for (int i = 0; i < 3; i++)
		{
			auto star = static_cast<ImageView *>(Helper::seekWidgetByName(over_layout, StringUtils::format("imgo_star_%d", i + 1)));
			Sequence *seq = Sequence::createWithTwoActions(FadeOut::create(0.7), FadeIn::create(0.9));
			star->runAction(RepeatForever::create(seq));
		}

		//---结算框金币飞出
		if (endData.lUserScore > 0)
		{
			const Vec2 posBegin = Vec2(560, 300);
			const Vec2 posMid = Vec2(530, 200);
			const Vec2 posEnd = Vec2(147, 25);
			const int nodeCount = 10;
			const float timeGradually[nodeCount] = { 0.0, 0.2, 0.35, 0.45, 0.55, 0.65, 0.75, 0.83, 0.9, 0.95 };

			for (int i = 0; i < nodeCount; i++)
			{
				//曲线运动
				//0~1/3s变宽，变扁
				//1/3s~1s还原
				Sprite *spritegold = Sprite::create(SETPATH("sszp/sszp/mainScene/bg_Shangzhuang_1.png"));
				spritegold->setOpacity(0);
				spritegold->setPosition(posBegin);
				over_layout->addChild(spritegold, 18);

				ccBezierConfig bzConfig;
				bzConfig.controlPoint_1 = posBegin;
				bzConfig.controlPoint_2 = posMid;
				bzConfig.endPosition = posEnd;
				BezierTo *bezier = BezierTo::create(0.8, bzConfig);
				Spawn *spawn = Spawn::createWithTwoActions(FadeIn::create(0.3), ScaleTo::create(0.3, 1.1, 0.6));
				Sequence *seq = Sequence::create(spawn, ScaleTo::create(0.5, 1.0, 1.0), NULL);
				Spawn *spawnall = Spawn::createWithTwoActions(bezier, seq);
				Sequence *seqall = Sequence::create(DelayTime::create(timeGradually[i]), spawnall, RemoveSelf::create(true), NULL);
				spritegold->runAction(seqall);
			}
		}
	}

	void MainScene_sszp::onBackPressed()
	{

	}
	void MainScene_sszp::autoExit()
	{
		UserInfo::instance()->setCurrentServerID(0);
		MainController::getInstance()->setGameState(false);
		PokerMessageController::instance()->onQuitGame();
	}
}
