#include "MainScene_fruit.h"
#include "game/fruit/data/PokerData_fruit.h"
#include "game/fruit/message/PokerMessageController_fruit.h"
#include "game/fruit/message/SendGameMsg_fruit.h"
#include "game/fruit/ui/widget/helpLayer_fruit.h"
#include "game/fruit/ui/widget/OutGameLayer_fruit.h"
#include "game/fruit/data/SoundManager_fruit.h"
#include "hall/manager/SoundManager_hall.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <platform/android/jni/JniHelper.h>
#endif
namespace fruit
{
	MainScene_fruit::MainScene_fruit()
	{
		visible_twinkle_first = true;
		visible_twinkle_second = true;
		visible_twinkle = true;
		isGo_state = 0;
		isGo = false;
		itemp_guess_numberResult = 0;
		wCurrentGameButtonRightScore = 0;
		itemp_rotate_count_first = 0;
		itemp_rotate_count_second = 0;
		itemp_rotate_count_third = 0;
		itemp_rotate_count_fourth = 0;
		itemp_rotate_count = 0;
		guess_number_result = 0;
		is_guess_visible = false;
		wCurrentTimeType = TimeType_fruit_PlaceJetton;
		jessonUpNumber = 0;
		isSound = false;
		is_guess_useable = true;
		init_trut_or_false_Light = false;
		itemp_number = 0;
		itemp_numberLightRotate = 0;
		isStart = true;
		score_rank = 1;			//房间赔率
		scoreGameResult = 0;    //每局结束后，猜大小之前，房间最后分数总和
		itemp_gun_number = 0;  // 第一次lucky初始位置
		itemp_gun_second_number = 0; //第二次lucky起始位置加上初始位置
		itemp_gun_third_number = 0; //第三次lucky起始位置
		itemp_gun_fourth_number = 0;
		rand_x = 0;
		rand_y = 0;
		isContinueJetton = false;
		wCurrentJettonScore = 0;
		my_AllJettonScore = 0;
		socreRecord = 0;
		memset(init_ResultNumber, 0, sizeof(init_ResultNumber));
		continue_number = 0;
		isMenu = false;
		ove_layout = nullptr;
		wCurrentBankerState = CurrentBanker_fruit_None;
		memset(&wCurrentGameEnd, 0, sizeof(CMD_S_GameEnd));
		memset(&wCurrentGameGuess, 0, sizeof(wCurrentGameGuess));
		memset(&itemp_myJettonLabel, 0, sizeof(itemp_myJettonLabel));
		memset(&myRepeatJettonLabel, 0, sizeof(myRepeatJettonLabel));
		memset(&itemp_totalTtf, 0, sizeof(itemp_totalTtf));
	}


