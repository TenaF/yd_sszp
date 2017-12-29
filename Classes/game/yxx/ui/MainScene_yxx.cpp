#include "MainScene_yxx.h"
#include "game/yxx/data/PokerData_yxx.h"
#include "game/yxx/message/PokerMessageController_yxx.h"
#include "game/yxx/message/SendGameMsg_yxx.h"
#include "game/yxx/ui/widget/helpLayer_yxx.h"
#include "game/yxx/ui/widget/OutGameLayer_yxx.h"
#include "game/yxx/data/SoundManager_yxx.h"
#include "hall/manager/SoundManager_hall.h"

namespace yxx
{
	MainScene_yxx::MainScene_yxx()
	{
		m_LaterCount = 1;
		isSound = false;
		isSound_music = false;
		isAllowBank = true;
		isWelcomeSound = true;
		StartMovesound = 0;
		is_trueCheckbox = true;
		isContinueJetton = false;
		armatureBird = nullptr;
		BankerCount = 0;
		lUserSingleLimitScore = 0;
		wCurrentJettonScore = 0;
		my_AllJettonScore = 0;
		lAreaLimitScore = 0;
		lAllLimitScore = 0;
		socreRecord = 0;
		memset(init_ResultNumber, 0, sizeof(init_ResultNumber));
		continue_number = 0;
		usableL = nullptr;
		scoreL = nullptr;
		isBanerMove = false;
		isLudanMove = false;
		isMenu = false;
		isPlayerList = true;
		timeLabelTop = nullptr;
		nameL = nullptr;
		over_layout = nullptr;
		jettonSprite = nullptr;
		wCurrentBankerState = CurrentBanker_yxx_None;
		memset(&wCurrentGameEnd, 0, sizeof(CMD_S_GameEnd));
		memset(&itemp_myJettonLabel, 0, sizeof(itemp_myJettonLabel));
		memset(&itemp_totalTtf, 0, sizeof(itemp_totalTtf));
		memset(&tagChange, 0, sizeof(tagChange));
		memset(&jettonProbability, 0, sizeof(jettonProbability));
	}


	MainScene_yxx::~MainScene_yxx()
	{
	}
	void MainScene_yxx::onEnter()
	{
		SceneBase::onEnter();
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainScene_yxx::closeSocket_noifyFial), GAME_DISCONNECT, nullptr);
		SoundManager::instance()->playBgMusic(SETPATH("yxx/sound/BetBk.mp3").c_str());
	}
	void MainScene_yxx::onExit()
	{
		jettonLabelVec.clear();
		this->unscheduleUpdate();
		NotificationCenter::getInstance()->removeObserver(this, GAME_DISCONNECT);
		SceneBase::onExit();
		SoundManager::instance()->stopBgMusic();
	}
	void MainScene_yxx::closeSocket_noifyFial(Object *obj)
	{
		PromptLayer::createObject(U2F("与服务器断开连接，3秒后自动退出游戏").c_str());
		NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_GAME);

		this->runAction(Sequence::create(DelayTime::create(3), CallFuncN::create([=](Node *node){
			UserInfo::instance()->setCurrentServerID(0);
			MainController::getInstance()->setGameState(false);
			PokerMessageController::instance()->onQuitGame();
		}), NULL));
	}

	bool MainScene_yxx::init()
	{
		if (!Layer::init())
		{
			return false;
		}
		pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(SETPATH("yxx/yxx_1.json").c_str());
		pRoot->addTouchEventListener(this, toucheventselector(MainScene_yxx::onTouch));
		//CCSize visibleSize = Director::getInstance()->getVisibleSize();
		pRoot->setAnchorPoint(CCPoint(0.0f, 0.0f));
		pRoot->setPosition(Vec2(0, 0));
		this->addChild(pRoot);

		over_layout = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(SETPATH("yxx/yxx_over.json").c_str());
		over_layout->addTouchEventListener(this, toucheventselector(MainScene_yxx::onTouch));
		over_layout->setVisible(false);
		pRoot->addChild(over_layout, 100);

		BgPanel = static_cast<ImageView *>(pRoot->getChildByName("bgPanel"));
		//顶上倒计时时间
		timeLabelTop = dynamic_cast<Text *>(Helper::seekWidgetByName(pRoot, "timeLabelTop"));
		//倒计时时间旁边的标签
		timeLable = dynamic_cast<Text *>(Helper::seekWidgetByName(pRoot, "timeLable"));

		//旋转的圆圈
		S_circleLight = Sprite::create(SETPATH("yxx/Image/brc_quan.png").c_str());
		S_circleLight->setVisible(false);
		BgPanel->addChild(S_circleLight, 9);
		S_circleLight->runAction(RepeatForever::create(RotateBy::create(2.0f, 360)));

		//所有按钮至灰,并且开启点击状态
		initLight();
		//筹码
		B_CurMoney[0] = static_cast<Button *>(BgPanel->getChildByName("cur_1"));
		B_CurMoney[0]->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onJessonTouch, this));
		B_CurMoney[1] = static_cast<Button *>(BgPanel->getChildByName("cur_2"));
		B_CurMoney[1]->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onJessonTouch, this));
		B_CurMoney[2] = static_cast<Button *>(BgPanel->getChildByName("cur_3"));
		B_CurMoney[2]->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onJessonTouch, this));
		B_CurMoney[3] = static_cast<Button *>(BgPanel->getChildByName("cur_4"));
		B_CurMoney[3]->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onJessonTouch, this));
		B_CurMoney[4] = static_cast<Button *>(BgPanel->getChildByName("cur_5"));
		B_CurMoney[4]->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onJessonTouch, this));
		B_CurMoney[5] = static_cast<Button *>(BgPanel->getChildByName("cur_6"));
		B_CurMoney[5]->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onJessonTouch, this));

		for (int i = 0; i < 6; i++)
		{
			CurMoneyLabel[i] = static_cast<TextBMFont *>(B_CurMoney[i]->getChildByName("cur1_Text"));
		}
		// 当前庄家，玩家金币、成绩、分数
		nameL = static_cast<Text *>(BgPanel->getChildByName("nameL"));
		nameL->setString(UserInfo::instance()->getNickName().c_str());
		auto head_frame = static_cast<ImageView *>(BgPanel->getChildByName("head_frame"));

		ClippingNode* clippingNode = ClippingNode::create();
		clippingNode->setContentSize(head_frame->getContentSize());
		clippingNode->setPosition(head_frame->getPosition());
		BgPanel->addChild(clippingNode, head_frame->getLocalZOrder());
		Sprite* sencil = Sprite::create(SETPATH("yxx/Res/head.png"));
		clippingNode->setStencil(sencil);
		clippingNode->addChild(sencil);
		//--_playerHeadSprite真实头像可以换的
		string headUrl = StringUtils::format("user_info/bighead_%d.png", UserInfo::instance()->getFaceId());
		Sprite* _playerHeadSprite = nullptr;
		if (!FileUtils::getInstance()->isFileExist(headUrl))//如果faceId不在我们头像的区间，初始化faceId
		{
			//如果faceId不在我们头像的区间，在手机头像随机一个
			_playerHeadSprite = Sprite::create("user_info/bighead_0.png");
		}
		else
		{
			_playerHeadSprite = Sprite::create(headUrl);
		}
		_playerHeadSprite->setScale(0.6);
		clippingNode->setAlphaThreshold(0);
		clippingNode->addChild(_playerHeadSprite);

		usableL = static_cast<Text *>(BgPanel->getChildByName("usableL"));
		usableL->setString(__String::createWithFormat("%lld", UserInfo::instance()->getScore())->getCString());
		scoreL = static_cast<Text *>(BgPanel->getChildByName("scoreL"));
		scoreL->setString("0");
		//庄家信息
		bankerUserL = static_cast<Text *>(BgPanel->getChildByName("bankerUserL"));
		bankerScoreL = static_cast<Text *>(BgPanel->getChildByName("bankerScoreL"));
		bankerScoreL->setString("0");
		bankerTimeL = static_cast<Text *>(BgPanel->getChildByName("bankerTimeL"));

		//动画
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(SETPATH("yxx/animation/Yxx0.plist"), SETPATH("yxx/animation/Yxx0.png"));
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(SETPATH("yxx/animation/Yxx1.plist"), SETPATH("yxx/animation/Yxx1.png"));
		ArmatureDataManager::getInstance()->addArmatureFileInfo(
			SETPATH("yxx/animation/Yxx0.png"),
			SETPATH("yxx/animation/Yxx0.plist"),
			SETPATH("yxx/animation/Yxx.ExportJson"));
		//摇筛子动画
		armatureBird = Armature::create("Yxx");
		armatureBird->setVisible(false);
		armatureBird->setPosition(ccp(684, 380));
		BgPanel->addChild(armatureBird, 20);

		//复选框勾 是否选择筹码 鱼虾蟹不显示
		auto Image_77 = static_cast<CheckBox *>(BgPanel->getChildByName("Image_77"));
		Image_77->setVisible(false);

		checkBox = static_cast<CheckBox *>(BgPanel->getChildByName("CheckBox_75"));
		checkBox->setVisible(false);
		checkBox->addEventListenerCheckBox(this, checkboxselectedeventselector(MainScene_yxx::selectedEvent));
		//---------在线人数，申请上庄---------------------------------
		list_planeL = static_cast<ImageView *>(BgPanel->getChildByName("list_planeL"));
		list_planeL->setVisible(false);
		list_planeL->setZOrder(15);
		list_planeL->setTag(ButtonType_yxx_listplantL_move);
		list_planeL->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onTouch, this));

		banker_list = static_cast<ListView *>(list_planeL->getChildByName("banker_list"));
		banker_list->setVisible(true);
		ohter_list = static_cast<ListView *>(list_planeL->getChildByName("ohter_list"));
		ohter_list->setVisible(false);
		//我要上庄按钮
		banker_Btn = static_cast<Button *>(list_planeL->getChildByName("banker_btn"));
		banker_Btn->setTag(ButtonType_yxx_banker);
		banker_Btn->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onTouch, this));
	
		//侧面申请上庄按钮
		auto banker_move = static_cast<Button *>(list_planeL->getChildByName("banker_move"));
		banker_move->setTag(ButtonType_yxx_banker_move);
		banker_move->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onTouch, this));
		//---------------左侧结束----------------------
		//路单-----------------------------------------
		ludan_total = static_cast<ImageView *>(BgPanel->getChildByName("ludan_total"));
		ludan_total->setZOrder(15);
		ludan_total->setTag(ButtonType_yxx_ludan_Frame_move);
		ludan_total->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onTouch, this));

		record_btn = static_cast<Button *>(ludan_total->getChildByName("record_btn"));
		record_btn->setTag(ButtonType_yxx_ludan_move);
		record_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onTouch, this));

		record_list = static_cast<ListView *>(ludan_total->getChildByName("record_list"));
		//--------------------------路单结束
		//剩余跟注显示
		automaticJ_label = static_cast<TextBMFont *>(BgPanel->getChildByName("automaticJ_label"));
		automaticJ_label->setVisible(false);
		jessonPanelBlack = static_cast<ImageView *>(BgPanel->getChildByName("jessonPanelBlack"));
		jessonPanelBlack->setVisible(false);
		//自动跟注按钮
		automaticJ_btn = static_cast<Button *>(BgPanel->getChildByName("automaticJ_btn"));
		automaticJ_btn->setTag(ButtonType_yxx_automaticJ);
		automaticJ_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onTouch, this));
		//跟注次数
		btn_plane = static_cast<Widget *>(BgPanel->getChildByName("btn_plane"));  //自动下注按钮移动plane
		btn_plane->setVisible(false);

		auto btn_10 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_10"));
		btn_10->setTag(ButtonType_yxx_automaticJ_10);
		btn_10->addEventListenerCheckBox(this, checkboxselectedeventselector(MainScene_yxx::selectedEventautomatic));

		auto btn_20 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_20"));
		btn_20->setTag(ButtonType_yxx_automaticJ_20);
		btn_20->addEventListenerCheckBox(this, checkboxselectedeventselector(MainScene_yxx::selectedEventautomatic));

		auto btn_50 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_50"));
		btn_50->setTag(ButtonType_yxx_automaticJ_Infinite);
		btn_50->addEventListenerCheckBox(this, checkboxselectedeventselector(MainScene_yxx::selectedEventautomatic));
		//重复下注
		repeatJ_btn = static_cast<Button *>(BgPanel->getChildByName("repeatJ_btn"));//重复下注按钮
		repeatJ_btn->setTag(ButtonType_yxx_repeatJ);
		repeatJ_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onTouch, this));

		//设置帮助退出按钮
		auto menuPanel = static_cast<Widget *>(BgPanel->getChildByName("menuPanel"));
		menuPanel->setZOrder(20);
		//menuPanel->setPosition(menuPanel->getPosition() - Vec2(menuPanel->getContentSize().width + 20, 0));

		auto menu_btn = static_cast<Button *>(BgPanel->getChildByName("menu_btn"));
		menu_btn->setTag(ButtonType_yxx_menu);
		menu_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onTouch, this));

		auto setting_btn = static_cast<Button *>(menuPanel->getChildByName("setting_btn"));//设置音效按钮
		setting_btn->setTag(ButtonType_yxx_setting);
		setting_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onTouch, this));

		auto setting_music = static_cast<Button *>(menuPanel->getChildByName("setting_btn_0"));//设置音乐按钮
		setting_music->setTag(ButtonType_yxx_settingMusic);
		setting_music->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onTouch, this));

		//根据原来的音效设置声音
		isSound_music = HallSoundManager::instance()->getBgMusicConfig();
		if (isSound_music)
		{
			SoundManager::instance()->playBgMusic(SETPATH("yxx/sound/BetBk.mp3").c_str());
			setting_music->loadTextures(SETPATH("yxx/menu/menu_music.png"), SETPATH("yxx/menu/menu_music_2.png"));
		}
		else
		{
			SoundManager::instance()->stopBgMusic();
			setting_music->loadTextures(SETPATH("yxx/menu/menu_music3.png"), SETPATH("yxx/menu/menu_music3_2.png"));
		}
		//---------------------
		isSound = HallSoundManager::instance()->getEffectConfig();
		if (isSound)
		{
			setting_btn->loadTextures(SETPATH("yxx/menu/btn_set.png"), SETPATH("yxx/menu/btn_set_2.png"));
		}
		else
		{
			setting_btn->loadTextures(SETPATH("yxx/menu/menu_sound3.png"), SETPATH("yxx/menu/menu_sound3_2.png"));
		}
		auto help_btn = static_cast<Button *>(menuPanel->getChildByName("help_btn"));//帮助按钮
		help_btn->setTag(ButtonType_yxx_help);
		help_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onTouch, this));

		auto out_btn = static_cast<Button *>(menuPanel->getChildByName("out_btn"));//退出按钮
		out_btn->setTag(ButtonType_yxx_out);
		out_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onTouch, this));

		auto Image_27_0 = static_cast<ImageView *>(menuPanel->getChildByName("Image_27_0"));//退出按钮蒙版
		Image_27_0->setVisible(false);
		auto online_number = static_cast<Text *>(BgPanel->getChildByName("online_number"));
		online_number->setString("0");
		return true;
	}
	void MainScene_yxx::closeLight(float dt)
	{
		for (int itemp_initLightNumber = 100; itemp_initLightNumber < 122; itemp_initLightNumber++)
		{
			auto itempImage = (ImageView *)BgPanel->getChildByTag(itemp_initLightNumber);
			itempImage->setOpacity(0);
		}
	}

	void MainScene_yxx::selectedEvent(cocos2d::Ref *pSender, CheckBoxEventType type)
	{
		switch (type) {
		case cocos2d::ui::CHECKBOX_STATE_EVENT_SELECTED:
			is_trueCheckbox = false;
			if (jettonSprite)
			{
				auto childs = BgPanel->getChildren();
				Vector<Node*>::iterator itor;
				for (auto itor = childs.begin(); itor != childs.end(); itor++)
				{
					auto node = (*itor);
					if (node && node->getName() == "jettonSprite"){
						node->setVisible(false);
					}
				}
			}
			break;

		case cocos2d::ui::CHECKBOX_STATE_EVENT_UNSELECTED:
			is_trueCheckbox = true;
			if (jettonSprite)
			{
				auto childs = BgPanel->getChildren();
				Vector<Node*>::iterator itor;
				for (auto itor = childs.begin(); itor != childs.end(); itor++)
				{
					auto node = (*itor);
					if (node && node->getName() == "jettonSprite"){
						node->setVisible(true);
					}
				}
			}
			break;
		default:
			break;
		}
	}