	MainScene_fruit::~MainScene_fruit()
	{
	}
	void MainScene_fruit::onEnter()
	{
		SceneBase::onEnter();
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainScene_fruit::closeSocket_noifyFial), GAME_DISCONNECT, nullptr);
		SoundManager::instance()->stopBgMusic();
	}
	void MainScene_fruit::onExit()
	{
		this->unscheduleUpdate();
		HallSoundManager::instance()->stopAllEffect();
		NotificationCenter::getInstance()->removeObserver(this, GAME_DISCONNECT);
		SceneBase::onExit();
		SoundManager::instance()->stopBgMusic();
	}
	void MainScene_fruit::closeSocket_noifyFial(Object *obj)
	{
		PromptLayer::createObject(U2F("与服务器断开连接，3秒后自动退出游戏").c_str());

		NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_GAME);

		this->runAction(Sequence::create(DelayTime::create(3), CallFuncN::create([=](Node *node){
			UserInfo::instance()->setCurrentServerID(0);
			MainController::getInstance()->setGameState(false);
			PokerMessageController::instance()->onQuitGame();
		}), NULL));
	}
	bool MainScene_fruit::init()
	{
		if (!Layer::init())
		{
			return false;
		}
		pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(SETPATH("fruit/fruit_vertical_1.json").c_str());
		pRoot->addTouchEventListener(this, toucheventselector(MainScene_fruit::onTouch));
		pRoot->setAnchorPoint(CCPoint(0.0f, 0.0f));
		pRoot->setPosition(Vec2(0, 0));
		this->addChild(pRoot);

		BgPanel = static_cast<ImageView *>(pRoot->getChildByName("BgPanel"));
		BgPanel->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));

		//倒计时时间
		timeLabel_Mid = static_cast<TextBMFont *>(BgPanel->getChildByName("timeLabelMid"));
		timeLabel_Mid->setString("00");

		//初始化旋转灯和底下左右灯
		initLight();

		//两边的灯亮起
		blockSideLight = static_cast<ImageView *>(BgPanel->getChildByName("Image_62_0"));
		blockSideLight->setVisible(false);
		blockSideLightTwo = static_cast<ImageView *>(BgPanel->getChildByName("Image_62"));
		blockSideLightTwo->setVisible(false);

		//左边右边金币
		scoreTopRight = static_cast<TextBMFont *>(BgPanel->getChildByName("scoreTopRight"));
		scoreTopLeft = static_cast<TextBMFont *>(BgPanel->getChildByName("scoreTopLeft"));

		//左右比大小得分开始按钮
		anniu_left = static_cast<Button *>(BgPanel->getChildByName("anniu_left"));
		anniu_left->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));
		anniu_right = static_cast<Button *>(BgPanel->getChildByName("anniu_right"));
		anniu_right->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));

		anniu_1_7 = static_cast<Button *>(BgPanel->getChildByName("anniu_1_6"));
		anniu_1_7->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));
		anniu_8_14 = static_cast<Button *>(BgPanel->getChildByName("anniu_8_13"));
		anniu_8_14->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));

		anniu_all = static_cast<Button *>(BgPanel->getChildByName("anniu_all"));
		anniu_all->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));

		anniu_go = static_cast<Button *>(BgPanel->getChildByName("anniu_go"));
		anniu_go->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));

		//长按自动跟注的方框，现在已经屏蔽
		goFrame = static_cast<ImageView *>(BgPanel->getChildByName("goFrame"));
		goFrame->setVisible(false);
		goFrame_1 = static_cast<ImageView *>(BgPanel->getChildByName("goFrame_1"));
		goFrame_1->setVisible(false);

		//设置按钮
		auto menu_btn = static_cast<Button *>(BgPanel->getChildByName("menu_btn"));
		menu_btn->setTag(ButtonType_fruit_menu);
		menu_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));

		//音效帮助退出按钮
		auto help_btn = static_cast<Button *>(BgPanel->getChildByName("help_btn"));//帮助按钮
		help_btn->setTag(ButtonType_fruit_help);
		help_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));

		auto menuPanel = static_cast<Widget *>(BgPanel->getChildByName("menuPanel"));
		menuPanel->setScale(0.0);
		menuPanel->setPosition(Vec2(110,1280));

		auto out_btn = static_cast<Button *>(menuPanel->getChildByName("out_btn"));//退出按钮
		out_btn->setTag(ButtonType_fruit_out);
		out_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));

		auto setting_btn = static_cast<Button *>(menuPanel->getChildByName("setting_btn"));//音效
		setting_btn->setTag(ButtonType_fruit_setting);
		setting_btn->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));

		isSound = HallSoundManager::instance()->getEffectConfig();
		if (isSound)
		{
			setting_btn->loadTextures(SETPATH("fruit/Button/btn_music.png"), SETPATH("fruit/Button/btn_music_gery.png"));
		}
		else
		{
			setting_btn->loadTextures(SETPATH("fruit/Button/btn_music2.png"), SETPATH("fruit/Button/btn_music2_gery.png"));
		}

		//开始前发送兑命令，金币全部兑换筹码
		SendGameMsg::sendPlacePlaceJetton(NetworkManager::FLAG_GAME, UserInfo::instance()->getScore());

		return true;
	}

	void MainScene_fruit::sideLight(float dt)
	{
		blockSideLight->setVisible(init_trut_or_false_Light);
		blockSideLightTwo->setVisible(init_trut_or_false_Light);
		if (init_ResultNumber[0] != 10 && init_ResultNumber[0] != 22)
		{
			blockLight[wCurrentGameEnd.cbTableCardArray[0] - 1]->setVisible(init_trut_or_false_Light);
			light_right[changeVoice(init_ResultNumber[0]) - 1]->setVisible(init_trut_or_false_Light);
		}
		else
		{
			for (int i = 1; i < 5; i++)
			{
				blockLight[wCurrentGameEnd.cbTableCardArray[0] - 1]->setVisible(false);
				if (wCurrentGameEnd.cbTableCardArray[i] != 0)
				{
					blockLight[wCurrentGameEnd.cbTableCardArray[i] - 1]->setVisible(init_trut_or_false_Light);
					light_right[changeVoice(init_ResultNumber[i]) - 1]->setVisible(init_trut_or_false_Light);
				}
			}
		}
		init_trut_or_false_Light = init_trut_or_false_Light ? false : true;
	}

	void MainScene_fruit::initLight()
	{
		for (int itemp_initLightNumber = 0; itemp_initLightNumber < 24; itemp_initLightNumber++)
		{
			blockLight[itemp_initLightNumber] = static_cast<ImageView *>(BgPanel->getChildByName(
				__String::createWithFormat("fruit_panel_%d", itemp_initLightNumber)->getCString()));
			blockLight[itemp_initLightNumber]->setVisible(false);
		}

		//初始化底下左右灯
		for (int itemp_light = 0; itemp_light < 8; itemp_light++)
		{
			light_right[itemp_light] = static_cast<ImageView *>(BgPanel->getChildByName(
				__String::createWithFormat("liang_right_%d", itemp_light)->getCString()));
			light_right[itemp_light]->setVisible(false);
		}

		//筹码
		B_CurMoney[0] = static_cast<Button *>(BgPanel->getChildByName("anniu_apple"));
		B_CurMoney[0]->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));
		B_CurMoney[1] = static_cast<Button *>(BgPanel->getChildByName("anniu_orange"));
		B_CurMoney[1]->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));
		B_CurMoney[2] = static_cast<Button *>(BgPanel->getChildByName("anniu_lemon"));
		B_CurMoney[2]->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));
		B_CurMoney[3] = static_cast<Button *>(BgPanel->getChildByName("anniu_bell"));
		B_CurMoney[3]->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));
		B_CurMoney[4] = static_cast<Button *>(BgPanel->getChildByName("anniu_watermelon"));
		B_CurMoney[4]->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));
		B_CurMoney[5] = static_cast<Button *>(BgPanel->getChildByName("anniu_doubleStar"));
		B_CurMoney[5]->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));
		B_CurMoney[6] = static_cast<Button *>(BgPanel->getChildByName("anniu_double7"));
		B_CurMoney[6]->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));
		B_CurMoney[7] = static_cast<Button *>(BgPanel->getChildByName("anniu_bar"));
		B_CurMoney[7]->addTouchEventListener(CC_CALLBACK_2(MainScene_fruit::onTouch, this));

		for (int itemp_Score = 0; itemp_Score < 8; itemp_Score++)
		{
			myJettonScoreShow[itemp_Score] = static_cast<TextBMFont *>(BgPanel->getChildByName(
				__String::createWithFormat("score_%d", itemp_Score + 1)->getCString()));
		}

	}
	void MainScene_fruit::closeLight(float dt)
	{
		for (int itemp_initLightNumber = 100; itemp_initLightNumber < 152; itemp_initLightNumber++)
		{
			auto itempImage = (ImageView *)BgPanel->getChildByTag(itemp_initLightNumber);
			itempImage->setOpacity(0);
		}
	}

	//底下显示灯
	void MainScene_fruit::rotateLight(float dt)
	{
		blockLight[itemp_number % 24]->setVisible(true);
		if (itemp_number >2)
		{
			blockLight[(itemp_number - 3) % 24]->setVisible(false);
		}

		//底下左右灯
		light_right[itemp_numberLightRotate % 8]->setVisible(true);
		if (itemp_numberLightRotate != 0)
		{
			light_right[(itemp_numberLightRotate - 1) % 8]->setVisible(false);
		}
		itemp_number++;
		itemp_numberLightRotate++;
	}

	void MainScene_fruit::rotateLightTwo(float dt)
	{
		blockLight[itemp_number % 24]->setVisible(true);
		blockLight[(itemp_number - 3) % 24]->setVisible(false);

		//底下左右灯
		if (rand_y < 3)
		{
			for (int i = 0; i < 8; i++)
			{
				light_right[i]->setVisible(false);
			}
		}
		else
		{
			light_right[itemp_numberLightRotate % 8]->setVisible(true);
			light_right[(itemp_numberLightRotate - 1) % 8]->setVisible(false);
		}
		itemp_numberLightRotate++;
		itemp_number++;
		rand_y--;
	}

	void MainScene_fruit::rotateLightThree(float dt)
	{
		if (rand_x == 6)
		{
			HallSoundManager::instance()->stopAllEffect();
			SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y030.mp3").c_str());
		}
		blockLight[itemp_number % 24]->setVisible(true);
		if (itemp_number != 0)
		{
			blockLight[(itemp_number - 1) % 24]->setVisible(false);
			blockLight[(itemp_number - 2) % 24]->setVisible(false);
			blockLight[(itemp_number - 3) % 24]->setVisible(false);
		}
		//底下左右灯
		if (init_ResultNumber[0] != 10 && init_ResultNumber[0] != 22)
		{
			int number = changeVoice(init_ResultNumber[0]) + 16 - rand_x;
			light_right[number % 8]->setVisible(true);
			light_right[(number - 1) % 8]->setVisible(false);
		}
		else
		{
			light_right[itemp_number % 8]->setVisible(true);
			light_right[(itemp_number - 1) % 8]->setVisible(false);
		}
		rand_x--;
		if (rand_x == 0)
		{
			SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y002.mp3").c_str());
		}
		if (init_ResultNumber[0] == 10 || init_ResultNumber[0] == 22)
		{
			if (rand_x == 0)
			{
				for (int i = 0; i < 8; i++)
				{
					light_right[i]->setVisible(false);
				}
			}
		}
		itemp_number++;
	}

	//开始时服务器下发下来的玩家金币数以及房间倍率显示在界面上
	void MainScene_fruit::subChangeGold(CMD_S_ChangeGold& end)
	{
		scoreTopRight->setString(__String::createWithFormat("%lld", UserInfo::instance()->getScore())->getCString());
		score_rank = end.nBetRatio;
		auto rank_title = static_cast<ImageView *>(BgPanel->getChildByName("jesson_rank"));
		rank_title->loadTexture(__String::createWithFormat(SETPATH("fruit/res/tag%d.png").c_str(), score_rank)->getCString());
	}

	//左移右移分数
	void MainScene_fruit::subButtonRight(CMD_S_ButtonRight& end)
	{
		HallSoundManager::instance()->stopAllEffect();
		SCORE rightScoreChange = end.lUserRightScore;
		wCurrentGameButtonRightScore = end.LChangeScore;

		//如果分数移为0，那么将结束比大小状态
		if (rightScoreChange == 0)
		{
			//状态更改为下注状态
			wCurrentTimeType = TimeType_fruit_PlaceJetton;
			for (int itemp_count = 0; itemp_count < 8; itemp_count++)
			{
				myJettonScoreShow[itemp_count]->setString("0");
			}

			//两边灯灭掉
			blockSideLight->setVisible(false);
			blockSideLightTwo->setVisible(false);

			LONGLONG itemp_count_lastScore = 0;
			for (int i = 0; i < 8; i++)
			{
				itemp_count_lastScore += itemp_myJettonLabel[i];
			}
			if (itemp_count_lastScore != 0)
			{
				memcpy(&myRepeatJettonLabel, itemp_myJettonLabel, sizeof(myRepeatJettonLabel));
			}
			memset(&itemp_myJettonLabel, 0, sizeof(itemp_myJettonLabel));
			scoreGameResult = 0;

			//上一个中奖区域清零
			if (wCurrentGameEnd.cbTableCardArray[0] == 10 || wCurrentGameEnd.cbTableCardArray[0] == 22)
			{
				for (int i = 1; i < 5; i++)
				{
					if (wCurrentGameEnd.cbTableCardArray[i] != 0)
					{
						blockLight[wCurrentGameEnd.cbTableCardArray[i - 1] - 1]->setVisible(false);
						light_right[changeVoice(wCurrentGameEnd.cbTableCardArray[i - 1]) - 1]->setVisible(false);
					}
				}
			}
			else
			{
				blockLight[wCurrentGameEnd.cbTableCardArray[0] - 1]->setVisible(true);
			}

			//左右分移动
			LONGLONG score_number_left = std::atoll(scoreTopLeft->getString().c_str());
			LONGLONG score_number_temp = 0;
			if (score_number_left >= 3)
			{
				score_number_temp = 2;
			}
			else if (score_number_left == 2)
			{
				score_number_temp = 1;
			}
			else if (score_number_left == 1)
			{
				score_number_temp = 0;
			}
			if (score_number_left != 0)
			{
				SoundManager::instance()->playEffect(SETPATH("fruit/sounds/push_coin.mp3").c_str());
				schedule(schedule_selector(MainScene_fruit::continueScoreJetton), 0.08f, score_number_temp, 0.0f);
			}

			//分数从左边到右边，判断需要重复的次数
			LONGLONG score_number_left_Second = 0;
			if (score_number_left > 4 && score_number_left < 104)
			{
				score_number_left_Second = (score_number_left - 4) / 10 + (score_number_left - 4) % 10;
			}
			else if (score_number_left >= 104 && score_number_left < 1004)
			{
				score_number_left_Second = (score_number_left - 4) / 100 + ((score_number_left - 4) % 100) / 10 + (score_number_left - 4) % 10;
			}
			else if (score_number_left >= 1004 && score_number_left < 10004)
			{
				score_number_left_Second = (score_number_left - 4) / 1000 + ((score_number_left - 4) % 1000) / 100;
				score_number_left_Second = score_number_left_Second + ((score_number_left - 4) % 100) / 10 + (score_number_left - 4) % 10;
			}
			else if (score_number_left >= 10004 && score_number_left < 100004)
			{
				score_number_left_Second =
					(score_number_left - 4) / 10000 + ((score_number_left - 4) % 10000) / 1000 + ((score_number_left - 4) % 1000) / 100;
				score_number_left_Second = score_number_left_Second + ((score_number_left - 4) % 100) / 10 + (score_number_left - 4) % 10;
			}
			else if (score_number_left >= 100004 && score_number_left < 1000004)
			{
				score_number_left_Second = (score_number_left - 4) / 100000 + ((score_number_left - 4) % 100000) / 10000
					+ ((score_number_left - 4) % 10000) / 1000;
				score_number_left_Second = score_number_left_Second + ((score_number_left - 4) % 1000) / 100
					+ ((score_number_left - 4) % 100) / 10 + (score_number_left - 4) % 10;
			}
			else if (score_number_left >= 1000004 && score_number_left < 10000004)
			{
				score_number_left_Second = (score_number_left - 4) / 1000000 + ((score_number_left - 4) % 1000000) / 100000
					+ ((score_number_left - 4) % 100000) / 10000;
				score_number_left_Second = score_number_left_Second + ((score_number_left - 4) % 10000) / 1000
					+ ((score_number_left - 4) % 1000) / 100;
				score_number_left_Second = score_number_left_Second + ((score_number_left - 4) % 100) / 10 + (score_number_left - 4) % 10;
			}
			else if (score_number_left >= 10000004 && score_number_left < 100000004)
			{
				score_number_left_Second = (score_number_left - 4) / 10000000 + ((score_number_left - 4) % 10000000) / 1000000
					+ ((score_number_left - 4) % 1000000) / 100000;
				score_number_left_Second = score_number_left_Second + ((score_number_left - 4) % 100000) / 10000 +
					((score_number_left - 4) % 10000) / 1000 + ((score_number_left - 4) % 1000) / 100;
				score_number_left_Second = score_number_left_Second + ((score_number_left - 4) % 100) / 10 + (score_number_left - 4) % 10;
			}

			if (score_number_left > 4)
			{
				//分数从左边到右边重复的时候，分数循环
				schedule(schedule_selector(MainScene_fruit::continueScoreJettonTwo), 0.04f, score_number_left_Second, 0.3f);
			}
		}

		//如果分数不为0，则继续比大小的操作
		else
		{
			SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y202.mp3").c_str());
			scoreTopLeft->setString(__String::createWithFormat("%lld", rightScoreChange)->getCString());
			scoreTopRight->setString(__String::createWithFormat("%lld", wCurrentGameButtonRightScore)->getCString());
		}
		if (isGo == true)
		{
			JettonRepeat();
		}
	}

	void MainScene_fruit::continueScoreJetton(float dt)
	{
		LONGLONG score_number_left = std::atoll(scoreTopLeft->getString().c_str()) - 1;
		scoreTopLeft->setString(__String::createWithFormat("%lld", score_number_left)->getCString());
		LONGLONG score_number_Right = std::atoll(scoreTopRight->getString().c_str()) + 1;
		scoreTopRight->setString(__String::createWithFormat("%lld", score_number_Right)->getCString());
	}

	//处理分数循环·递减
	void MainScene_fruit::continueScoreJettonTwo(float dt)
	{
		if (std::atoll(scoreTopLeft->getString().c_str()) > 100000000)
		{
			LONGLONG score_number_left = std::atoll(scoreTopLeft->getString().c_str()) - 100000000;
			scoreTopLeft->setString(__String::createWithFormat("%lld", score_number_left)->getCString());
			LONGLONG score_number_Right = std::atoll(scoreTopRight->getString().c_str()) + 100000000;
			scoreTopRight->setString(__String::createWithFormat("%lld", score_number_Right)->getCString());
		}
		else if (std::atoll(scoreTopLeft->getString().c_str()) > 10000000)
		{
			LONGLONG score_number_left = std::atoll(scoreTopLeft->getString().c_str()) - 10000000;
			scoreTopLeft->setString(__String::createWithFormat("%lld", score_number_left)->getCString());
			LONGLONG score_number_Right = std::atoll(scoreTopRight->getString().c_str()) + 10000000;
			scoreTopRight->setString(__String::createWithFormat("%lld", score_number_Right)->getCString());
		}
		else if (std::atoll(scoreTopLeft->getString().c_str()) > 1000000)
		{
			LONGLONG score_number_left = std::atoll(scoreTopLeft->getString().c_str()) - 1000000;
			scoreTopLeft->setString(__String::createWithFormat("%lld", score_number_left)->getCString());
			LONGLONG score_number_Right = std::atoll(scoreTopRight->getString().c_str()) + 1000000;
			scoreTopRight->setString(__String::createWithFormat("%lld", score_number_Right)->getCString());
		}
		else if (std::atoll(scoreTopLeft->getString().c_str()) > 100000)
		{
			LONGLONG score_number_left = std::atoll(scoreTopLeft->getString().c_str()) - 100000;
			scoreTopLeft->setString(__String::createWithFormat("%lld", score_number_left)->getCString());
			LONGLONG score_number_Right = std::atoll(scoreTopRight->getString().c_str()) + 100000;
			scoreTopRight->setString(__String::createWithFormat("%lld", score_number_Right)->getCString());
		}
		else if (std::atoll(scoreTopLeft->getString().c_str()) > 10000)
		{
			LONGLONG score_number_left = std::atoll(scoreTopLeft->getString().c_str()) - 10000;
			scoreTopLeft->setString(__String::createWithFormat("%lld", score_number_left)->getCString());
			LONGLONG score_number_Right = std::atoll(scoreTopRight->getString().c_str()) + 10000;
			scoreTopRight->setString(__String::createWithFormat("%lld", score_number_Right)->getCString());
		}
		else if (std::atoll(scoreTopLeft->getString().c_str()) > 1000)
		{
			LONGLONG score_number_left = std::atoll(scoreTopLeft->getString().c_str()) - 1000;
			scoreTopLeft->setString(__String::createWithFormat("%lld", score_number_left)->getCString());
			LONGLONG score_number_Right = std::atoll(scoreTopRight->getString().c_str()) + 1000;
			scoreTopRight->setString(__String::createWithFormat("%lld", score_number_Right)->getCString());
		}
		else if (std::atoll(scoreTopLeft->getString().c_str()) > 100)
		{
			LONGLONG score_number_left = std::atoll(scoreTopLeft->getString().c_str()) - 100;
			scoreTopLeft->setString(__String::createWithFormat("%lld", score_number_left)->getCString());
			LONGLONG score_number_Right = std::atoll(scoreTopRight->getString().c_str()) + 100;
			scoreTopRight->setString(__String::createWithFormat("%lld", score_number_Right)->getCString());
		}
		else if (std::atoll(scoreTopLeft->getString().c_str()) > 10)
		{
			LONGLONG score_number_left = std::atoll(scoreTopLeft->getString().c_str()) - 10;
			scoreTopLeft->setString(__String::createWithFormat("%lld", score_number_left)->getCString());
			LONGLONG score_number_Right = std::atoll(scoreTopRight->getString().c_str()) + 10;
			scoreTopRight->setString(__String::createWithFormat("%lld", score_number_Right)->getCString());
		}
		else
		{
			LONGLONG score_number_left = std::atoll(scoreTopLeft->getString().c_str()) - 1;
			scoreTopLeft->setString(__String::createWithFormat("%lld", score_number_left)->getCString());
			LONGLONG score_number_Right = std::atoll(scoreTopRight->getString().c_str()) + 1;
			scoreTopRight->setString(__String::createWithFormat("%lld", score_number_Right)->getCString());
		}
	}

	void MainScene_fruit::continueScoreJettonFour(float dt)
	{
		SoundManager::instance()->playEffect(SETPATH("fruit/sounds/repeat.mp3").c_str());
	}

	void MainScene_fruit::subButtonLeft(CMD_S_ButtonLeft& end)
	{
		HallSoundManager::instance()->stopAllEffect();
		SCORE LeftScoreChange = end.lUserLeftScore;
		SCORE LeftScoreAll = end.LChangeScore;
		SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y202.mp3").c_str());
		scoreTopLeft->setString(__String::createWithFormat("%lld", LeftScoreChange)->getCString());
		scoreTopRight->setString(__String::createWithFormat("%lld", LeftScoreAll)->getCString());
	}

	void MainScene_fruit::guessCompareone(float dt)
	{
		int itemp_guess_number = rand() % 14;
		SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y010.mp3").c_str());
		timeLabel_Mid->setString(__String::createWithFormat("%d", itemp_guess_number)->getCString());
	}

	void MainScene_fruit::guessComparetwo(float dt)
	{
		int itemp_guess_number = rand() % 14;
		SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y010.mp3").c_str());
		timeLabel_Mid->setString(__String::createWithFormat("%d", itemp_guess_number)->getCString());
	}

	void MainScene_fruit::guessComparethrid(float dt)
	{
		timeLabel_Mid->setString(__String::createWithFormat("%d", wCurrentGameGuess.nGuessNum)->getCString());
		is_guess_visible = is_guess_visible ? false : true;
		timeLabel_Mid->setVisible(is_guess_visible);

		//显示最后的得分
		scoreTopLeft->setString(__String::createWithFormat("%lld", wCurrentGameGuess.lUserReturnScore)->getCString());
		if (itemp_guess_numberResult == 6)
		{
			if (wCurrentGameGuess.lUserReturnScore == 0)
			{
				SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Ypity.mp3").c_str());
			}
			else
			{
				SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Yzhongle.mp3").c_str());
				schedule(schedule_selector(MainScene_fruit::guessWinSong), 0, 0, 1.0f);
			}
		}
		if (itemp_guess_numberResult == 0)
		{
			SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y008.mp3").c_str());
		}
		itemp_guess_numberResult++;
	}

	void MainScene_fruit::guessWinSong(float dt)
	{
		schedule(schedule_selector(MainScene_fruit::sideLight), 0.35f);
		SoundManager::instance()->playEffect(SETPATH("fruit/sounds/C11.mp3").c_str());
	}

	void MainScene_fruit::guessComparefinal(float dt)
	{
		is_guess_useable = true;
		if (wCurrentGameGuess.lUserReturnScore == 0)
		{
			SendGameMsg::sendStartOrStop(NetworkManager::FLAG_GAME, false);
			isStart = true;
		}
	}

	void MainScene_fruit::subCompare(CMD_S_Guess& end)
	{
		wCurrentGameGuess = end;
		if (wCurrentGameGuess.nGuessNum == 0)
		{
			schedule(schedule_selector(MainScene_fruit::guessComparefinal), 0.0f, 0, 0.0);
		}
		else
		{
			itemp_guess_numberResult = 0;
			schedule(schedule_selector(MainScene_fruit::guessCompareone), 0.05f, 20, 0.0f);
			schedule(schedule_selector(MainScene_fruit::guessComparetwo), 0.2f, 4, 1.0f);
			schedule(schedule_selector(MainScene_fruit::guessComparethrid), 0.2f, 6, 2.0f);
			schedule(schedule_selector(MainScene_fruit::guessComparefinal), 0.2f, 0, 4.5f);
		}
	}

	MainScene_fruit * MainScene_fruit::instance = nullptr;
	MainScene_fruit * MainScene_fruit::createScene()
	{
		if (!instance)
		{
			instance = MainScene_fruit::create();
		}
		return instance;
	}

	void MainScene_fruit::lightTwinkle(float dt)
	{
		blockLight[init_ResultNumber[0] - 1]->setVisible(init_trut_or_false_Light);
		init_trut_or_false_Light = init_trut_or_false_Light ? false : true;
	}

	void MainScene_fruit::showScoreOneSounds(float dt)
	{
		int itemp_voice = changeVoice(wCurrentGameEnd.cbTableCardArray[1]);
		SoundManager::instance()->playEffect(__String::createWithFormat(SETPATH("fruit/sounds/Y10%d.mp3").c_str(), itemp_voice)->getCString());
	}

	void MainScene_fruit::showScoreTwoSounds(float dt)
	{
		int itemp_voice = changeVoice(wCurrentGameEnd.cbTableCardArray[2]);
		SoundManager::instance()->playEffect(__String::createWithFormat(SETPATH("fruit/sounds/Y10%d.mp3").c_str(), itemp_voice)->getCString());
	}

	void MainScene_fruit::showScoreThirdSounds(float dt)
	{
		int itemp_voice = changeVoice(wCurrentGameEnd.cbTableCardArray[3]);
		SoundManager::instance()->playEffect(__String::createWithFormat(SETPATH("fruit/sounds/Y10%d.mp3").c_str(), itemp_voice)->getCString());
	}

	void MainScene_fruit::showScoreFourthSounds(float dt)
	{
		int itemp_voice = changeVoice(wCurrentGameEnd.cbTableCardArray[4]);
		SoundManager::instance()->playEffect(__String::createWithFormat(SETPATH("fruit/sounds/Y10%d.mp3").c_str(), itemp_voice)->getCString());
	}

	void MainScene_fruit::rotateLightLuckyFirst(float dt)
	{
		//-----第一个中奖逻辑---
		blockLight[(itemp_gun_number) % 24]->setVisible(true);
		blockLight[(itemp_gun_number + 1) % 24]->setVisible(false);

		//------闪烁的灯-------
		init_trut_or_false_Light = init_trut_or_false_Light ? false : true;
		blockLight[init_ResultNumber[0] - 1]->setVisible(init_trut_or_false_Light);
		itemp_gun_number--;
		itemp_rotate_count++;

		if (itemp_rotate_count == 1)
		{
			SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y_luckyOne.mp3").c_str());
		}
		if (itemp_rotate_count == itemp_rotate_count_first + 1)
		{
			itemp_rotate_count = 0;
			blockLight[init_ResultNumber[0] - 1]->setVisible(true);
			blockLight[init_ResultNumber[1] - 1]->setVisible(true);
			light_right[changeVoice(wCurrentGameEnd.cbTableCardArray[1]) - 1]->setVisible(true);

			HallSoundManager::instance()->stopAllEffect();

			schedule(schedule_selector(MainScene_fruit::continueZha), 0.0f, 0, 0.2f);
			int number = itemp_myJettonLabel[changeVoice(init_ResultNumber[1]) - 1];
			SCORE LeftOne = rotateScore(init_ResultNumber[1]) * score_rank *number;
			scoreTopLeft->setString(__String::createWithFormat("%lld", LeftOne)->getCString());

			if (wCurrentGameEnd.nProbability == 1 && wCurrentGameEnd.nRand == 1) //打枪
			{
				SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Ydaqiang.mp3").c_str());
				schedule(schedule_selector(MainScene_fruit::showScoreOneSounds), 0.0f, 0, 1.0f);
				schedule(schedule_selector(MainScene_fruit::showScoreLeft), 0.0f, 0, 2.0f);
			}
			else
			{
				visible_twinkle_first = true;
				schedule(schedule_selector(MainScene_fruit::showSoundsTwo), 0.05f, 40, 0.3f);
				schedule(schedule_selector(MainScene_fruit::rotateLightLuckySecond), 0.05f, itemp_rotate_count_second, 2.2f);
			}
		}
		
	}

	void MainScene_fruit::rotateLightLuckySecond(float dt)
	{
		blockLight[(itemp_gun_second_number) % 24]->setVisible(true);
		if (itemp_gun_second_number != 0)
		{
			blockLight[(itemp_gun_second_number - 1) % 24]->setVisible(false);
		}

		//------闪烁的灯-------
		init_trut_or_false_Light = init_trut_or_false_Light ? false : true;
		blockLight[init_ResultNumber[0] - 1]->setVisible(init_trut_or_false_Light);
		blockLight[init_ResultNumber[1] - 1]->setVisible(init_trut_or_false_Light);
		itemp_gun_second_number++;
		itemp_rotate_count++;
		if (itemp_rotate_count == 1)
		{
			SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y_luckyTwo.mp3").c_str());
		}
		if (itemp_rotate_count == itemp_rotate_count_second + 1)
		{
			itemp_rotate_count = 0;
			blockLight[init_ResultNumber[0] - 1]->setVisible(true);
			blockLight[init_ResultNumber[1] - 1]->setVisible(true);
			blockLight[init_ResultNumber[2] - 1]->setVisible(true);
			light_right[changeVoice(wCurrentGameEnd.cbTableCardArray[2]) - 1]->setVisible(true);

			HallSoundManager::instance()->stopAllEffect();

			schedule(schedule_selector(MainScene_fruit::continueZha), 0.0f, 0, 0.2f);
			int number = itemp_myJettonLabel[changeVoice(init_ResultNumber[2]) - 1];
			SCORE leftLast = std::atoll(scoreTopLeft->getString().c_str());
			SCORE LeftTwo = rotateScore(init_ResultNumber[2]) * score_rank *number;
			scoreTopLeft->setString(__String::createWithFormat("%lld", LeftTwo + leftLast)->getCString());
			if (wCurrentGameEnd.nProbability == 1 && wCurrentGameEnd.nRand == 2) //打枪
			{
				SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Ydaqiang.mp3").c_str());
				schedule(schedule_selector(MainScene_fruit::showScoreOneSounds), 0.0f, 0, 1.0f);
				schedule(schedule_selector(MainScene_fruit::showScoreTwoSounds), 0.0f, 0, 2.0f);
				schedule(schedule_selector(MainScene_fruit::showScoreLeft), 0.0f, 0, 3.0f);
			}
			else
			{
				visible_twinkle_second = true;
				schedule(schedule_selector(MainScene_fruit::showSoundsThrid), 0.05f, 40, 0.3f);
				schedule(schedule_selector(MainScene_fruit::rotateLightLuckyThird), 0.05f, itemp_rotate_count_third, 2.3f);
			}
		}
	}

	void MainScene_fruit::rotateLightLuckyThird(float dt)
	{
		blockLight[(itemp_gun_third_number) % 24]->setVisible(true);
		blockLight[(itemp_gun_third_number + 1) % 24]->setVisible(false);

		//------闪烁的灯-------
		init_trut_or_false_Light = init_trut_or_false_Light ? false : true;
		blockLight[init_ResultNumber[0] - 1]->setVisible(init_trut_or_false_Light);
		blockLight[init_ResultNumber[1] - 1]->setVisible(init_trut_or_false_Light);
		blockLight[init_ResultNumber[2] - 1]->setVisible(init_trut_or_false_Light);
		itemp_gun_third_number--;
		itemp_rotate_count++;

		if (itemp_rotate_count == 1)
		{
			SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y_luckyThird.mp3").c_str());
		}
		if (itemp_rotate_count == itemp_rotate_count_third + 1)
		{
			itemp_rotate_count = 0;
			blockLight[init_ResultNumber[0] - 1]->setVisible(true);
			blockLight[init_ResultNumber[1] - 1]->setVisible(true);
			blockLight[init_ResultNumber[2] - 1]->setVisible(true);
			blockLight[init_ResultNumber[3] - 1]->setVisible(true);
			light_right[changeVoice(wCurrentGameEnd.cbTableCardArray[3]) - 1]->setVisible(true);

			HallSoundManager::instance()->stopAllEffect();

			schedule(schedule_selector(MainScene_fruit::continueZha), 0.0f, 0, 0.2f);

			int number = itemp_myJettonLabel[changeVoice(init_ResultNumber[3]) - 1];
			SCORE leftLast = std::atoll(scoreTopLeft->getString().c_str());
			SCORE LeftThird = rotateScore(init_ResultNumber[3]) * score_rank *number;
			scoreTopLeft->setString(__String::createWithFormat("%lld", LeftThird + leftLast)->getCString());

			if (wCurrentGameEnd.nProbability == 3) //小三元
			{
				SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Yxiaosanyuan.mp3").c_str());
				schedule(schedule_selector(MainScene_fruit::showScoreLeft), 0.0f, 0, 1.0f);
			}
			else if (wCurrentGameEnd.nProbability == 2) //大三元
			{
				SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Ydasanyuan.mp3").c_str());
				schedule(schedule_selector(MainScene_fruit::showScoreLeft), 0.0f, 0, 1.0f);
			}
			else if (wCurrentGameEnd.nProbability == 1 && wCurrentGameEnd.nRand == 3) //打枪
			{
				SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Ydaqiang.mp3").c_str());
				schedule(schedule_selector(MainScene_fruit::showScoreOneSounds), 0.0f, 0, 1.0f);
				schedule(schedule_selector(MainScene_fruit::showScoreTwoSounds), 0.0f, 0, 2.0f);
				schedule(schedule_selector(MainScene_fruit::showScoreThirdSounds), 0.0f, 0, 3.0f);
				schedule(schedule_selector(MainScene_fruit::showScoreLeft), 0.0f, 0, 3.0f);
			}
			else
			{
				visible_twinkle_first = true;
				schedule(schedule_selector(MainScene_fruit::showSoundsThourth), 0.05f, 40, 0.3f);
				schedule(schedule_selector(MainScene_fruit::rotateLightLuckyFourth), 0.05f, itemp_rotate_count_fourth, 2.2f);
			}
		}
	}

	void MainScene_fruit::rotateLightLuckyFourth(float dt)
	{
		blockLight[(itemp_gun_fourth_number) % 24]->setVisible(true);
		if (itemp_gun_second_number != 0)
		{
			blockLight[(itemp_gun_fourth_number - 1) % 24]->setVisible(false);
		}

		//------闪烁的灯-------
		init_trut_or_false_Light = init_trut_or_false_Light ? false : true;
		blockLight[init_ResultNumber[0] - 1]->setVisible(init_trut_or_false_Light);
		blockLight[init_ResultNumber[1] - 1]->setVisible(init_trut_or_false_Light);
		blockLight[init_ResultNumber[2] - 1]->setVisible(init_trut_or_false_Light);
		blockLight[init_ResultNumber[3] - 1]->setVisible(init_trut_or_false_Light);
		itemp_gun_fourth_number++;
		itemp_rotate_count++;

		if (itemp_rotate_count == 1)
		{
			SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y_luckyFourth.mp3").c_str());
		}
		if (itemp_rotate_count == itemp_rotate_count_fourth + 1)
		{
			itemp_rotate_count = 0;
			HallSoundManager::instance()->stopAllEffect();
			blockLight[init_ResultNumber[0] - 1]->setVisible(true);
			blockLight[init_ResultNumber[1] - 1]->setVisible(true);
			blockLight[init_ResultNumber[2] - 1]->setVisible(true);
			blockLight[init_ResultNumber[3] - 1]->setVisible(true);
			light_right[changeVoice(wCurrentGameEnd.cbTableCardArray[4]) - 1]->setVisible(true);
			schedule(schedule_selector(MainScene_fruit::continueZha), 0.0f, 0, 0.2f);
			int number = itemp_myJettonLabel[changeVoice(init_ResultNumber[4]) - 1];
			SCORE leftLast = std::atoll(scoreTopLeft->getString().c_str());
			SCORE LeftFourth = rotateScore(init_ResultNumber[4]) * score_rank *number;
			scoreTopLeft->setString(__String::createWithFormat("%lld", LeftFourth + leftLast)->getCString());
			if (wCurrentGameEnd.nProbability == 4)
			{
				SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Ydasixi.mp3").c_str());
				schedule(schedule_selector(MainScene_fruit::showScoreLeft), 0.0f, 0, 1.0f);
			}
			else if (wCurrentGameEnd.nProbability == 1 && wCurrentGameEnd.nRand == 4) //打枪
			{
				SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Ydaqiang.mp3").c_str());
				schedule(schedule_selector(MainScene_fruit::showScoreOneSounds), 0.0f, 0, 1.3f);
				schedule(schedule_selector(MainScene_fruit::showScoreTwoSounds), 0.0f, 0, 2.3f);
				schedule(schedule_selector(MainScene_fruit::showScoreThirdSounds), 0.0f, 0, 3.0f);
				schedule(schedule_selector(MainScene_fruit::showScoreFourthSounds), 0.0f, 0, 4.0f);
				schedule(schedule_selector(MainScene_fruit::showScoreLeft), 0.0f, 0, 4.0f);
			}
		}
	}

	void MainScene_fruit::continueZha(float dt)
	{
		SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Yzha.mp3").c_str());
	}

	void MainScene_fruit::showSounds(float dt)
	{
		if (wCurrentGameEnd.nProbability == 5)
		{
			SoundManager::instance()->playEffect(SETPATH("fruit/sounds/lucky_null.mp3").c_str());
		}
		else
		{
			SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y_twinkin1.mp3").c_str());
		}
	}

	void MainScene_fruit::showSoundsTwo(float dt)
	{
		if (visible_twinkle_first == true)
		{
			SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y_twinkin2.mp3").c_str());
			visible_twinkle_first = false;
		}
		blockLight[init_ResultNumber[1] - 1]->setVisible(visible_twinkle);
		visible_twinkle = visible_twinkle ? false : true;
	}

	void MainScene_fruit::showSoundsThrid(float dt)
	{
		if (visible_twinkle_second == true)
		{
			SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y_twinkin3.mp3").c_str());
			visible_twinkle_second = false;
		}
		blockLight[init_ResultNumber[1] - 1]->setVisible(true);
		blockLight[init_ResultNumber[2] - 1]->setVisible(visible_twinkle);
		visible_twinkle = visible_twinkle ? false : true;
	}

	void MainScene_fruit::showSoundsThourth(float dt)
	{
		if (visible_twinkle_first == true)
		{
			SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y_twinkin4.mp3").c_str());
			visible_twinkle_first = false;
		}
		blockLight[init_ResultNumber[2] - 1]->setVisible(true);
		blockLight[init_ResultNumber[3] - 1]->setVisible(visible_twinkle);
		visible_twinkle = visible_twinkle ? false : true;
	}

	//显示左边的分数
	void MainScene_fruit::showScoreLeft(float dt)
	{

		scoreTopLeft->setString(__String::createWithFormat("%lld", wCurrentGameEnd.lUserReturnScore[0])->getCString());
		if (wCurrentGameEnd.lUserReturnScore[0] == 0)
		{
			blockSideLight->setVisible(false);
			blockSideLightTwo->setVisible(false);
			SendGameMsg::sendStartOrStop(NetworkManager::FLAG_GAME, isStart);
			isStart = true;
		}
		else
		{
			if (wCurrentGameEnd.cbTableCardArray[0] != 10 && wCurrentGameEnd.cbTableCardArray[0] != 22)
			{
				int itemp_voice = changeVoice(wCurrentGameEnd.cbTableCardArray[0]);
				SoundManager::instance()->playEffect(
					__String::createWithFormat(SETPATH("fruit/sounds/Y10%d.mp3").c_str(), itemp_voice)->getCString());//这里添加声音
			}
			schedule(schedule_selector(MainScene_fruit::sideLight), 0.35f);
			if (isGo == true)
			{
				schedule(schedule_selector(MainScene_fruit::showScoreLater), 0.0f, 0, 3.0f);
			}
			else
			{
				schedule(schedule_selector(MainScene_fruit::showScoreLater), 0.0f, 0, 0.8f);
			}
			
			SoundManager::instance()->playEffect(SETPATH("fruit/sounds/C02.mp3").c_str());
		}

	}

	void MainScene_fruit::showScoreLater(float dt)
	{
		is_guess_useable = true; //猜大小状态
		if (wCurrentGameEnd.lUserReturnScore[0] == 0)
		{
			isStart = true;
		}
		else
		{
			wCurrentTimeType = TimeType_fruit_Leisure;
		}
		if (isGo == true)
		{
			JettonRepeat();
		}
	}

	int MainScene_fruit::changeVoice(int voice)
	{
		switch (voice)
		{
		case 5: case 6: case 11: case 17: case 23:
		{
					return 1;
		}
			break;
		case 1: case 12: case 13:
		{
					return 2;
		}
			break;
		case 7: case 18: case 19:
		{
					return 3;
		}
			break;
		case 2: case 14: case 24:
		{
					return 4;
		}
			break;
		case 8: case 9:
		{
					return 5;
		}
			break;
		case 20: case 21:
		{
					 return 6;
		}
			break;
		case 15: case 16:
		{
					 return 7;
		}
			break;
		case 3: case 4:
		{
					return 8;
		}
		default:
			break;
		}
	}

	//rotate
	int MainScene_fruit::rotateScore(int voice)
	{
		switch (voice)
		{
		case 5: case 11: case 17: case 23:
		{
					return 5;
		}
			break;
		case 1:  case 13:
		{
					 return 10;
		}
			break;
		case 7:  case 19:
		{
					 return 15;
		}
			break;
		case 2: case 14: case 8:
		{
					return 20;
		}
			break;
		case 20:
		{
				   return 30;
		}
			break;
		case 16:
		{
				   return 40;
		}
			break;
		case 3:
		{
				  return 50;
		}
			break;
		case 4:
		{
				  return 100;
		}
			break;
		case 6:case 12:case 18:case 24:case 9:case 15: case 21:
		{
				   return 3;
		}
		default:
			break;
		}
	}

	//开奖
	void MainScene_fruit::startMoveBlock(CMD_S_GameEnd& gameEnd)
	{
		//选装声音
		SoundManager::instance()->playEffect(SETPATH("fruit/sounds/all.mp3").c_str());
		timeLabel_Mid->setString("00"); //猜大小状态至为00
		is_guess_useable = false; //猜大小状态
		rand_y = 56 + gameEnd.cbTableCardArray[0];
		if (wCurrentGameEnd.cbTableCardArray[0] != 0)
		{
			if (wCurrentGameEnd.cbTableCardArray[0] != 10 && wCurrentGameEnd.cbTableCardArray[0] != 22)
			{
				itemp_number = wCurrentGameEnd.cbTableCardArray[0] - 1;
				itemp_numberLightRotate = changeVoice(wCurrentGameEnd.cbTableCardArray[0]) - 1;
				rand_y = 56 + gameEnd.cbTableCardArray[0] + 24 - itemp_number;
				if (rand_y > 78)
				{
					rand_y = rand_y - 24;
				}
			}
			else
			{
				for (int i = 0; i <5; i++)
				{
					if (wCurrentGameEnd.cbTableCardArray[i] != 0)
					{
						itemp_number = wCurrentGameEnd.cbTableCardArray[i] - 1;
						itemp_numberLightRotate = changeVoice(wCurrentGameEnd.cbTableCardArray[i]) - 1;
						rand_y = 56 + gameEnd.cbTableCardArray[0] + 24 - itemp_number;
						if (rand_y>78)
						{
							rand_y = rand_y - 24;
						}
					}
				}
			}
		}
		wCurrentGameEnd = gameEnd;
		itemp_gun_count = gameEnd.nRand;
		for (int itemp_numberReward = 0; itemp_numberReward < 5; itemp_numberReward++)
		{
			init_ResultNumber[itemp_numberReward] = gameEnd.cbTableCardArray[itemp_numberReward];
		}
		//开牌旋转
		rand_x = 6;

		schedule(schedule_selector(MainScene_fruit::rotateLight), 0.2f, 8, 0.0f);
		schedule(schedule_selector(MainScene_fruit::rotateLightTwo), 0.016f, rand_y, 1.6f);
		schedule(schedule_selector(MainScene_fruit::rotateLightThree), 0.29f, 5, 2.8f);
		if (init_ResultNumber[0] != 10 && init_ResultNumber[0] != 22)
		{
			schedule(schedule_selector(MainScene_fruit::showScoreLeft), 0.0f, 0, 4.7f);
		}
		else
		{
			itemp_rotate_count_first = init_ResultNumber[1] < init_ResultNumber[0] ? init_ResultNumber[0] - init_ResultNumber[1] : init_ResultNumber[0] + (24 - init_ResultNumber[1]);
			itemp_rotate_count_second = init_ResultNumber[2] < init_ResultNumber[1] ? init_ResultNumber[2] + (24 - init_ResultNumber[1]) : init_ResultNumber[2] - init_ResultNumber[1];
			itemp_rotate_count_third = init_ResultNumber[3] < init_ResultNumber[2] ? init_ResultNumber[2] - init_ResultNumber[3] : init_ResultNumber[2] + (24 - init_ResultNumber[3]);
			itemp_rotate_count_fourth = init_ResultNumber[4] < init_ResultNumber[3] ? init_ResultNumber[4] + (24 - init_ResultNumber[3]) : init_ResultNumber[4] - init_ResultNumber[3];
			itemp_rotate_count = 0;
			itemp_gun_number = init_ResultNumber[0] + 23;
			itemp_gun_second_number = init_ResultNumber[1] - 1;
			itemp_gun_third_number = init_ResultNumber[2] + 23;
			itemp_gun_fourth_number = init_ResultNumber[3] - 1;
			schedule(schedule_selector(MainScene_fruit::lightTwinkle), 0.05f, 30, 4.8f);
			if (gameEnd.nProbability == 5) //通吃
			{
				schedule(schedule_selector(MainScene_fruit::showSounds), 0.0f, 0, 4.8f);
				schedule(schedule_selector(MainScene_fruit::showScoreLeft), 0.0f, 0, 7.0f);
				return;
			}
			if (gameEnd.nProbability == 1) //打枪
			{
				switch (gameEnd.nRand)
				{
				case 1:
				{
						  schedule(schedule_selector(MainScene_fruit::showSounds), 0.0f, 0, 4.8f);
						  schedule(schedule_selector(MainScene_fruit::rotateLightLuckyFirst), 0.05f, itemp_rotate_count_first, 7.0f);
				}
					break;
				case 2:
				{
						  schedule(schedule_selector(MainScene_fruit::showSounds), 0.0f, 0, 4.8f);
						  schedule(schedule_selector(MainScene_fruit::rotateLightLuckyFirst), 0.05f, itemp_rotate_count_first, 7.0f);
				}
					break;
				case 3:
				{
						  schedule(schedule_selector(MainScene_fruit::showSounds), 0.0f, 0, 4.8f);
						  schedule(schedule_selector(MainScene_fruit::rotateLightLuckyFirst), 0.05f, itemp_rotate_count_first, 6.0f);
				}
					break;
				case 4:
				{
						  schedule(schedule_selector(MainScene_fruit::showSounds), 0.0f, 0, 4.8f);
						  schedule(schedule_selector(MainScene_fruit::rotateLightLuckyFirst), 0.05f, itemp_rotate_count_first, 6.0f);
				}
				default:
					break;
				}
			}
			if (gameEnd.nProbability == 2) //大三元
			{
				schedule(schedule_selector(MainScene_fruit::showSounds), 0.0f, 0, 4.8f);
				schedule(schedule_selector(MainScene_fruit::rotateLightLuckyFirst), 0.05f, itemp_rotate_count_first, 6.0f);
			}
			if (gameEnd.nProbability == 3) //小三元
			{
				schedule(schedule_selector(MainScene_fruit::showSounds), 0.0f, 0, 4.8f);
				schedule(schedule_selector(MainScene_fruit::rotateLightLuckyFirst), 0.05f, itemp_rotate_count_first, 6.0f);
			}
			if (gameEnd.nProbability == 4) //大四喜
			{
				schedule(schedule_selector(MainScene_fruit::showSounds), 0.0f, 0, 4.8f);
				schedule(schedule_selector(MainScene_fruit::rotateLightLuckyFirst), 0.05f, itemp_rotate_count_first, 6.0f);
			}
		}
	}

	void MainScene_fruit::continueUpJetton(float dt)
	{
		if (jessonUpNumber != 0)
		{
			JettonArea(jessonUpNumber);
		}
	}

	void MainScene_fruit::continueUpJettonAll(float dt)
	{
		if (wCurrentTimeType == TimeType_fruit_PlaceJetton)
		{
			CMD_C_PlaceJettons repeatAllPlaceJetton;
			memset(&repeatAllPlaceJetton, 0, sizeof(CMD_C_PlaceJettons));
			if (std::atoll(scoreTopRight->getString().c_str()) < 8 * score_rank)
			{
				return;
			}
			for (int i = 1; i < 9; i++)
			{
				if (itemp_myJettonLabel[i - 1] < 99)
				{
					repeatAllPlaceJetton.lJettonScore[i] = score_rank;
				}
			}
			NetworkManager::getInstance()->sendData(NetworkManager::FLAG_GAME, MDM_GF_GAME, SUB_C_REPEAT_JETTON,
				&repeatAllPlaceJetton, sizeof(CMD_C_PlaceJettons));
		}
	}

	void MainScene_fruit::continueRightJetton(float dt)
	{
		if (wCurrentTimeType == TimeType_fruit_Leisure && std::atoi(scoreTopLeft->getString().c_str()) > 0)
		{
			NetworkManager::getInstance()->sendData(NetworkManager::FLAG_GAME, MDM_GF_GAME, SUB_C_BUTTON_RIGHT, NULL, 0);
		}
	}

	//
	void MainScene_fruit::continueGoButton(float dt)
	{
		if (isGo == false)
		{
			if (isGo_state == 1)
			{
				isGo = false;
				goFrame->setVisible(false);
				isGo_state = 0;
			}
			else
			{
				isGo = true;
				anniu_go->loadTextures(SETPATH("fruit/Button/btn_go_6.png"), SETPATH("fruit/Button/btn_go_5.png"));
			}
		}
	}

	void MainScene_fruit::onTouch(Ref* target, Widget::TouchEventType type)
	{
		Button *btn = (Button *)target;
		if (Widget::TouchEventType::BEGAN == type)
		{
			int itemp_tag = btn->getTag();
			switch (itemp_tag)
			{
			case 101: case 102: case 103:case 104:case 105:case 106:case 107:case 108:
			{
						  jessonUpNumber = itemp_tag - 100;
						  JettonArea(jessonUpNumber);
						  schedule(schedule_selector(MainScene_fruit::continueUpJetton), 0.1f, 97, 0.6f);
			}
				break;
			case 48:  //ALL按钮，全部下注一遍
			{
						  continueUpJettonAll(0.0f);
						  schedule(schedule_selector(MainScene_fruit::continueUpJettonAll), 0.1f, 98, 0.6f);
			}
				break;
			case 50://右移按钮
			{
						continueRightJetton(0.0f);
						schedule(schedule_selector(MainScene_fruit::continueRightJetton), 0.1f, 98, 0.6f);
			}
				break;
			case 53:  //go按钮
			{
						  if (wCurrentTimeType == TimeType_fruit_PlaceJetton)
						  {
							  if (isDataNull(itemp_myJettonLabel, 8) || isDataNull(myRepeatJettonLabel, 8))
							  {
								  schedule(schedule_selector(MainScene_fruit::continueGoButton), 0.0f, 0, 1.0f);
							  }

						  }
						  if (isGo == true)
						  {
							  isGo = false;
							  anniu_go->loadTextures(SETPATH("fruit/Button/btn_go_2.png"), SETPATH("fruit/Button/btn_go_1.png"));
							  goFrame->setVisible(false);
						  }
						  isGo_state = 0;
			}
				break;
			default:
				break;
			}
		}
		else if (Widget::TouchEventType::MOVED == type)
		{

			switch (btn->getTag())
			{
			case 101: case 102: case 103: case 104: case 105: case 106: case 107: case 108: case 48: case 50:
			{
						  bool is_point = btn->boundingBox().containsPoint(btn->getTouchMovePos());
						  if (!is_point)
						  {
							  if (btn->getTag() == 48)
							  {
								  unschedule(schedule_selector(MainScene_fruit::continueUpJettonAll));
							  }
							  else if (btn->getTag() == 50)
							  {
								  unschedule(schedule_selector(MainScene_fruit::continueUpJettonAll));
							  }
							  else
							  {
								  unschedule(schedule_selector(MainScene_fruit::continueUpJetton));
							  }
						  }
			}
				break;
			
			case 53://go按钮
			{
						bool is_point = btn->boundingBox().containsPoint(btn->getTouchMovePos());
						if (!is_point)
						{
							isGo_state = 1;
							anniu_go->loadTextures(SETPATH("fruit/Button/btn_go_2.png"), SETPATH("fruit/Button/btn_go_1.png"));
							goFrame_1->setVisible(false);
						}
			}
				break;
			default:
				break;
			}
		}
		else if (Widget::TouchEventType::CANCELED == type)
		{
			switch (btn->getTag())
			{
			case 101: case 102: case 103:case 104:case 105:case 106:case 107:case 108:
			{
						  unschedule(schedule_selector(MainScene_fruit::continueUpJetton));
			}
				break;
			case 48:  //ALL按钮，停止下注
			{
						  unschedule(schedule_selector(MainScene_fruit::continueUpJettonAll));
			}
				break;
			case 50://右移按钮
			{
						unschedule(schedule_selector(MainScene_fruit::continueRightJetton));
			}
				break;
			case 53://go按钮
			{
						isGo_state = 1;
						isGo = false;
						goFrame_1->setVisible(false);
			}
				break;
			default:
				break;
			}
		}
		else if (Widget::TouchEventType::ENDED == type)
		{
			if (isMenu && btn->getTag() != ButtonType_fruit_menu)
			{
				//声音，退出，帮助
				setMenuState(isMenu);
			}
			switch (btn->getTag())
			{
			case ButtonType_fruit_menu:
			{
										  setMenuState(isMenu);
			}
				break;

			case ButtonType_fruit_setting:
			{
											 isSound = HallSoundManager::instance()->getEffectConfig();
											 HallSoundManager::instance()->setEffectConfig(!isSound);
											 auto menuPanel = static_cast<Widget *>(BgPanel->getChildByName("menuPanel"));
											 auto setting_btn = static_cast<Button *>(menuPanel->getChildByName("setting_btn"));
											 if (isSound)
											 {
												 setting_btn->loadTextures(SETPATH("fruit/Button/btn_music2.png"), SETPATH("fruit/Button/btn_music2_gery.png"));
											 }
											 else
											 {
												 setting_btn->loadTextures(SETPATH("fruit/Button/btn_music.png"), SETPATH("fruit/Button/btn_music_gery.png"));
											 }
			}
				break;
			case ButtonType_fruit_help:
			{
										  helpLayer::createObject();
			}
				break;
			case ButtonType_fruit_out:
			{
										 SendMsg::sendStandUpReq(NetworkManager::FLAG_GAME, UserInfo::instance()->getTableId(), UserInfo::instance()->getSeatId(), 1, 0);
			}
				break;
			case 51:case 52:  //比大小
			{
						if (wCurrentTimeType == TimeType_fruit_Leisure && std::atoi(scoreTopLeft->getString().c_str()) > 0 && is_guess_useable == true)
						{
							HallSoundManager::instance()->stopAllEffect();
							//开奖画面清除
							unschedule(schedule_selector(MainScene_fruit::sideLight));
							if (init_ResultNumber[0] != 10 && init_ResultNumber[0] != 22)
							{
								blockLight[wCurrentGameEnd.cbTableCardArray[0] - 1]->setVisible(true);
								light_right[changeVoice(init_ResultNumber[0]) - 1]->setVisible(true);
							}
							else
							{
								for (int i = 1; i < 5; i++)
								{
									blockLight[wCurrentGameEnd.cbTableCardArray[0] - 1]->setVisible(false);
									if (wCurrentGameEnd.cbTableCardArray[i] != 0)
									{
										blockLight[wCurrentGameEnd.cbTableCardArray[i] - 1]->setVisible(true);
										light_right[changeVoice(init_ResultNumber[i]) - 1]->setVisible(true);
									}
								}
							}
							is_guess_visible = false;
							int isBig = (btn->getTag() == 51) ? 0 : 1;
							//比大小发消息
							SendGameMsg::sendBigorSmallJetton(NetworkManager::FLAG_GAME, isBig);
							is_guess_useable = false;
						}
			}
				break;
			case 53:  //go按钮
			{
						  isGo_state = 1;
						  if (isGo == true)
						  {
							  goFrame_1->setVisible(false);
							  anniu_go->loadTextures(SETPATH("fruit/Button/btn_go_6.png"), SETPATH("fruit/Button/btn_go_5.png"));
						  }
						  JettonRepeat();
			}
				break;
			case 49://左移按钮
			{
						if (wCurrentTimeType == TimeType_fruit_Leisure && std::atoi(scoreTopLeft->getString().c_str()) > 0)
						{
							NetworkManager::getInstance()->sendData(NetworkManager::FLAG_GAME, MDM_GF_GAME, SUB_C_BUTTON_LEFT, NULL, 0);
						}
			}
				break;
			case 50://右移按钮
			{
						unschedule(schedule_selector(MainScene_fruit::continueRightJetton));
			}
				break;

			case 101: case 102: case 103:case 104:case 105:case 106:case 107:case 108:
			{
						  unschedule(schedule_selector(MainScene_fruit::continueUpJetton));
			}
				break;
			case 48:  //ALL按钮，停止下注
			{
						  unschedule(schedule_selector(MainScene_fruit::continueUpJettonAll));
			}
			default:
				break;
			}
		}
	}

	void MainScene_fruit::JettonRepeat()
	{
		if (wCurrentTimeType == TimeType_fruit_PlaceJetton)
		{
			if (isDataNull(itemp_myJettonLabel, 8)) //检测到下注后，正常开牌
			{
				HallSoundManager::instance()->stopAllEffect();
				SendGameMsg::sendStartOrStop(NetworkManager::FLAG_GAME, true);
				isStart = false;
			}
			else if (isDataNull(myRepeatJettonLabel, 8)) //重复下注
			{
				if (std::atoll(scoreTopLeft->getString().c_str()) > 0)
				{
					unschedule(schedule_selector(MainScene_fruit::continueScoreJetton));
					unschedule(schedule_selector(MainScene_fruit::continueScoreJettonTwo));
					scoreTopLeft->setString("0");
					scoreTopRight->setString(__String::createWithFormat("%lld", wCurrentGameButtonRightScore)->getCString());
				}
				CMD_C_PlaceJettons repeatAllPlaceJetton;
				memset(&repeatAllPlaceJetton, 0, sizeof(CMD_C_PlaceJettons));
				SCORE itemp_score = 0;
				for (int i = 0; i < 8; i++)
				{
					repeatAllPlaceJetton.lJettonScore[i + 1] = myRepeatJettonLabel[i] * score_rank;
					itemp_score += myRepeatJettonLabel[i] * score_rank;
				}
				if (std::atoll(scoreTopRight->getString().c_str()) > itemp_score)
				{
					HallSoundManager::instance()->stopAllEffect();
					NetworkManager::getInstance()->sendData(NetworkManager::FLAG_GAME, MDM_GF_GAME, SUB_C_REPEAT_JETTON,
						&repeatAllPlaceJetton, sizeof(CMD_C_PlaceJettons));
					SendGameMsg::sendStartOrStop(NetworkManager::FLAG_GAME, true);
					isStart = false;
				}
				else
				{
					if (isGo == true)
					{
						isGo = false;
						anniu_go->loadTextures(SETPATH("fruit/Button/btn_go_2.png"), SETPATH("fruit/Button/btn_go_1.png"));
						goFrame->setVisible(false);
					}
				}
			}
		}
		if (isStart == false && wCurrentTimeType == TimeType_fruit_Leisure)
		{
			if (is_guess_useable == false)
			{
				return;
			}
			//结束开奖动画,并且常亮中奖的选项
			unschedule(schedule_selector(MainScene_fruit::sideLight));
			if (init_ResultNumber[0] != 10 && init_ResultNumber[0] != 22)
			{
				blockLight[wCurrentGameEnd.cbTableCardArray[0] - 1]->setVisible(true);
				light_right[changeVoice(init_ResultNumber[0]) - 1]->setVisible(true);
			}
			else
			{
				for (int i = 1; i < 5; i++)
				{
					blockLight[wCurrentGameEnd.cbTableCardArray[0] - 1]->setVisible(false);
					if (wCurrentGameEnd.cbTableCardArray[i] != 0)
					{
						blockLight[wCurrentGameEnd.cbTableCardArray[i] - 1]->setVisible(true);
						light_right[changeVoice(init_ResultNumber[i]) - 1]->setVisible(true);
					}
				}
			}
			HallSoundManager::instance()->stopAllEffect();
			SendGameMsg::sendStartOrStop(NetworkManager::FLAG_GAME, isStart);
			isStart = true;
		}
	}

	void MainScene_fruit::JettonArea(int btn_number)
	{
		if (wCurrentTimeType != TimeType_fruit_PlaceJetton)
		{
			return;
		}
		if (itemp_myJettonLabel[btn_number - 1] >= 99)
		{
			return;
		}
		if (std::atoll(scoreTopRight->getString().c_str()) < score_rank)
		{
			return;
		}
		SendGameMsg::sendPlace_Jetton(NetworkManager::FLAG_GAME, btn_number, score_rank);

	}

	void MainScene_fruit::initGameScene(CMD_S_StatusFree& obj)
	{
		PokerPlayerInfo * player = PokerData::instance()->getPlayerInfoBySeatId(obj.wBankerUser);
		if (obj.wBankerUser == INVALID_CHAIR)
		{
			wCurrentBanker = INVALID_CHAIR;
		}
		else
		{
			if (player)
			{
				wCurrentBanker = player->getSeatId();
			}
		}
		if (wCurrentBanker == UserInfo::instance()->getSeatId())
		{
			wCurrentBankerState = CurrentBanker_fruit_Already;
		}
	}

	void MainScene_fruit::initGameScene(CMD_S_StatusPlay& obj)
	{
		PokerPlayerInfo * player = PokerData::instance()->getPlayerInfoBySeatId(obj.wBankerUser);
		if (obj.wBankerUser == INVALID_CHAIR)
		{
			wCurrentBanker = INVALID_CHAIR;
		}
		else
		{
			if (player)
			{
				wCurrentBanker = player->getSeatId();
			}
		}
		if (wCurrentBanker == UserInfo::instance()->getSeatId())
		{
			wCurrentBankerState = CurrentBanker_fruit_Already;
			automaticJ_label->setString(__String::createWithFormat(U2F("坐庄中,连庄%d轮!").c_str(), obj.cbBankerTime + 1)->getCString());
			automaticJ_label->setVisible(true);
		}

		if (PokerData::instance()->getMatchRule() & 0x00000040)
		{
			if (wCurrentTimeType == TimeType_fruit_Start)
			{
				CMD_S_GameEnd endData;
				memset(&endData, 0, sizeof(CMD_S_GameEnd));
				endData.lBankerScore = obj.lEndBankerScore;
				endData.lUserScore = obj.lEndUserScore;
			}
		}
	}

	void MainScene_fruit::updateScore(CMD_GR_MobileUserScore& buf)
	{
		if (buf.dwUserID == UserInfo::instance()->getUserId())
		{
			scoreGameResult = buf.UserScore.lScore;
		}
		else
		{
			if (wCurrentBanker == buf.dwUserID)
			{
				PokerPlayerInfo * player = PokerData::instance()->getPlayerInfo(buf.dwUserID);
			}
		}
	}

	void MainScene_fruit::PlaceJettonFunction(CMD_S_PlaceJetton& jetton)
	{
		SoundManager::instance()->playEffect(__String::createWithFormat(SETPATH("fruit/sounds/Y20%d.mp3").c_str(), jetton.cbJettonArea)->getCString());//这里添加声音
		if (jetton.wChairID == UserInfo::instance()->getSeatId())
		{
			updateShowScore(true, jetton.cbJettonArea, jetton.lJettonScore / score_rank);
		}
		else
		{
			updateShowScore(false, jetton.cbJettonArea, jetton.lJettonScore / score_rank);
		}
	}

	void MainScene_fruit::updateShowScore(bool seatId, BYTE jettonArea, SCORE jettonScore)
	{
		if (seatId)//如果是自己
		{
			itemp_myJettonLabel[jettonArea - 1] += jettonScore;
			myJettonScoreShow[jettonArea - 1]->setString(__String::createWithFormat("%lld", itemp_myJettonLabel[jettonArea - 1])->getCString());
			SCORE itemp_score = std::atoll(scoreTopRight->getString().c_str()) - score_rank;
			scoreTopRight->setString(__String::createWithFormat("%lld", itemp_score)->getCString());
		}
	}

	void MainScene_fruit::setLabelType(int time, TimeType_fruit type)
	{
		//当前阶段
		wCurrentTimeType = type;
	}
	void MainScene_fruit::userApplyBanker(int seatId)
	{

	}

	//复投消息处理
	void MainScene_fruit::userRepeatPlaceJetton(bool istrue, CMD_S_RepeatBet&  pQueryStorage)
	{
		SoundManager::instance()->playEffect(SETPATH("fruit/sounds/Y202.mp3").c_str());//这里添加声音
		for (int j = 1; j < AREA_COUNT + 1; j++)
		{
			if (pQueryStorage.lJettonScore[j]>0)
			{
				for (int m = pQueryStorage.lJettonScore[j] / score_rank; m > 0; m--)
				{
					updateShowScore(istrue, j, 1);
				}
			}
		}
	}

	void MainScene_fruit::setMenuState(bool orisMenu)
	{
		auto menuPanel = static_cast<Widget *>(BgPanel->getChildByName("menuPanel"));
		menuPanel->stopAllActions();
		if (orisMenu) //收缩
		{
			isMenu = false;
			menuPanel->runAction(Spawn::createWithTwoActions(ScaleTo::create(0.3, 0.0f), MoveTo::create(0.3, Vec2(110, 1280))));
		}
		else //下拉
		{
			isMenu = true;
			menuPanel->runAction(Spawn::createWithTwoActions(ScaleTo::create(0.3, 1.0f), MoveTo::create(0.3, Vec2(75, 1050))));
		}
	}

	void MainScene_fruit::onBackPressed()
	{

	}

	bool MainScene_fruit::isDataNull(LONGLONG *obj, int length)
	{
		for (int i = 0; i<length; i++)
		{
			if (obj[i]>0)
			{
				return true;
			}
		}
		return false;
	}
}