//自动跟注回调事件
	void MainScene_yxx::selectedEventautomatic(cocos2d::Ref *pSender, CheckBoxEventType type)
	{
		CheckBox * itep_check = dynamic_cast<CheckBox *>(pSender);
		continueJetton_number = itep_check->getTag();
		auto btn_10 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_10"));
		auto btn_20 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_20"));
		auto btn_50 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_50"));
		switch (type)
		{
		case cocos2d::ui::CHECKBOX_STATE_EVENT_SELECTED:
			if (checkAutoMatic())
			{
				showContinueJetton();
				switch (itep_check->getTag())
				{
				case ButtonType_yxx_automaticJ_10:
					btn_20->setSelectedState(false);
					btn_50->setSelectedState(false);
					break;
				case ButtonType_yxx_automaticJ_20:
					btn_10->setSelectedState(false);
					btn_50->setSelectedState(false);
					break;
				case ButtonType_yxx_automaticJ_Infinite:
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
				case ButtonType_yxx_automaticJ_10:
					btn_10->setSelectedState(true);
					break;
				case ButtonType_yxx_automaticJ_20:
					btn_20->setSelectedState(true);
					break;
				case ButtonType_yxx_automaticJ_Infinite:
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
	void MainScene_yxx::closeBlock()
	{
		for (int itemp_initLightNumber = 0; itemp_initLightNumber < 22; itemp_initLightNumber++)
		{
			blockLight[itemp_initLightNumber]->setVisible(false);
		}
	}
	void MainScene_yxx::initLight()
	{
		const BlendFunc blendFunc = { GL_ONE, GL_ONE };//去黑色发亮处理
		jettonLabelVec.reserve(22);
		S_circleLight->setBlendFunc(blendFunc); //旋转圆圈加高亮
		//金币加亮
		auto itemp_gold_light = static_cast<ImageView *>(BgPanel->getChildByName("gold_light"));
		auto itemp_gold_light_0 = static_cast<ImageView *>(BgPanel->getChildByName("gold_light_0"));
		Sprite* itemp_GoldLight = (Sprite*)itemp_gold_light->getVirtualRenderer();
		Sprite* itemp_GoldLight_0 = (Sprite*)itemp_gold_light_0->getVirtualRenderer();
		itemp_GoldLight->setBlendFunc(blendFunc); //旋转圆圈加高亮
		itemp_GoldLight_0->setBlendFunc(blendFunc); //旋转圆圈加高亮
		for (int itemp_initLightNumber = 100; itemp_initLightNumber < 122; itemp_initLightNumber++)
		{
			auto itempImage = (ImageView *)BgPanel->getChildByTag(itemp_initLightNumber);
			Sprite* GoldLight = (Sprite*)itempImage->getVirtualRenderer();
			GoldLight->setBlendFunc(blendFunc);
			itempImage->setTouchEnabled(true);
			itempImage->setTag(itemp_initLightNumber);
			itempImage->addTouchEventListener(CC_CALLBACK_2(MainScene_yxx::onTouch, this));
			itempImage->setOpacity(0);
			//下注后的标签
			blockLight[itemp_initLightNumber - 100] = ImageView::create(SETPATH("yxx/Image/score_back.png"));
			blockLight[itemp_initLightNumber - 100]->setPosition(itempImage->getPosition());
			blockLight[itemp_initLightNumber - 100]->setVisible(false);
			BgPanel->addChild(blockLight[itemp_initLightNumber - 100], 5);
			//添加容器以及里面的内容
			TextBMFont * ttf = TextBMFont::create();
			ttf->setFntFile(SETPATH("yxx/fnt/jetton_gold.fnt"));
			ttf->setVisible(false);
			ttf->setPosition(itempImage->getPosition());
			ttf->setScale(0.7);
			ttf->setString("0");
			BgPanel->addChild(ttf, 6);
			jettonLabelVec.pushBack(ttf);

			myJettonLabel[itemp_initLightNumber - 100] = TextBMFont::create();
			myJettonLabel[itemp_initLightNumber - 100]->setFntFile(SETPATH("yxx/fnt/myJetton_gold.fnt"));
			myJettonLabel[itemp_initLightNumber - 100]->setVisible(false);
			myJettonLabel[itemp_initLightNumber - 100]->setPosition(itempImage->getPosition());
			myJettonLabel[itemp_initLightNumber - 100]->setScale(0.7);
			myJettonScore[itemp_initLightNumber - 100] = 0;
			BgPanel->addChild(myJettonLabel[itemp_initLightNumber - 100], 6);
			if (itemp_initLightNumber == 100)
			{
				blockLight[0]->setPosition(itempImage->getPosition() + Vec2(0, 15));
				ttf->setPosition(itempImage->getPosition() + Vec2(0, 15));
				myJettonLabel[0]->setPosition(itempImage->getPosition() + Vec2(0, 15));
			}
		}
	}

	MainScene_yxx * MainScene_yxx::instance = nullptr;
	MainScene_yxx * MainScene_yxx::createScene()
	{
		if (!instance)
		{
			instance = MainScene_yxx::create();
		}
		return instance;
	}
	
	//开始转动转盘
	void MainScene_yxx::startMoveBlock(CMD_S_GameEnd& gameEnd)
	{
		StartMovesound = 0;
		wCurrentGameEnd = gameEnd;
		init_ResultNumber[0] = gameEnd.cbDiceValue[0];
		init_ResultNumber[1] = gameEnd.cbDiceValue[1];
		init_ResultNumber[2] = gameEnd.cbDiceValue[2];
		schedule(schedule_selector(MainScene_yxx::startDelayMove), 0.0f, 0, 1.0f);
	}
	void MainScene_yxx::startDelayMove(float dt)
	{
		//筛子声音
		SoundManager::instance()->playEffect(SETPATH("yxx/sound/shake_dice.mp3").c_str());
		armatureBird->setVisible(true);
		armatureBird->getAnimation()->play("Yxx_dicecup");

		//放置筛子
		int index[3] = { 0, 0, 0 };
		cocostudio::Skin *skin[3] = { 0, 0, 0 };
		
		for (int itemp_shaizi = 0; itemp_shaizi < 3; itemp_shaizi++)
		{
			skin[itemp_shaizi] = Skin::create(SETPATH(__String::createWithFormat("yxx/shaizi/shaizi_%d_1.png", init_ResultNumber[itemp_shaizi])->getCString()).c_str());
			skin[itemp_shaizi]->setName("itemp_shaizi");
			armatureBird->getBone(__String::createWithFormat("Yxx_dice%d", itemp_shaizi + 1)->getCString())->addDisplay(skin[itemp_shaizi], 0);
			armatureBird->getBone(__String::createWithFormat("Yxx_dice%d", itemp_shaizi + 1)->getCString())->changeDisplayWithIndex(0, true);
		}

		//筛子声音
		schedule(schedule_selector(MainScene_yxx::soundsStartMove), 1.0f, 2, 5.0f);

		//开奖区域
		schedule(schedule_selector(MainScene_yxx::moveEndPanel), 0.0f, 0, 6.0f);

		//结算界面
		auto childs = BgPanel->getChildren();
		this->runAction(
			Sequence::create(
			DelayTime::create(9.5f),
			CallFunc::create([=]{
			overLayoutFunc(wCurrentGameEnd);
		}),
			DelayTime::create(6.0),
			CallFunc::create([=]{
			over_layout->setVisible(false);

		}),
			DelayTime::create(1.0),
			CallFunc::create([=]{
			for (auto itor = childs.begin(); itor != childs.end(); itor++)
			{
				auto node = (*itor);
				if (node && node->getName() == "jettonSprite"){
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
	//随机的角度
	Vec2 MainScene_yxx::randAngel(int jettonArea)
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
	//报声音
	void MainScene_yxx::soundsStartMove(float dt)
	{
		SoundManager::instance()->playEffect(__String::createWithFormat(SETPATH("yxx/sound/number_%d.mp3").c_str(), init_ResultNumber[StartMovesound])->getCString());//这里添加声音
		StartMovesound++;
	}


	void MainScene_yxx::moveEndPanel(float dt)
	{
		//下注区域的渐隐渐现
		Sequence *seq = Sequence::createWithTwoActions(FadeOut::create(0.7), FadeIn::create(1.0));
		Sequence *seq1 = Sequence::createWithTwoActions(FadeOut::create(0.7), FadeIn::create(1.0));
		Sequence *seq2 = Sequence::createWithTwoActions(FadeOut::create(0.7), FadeIn::create(1.0));
		Sequence *seq3 = Sequence::createWithTwoActions(FadeOut::create(0.7), FadeIn::create(1.0));
		Sequence *seq4 = Sequence::createWithTwoActions(FadeOut::create(0.7), FadeIn::create(1.0));
		Sequence *seq5 = Sequence::createWithTwoActions(FadeOut::create(0.7), FadeIn::create(1.0));
		Sequence *seq6 = Sequence::createWithTwoActions(FadeOut::create(0.7), FadeIn::create(1.0));
		
		//判断非空
		if (init_ResultNumber[0] == 0)
		{
			return;
		}
		if (init_ResultNumber[0] != init_ResultNumber[1] && init_ResultNumber[0] != init_ResultNumber[2] && init_ResultNumber[1] != init_ResultNumber[2])
		{
			//各个点数分别显示
			auto down_X = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d", init_ResultNumber[0])->getCString()));
			auto down_Y = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d", init_ResultNumber[1])->getCString()));
			auto down_Z = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d", init_ResultNumber[2])->getCString()));

			down_X->setOpacity(80);
			down_Y->setOpacity(80);
			down_Z->setOpacity(80);

			down_X->runAction(RepeatForever::create(seq));
			down_Y->runAction(RepeatForever::create(seq1));
			down_Z->runAction(RepeatForever::create(seq2));

			//倒数第二个分别显示
			ImageView *down_up_First = nullptr;
			if (init_ResultNumber[0] > init_ResultNumber[1])
			{
				down_up_First = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d_%d", init_ResultNumber[1], init_ResultNumber[0])->getCString()));
			}
			else
			{
				down_up_First = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d_%d", init_ResultNumber[0], init_ResultNumber[1])->getCString()));
			}
			down_up_First->setOpacity(80);
			down_up_First->runAction(RepeatForever::create(seq3));
			ImageView *down_up_Second = nullptr;
			if (init_ResultNumber[0] > init_ResultNumber[2])
			{
				down_up_Second = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d_%d", init_ResultNumber[2], init_ResultNumber[0])->getCString()));
			}
			else
			{
				down_up_Second = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d_%d", init_ResultNumber[0], init_ResultNumber[2])->getCString()));
			}
			down_up_Second->setOpacity(80);
			down_up_Second->runAction(RepeatForever::create(seq4));
			ImageView *down_up_Third = nullptr;
			if (init_ResultNumber[1] > init_ResultNumber[2])
			{
				down_up_Third = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d_%d", init_ResultNumber[2], init_ResultNumber[1])->getCString()));
			}
			else
			{
				down_up_Third = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d_%d", init_ResultNumber[1], init_ResultNumber[2])->getCString()));
			}
			down_up_Third->setOpacity(80);
			down_up_Third->runAction(RepeatForever::create(seq5));
		}
		else if (init_ResultNumber[0] != init_ResultNumber[1])
		{
			//各个点数分别显示
			auto down_X = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d", init_ResultNumber[0])->getCString()));
			auto down_Y = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d", init_ResultNumber[1])->getCString()));
			down_X->setOpacity(80);
			down_Y->setOpacity(80);
			down_X->runAction(RepeatForever::create(seq));
			down_Y->runAction(RepeatForever::create(seq2));

			//倒数第二个内容，其中两个组合分别显示
			ImageView *down_up_First = nullptr;
			if (init_ResultNumber[0] > init_ResultNumber[1])
			{
				down_up_First = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d_%d", init_ResultNumber[1], init_ResultNumber[0])->getCString()));
			}
			else
			{
				down_up_First = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d_%d", init_ResultNumber[0], init_ResultNumber[1])->getCString()));
			}
			down_up_First->setOpacity(80);
			down_up_First->runAction(RepeatForever::create(seq3));
		}
		else if (init_ResultNumber[0] != init_ResultNumber[2])
		{
			//各个点数分别显示
			auto down_X = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d", init_ResultNumber[0])->getCString()));
			auto down_Z = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d", init_ResultNumber[2])->getCString()));
			down_X->setOpacity(80);
			down_Z->setOpacity(80);
			down_X->runAction(RepeatForever::create(seq));
			down_Z->runAction(RepeatForever::create(seq1));
			
			//倒数第二个内容，其中两个组合分别显示
			ImageView *down_up_First = nullptr;
			if (init_ResultNumber[0] > init_ResultNumber[2])
			{
				down_up_First = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d_%d", init_ResultNumber[2], init_ResultNumber[0])->getCString()));
			}
			else
			{
				down_up_First = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d_%d", init_ResultNumber[0], init_ResultNumber[2])->getCString()));
			}
			down_up_First->setOpacity(80);
			down_up_First->runAction(RepeatForever::create(seq2));
		}
		else if (init_ResultNumber[1] != init_ResultNumber[2])
		{
			//各个点数分别显示
			auto down_Y = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d", init_ResultNumber[1])->getCString()));
			auto down_Z = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d", init_ResultNumber[2])->getCString()));
			down_Y->setOpacity(80);
			down_Z->setOpacity(80);
			down_Y->runAction(RepeatForever::create(seq));
			down_Z->runAction(RepeatForever::create(seq1));
			
			//倒数第二个内容，其中两个组合分别显示
			ImageView *down_up_First = nullptr;
			if (init_ResultNumber[1] > init_ResultNumber[2])
			{
				down_up_First = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d_%d", init_ResultNumber[2], init_ResultNumber[1])->getCString()));
			}
			else
			{
				down_up_First = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d_%d", init_ResultNumber[1], init_ResultNumber[2])->getCString()));
			}
			down_up_First->setOpacity(80);
			down_up_First->runAction(RepeatForever::create(seq2));
		}
		else
		{
			//显示最底下的豹子
			auto down_Y = static_cast<ImageView *>(BgPanel->getChildByName(__String::createWithFormat("btn_%d", init_ResultNumber[1])->getCString()));
			down_Y->setOpacity(80);
			down_Y->runAction(RepeatForever::create(seq));
			auto down_X = static_cast<ImageView *>(BgPanel->getChildByName("btn_0"));
			down_X->setOpacity(80);
			down_X->runAction(RepeatForever::create(seq1));
		}
	}

	void MainScene_yxx::onTouch(Ref* target, Widget::TouchEventType type)
	{
		Button *btn = (Button *)target;
		int itemp_tag = btn->getTag();
		if (Widget::TouchEventType::BEGAN == type)
		{
			if (isBanerMove == true)
			{
				if (itemp_tag != ButtonType_yxx_listplantL_move&&itemp_tag != ButtonType_yxx_banker &&itemp_tag != ButtonType_yxx_b  &&itemp_tag != ButtonType_yxx_banker_move&&itemp_tag != ButtonType_yxx_ludan_Frame_move&&itemp_tag != ButtonType_yxx_ludan_move)
				{
					setListState(isBanerMove);
				}
			}
			if (isLudanMove == true)
			{
				if (itemp_tag != ButtonType_yxx_listplantL_move&&itemp_tag != ButtonType_yxx_banker &&itemp_tag != ButtonType_yxx_b  &&itemp_tag != ButtonType_yxx_banker_move&&itemp_tag != ButtonType_yxx_ludan_Frame_move&&itemp_tag != ButtonType_yxx_ludan_move)
				{
					setludanState(isLudanMove);
				}
			}

			if (itemp_tag == ButtonType_yxx_out || itemp_tag == ButtonType_yxx_help || itemp_tag == ButtonType_yxx_setting || itemp_tag == ButtonType_yxx_settingMusic)
			{
				auto menuPanel = static_cast<Widget *>(BgPanel->getChildByName("menuPanel"));
				auto Image_27_0 = static_cast<ImageView *>(menuPanel->getChildByName("Image_27_0"));//退出按钮蒙版
				auto m_itempButton = (ImageView *)menuPanel->getChildByTag(itemp_tag);
				Image_27_0->setPosition(m_itempButton->getPosition() + Vec2(-2, 0));
				Image_27_0->setVisible(true);
			}
		}
		else if (Widget::TouchEventType::MOVED == type)
		{
		
				auto menuPanel = (Widget *)BgPanel->getChildByName("menuPanel");
				Vec2 m_vPointTouch = btn->getTouchMovePos();
				Vec2 m_vPointUse = menuPanel->convertToNodeSpace(m_vPointTouch);
				bool m_bIsTouchBudding = btn->getBoundingBox().containsPoint(m_vPointUse);
				if (!m_bIsTouchBudding)
				{
					menuButtonMask(false); //menu上蒙版的状态
				}
			
		}
		else if (Widget::TouchEventType::CANCELED == type)
		{

			if (itemp_tag == ButtonType_yxx_out || itemp_tag == ButtonType_yxx_help || itemp_tag == ButtonType_yxx_setting || itemp_tag == ButtonType_yxx_settingMusic)
			{
				menuButtonMask(false); //menu上蒙版的状态
			}
		}
		else if (Widget::TouchEventType::ENDED == type)
		{
			if (isMenu)
			{
				//声音，退出，帮助
				setMenuState(isMenu);
			}
			switch (btn->getTag())
			{
			case ButtonType_yxx_menu:
			{
										setMenuState(isMenu);
			}
				break;
			case ButtonType_yxx_banker_move:
			{
											   setListState(isBanerMove);
			}
				break;
			case ButtonType_yxx_ludan_move:
			{
											  setludanState(isLudanMove);
			}
				break;
			case ButtonType_yxx_banker:
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
										  case CurrentBanker_yxx_Already://当前已经是庄，我要下装
										  {
																			 SendGameMsg::sendApplyBanker(false);
										  }
											  break;
										  case CurrentBanker_yxx_Ready://当前是准备上庄，申请上庄
										  {
																		   SendGameMsg::sendApplyBanker(false);
										  }
											  break;
										  case CurrentBanker_yxx_None://没有状态，取消上庄
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

			case ButtonType_yxx_repeatJ:
			{
										   repaetButtonNoifyFial();
			}
				break;
			case ButtonType_yxx_continue:
			{
											over_layout->setVisible(false);
			}
				break;
			case ButtonType_yxx_setting:
			{
										   menuButtonMask(false); //menu上蒙版的状态
										   isSound = HallSoundManager::instance()->getEffectConfig();
										   HallSoundManager::instance()->setEffectConfig(!isSound);
										   auto menuPanel = static_cast<Widget *>(BgPanel->getChildByName("menuPanel"));
										   auto setting_btn = static_cast<Button *>(menuPanel->getChildByName("setting_btn"));
										   if (isSound)
										   {
											   setting_btn->loadTextures(SETPATH("yxx/menu/menu_sound3.png"), SETPATH("yxx/menu/menu_sound3_2.png"));
										   }
										   else
										   {
											   setting_btn->loadTextures(SETPATH("yxx/menu/btn_set.png"), SETPATH("yxx/menu/btn_set_2.png"));
										   }
			}
				break;
			case ButtonType_yxx_settingMusic:
			{
												menuButtonMask(false); //menu上蒙版的状态
												isSound_music = HallSoundManager::instance()->getBgMusicConfig();
												HallSoundManager::instance()->setBgMusicConfig(!isSound_music);
												auto menuPanel = static_cast<Widget *>(BgPanel->getChildByName("menuPanel"));
												auto setting_music = static_cast<Button *>(menuPanel->getChildByName("setting_btn_0"));
												if (isSound_music)
												{
													SoundManager::instance()->stopBgMusic();
													setting_music->loadTextures(SETPATH("yxx/menu/menu_music3.png"), SETPATH("yxx/menu/menu_music3_2.png"));
												}
												else
												{
													SoundManager::instance()->playBgMusic(SETPATH("yxx/sound/BetBk.mp3").c_str());
													setting_music->loadTextures(SETPATH("yxx/menu/menu_music.png"), SETPATH("yxx/menu/menu_music_2.png"));
												}
			}
				break;
			case ButtonType_yxx_help:
			{
										menuButtonMask(false); //menu上蒙版的状态
										helpLayer::createObject();
			}
				break;
			case ButtonType_yxx_out:
			{
									   menuButtonMask(false); //menu上蒙版的状态
									   bool istrue = false;
									   if (wCurrentTimeType == TimeType_yxx_PlaceJetton && my_AllJettonScore != 0)
									   {
										   istrue = true;
									   }
									   if (!strcmp(bankerUserL->getString().c_str(), UserInfo::instance()->getNickName().c_str()))
									   {
										   istrue = true;
									   }
									   if (istrue)
									   {
										   OutGameLayer::createObject(istrue);
									   }
									   else
									   {
										   SendMsg::sendStandUpReq(NetworkManager::FLAG_GAME, UserInfo::instance()->getTableId(), UserInfo::instance()->getSeatId(), 1, 0);
									   }
			}
				break;
			case ButtonType_yxx_automaticJ:
			{
											  bool itemp_isLastJesson = checkAutoMatic();
											  if (!itemp_isLastJesson)
											  {
												  return;
											  }

											  //取消掉所有状态
											  hideContinueJetton(false);

											  float h = 0.0f;
											  if (btn_plane->isVisible())
											  {
												  automaticJ_btn->loadTextures(SETPATH("yxx/Res/automaticJ_btn_n.png"), "");
												  h = -btn_plane->getContentSize().height + 195;
											  }
											  else
											  {
												  automaticJ_btn->loadTextures(SETPATH("yxx/Res/c_automaticJ_btn_n.png"), "");
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
			case 100: case 101: case 102: case 103:case 104:case 105:case 106:case 107:case 108:case 109:
			case 110: case 111: case 112: case 113:case 114:case 115:case 116:case 117:case 118:case 119:
			case 120: case 121: case 122: case 123:case 124:case 125:case 126:case 127:case 128:case 129:
			case 130: case 131: case 132: case 133:case 134:case 135:case 136:case 137:case 138:case 139:
			case 140: case 141: case 142: case 143:case 144:case 145:case 146:case 147:case 148:case 149:
			case 150: case 151:
			{
						  JettonArea(btn);

			}
			default:
				break;
			}
		}
	}

	//设置menu上的蒙版改变状态
	void MainScene_yxx::menuButtonMask(bool m_isTrue) 
	{
		auto menuPanel = static_cast<Widget *>(BgPanel->getChildByName("menuPanel"));
		auto Image_27_0 = static_cast<ImageView *>(menuPanel->getChildByName("Image_27_0"));//退出按钮蒙版
		Image_27_0->setVisible(m_isTrue);
	}

	void MainScene_yxx::JettonArea(Button *btn)
	{
		if (!strcmp(bankerUserL->getString().c_str(), UserInfo::instance()->getNickName().c_str()))
		{
			PromptLayer::createObject(U2F("做庄中无法下注").c_str());
			return;
		}
		if (wCurrentTimeType != TimeType_yxx_PlaceJetton)
		{
			//PromptLayer::createObject(U2F("当前时间，停止下注").c_str());
			return;
		}
		if (isContinueJetton)
		{
			PromptLayer::createObject(U2F("自动跟注中,无法下注").c_str());
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
		if (itemp_myJettonLabel[btn->getTag() - 100] + wCurrentJettonScore > lUserSingleLimitScore)
		{
			PromptLayer::createObject(U2F("已达区域个人下注上限，请选择其他区域下注").c_str());
			return;
		}
		if (itemp_totalTtf[btn->getTag() - 100] + wCurrentJettonScore > lAreaLimitScore)
		{
			PromptLayer::createObject(U2F("该区域下注已满，请选择其他区域下注").c_str());
			return;
		}
		int jettonType = 0;
		jettonType = btn->getTag() - 100;
		if (true)
		{
			SendGameMsg::sendPlace_Jetton(NetworkManager::FLAG_GAME, jettonType, wCurrentJettonScore);
		}
		else
		{
			PromptLayer::createObject(U2F("已达庄家赔付上限，无法下注").c_str());
		}
	}

	//押注限制目前这里没用到
	bool MainScene_yxx::PlaceJettonLimit(int type)
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

	void MainScene_yxx::clearPlayer(PokerPlayerInfo * player)
	{
		PokerPlayerInfo* playerInfo = player;

		for (int i = 0; i < ohter_list->getItems().size(); i++)
		{
			if (!strcmp(ohter_list->getItem(i)->getName().c_str(), playerInfo->getNickName().c_str()))
			{
				ohter_list->removeItem(i);
			}
		}

		//刷新在线人数
		int itemp_onlineNumber = ohter_list->getItems().size();
		auto online_number = static_cast<Text *>(BgPanel->getChildByName("online_number"));
		online_number->setString(__String::createWithFormat("%d", itemp_onlineNumber)->getCString());
	}
	//设置筹码上的金额
	void MainScene_yxx::setJessonValue(const LONGLONG itemp_chip[])
	{
		//筹码保存值
		for (int i = 0; i < 6; i++)
		{
			tagChange[i] = itemp_chip[i];
			if (itemp_chip[i] < 10000)
			{
				CurMoneyLabel[i]->setString(__String::createWithFormat("%lld", itemp_chip[i])->getCString());
			}
			else if (itemp_chip[i] >= 10000)
			{
				CurMoneyLabel[i]->setString(__String::createWithFormat("%lld%s", (itemp_chip[i]) / 10000, U2F("万").c_str())->getCString());
				if (itemp_chip[i] >= 10000000)
				{
					Label *itemp_CurMoneyLabel = dynamic_cast<Label *>(CurMoneyLabel[i]->getVirtualRenderer());
					itemp_CurMoneyLabel->setAdditionalKerning(-4);
				}
			}
		}
	}
	void MainScene_yxx::initGameScene(CMD_S_StatusFree *obj)
	{
		if (isWelcomeSound == true)
		{
			SoundManager::instance()->playEffect(SETPATH("yxx/sound/welcome.mp3").c_str());
			isWelcomeSound = false;
		}
		auto list_planeL = static_cast<Widget *>(BgPanel->getChildByName("list_planeL"));
		auto list_banker = static_cast<Text *>(list_planeL->getChildByName("list_banker"));
		auto list_score = static_cast<Text *>(list_planeL->getChildByName("list_score"));
		PokerPlayerInfo * player = PokerData::instance()->getPlayerInfoBySeatId(obj->wBankerUser);

		//设置筹码的金额
		LONGLONG itemp_chip[6] = { 0 };
		memcpy(&itemp_chip, obj->nChip, sizeof(itemp_chip));
		setJessonValue(itemp_chip);

		if (obj->wBankerUser == INVALID_CHAIR)
		{
			list_banker->setString(U2F("系统坐庄"));
			list_score->setString(__String::createWithFormat("%lld", obj->lBankerScore)->getCString());
			bankerUserL->setString(U2F("系统坐庄"));
			bankerScoreL->setString(__String::createWithFormat("%lld", obj->lBankerScore)->getCString());
			wCurrentBanker = INVALID_CHAIR;
		}
		else
		{
			if (player)
			{
				list_banker->setString(U2F(player->getNickName().c_str()));
				bankerUserL->setString(U2F(player->getNickName().c_str()));
				wCurrentBanker = player->getSeatId();
			}
			list_score->setString(__String::createWithFormat("%lld", obj->lBankerScore)->getCString());
			bankerScoreL->setString(__String::createWithFormat("%lld", obj->lBankerScore)->getCString());
		}
		bankerTimeL->setString(__String::createWithFormat("%lld", obj->lBankerWinScore)->getCString());
		lApplyBankerCondition = obj->lApplyBankerCondition;

		auto list_bankerScore = static_cast<Text *>(list_planeL->getChildByName("list_bankerScore"));
		list_bankerScore->setString(__String::createWithFormat("%s%lld", U2F("上庄需要 ").c_str(), lApplyBankerCondition)->getCString());

		lUserSingleLimitScore = obj->lUserAlearLimitScore;//个人区域下注
		lAllLimitScore = obj->lUserMaxScore;  //玩家总下注
		lAreaLimitScore = obj->lAreaLimitScore;
		isAllowBank = obj->bAllowBank;

		//上庄界面是否显示出来
		if (!isAllowBank)
		{
			list_planeL->setVisible(false);
		}
		else
		{
			list_planeL->setVisible(true);
		}
		
		if (wCurrentBanker == UserInfo::instance()->getSeatId())
		{
			wCurrentBankerState = CurrentBanker_yxx_Already;
			this->changeBankerButton();
			this->showUpeerBanker(true);
		}
	}

	void MainScene_yxx::initGameScene(CMD_S_StatusPlay *obj)
	{
		if (isWelcomeSound == true)
		{
			if (obj->cbGameStatus == 101)
			{
				SoundManager::instance()->playEffect(SETPATH("yxx/sound/welcome.mp3").c_str());
			}
			else
			{
				SoundManager::instance()->playEffect(SETPATH("yxx/sound/welcome.mp3").c_str());

			}
			isWelcomeSound = false;
		}
		PokerPlayerInfo * player = PokerData::instance()->getPlayerInfoBySeatId(obj->wBankerUser);
		auto list_planeL = static_cast<Widget *>(BgPanel->getChildByName("list_planeL"));
		auto list_banker = static_cast<Text *>(list_planeL->getChildByName("list_banker"));
		auto list_score = static_cast<Text *>(list_planeL->getChildByName("list_score"));
		//筹码保存值
		LONGLONG itemp_chip[6] = { 0 };
		memcpy(&itemp_chip, obj->nChip, sizeof(itemp_chip));
		setJessonValue(itemp_chip);

		lApplyBankerCondition = obj->lApplyBankerCondition;	//上庄需要的金币
		lAllLimitScore = obj->lUserMaxScore;				//玩家总下注
		lAreaLimitScore = obj->lAreaLimitScore;
		lUserSingleLimitScore = obj->lUserAlearLimitScore;	//个人区域下注
		//是否准许上庄
		isAllowBank = obj->bAllowBank;
		if (!isAllowBank)
		{
			list_planeL->setVisible(false);
		}
		else
		{
			list_planeL->setVisible(true);
		}
		//庄家处理
		if (obj->wBankerUser == INVALID_CHAIR)
		{
			list_banker->setString(U2F("系统坐庄"));
			list_score->setString(__String::createWithFormat("%lld", obj->lBankerScore)->getCString());
			bankerUserL->setString(U2F("系统坐庄"));
			bankerScoreL->setString(__String::createWithFormat("%lld", obj->lBankerScore)->getCString());
			wCurrentBanker = INVALID_CHAIR;
		}
		else
		{
			if (player)
			{
				list_banker->setString(U2F(player->getNickName().c_str()));
				bankerUserL->setString(U2F(player->getNickName().c_str()));
				wCurrentBanker = player->getSeatId();
			}
			list_score->setString(__String::createWithFormat("%lld", obj->lBankerScore)->getCString());
			bankerScoreL->setString(__String::createWithFormat("%lld", obj->lBankerScore)->getCString());
		}
		bankerTimeL->setString(__String::createWithFormat("%lld", obj->lBankerWinScore)->getCString());
		
		auto list_bankerScore = static_cast<Text *>(list_planeL->getChildByName("list_bankerScore"));
		list_bankerScore->setString(__String::createWithFormat("%s%lld", U2F("上庄需要 ").c_str(), lApplyBankerCondition)->getCString());

		//如果自己是庄家
		if (wCurrentBanker == UserInfo::instance()->getSeatId())
		{
			wCurrentBankerState = CurrentBanker_yxx_Already;
			banker_Btn->setTouchEnabled(false);
			banker_Btn->loadTextureNormal(SETPATH("yxx/Image/banker_Btn2_s.png").c_str());
			this->showUpeerBanker(true);
			automaticJ_label->setString(__String::createWithFormat(U2F("您现在是庄家...(已坐庄%d轮)").c_str(), obj->cbBankerTime + 1)->getCString());
			BankerCount = obj->cbBankerTime;
			jessonPanelBlack->setVisible(true);
			automaticJ_label->setVisible(true);
			for (int i = 0; i < 6; i++)
			{
				B_CurMoney[i]->setTouchEnabled(false);
				CurMoneyLabel[i]->setColor(Color3B(133, 133, 133));
			}
		}

		//重复下注
		CMD_S_RepeatBet itemp_jesson_init;
		memset(&itemp_jesson_init, 0, sizeof(CMD_S_RepeatBet));
		memcpy(&itemp_jesson_init.lJettonScore, obj->lAllJettonScore, sizeof(obj->lAllJettonScore));

		CMD_S_RepeatBet itemp_jesson_my_init;
		memset(&itemp_jesson_my_init, 0, sizeof(CMD_S_RepeatBet));
		memcpy(&itemp_jesson_my_init.lJettonScore, obj->lUserJettonScore, sizeof(obj->lUserJettonScore));

		//记录
		for (int i = 0; i < 22; i++)
		{
			myJettonScore[i] = itemp_jesson_my_init.lJettonScore[i];
		}
		//其他人下注
		for (int i = 0; i < AREA_COUNT; i++)
		{
			itemp_jesson_init.lJettonScore[i] = itemp_jesson_init.lJettonScore[i] - itemp_jesson_my_init.lJettonScore[i];
		}

		//显示筛子
		if (obj->cbGameStatus != 101)
		{
			auto itemp_shaizi_1 = Sprite::create(SETPATH(__String::createWithFormat("yxx/shaizi/shaizi_%d_1.png", obj->cbDiceValue[0])->getCString()).c_str());
			itemp_shaizi_1->setName("itemp_shaizi");
			itemp_shaizi_1->setPosition(1095, 724);
			BgPanel->addChild(itemp_shaizi_1, 4);

			auto itemp_shaizi_2 = Sprite::create(SETPATH(__String::createWithFormat("yxx/shaizi/shaizi_%d_1.png", obj->cbDiceValue[1])->getCString()).c_str());
			itemp_shaizi_2->setName("itemp_shaizi");
			itemp_shaizi_2->setPosition(1167, 724);
			BgPanel->addChild(itemp_shaizi_2, 4);

			auto itemp_shaizi_3 = Sprite::create(SETPATH(__String::createWithFormat("yxx/shaizi/shaizi_%d_1.png", obj->cbDiceValue[2])->getCString()).c_str());
			itemp_shaizi_3->setName("itemp_shaizi");
			itemp_shaizi_3->setPosition(1239, 725);
			BgPanel->addChild(itemp_shaizi_3);
			//复投消息处理
			userRepeatPlaceJetton(true, itemp_jesson_my_init, false);
			userRepeatPlaceJetton(false, itemp_jesson_init, false);
		}
		else
		{
			userRepeatPlaceJetton(true, itemp_jesson_my_init);
			userRepeatPlaceJetton(false, itemp_jesson_init);
		}
		if (PokerData::instance()->getMatchRule() & 0x00000040)
		{
			if (wCurrentTimeType == TimeType_yxx_Start)
			{
				CMD_S_GameEnd endData;
				memset(&endData, 0, sizeof(CMD_S_GameEnd));
				endData.lBankerScore = obj->lEndBankerScore;
				endData.lUserScore = obj->lEndUserScore;
				scoreL->setString(__String::createWithFormat("%lld", obj->lEndUserScore)->getCString());
			}
		}
	}

	void MainScene_yxx::changeBankerButton()
	{
		switch (wCurrentBankerState)
		{
		case CurrentBanker_yxx_Already:
		{
										  banker_Btn->loadTextureNormal(SETPATH("yxx/Image/banker_Btn2.png"));
		}
			break;
		case CurrentBanker_yxx_Ready:
		{
										banker_Btn->loadTextureNormal(SETPATH("yxx/Image/banker_Btn3.png"));
		}
			break;
		case CurrentBanker_yxx_None:
		{
									   banker_Btn->loadTextureNormal(SETPATH("yxx/onlineFrame/banker_Btn1.png"));
		}
			break;
		default:
			break;
		}
	}

	void MainScene_yxx::setOtherPlayer(PokerPlayerInfo * player)
	{
		PokerPlayerInfo* playerInfo = (PokerPlayerInfo*)player;
		
		for (int i = 0; i < ohter_list->getItems().size(); i++)
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

		//在线人数
		int itemp_onlineNumber = ohter_list->getItems().size();
		auto online_number = static_cast<Text *>(BgPanel->getChildByName("online_number"));
		online_number->setString(__String::createWithFormat("%d", itemp_onlineNumber + 1)->getCString());

		auto banker = Sprite::create(SETPATH("yxx/Image/playerList.png"));
		banker->setName(playerInfo->getNickName().c_str());

		auto bg = Layout::create();
		bg->setContentSize(banker->getContentSize());
		bg->setTag(playerInfo->getSeatId());
		bg->setName(playerInfo->getNickName().c_str());
		banker->setAnchorPoint(Vec2::ZERO);
		bg->addChild(banker); int faceId = playerInfo->getFaceId();
		string headUrl = StringUtils::format("user_info/bighead_%d.png", faceId);
		if (!FileUtils::getInstance()->isFileExist(headUrl))//如果faceId不在我们头像的区间，初始化faceId
		{
			//如果faceId不在我们头像的区间，在手机头像随机一个
			faceId = 0;
		}
		auto feath = Sprite::create(__String::createWithFormat("user_info/bighead_%d.png", faceId)->getCString());
		feath->setPosition(30, 11);
		feath->setScale(0.4f);
		feath->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		banker->addChild(feath);
		auto nameTTf = LabelTTF::create();
		nameTTf->setString(playerInfo->getNickName().c_str());
		nameTTf->setPosition(Vec2(115, 58));
		nameTTf->setAnchorPoint(Vec2(0.0f, 0.5f));
		nameTTf->setColor((Color3B(243, 208, 119)));
		nameTTf->setFontSize(20);
		banker->addChild(nameTTf);
		auto scoreTTf = LabelTTF::create();
		scoreTTf->setString(__String::createWithFormat("%lld", playerInfo->getScore())->getCString());
		scoreTTf->setPosition(Vec2(140, 28));
		scoreTTf->setAnchorPoint(Vec2(0.0f, 0.5f));
		scoreTTf->setColor((Color3B(243, 208, 119)));
		scoreTTf->setFontSize(20);
		scoreTTf->setName("scoreTTf");
		banker->addChild(scoreTTf);

		auto gold = Sprite::create(SETPATH("yxx/Image/gold.png"));
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

	void MainScene_yxx::updateScore(CMD_GR_MobileUserScore& buf)
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

	//系统坐庄时金币更新
	void MainScene_yxx::updateBankerScore(SCORE itemp_bankerScore)
	{
		bankerScoreL->setString(__String::createWithFormat("%lld", itemp_bankerScore)->getCString());
		auto list_score = static_cast<Text *>(list_planeL->getChildByName("list_score"));
		list_score->setString(__String::createWithFormat("%lld", std::atoll(bankerScoreL->getString().c_str()))->getCString());
	}

	void MainScene_yxx::createUpdate(int time)
	{
		timeLabelTop->setString(__String::createWithFormat("%d%s", time, "s")->getCString());
		timeLabelTop->setVisible(true);
		schedule(schedule_selector(MainScene_yxx::timeUpdate), 1.0f);
	}

	void MainScene_yxx::timeUpdate(float dt)
	{
		int num = std::atoll(timeLabelTop->getString().c_str());
		if (num > 1)
		{
			num--;
			if (timeLabelTop)
			{
				timeLabelTop->setString(__String::createWithFormat("%d%s", num, "s")->getCString());
				if (num <= 4 && wCurrentTimeType == TimeType_yxx_PlaceJetton)
				{
					SoundManager::instance()->playEffect(SETPATH("yxx/sound/time_waring.mp3").c_str());
				}
			}
		}
		else
		{
			removeTimeLabel();
		}
	}

	void MainScene_yxx::removeTimeLabel()
	{
		unschedule(schedule_selector(MainScene_yxx::timeUpdate));
		timeLabelTop->setString(__String::createWithFormat("%d%s", 0, "s")->getCString());
	}

	void MainScene_yxx::PlaceJettonFunction(CMD_S_PlaceJetton& jetton)
	{
		SoundManager::instance()->playEffect(SETPATH("yxx/sound/add_gold.mp3").c_str());//这里添加声音
		if (jetton.wChairID == UserInfo::instance()->getSeatId())
		{
			updateShowScore(true, jetton.cbJettonArea, jetton.lJettonScore);
		}
		else
		{
			updateShowScore(false, jetton.cbJettonArea, jetton.lJettonScore);
		}
	}

	void MainScene_yxx::updateShowScore(bool seatId, BYTE jettonArea, SCORE jettonScore, bool isFirstBet)
	{
		if (seatId)//如果是自己
		{
			usableL->setString(__String::createWithFormat("%lld", std::atoll(usableL->getString().c_str()) - jettonScore)->getCString());
			my_AllJettonScore += jettonScore;
			itemp_myJettonLabel[jettonArea] += jettonScore;
			myJettonLabel[jettonArea]->setVisible(true);
			blockLight[jettonArea]->setVisible(true);
			myJettonLabel[jettonArea]->setString(__String::createWithFormat("%lld", itemp_myJettonLabel[jettonArea])->getCString());

			//每次下注检查筹码显示
			if (isContinueJetton == false && wCurrentTimeType == TimeType_yxx_PlaceJetton)
			{
				for (int i = 0; i < 6; i++)
				{
					if (i == getJettonCurType(jettonScore) - 1)
					{
						if (std::atoll(usableL->getString().c_str()) < getJettonNumber(i + 1))
						{
							wCurrentJettonScore = 0;
							B_CurMoney[i]->setTouchEnabled(false);
							B_CurMoney[i]->setBright(false);
							B_CurMoney[i]->stopAllActions();
							B_CurMoney[i]->runAction(MoveTo::create(0.1f, Vec2(B_CurMoney[i]->getPosition().x, 54)));
							CurMoneyLabel[i]->setColor(Color3B(133, 133, 133));
							
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
										S_circleLight->setPosition(Vec2(btnsv->getPosition().x, 70));
										btnsv->stopAllActions();
										btnsv->runAction(MoveTo::create(0.1f, Vec2(btnsv->getPosition().x, 70)));
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
			PlaceJettonEffect(jettonArea, jettonScore, true, isFirstBet);
		}
		else
		{
			PlaceJettonEffect(jettonArea, jettonScore, false, isFirstBet);
		}

		//其他人下的注显示
		TextBMFont *ttf = jettonLabelVec.at(jettonArea);
		ttf->setVisible(true);
		ttf->setPosition(JettonTotalPlace(jettonArea));
		itemp_totalTtf[jettonArea] += jettonScore;

		ttf->setString(__String::createWithFormat("%lld", itemp_totalTtf[jettonArea])->getCString());
	}
	void MainScene_yxx::PlaceJettonEffect(int Area, int JessonNumber, bool isMySealt, bool isFirstBet = true)
	{
		//筹码效果
		jettonSprite = Sprite::create(SETPATH("yxx/Res/gold.png"));
		jettonSprite->setName("jettonSprite");
		if (!isFirstBet)
		{
			jettonSprite->setVisible(false);
		}
		
		if (is_trueCheckbox == false)
		{
			jettonSprite->setVisible(false);
		}
		int curType = getJettonCurType(JessonNumber) - 1;
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

		BgPanel->addChild(jettonSprite, 4);

		jettonSprite->runAction(Sequence::create(
			ScaleTo::create(0.2f, 1.0f),
			EaseSineOut::create(MoveTo::create(0.3f, randJettonPos(Area))),
			NULL));
	}

	//总注下注位置
	Vec2 MainScene_yxx::JettonTotalPlace(int jettonArea)
	{
		switch (jettonArea)
		{
		case 16:case 17:case 18:case 19:case 20:case 21:
			return(Vec2(2, -34) + myJettonLabel[jettonArea]->getPosition());;
			break;
		case 1:case 2:case 3:case 4:case 5:case 6:case 7:
		case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:
			return(Vec2(2, -34) + myJettonLabel[jettonArea]->getPosition());
			break;
		default:
			return(Vec2(2, -24) + myJettonLabel[jettonArea]->getPosition());
			break;
		}
	}

	//随机一个下注区域的坐标 
	Vec2 MainScene_yxx::randJettonPos(int jettonArea)
	{
		auto itempImage = (ImageView *)BgPanel->getChildByTag(jettonArea + 100);
		float rand_x = rand() % (int)(itempImage->getContentSize().width - (B_CurMoney[1]->getContentSize().width *0.3f) - 15.0f);
		float rand_y = rand() % (int)(itempImage->getContentSize().height - (B_CurMoney[1]->getContentSize().height *0.3f));
		return Vec2(itempImage->getPosition() - Vec2(itempImage->getContentSize() / 2) + Vec2(B_CurMoney[1]->getContentSize() *0.15f) + Vec2(rand_x + 8, rand_y));
	}
	
	SCORE MainScene_yxx::getJettonNumber(int jType)
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

	int	MainScene_yxx::getJettonCurType(SCORE jScore)
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

	void MainScene_yxx::clearRecordList()
	{
		memset(&jettonProbability, 0, sizeof(jettonProbability));
		record_list->removeAllChildren();
	}

	void MainScene_yxx::addRecordList(tagGameRecord& obj, int itemp_LaterCount)
	{
		for (int i = 0; i < 3; i++)
		{
			int itemp_Number = obj.cbDiceValue[i];
			switch (itemp_Number)
			{
			case 1:
				jettonProbability[0]++;
				break;
			case 2:
				jettonProbability[1]++;
				break;
			case 3:
				jettonProbability[2]++;
				break;
			case 4:
				jettonProbability[3]++;
				break;
			case 5:
				jettonProbability[4]++;
				break;
			case 6:
				jettonProbability[5]++;
				break;
			default:
				break;
			}
		}

		if (record_list->getItems().size() == 24)
		{
			record_list->removeItem(0);
		}

		ImageView * recordNumber1 = ImageView::create(SETPATH(__String::createWithFormat("yxx/shaizi/pic%d.png", obj.cbDiceValue[0])->getCString()));
		recordNumber1->setPosition(ccp(23, 30));
		ImageView * recordNumber2 = ImageView::create(SETPATH(__String::createWithFormat("yxx/shaizi/pic%d.png", obj.cbDiceValue[1])->getCString()));
		recordNumber2->setPosition(ccp(77, 30));
		ImageView * recordNumber3 = ImageView::create(SETPATH(__String::createWithFormat("yxx/shaizi/pic%d.png", obj.cbDiceValue[2])->getCString()));
		recordNumber3->setPosition(ccp(130, 30));

		auto record_item = Sprite::create(SETPATH("yxx/onlineFrame/line3.png"));
		record_item->setAnchorPoint(Vec2::ZERO);

		auto itemBg = Sprite::create(SETPATH("yxx/onlineFrame/line3.png"));
		itemBg->setPosition(itemBg->getPosition() + Vec2(-8, 0));
		itemBg->setAnchorPoint(Vec2::ZERO);
		itemBg->addChild(record_item);

		auto bg = Layout::create();
		bg->setContentSize(CCSize(160, 60));
		bg->setAnchorPoint(Vec2::ZERO);
		bg->addChild(itemBg);
		bg->addChild(recordNumber1);
		bg->addChild(recordNumber2);
		bg->addChild(recordNumber3);

		record_list->pushBackCustomItem(bg);
		record_list->setInnerContainerSize(record_list->getInnerContainerSize() + CCSize(bg->getContentSize().width, 0));
		record_list->scrollToBottom(0.1f, true);

		//显示概率
		float itemp_recordNumber = (record_list->getItems().size() + itemp_LaterCount) * 3;
		Text * probability;
		for (int j = 0; j < 6; j++)
		{
			probability = static_cast<Text *>(ludan_total->getChildByName(__String::createWithFormat("Label_%d", j + 1)->getCString()));
			float itemp_jettonProbility = (jettonProbability[j] * 100 / itemp_recordNumber);
			probability->setString(__String::createWithFormat("%.1f%%", itemp_jettonProbility)->getCString());
		}
	}
	
	//自动跟注勾全部取消掉
	void MainScene_yxx::automaticJCancel() 
	{
		auto btn_10 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_10"));
		auto btn_20 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_20"));
		auto btn_50 = static_cast<CheckBox *>(btn_plane->getChildByName("btn_50"));
		btn_10->setSelectedState(false);
		btn_20->setSelectedState(false);
		btn_50->setSelectedState(false);
	}
	
	void MainScene_yxx::subChangeBanker(CMD_S_ChangeBanker user)
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
			list_banker->setString(U2F("系统坐庄"));
			list_score->setString(__String::createWithFormat("%lld", user.lBankerScore)->getCString());
			bankerTimeL->setString("0");
			bankerScoreL->setString(__String::createWithFormat("%lld", user.lBankerScore)->getCString());
			
			wCurrentBanker = user.wBankerUser;
			if (wCurrentBankerState == CurrentBanker_yxx_Already)
			{
				wCurrentBankerState = CurrentBanker_yxx_None;
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
				wCurrentBanker = player->getSeatId();
			}
			list_score->setString(__String::createWithFormat("%lld", user.lBankerScore)->getCString());
			bankerScoreL->setString(__String::createWithFormat("%lld", user.lBankerScore)->getCString());
		}
		if (wCurrentBanker == UserInfo::instance()->getSeatId())
		{
			wCurrentBankerState = CurrentBanker_yxx_Already;
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
				automaticJCancel();
				automaticJ_btn->loadTextures(SETPATH("yxx/Res/automaticJ_btn_n.png"), "");
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
					wCurrentBankerState = CurrentBanker_yxx_Ready;
					this->changeBankerButton();
					banker_Btn->setTouchEnabled(true);
					return;
				}
			}
		}
		wCurrentBankerState = CurrentBanker_yxx_None;
		this->changeBankerButton();
		banker_Btn->setTouchEnabled(true);
	}
	
	//如果是自己上庄后的界面显示
	void MainScene_yxx::showUpeerBanker(bool isBanker)
	{
		repeatJ_btn->setTouchEnabled(!isBanker);
		automaticJ_btn->setTouchEnabled(!isBanker);

		if (isBanker)
		{
			repeatJ_btn->setColor(Color3B(133, 133, 133));
			automaticJ_btn->setColor(Color3B(133, 133, 133));
		}
		else
		{
			repeatJ_btn->setColor(Color3B(255, 255, 255));
			automaticJ_btn->setColor(Color3B(255, 255, 255));
			automaticJ_btn->loadTextures(SETPATH("yxx/Res/automaticJ_btn_n.png"), "");
			repeatJ_btn->loadTextures(SETPATH("yxx/Res/repeatJ_btn_n.png"), "");
		}
	}
	
	void MainScene_yxx::removeBanker(WORD id)
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
			wCurrentBankerState = CurrentBanker_yxx_Already;
			this->changeBankerButton();
			banker_Btn->setTouchEnabled(true);
		}
	}

	void MainScene_yxx::removeBankerSub(WORD id)
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
			wCurrentBankerState = CurrentBanker_yxx_None;
			this->changeBankerButton();
			banker_Btn->setTouchEnabled(true);
		}
	}

	//影藏自动跟注的显示
	void MainScene_yxx::hideContinueJetton(bool istrue)
	{
		if (!isContinueJetton)
		{
			return;
		}
		isContinueJetton = false;

		//取消全部自动跟注
		automaticJCancel();
		
		continue_number = 0;
		continueJetton_number = 0;
		

		if (wCurrentTimeType == TimeType_yxx_PlaceJetton)
		{
			for (int i = 0; i < 6; i++)
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
						btnsv->runAction(MoveTo::create(0.1f, Vec2(btnsv->getPosition().x, 70)));
					}
				}
				else
				{
					B_CurMoney[i]->setTouchEnabled(false);
					B_CurMoney[i]->setBright(false);
					CurMoneyLabel[i]->setColor(Color3B(133, 133, 133));
				}
			}
			if (wCurrentJettonScore == 0)
			{
				if (std::atoll(usableL->getString().c_str()) >= getJettonNumber(1))
				{
					wCurrentJettonScore = getJettonNumber(1);
					auto btns = (Button *)BgPanel->getChildByName(CurrentButtonName(wCurrentJettonScore));
					S_circleLight->setVisible(true);
					S_circleLight->setPosition(Vec2(btns->getPosition().x, 70));
					btns->setBrightStyle(BrightStyle::HIGHLIGHT);
					btns->stopAllActions();
					btns->runAction(MoveTo::create(0.1f, Vec2(btns->getPosition().x, 70)));
				}
			}
		}

		automaticJ_label->setVisible(false);
		jessonPanelBlack->setVisible(false);
		automaticJ_btn->setTouchEnabled(true);
		automaticJ_btn->setColor(Color3B(255, 255, 255));
	}
	void MainScene_yxx::setLabelType(int time, TimeType_yxx type)
	{
		createUpdate(time);//倒计时时间
		wCurrentTimeType = type;

		//判断是否庄家
		bool isBanker = !strcmp(bankerUserL->getString().c_str(), UserInfo::instance()->getNickName().c_str());
		switch (type)
		{
		case TimeType_yxx_PlaceJetton:   //下注时间
		{
											 //声音
											 if (isWelcomeSound == false)
											 {
												 SoundManager::instance()->playEffect(SETPATH("yxx/sound/game_state.mp3").c_str());
											 }
											 armatureBird->setVisible(true);
											 armatureBird->getAnimation()->play("Yxx_lhd_betsstart");
											 timeLable->setString(U2F("下注时间请下注"));
											 socreRecord = std::atoll(usableL->getString().c_str());

											 //如果自己坐庄,如果当前是上庄按钮消失，改成我要下庄
											 if (isBanker)
											 {
												 automaticJ_label->setString(__String::createWithFormat(U2F("您现在是庄家...(已坐庄%d轮)").c_str(), ++BankerCount)->getCString());
												 banker_Btn->setTouchEnabled(false);
												 banker_Btn->loadTextureNormal(SETPATH("yxx/Image/banker_Btn2_s.png"));
												 for (int i = 0; i < 6; i++)
												 {
													 B_CurMoney[i]->setTouchEnabled(false);
													 B_CurMoney[i]->setBright(false);
													 CurMoneyLabel[i]->setColor(Color3B(133, 133, 133));
												 }
											 }
											 else
											 {
												 //是否自动跟注，上庄图片隐藏掉,按钮变为我要上庄按钮
												 if (isContinueJetton)
												 {
													 if (continueJetton())//是的话，自动跟注功能，按钮置灰
													 {
														 for (int i = 0; i < 6; i++)
														 {
															 B_CurMoney[i]->setTouchEnabled(false);
															 B_CurMoney[i]->setBright(false);
															 CurMoneyLabel[i]->setColor(Color3B(133, 133, 133));
															 return;
														 }
													 }
												 }
												 else
												 {
													 automaticJ_label->setVisible(false);
													 jessonPanelBlack->setVisible(false);
													 if (wCurrentBankerState != CurrentBanker_yxx_Ready)
													 {
														 banker_Btn->loadTextureNormal(SETPATH("yxx/onlineFrame/banker_Btn1.png"));
													 }
												 }
												 for (int i = 0; i < 6; i++)
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
												 //默认下筹码选择最小的一个吧
												 if (std::atoll(usableL->getString().c_str()) >= getJettonNumber(1))
												 {
													 wCurrentJettonScore = getJettonNumber(1);
													 auto btns = (Button *)BgPanel->getChildByName(CurrentButtonName(wCurrentJettonScore));
													 S_circleLight->setVisible(true);
													 Vec2 abcd = btns->getPosition();
													 S_circleLight->setPosition(Vec2(btns->getPosition().x, 70));
													 btns->setBrightStyle(BrightStyle::HIGHLIGHT);
													 btns->stopAllActions();
													 btns->runAction(MoveTo::create(0.1f, Vec2(btns->getPosition().x, 70)));
												 }
											 }

		}
			break;
		case TimeType_yxx_Start:
		{
								   //声音
								   if (isWelcomeSound == false)
								   {
									   SoundManager::instance()->playEffect(SETPATH("yxx/sound/stop_bet.mp3").c_str());
								   }
								   S_circleLight->setVisible(false);
								   armatureBird->setVisible(true);
								   armatureBird->getAnimation()->play("Yxx_lhd_betsstop");
								   timeLable->setString(U2F("本局游戏开奖中"));
								   for (int i = 0; i < 22; i++)
								   {
									   myJettonScore[i] = itemp_myJettonLabel[i];
								   }

								   //个人下注总数至零
								   my_AllJettonScore = 0;

								   //按钮不能点击
								   for (int i = 0; i < 6; i++)
								   {
									   B_CurMoney[i]->setTouchEnabled(false);
									   B_CurMoney[i]->setBright(false);
									   CurMoneyLabel[i]->setColor(Color3B(133, 133, 133));
									  
								   }
								   if (wCurrentJettonScore != 0 && !isContinueJetton)
								   {
									   auto btns = (Button *)BgPanel->getChildByName(CurrentButtonName(wCurrentJettonScore));
									   btns->stopAllActions();
									   btns->runAction(MoveTo::create(0.1f, Vec2(btns->getPosition().x, 54)));
								   }
		}
			break;
		case TimeType_yxx_Leisure:
		{
									 for (int i = 0; i < 22; i++)//停止渐变
									 {
										 auto star = static_cast<ImageView *>(Helper::seekWidgetByTag(pRoot, 100 + i));
										 star->setOpacity(255);
										 star->stopAllActions();
									 }
									 banker_Btn->setTouchEnabled(true);
									 if (wCurrentBankerState != CurrentBanker_yxx_Ready)
									 {
										 if (!isBanker)
										 {
											 banker_Btn->loadTextureNormal(SETPATH("yxx/onlineFrame/banker_Btn1.png"));
										 }
										 else
										 {
											 banker_Btn->loadTextureNormal(SETPATH("yxx/Image/banker_Btn2.png"));
										 }
									 }

									 //筹码变灰
									 for (int i = 0; i < 6; i++)
									 {
										 B_CurMoney[i]->setTouchEnabled(false);
										 CurMoneyLabel[i]->setColor(Color3B(133, 133, 133));
										 B_CurMoney[i]->setBright(false);
									 }
									 closeLight(0.0f); //所有显示亮灯关闭
									 closeBlock();//下注亮的灯关闭
									 memset(init_ResultNumber, 0, sizeof(init_ResultNumber));
									 timeLable->setString(U2F("空闲时间请等待游戏开始"));
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
									 
									 //下注筹码至为0
									 wCurrentJettonScore = 0;  
									 
									 //桌面上金币移除
									 auto childs = BgPanel->getChildren();
									 for (auto itor = childs.begin(); itor != childs.end(); itor++)
									 {
										 auto node = (*itor);
										 if (node && node->getName() == "jettonSprite"){
											 node->removeFromParentAndCleanup(true);
										 }
										 if (node && node->getName() == "itemp_shaizi"){
											 node->removeFromParentAndCleanup(true);
										 }
									 }
									 
									 //摇色子动画隐藏掉
									 jettonSprite = nullptr;
									 if (armatureBird)
									 {
										 armatureBird->setVisible(false);
									 }
									 //结算面板隐藏
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
	void MainScene_yxx::userApplyBanker(int seatId)
	{
		if (UserInfo::instance()->getSeatId() == seatId)
		{
			wCurrentBankerState = CurrentBanker_yxx_Ready;
			banker_Btn->loadTextureNormal(SETPATH("yxx/Image/banker_Btn3.png"));
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

			auto banker = Sprite::create(SETPATH("yxx/Image/playerList.png"));
			auto bg = Layout::create();
			bg->setContentSize(banker->getContentSize());
			bg->setTag(seatId);
			bg->setName(player->getNickName().c_str());

			banker->setAnchorPoint(Vec2::ZERO);
			bg->addChild(banker);
			int faceId = player->getFaceId();
			string headUrl = StringUtils::format("user_info/bighead_%d.png", faceId);
			if (!FileUtils::getInstance()->isFileExist(headUrl))//如果faceId不在我们头像的区间，初始化faceId
			{
				//如果faceId不在我们头像的区间，在手机头像随机一个
				faceId = 0;
			}
			auto feath = Sprite::create(__String::createWithFormat("user_info/bighead_%d.png", faceId)->getCString());
			feath->setPosition(115, 11);
			feath->setScale(0.42f);
			feath->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			banker->addChild(feath);

			auto m_sItempFrame = Sprite::create(SETPATH("yxx/Res/head2.png"));
			m_sItempFrame->setPosition(feath->getPosition() + Vec2(-7, -7));
			m_sItempFrame->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			m_sItempFrame->setScale(0.8f);
			banker->addChild(m_sItempFrame, 1);

			auto nameTTf = LabelTTF::create();
			nameTTf->setString(player->getNickName().c_str());
			nameTTf->setPosition(Vec2(190, 58));
			nameTTf->setAnchorPoint(Vec2(0.0f, 0.5f));
			nameTTf->setColor(Color3B(243, 81, 0));
			nameTTf->setFontSize(20);
			bg->addChild(nameTTf);

			auto scoreTTf = LabelTTF::create();
			scoreTTf->setString(__String::createWithFormat("%lld", player->getScore())->getCString());
			scoreTTf->setName("scoreTTf");
			scoreTTf->setPosition(Vec2(215, 28));
			scoreTTf->setColor(Color3B(243, 81, 0));
			scoreTTf->setAnchorPoint(Vec2(0.0f, 0.5f));
			scoreTTf->setFontSize(20);
			bg->addChild(scoreTTf);

			auto gold = Sprite::create(SETPATH("yxx/Image/gold.png"));
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
	//复投消息处理
	void MainScene_yxx::userRepeatPlaceJetton(bool istrue, CMD_S_RepeatBet&  pQueryStorage, bool isFirstBet)
	{
		LONGLONG jetton_repeatNumberVec[22];
		for (int j = 0; j < AREA_COUNT; j++)
		{
			jetton_repeatNumberVec[j] = pQueryStorage.lJettonScore[j];
			for (int area = 5; area >= 0; area--)
			{
				if (jetton_repeatNumberVec[j] >= tagChange[area])
				{
					int repeatTimes = jetton_repeatNumberVec[j] / tagChange[area];
					for (int repreatJessonNumber = repeatTimes; repreatJessonNumber > 0; repreatJessonNumber--)
					{
						updateShowScore(istrue, j, tagChange[area], isFirstBet);
					}
					jetton_repeatNumberVec[j] = jetton_repeatNumberVec[j] - tagChange[area] * repeatTimes;
				}
			}
		}
	}

	//重复下注
	void MainScene_yxx::repaetButtonNoifyFial()
	{
		if (isContinueJetton)
		{
			PromptLayer::createObject(U2F("当前自动跟注状态，不允许重复下注！").c_str());
			return;
		}
		if (wCurrentTimeType == TimeType_yxx_PlaceJetton)
		{
			int number = 0;
			for (int i = 0; i < 22; i++)
			{
				number += myJettonScore[i];
			}
			if (number == 0)
			{
				PromptLayer::createObject(U2F("您上局无押注，无法重复下注！").c_str());
				return;
			}

			if (std::atoll(usableL->getString().c_str()) < 1000 || std::atoll(usableL->getString().c_str()) < number)
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

	int MainScene_yxx::getCurrentRate(int type)
	{
		int rateNumber = 0;
		switch (type)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			rateNumber = 20;
			break;
		default:
			rateNumber = 4;
			break;
		}
		return rateNumber;
	}

	//继续加注
	bool MainScene_yxx::continueJetton()
	{
		int itemp_Score = 0;
		for (int i = 0; i <AREA_COUNT; i++)
		{
			itemp_Score += myJettonScore[i];
		}
		if (itemp_Score>std::atoll(usableL->getString().c_str()))
		{
			btn_plane->setVisible(false);//自动跟注板隐藏
			isContinueJetton = false;
			automaticJ_label->setVisible(false);
			jessonPanelBlack->setVisible(false);
			automaticJ_btn->loadTextures(SETPATH("yxx/Res/automaticJ_btn_n.png"), "");
			automaticJCancel(); //取消全部自动跟注
			PromptLayer::createObject(U2F("金币不足，自动跟注失败").c_str());
			return false;
		}
		CMD_C_RepeatBet repeatPlaceJetton;
		memset(&repeatPlaceJetton, 0, sizeof(CMD_C_RepeatBet));

		for (int i = 0; i < AREA_COUNT; i++)
		{
			repeatPlaceJetton.lBetScoreAry[i] = myJettonScore[i];
		}
		NetworkManager::getInstance()->sendData(NetworkManager::FLAG_GAME, MDM_GF_GAME, SUB_C_REPEAT_JETTON, &repeatPlaceJetton, sizeof(CMD_C_RepeatBet));//发送复投命令

		if (isContinueJetton)
		{
			if (continueJetton_number != ButtonType_yxx_automaticJ_Infinite)//如果是无限跟注，不递减次数,递增跟注次数
			{
				continueJetton_number--;
				if (continueJetton_number == 0)//如果跟注次数为0，停止自动跟注
				{
					btn_plane->setVisible(false);//自动跟注板隐藏
					isContinueJetton = false;
					automaticJ_label->setVisible(false);
					jessonPanelBlack->setVisible(false);
					automaticJ_btn->loadTextures(SETPATH("yxx/Res/automaticJ_btn_n.png"), "");
					automaticJCancel(); //取消全部自动跟注
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

	//自动下注的显示
	void MainScene_yxx::showContinueJetton()
	{
		if (continueJetton_number != ButtonType_yxx_automaticJ_Infinite)
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
		S_circleLight->setVisible(false);
		automaticJ_label->setVisible(true);
		jessonPanelBlack->setVisible(true);
		//将所有下注按钮至灰，不许点
		for (int i = 0; i < 6; i++)
		{
			B_CurMoney[i]->setTouchEnabled(false);
			CurMoneyLabel[i]->setColor(Color3B(133, 133, 133));
			B_CurMoney[i]->setBright(false);
		}
		if (wCurrentTimeType == TimeType_yxx_PlaceJetton && wCurrentJettonScore != 0)
		{
			auto btns = (Button *)BgPanel->getChildByName(CurrentButtonName(wCurrentJettonScore));
			btns->stopAllActions();
			btns->runAction(MoveTo::create(0.1f, Vec2(btns->getPosition().x, 54)));
		}
	}

	//在非下注时间，点击自动跟注多少轮，会进行判断是否满足自动跟注条件
	bool MainScene_yxx::checkAutoMatic()
	{
		int number = 0;
		if (wCurrentTimeType == TimeType_yxx_PlaceJetton)//如果不是下注时间自动跟注，那么比较上一轮的押注数目
		{
			for (int i = 0; i < 22; i++)
			{
				number += std::atoll(myJettonLabel[i]->getString().c_str());
			}
		}
		else
		{
			for (int i = 0; i < 22; i++)
			{
				number += myJettonScore[i];
			}
		}

		if (number == 0)
		{
			PromptLayer::createObject(U2F("您当前无押注，无法自动跟注！").c_str());
			return false;
		}

		if (std::atoll(usableL->getString().c_str()) < 1000 || std::atoll(usableL->getString().c_str()) < number)
		{
			PromptLayer::createObject(U2F("金币不足无法下注").c_str());
			return false;
		}

		return true;
	}

	void MainScene_yxx::setMenuState(bool orisMenu)
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

	void MainScene_yxx::setludanState(bool b_itempBanerMove)
	{
		if (b_itempBanerMove)
		{
			isLudanMove = false;
			ludan_total->runAction(MoveTo::create(0.2f, ludan_total->getPosition() + Vec2((ludan_total->getContentSize().width - 66 + record_btn->getContentSize().width), 0)));

		}
		else
		{
			isLudanMove = true;
			ludan_total->runAction(MoveTo::create(0.2f, ludan_total->getPosition() + Vec2(-(ludan_total->getContentSize().width - 66 + record_btn->getContentSize().width), 0)));

		}
	}

	void MainScene_yxx::setListState(bool b_itempBanerMove)
	{
		auto list_planeL = static_cast<Widget *>(BgPanel->getChildByName("list_planeL"));
		Button* btn = static_cast<Button *>(list_planeL->getChildByName("banker_move"));
		if (b_itempBanerMove)
		{
			isBanerMove = false;
			list_planeL->runAction(MoveTo::create(0.2f, list_planeL->getPosition() + Vec2(-(list_planeL->getContentSize().width + 56 - btn->getContentSize().width), 0)));
		}
		else
		{
			isBanerMove = true;
			list_planeL->runAction(MoveTo::create(0.2f, list_planeL->getPosition() + Vec2(list_planeL->getContentSize().width + 56 - btn->getContentSize().width, 0)));
		}
	}
	//结算界面
	void MainScene_yxx::overLayoutFunc(CMD_S_GameEnd& endData)
	{
		//路单更新
		tagGameRecord itemp_Record;
		for (int i = 0; i < 3; i++)
		{
			itemp_Record.cbDiceValue[i] = init_ResultNumber[i];
		}
		init_ResultNumber[0];
		addRecordList(itemp_Record, m_LaterCount++);

		over_layout->setVisible(true);

		//交叉闪烁
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
		//本家返还本金
		if (endData.lUserReturnScore > 0)
		{
			auto my_return = static_cast<TextBMFont *>(Helper::seekWidgetByName(over_layout, "my_return"));
			if (endData.lUserReturnScore > 10000)
			{
				my_return->setString(__String::createWithFormat("%.1f%s", double(endData.lUserReturnScore) / 10000, U2F("万").c_str())->getCString());
			}
			else
			{
				my_return->setString(__String::createWithFormat("%lld", endData.lUserReturnScore)->getCString());
			}
		}
		else
		{
			auto my_return = static_cast<TextBMFont *>(Helper::seekWidgetByName(over_layout, "my_return"));
			my_return->setString("0");
		}
		if (endData.lUserScore != 0)
		{
			usableL->setString(__String::createWithFormat("%lld", socreRecord + endData.lUserScore)->getCString());
		}
		else
		{
			usableL->setString(__String::createWithFormat("%lld", socreRecord)->getCString());
		}

		//刷新庄家分数
		bankerScoreL->setString(__String::createWithFormat("%lld", std::atoll(bankerScoreL->getString().c_str()) + endData.lBankerScore)->getCString());

		auto list_score = static_cast<Text *>(list_planeL->getChildByName("list_score"));
		list_score->setString(__String::createWithFormat("%lld", std::atoll(bankerScoreL->getString().c_str()))->getCString());
		bankerTimeL->setString(__String::createWithFormat("%lld", endData.lBankerTotallScore)->getCString());
		scoreL->setString(__String::createWithFormat("%lld", std::atoll(scoreL->getString().c_str()) + wCurrentGameEnd.lUserScore)->getCString());

		//特效星星闪烁
		for (int i = 0; i < 3; i++)
		{
			auto star = static_cast<ImageView *>(Helper::seekWidgetByName(over_layout, StringUtils::format("imgo_star_%d", i + 1)));
			Sequence *seq = Sequence::createWithTwoActions(FadeOut::create(0.7), FadeIn::create(0.9));
			star->runAction(RepeatForever::create(seq));
		}

		//---金币飞出
		if (endData.lUserScore > 0)
		{
			const Vec2 posBegin = Vec2(560, 300);
			const Vec2 posMid = Vec2(530, 200);
			const Vec2 posEnd = Vec2(137, 30);
			const int nodeCount = 10;
			const float timeGradually[nodeCount] = { 0.0, 0.2, 0.35, 0.45, 0.55, 0.65, 0.75, 0.83, 0.9, 0.95 };

			for (int i = 0; i < nodeCount; i++)
			{
				//曲线运动
				//0~1/3s变宽，变扁
				//1/3s~1s还原
				Sprite *spritegold = Sprite::create(SETPATH("yxx/Res/gold.png"));
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

	void MainScene_yxx::onBackPressed()
	{

	}

	void MainScene_yxx::onJessonTouch(Ref* target, Widget::TouchEventType type)
	{
		Button *btn = (Button *)target;
		if (Widget::TouchEventType::BEGAN == type)
		{
			int itemp_tag = btn->getTag();

			if (isBanerMove == true)
			{
				if (itemp_tag != ButtonType_yxx_listplantL_move&&itemp_tag != ButtonType_yxx_banker &&itemp_tag != ButtonType_yxx_b &&itemp_tag != ButtonType_yxx_p &&itemp_tag != ButtonType_yxx_banker_move&&itemp_tag != ButtonType_yxx_ludan_Frame_move&&itemp_tag != ButtonType_yxx_ludan_move)
				{
					setListState(isBanerMove);
				}
			}
			if (isLudanMove == true)
			{
				if (itemp_tag != ButtonType_yxx_listplantL_move&&itemp_tag != ButtonType_yxx_banker &&itemp_tag != ButtonType_yxx_b &&itemp_tag != ButtonType_yxx_p &&itemp_tag != ButtonType_yxx_banker_move&&itemp_tag != ButtonType_yxx_ludan_Frame_move&&itemp_tag != ButtonType_yxx_ludan_move)
				{
					setludanState(isLudanMove);
				}
			}
		}
		else if (Widget::TouchEventType::ENDED == type)
		{
			if (wCurrentTimeType == TimeType_yxx_PlaceJetton)
			{
				if (std::atoll(usableL->getString().c_str()) < ButtonNameChangeScore(btn->getName()))
				{
					PromptLayer::createObject(U2F("金币不足无法下注").c_str());
					if (wCurrentJettonScore != 0)
					{
						auto btns = (Button *)BgPanel->getChildByName(CurrentButtonName(wCurrentJettonScore));
						btns->setBrightStyle(BrightStyle::NORMAL);
						btns->stopAllActions();
						btns->runAction(MoveTo::create(0.1f, Vec2(btns->getPosition().x, 54)));
						S_circleLight->setVisible(false);
						wCurrentJettonScore = 0;
					}
					return;
				}

				if (wCurrentJettonScore == 0)
				{
					wCurrentJettonScore = ButtonNameChangeScore(btn->getName());
					auto btns = (Button *)BgPanel->getChildByName(CurrentButtonName(wCurrentJettonScore));
					S_circleLight->setVisible(true);
					S_circleLight->setPosition(Vec2(btns->getPosition().x, 70));
					btns->setBrightStyle(BrightStyle::HIGHLIGHT);
					btns->stopAllActions();
					btns->runAction(MoveTo::create(0.1f, Vec2(btns->getPosition().x, 70)));
				}
				else
				{
					auto btns = (Button *)BgPanel->getChildByName(CurrentButtonName(wCurrentJettonScore));
					btns->setBrightStyle(BrightStyle::NORMAL);
					btns->stopAllActions();
					btns->runAction(MoveTo::create(0.1f, Vec2(btns->getPosition().x, 54)));
					wCurrentJettonScore = ButtonNameChangeScore(btn->getName());
					auto btnsv = (Button *)BgPanel->getChildByName(CurrentButtonName(wCurrentJettonScore));
					btnsv->setBrightStyle(BrightStyle::HIGHLIGHT);
					S_circleLight->setPosition(Vec2(btnsv->getPosition().x, 70));
					btnsv->stopAllActions();
					btnsv->runAction(MoveTo::create(0.1f, Vec2(btnsv->getPosition().x, 70)));
				}
			}
			else
			{
				PromptLayer::createObject(U2F("请稍候，未到下注时间").c_str());
			}
		}
	}
	void MainScene_yxx::autoExit()
	{
		UserInfo::instance()->setCurrentServerID(0);
		MainController::getInstance()->setGameState(false);
		PokerMessageController::instance()->onQuitGame();
	}
	string MainScene_yxx::CurrentButtonName(int currentScore)
	{
		return B_CurMoney[getJettonCurType(currentScore) - 1]->getName();
	}
	int MainScene_yxx::ButtonNameChangeScore(string currentName)
	{
		for (int i = 0; i < 6; i++)
		{
			if (currentName == B_CurMoney[i]->getName())
			{
				return tagChange[i];
			}
		}
	}
}