#include "TableScene_yxx.h"
#include "game/yxx/data/SoundManager_yxx.h"
#include "sdk/ui/PromptLayer.h"
#include "sdk/sdk.h"

namespace yxx
{
#define TAG "TableScene"

	enum {
		TAG_TABLE_BG,
		TAG_TABLE_BG_STATICINFO,
		TAG_TABLE_BTN_BACK,
		TAG_TABLE_BTN_QUICK_ENTER,
		TAG_TABLE_BTN_NEW_ROOM,
		TAG_TABLE_INPUTPASSWORD,
		TAG_TABLE_LIST_INFO,//以下占用的ID很多
		TAG_TABLE_CHAIR = 2000,
		TAG_TABLE_PLAYER_INFO = 3000,
		TAG_TABLE_PLAYER_NAME = 4000,
	};

	//桌子资源配置信息
#define X_TABLE_COUNT					3
#define Y_TABLE_COUNT					3

#define TABLE_WIDTH						371
#define TABLE_HEIGHT					306

	//椅子坐标 by chen
	const Vec2 TableChairPos[GAME_PLAYER] = {
		Vec2(50, 306 - 131),
		Vec2(200, 306 - 90)

	};
	//椅子上人物坐标
	const Vec2 TablePlayerPos[GAME_PLAYER] = {
		Vec2(31, 306 - 62),
		Vec2(137, 306 - 38)
	};

	//玩家名字坐标
	const Vec2 TablePlayerNamePos[GAME_PLAYER] = {

		Vec2(80, 306 - 253),
		Vec2(300, 306 - 50)

	};

	const int TableHeadIndex[GAME_PLAYER] = { 3, 1 };

	TableScene::TableScene()
	{
		m_mapTableInfo.clear();

	}

	TableScene::~TableScene()
	{
	}

	void TableScene::heartbeat(float dt){

		SendMsg::sendHeartbeat(NetworkManager::FLAG_GAME);

	}

	void TableScene::addTableFlag()
	{
		m_pRoomIdflag.push_back(m_wSitdownTableID);
	}

	bool TableScene::isTableIsFlag(WORD wtableId)
	{
		std::vector<WORD>::iterator it = m_pRoomIdflag.begin();
		for (; it != m_pRoomIdflag.end(); ++it) {
			if (wtableId == *it)
				return true;
		}
		return false;
	}

	void TableScene::ChangeTable()
	{
		//查找一个有人的座位
		WORD wTableID;
		WORD wChairID;

		for (wTableID = 0; wTableID < m_GameData->getTableCount(); wTableID++)
		{
			std::map<int, tagTableInfo>::iterator itTable = m_mapTableInfo.find(wTableID);
			if (itTable != m_mapTableInfo.end() && itTable->second.bIsHavePassWord == false && itTable->second.cbSitDownCount <= GAME_PLAYER)
			{
				if (itTable->second.cbSitDownCount == 1)
				{
					for (wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
					{
						//是否已经坐下或者已经去过了有鱼阵的房间
						if (itTable->second.bIsSitDown[wChairID] == false && false == isTableIsFlag(wTableID))
						{

							m_wSitdownTableID = wTableID;
							m_wSitdownChairID = wChairID;
							schedule(schedule_selector(TableScene::sendSitDownMsg), 0.016f);

							schedule(schedule_selector(TableScene::outTime), 8.0f);

							return;
						}
					}
				}
			}
		}
		for (wTableID = 0; wTableID < m_GameData->getTableCount(); wTableID++)
		{
			std::map<int, tagTableInfo>::iterator itTable = m_mapTableInfo.find(wTableID);
			if (itTable != m_mapTableInfo.end() && itTable->second.bIsHavePassWord == false && itTable->second.cbSitDownCount <= GAME_PLAYER)
			{
				for (wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
				{
					//是否已经坐下或者已经去过了有鱼阵的房间
					if (itTable->second.bIsSitDown[wChairID] == false && false == isTableIsFlag(wTableID))
					{

						m_wSitdownTableID = wTableID;
						m_wSitdownChairID = wChairID;
						schedule(schedule_selector(TableScene::sendSitDownMsg), 0.016f);

						schedule(schedule_selector(TableScene::outTime), 8.0f);

						return;
					}
				}
			}
		}
	}

	void TableScene::disconnect(Ref * obj)
	{
		PromptLayer::createObject("与服务器断开连接，3秒后自动退出房间!");
		this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create(CC_CALLBACK_0(TableScene::autoExit, this)), NULL));
	}

	void TableScene::autoExit()
	{
		onBackPressed();
	}
	void TableScene::initView()
	{

		m_Layout = Layout::create();
		this->addChild(m_Layout);
		auto table = ImageView::create(SETPATH("hhsw/table/table_bg.png"));
		table->setPosition(Vec2(VisibleRect::center().x, VisibleRect::center().y));
		m_Layout->addChild(table);
		table->setTag(TAG_TABLE_BG);

		string serverName = UserInfo::instance()->getServerName();
		auto pRoomName = Text::create();
		pRoomName->setAnchorPoint(Vec2(0.5, 0.5));
		pRoomName->setPosition(Vec2(VisibleRect::center().x, VisibleRect::center().y + 338));
		pRoomName->setTextHorizontalAlignment(TextHAlignment::CENTER);
		pRoomName->setTextVerticalAlignment(TextVAlignment::CENTER);
		pRoomName->setString(serverName.c_str());
		pRoomName->setFontName("微软雅黑");
		pRoomName->setFontSize(34);
		pRoomName->setColor(Color3B(153, 51, 0));
		pRoomName->enableOutline(Color4B(Color3B(153, 51, 0)));
		m_Layout->addChild(pRoomName);

		// 返回按钮
		auto backbtn = Button::create(SETPATH("hhsw/table/btn_back_n.png"), SETPATH("hhsw/table/btn_back_d.png"));
		backbtn->setTag(TAG_TABLE_BTN_BACK);
		backbtn->setTouchEnabled(true);
		backbtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		backbtn->addTouchEventListener([this](Ref* target, Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				onBackPressed();
			}
		});
		backbtn->setPosition(Vec2(VisibleRect::getWidth() - 66, VisibleRect::getHeight()));
		m_Layout->addChild(backbtn);

		// 快速加入
		auto quickenterbtn = Button::create(SETPATH("hhsw/table/btn_quick_enter_n.png"), SETPATH("hhsw/table/btn_quick_enter_d.png"));
		quickenterbtn->setTag(TAG_TABLE_BTN_QUICK_ENTER);
		quickenterbtn->setTouchEnabled(true);
		quickenterbtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		quickenterbtn->addTouchEventListener([this](Ref* target, Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				//查找一个空座位
				WORD wTableID;
				WORD wChairID;
				for (wTableID = 0; wTableID < m_GameData->getTableCount(); wTableID++)
				{
					std::map<int, tagTableInfo>::iterator itTable = m_mapTableInfo.find(wTableID);
					if (itTable != m_mapTableInfo.end() && itTable->second.bIsHavePassWord == false && itTable->second.cbSitDownCount <= GAME_PLAYER)
					{
						for (wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
						{
							if (itTable->second.bIsSitDown[wChairID] == false)
							{
								m_wSitdownTableID = wTableID;
								m_wSitdownChairID = wChairID;
								schedule(schedule_selector(TableScene::sendSitDownMsg), 0.016f);

								schedule(schedule_selector(TableScene::outTime), 8.0f);
								((Button*)m_Layout->getChildByTag(TAG_TABLE_BTN_QUICK_ENTER))->setTouchEnabled(false);
								return;
							}
						}
					}
				}
			}
		});
		quickenterbtn->setPosition(Vec2(120, VisibleRect::getHeight()));
		m_Layout->addChild(quickenterbtn, 100);


		//绘制桌子
		m_pScrollView = cocos2d::ui::ScrollView::create();
		m_pScrollView->setDirection(SCROLLVIEW_DIR_VERTICAL);
		m_pScrollView->setAnchorPoint(ccp(0, 0));
		m_pScrollView->setSize(CCSize(VisibleRect::getWidth(), 616));
		m_pScrollView->setInnerContainerSize(CCSize(VisibleRect::getWidth(), 616));
		m_pScrollView->setPosition(ccp(VisibleRect::center().x - 640, VisibleRect::center().y - 355));
		m_Layout->addChild(m_pScrollView);

		int nNewHeight;
		if (m_GameData->getTableCount() % X_TABLE_COUNT == 0)
		{
			nNewHeight = TABLE_HEIGHT*m_GameData->getTableCount() / X_TABLE_COUNT;
		}
		else
		{
			nNewHeight = TABLE_HEIGHT*(m_GameData->getTableCount() / X_TABLE_COUNT + 1);
		}
		if (nNewHeight < 616)
		{
			m_pScrollView->setInnerContainerSize(CCSize(VisibleRect::getWidth(), 616));
			nNewHeight = 616;
		}
		else
		{
			m_pScrollView->setInnerContainerSize(CCSize(VisibleRect::getWidth(), nNewHeight));
		}

		//初始化桌子数据
		int nTableCount = m_GameData->getTableCount();
		for (int i = 0; i < nTableCount; i++)
		{
			tagTableInfo buf;
			memset(&buf, 0, sizeof(tagTableInfo));

			buf.wTableID = i;
			m_mapTableInfo.insert(pair<int, tagTableInfo>(i, buf));

			//桌子
			int tablePosX = 83 + TABLE_WIDTH / 2 + (i%X_TABLE_COUNT)*TABLE_WIDTH;
			int tablePosY = nNewHeight - (i / X_TABLE_COUNT)*TABLE_HEIGHT - TABLE_HEIGHT / 2;
			auto tableview = ImageView::create(SETPATH("hhsw/table/table_free.png"));
			tableview->setTag(TAG_TABLE_LIST_INFO + i);
			tableview->setPosition(ccp(tablePosX, tablePosY));
			m_pScrollView->addChild(tableview);

			//桌号
			auto txIndex = TextAtlas::create("0", SETPATH("hhsw/table/table_number.png"), 28, 33, "0");
			txIndex->setPosition(ccp(tablePosX, tablePosY - TABLE_HEIGHT / 2 + 33 / 2));
			txIndex->setString(StringUtils::toString(i + 1));
			m_pScrollView->addChild(txIndex);

			//椅子
			for (int nChairIndex = 0; nChairIndex < GAME_PLAYER; nChairIndex++)
			{
				auto chair = Button::create(SETPATH("hhsw/table/chair.png"), SETPATH("hhsw/table/chair.png"), SETPATH("hhsw/table/chair.png"));
				chair->setTag(TAG_TABLE_CHAIR + i*GAME_PLAYER + nChairIndex);
				chair->setTouchEnabled(true);
				chair->addTouchEventListener(CC_CALLBACK_2(TableScene::onTouch, this));
				chair->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
				chair->setPosition(TableChairPos[nChairIndex]);
				tableview->addChild(chair);

				auto name = Text::create("", "", 20);
				name->setTag(TAG_TABLE_PLAYER_NAME + i*GAME_PLAYER + nChairIndex);
				name->setPosition(TablePlayerNamePos[nChairIndex]);
				tableview->addChild(name);
			}

		}

		//显示密码输入框
		auto pInputPassword = new InputPassword(this);
		pInputPassword->setTag(TAG_TABLE_INPUTPASSWORD);
		pInputPassword->setPosition(VisibleRect::center());
		m_Layout->addChild(pInputPassword);
		pInputPassword->setZOrder(2);
		pInputPassword->setVisible(false);
		m_wCurClickTableID = INVALID_TABLE;


	}

	void TableScene::onEnter()
	{
		SceneBase::onEnter();
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(TableScene::notifyFail), GAME_REQUEST_FAILURE, nullptr);
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(TableScene::notifyFail), GAME_LOGIN_FINISH, nullptr);
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(TableScene::closeSocket_noifyFial), GAME_DISCONNECT, nullptr);
		this->scheduleUpdate();
		if (UserInfo::instance()->getIsChangeTable())
		{
			CallFunc* callback2 = CallFunc::create([this](){
				this->ChangeTable();
			}
			);
			this->runAction(Sequence::create(DelayTime::create(0.1f), callback2, NULL));

		}

		// 每3秒发送一个心跳包
		this->schedule(schedule_selector(TableScene::heartbeat), 3.0f);
	}

	void TableScene::onExit()
	{
		SceneBase::onExit();
		NotificationCenter::getInstance()->removeObserver(this, GAME_REQUEST_FAILURE);
		NotificationCenter::getInstance()->removeObserver(this, GAME_LOGIN_FINISH);
		NotificationCenter::getInstance()->removeObserver(this, GAME_DISCONNECT);
		unscheduleUpdate();
		this->unschedule(schedule_selector(TableScene::heartbeat));
	}

	void TableScene::bindData(PokerData *data)
	{
		m_GameData = data;
	}

	void TableScene::onBackPressed()
	{
		PokerData::instance()->m_mapUserSitDownInfo.clear();

		delete PokerData::instance()->m_pPlayerInfoMap;
		PokerData::instance()->m_pPlayerInfoMap = new map<int, PokerPlayerInfo*>();

		this->onMoblieUserStatusReset();
		UserInfo::instance()->setCurrentServerID(0);
		SendMsg::sendStandUpReq(NetworkManager::FLAG_GAME, INVALID_TABLE, INVALID_CHAIR, 1, 0);
		//PokerMessageController::instance()->onTableSceneQuit();
		//PokerMessageController::instance()->removeGameSceneData();
		NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_GAME);
		Director::getInstance()->popScene();
	}
	void TableScene::update(float dt)
	{
		//实时监测自己是否处于断网状态,如果是则提示退出房间！
		if (false == NetworkManager::getInstance()->isConnect(NetworkManager::FLAG_GAME))
		{
			disconnect(nullptr);
		}
	}
	void TableScene::onTouch(Ref* target, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			int tag = static_cast<Node*>(target)->getTag();
			int nTableCount = m_GameData->getTableCount();
			if (tag >= TAG_TABLE_CHAIR&&tag < TAG_TABLE_CHAIR + nTableCount*GAME_PLAYER)
			{
				int wTableID = (tag - TAG_TABLE_CHAIR) / GAME_PLAYER;
				int wChairID = (tag - TAG_TABLE_CHAIR) % GAME_PLAYER;
				std::map<int, tagTableInfo>::iterator itTable = m_mapTableInfo.find(wTableID);
				if (itTable != m_mapTableInfo.end())
				{
					//人数没满
					if (itTable->second.cbSitDownCount <= GAME_PLAYER)
					{
						//没有密码
						if (itTable->second.bIsHavePassWord == false)
						{
							if (itTable->second.bIsSitDown[wChairID] == false)
							{
								//如果没有人坐下 就进游戏
								m_wSitdownTableID = wTableID;
								m_wSitdownChairID = wChairID;
								schedule(schedule_selector(TableScene::sendSitDownMsg), 0.016f);//0.016

								return;
							}
						}
						//存在密码
						else
						{
							m_wCurClickTableID = wTableID;
							InputPassword* pInputPassword = nullptr;
							pInputPassword = (InputPassword*)m_Layout->getChildByTag(TAG_TABLE_INPUTPASSWORD);
							if (pInputPassword)
							{
								pInputPassword->showInputPasswordWindow(2);
							}
						}
					}
					//人数已满
					else
					{
						PromptLayer::createObject("本桌人数已满");
					}
				}
			}
		}
	}

	void TableScene::onMoblieUserStatusReset()
	{
		//清空数组数据
		int tempSize = m_mapTableInfo.size();
		for (int i = 0; i < tempSize; i++)
		{
			std::map<int, tagTableInfo>::iterator itTable = m_mapTableInfo.find(i);
			if (itTable != m_mapTableInfo.end())
			{
				itTable->second.bIsSitDown[0] = false;
				itTable->second.bIsSitDown[1] = false;
				itTable->second.bIsSitDown[2] = false;
				itTable->second.bIsSitDown[3] = false;
				itTable->second.cbSitDownCount = 0;

				auto enterBtn = (ImageView*)m_pScrollView->getChildByTag(TAG_TABLE_LIST_INFO + i);
				if (enterBtn)
				{
					Sprite* text1 = (Sprite*)enterBtn->getChildByTag(TAG_TABLE_PLAYER_INFO + GAME_PLAYER*i + 0);
					if (text1)
					{
						text1->removeFromParent();
					}
					Sprite* text2 = (Sprite*)enterBtn->getChildByTag(TAG_TABLE_PLAYER_INFO + GAME_PLAYER*i + 1);
					if (text2)
					{
						text2->removeFromParent();
					}
					Sprite* text3 = (Sprite*)enterBtn->getChildByTag(TAG_TABLE_PLAYER_INFO + GAME_PLAYER*i + 2);
					if (text3)
					{
						text3->removeFromParent();
					}
					Sprite* text4 = (Sprite*)enterBtn->getChildByTag(TAG_TABLE_PLAYER_INFO + GAME_PLAYER*i + 3);
					if (text4)
					{
						text4->removeFromParent();
					}


					enterBtn->loadTexture(SETPATH("hhsw/table/table_free.png"));
					enterBtn->setTouchEnabled(true);

					//清空人物
					int tempTag = TAG_TABLE_PLAYER_INFO + GAME_PLAYER*i + 0;
					Node*  tempSp1 = enterBtn->getChildByTag(tempTag);
					if (tempSp1)
					{
						tempSp1->removeFromParent();
					}

					tempTag = TAG_TABLE_PLAYER_INFO + GAME_PLAYER*i + 1;
					Node*  tempSp2 = enterBtn->getChildByTag(tempTag);
					if (tempSp2)
					{
						tempSp2->removeFromParent();
					}

					tempTag = TAG_TABLE_PLAYER_INFO + GAME_PLAYER*i + 2;
					Node*  tempSp3 = enterBtn->getChildByTag(tempTag);
					if (tempSp3)
					{
						tempSp3->removeFromParent();
					}

					tempTag = TAG_TABLE_PLAYER_INFO + GAME_PLAYER*i + 3;
					Node*  tempSp4 = enterBtn->getChildByTag(tempTag);
					if (tempSp4)
					{
						tempSp4->removeFromParent();
					}

					Text * name1 = (Text *)enterBtn->getChildByTag(TAG_TABLE_PLAYER_NAME + GAME_PLAYER*i + 0);
					if (name1)
					{
						name1->setText("");
					}
					Text * name2 = (Text *)enterBtn->getChildByTag(TAG_TABLE_PLAYER_NAME + GAME_PLAYER*i + 1);
					if (name2)
					{
						name2->setText("");
					}
					Text * name3 = (Text *)enterBtn->getChildByTag(TAG_TABLE_PLAYER_NAME + GAME_PLAYER*i + 2);
					if (name3)
					{
						name3->setText("");
					}
					Text * name4 = (Text *)enterBtn->getChildByTag(TAG_TABLE_PLAYER_NAME + GAME_PLAYER*i + 3);
					if (name4)
					{
						name4->setText("");
					}

				}

			}

		}
	}

	void TableScene::onMoblieUserStatusRefresh(std::map<DWORD, tagTableUserSitDownInfo> &tempMap)
	{
		//每次有状态改变都先刷新界面======================<<

		this->onMoblieUserStatusReset();

		//刷新界面
		std::map<DWORD, tagTableUserSitDownInfo>::iterator it = tempMap.begin();//有数据
		while (it != tempMap.end())
		{
			if (it->second.wTableID != INVALID_TABLE)
			{

				std::map<int, tagTableInfo>::iterator itTable = m_mapTableInfo.find(it->second.wTableID);//空数组数据

				if (itTable != m_mapTableInfo.end())
				{
					if (it->second.wChairID != INVALID_CHAIR)
					{
						itTable->second.bIsSitDown[it->second.wChairID] = true;
						itTable->second.cbSitDownCount++;//人数 ++1

						if (itTable->second.cbSitDownCount > GAME_PLAYER)
						{
							itTable->second.cbSitDownCount = GAME_PLAYER;
						}

					}

					//更新显示
					auto enterBtn = (ImageView*)m_pScrollView->getChildByTag(TAG_TABLE_LIST_INFO + it->second.wTableID);
					if (enterBtn)
					{

						//当前桌子有一个玩家的情况就显示座子在玩的画面
						if (itTable->second.cbSitDownCount >= 1)
						{
							enterBtn->loadTexture(SETPATH("hhsw/table/table_play.png"));
							enterBtn->setTouchEnabled(false);
						}

						//计算椅子上人物位置
						Vec2 pos;//位置
						int indexX = 0;//帧索引
						int indexY = 0;

						int wFaceID = 0;/////////////////

						if (wFaceID < 50) //男
						{
							pos = TablePlayerPos[it->second.wChairID];
							indexX = 8 + TableHeadIndex[it->second.wChairID];
							indexY = rand() % 11;
						}
						else //女
						{
							pos = TablePlayerPos[it->second.wChairID];
							indexX = 0 + TableHeadIndex[it->second.wChairID];
							indexY = rand() % 11;
						}

						if (it->second.wChairID != INVALID_CHAIR)
						{
							//绘画椅子上人物
							int  tempTag = TAG_TABLE_PLAYER_INFO + GAME_PLAYER*it->second.wTableID + it->second.wChairID;
							Sprite* PlayerInfo = Sprite::create(SETPATH("hhsw/table/playerinfo.png"));
							CCSize size = CCSize(PlayerInfo->getContentSize().width / 16, PlayerInfo->getContentSize().height / 13);
							PlayerInfo->setTextureRect(CCRect(indexX*size.width, indexY*size.height, size.width, size.height));
							PlayerInfo->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
							PlayerInfo->setPosition(pos);
							PlayerInfo->setTag(tempTag);
							enterBtn->addChild(PlayerInfo);
						}

						if (it->second.wChairID != INVALID_CHAIR)
						{

							Text * name = (Text *)enterBtn->getChildByTag(TAG_TABLE_PLAYER_NAME + GAME_PLAYER*it->second.wTableID + it->second.wChairID);
							if (name)
							{
								//wangbo 限制名字显示长度
								if (it->second.strName.length() > 12)
								{
									it->second.strName = it->second.strName.substr(0, 9) + "...";
								}
								name->setText(U2F(it->second.strName.c_str()));
							}
						}


					}

				}
			}
			++it;
		}
		//每次有状态改变都先刷新界面======================>>
	}

	void TableScene::onMoblieUserStatusChange(std::map<DWORD, tagTableUserSitDownInfo> &tempMap, CMD_GR_UserStatus* pBuf, WORD wFaceID, string strName)
	{

		this->onMoblieUserStatusRefresh(tempMap);

		//玩家离开处理
		if (pBuf->UserStatus.cbUserStatus == US_NULL || pBuf->UserStatus.cbUserStatus == US_LOOKON || pBuf->UserStatus.cbUserStatus == US_FREE)
		{
			std::map<DWORD, tagTableUserSitDownInfo>::iterator it = tempMap.find(pBuf->dwUserID);
			if (it != tempMap.end())
			{
				std::map<int, tagTableInfo>::iterator itTable = m_mapTableInfo.find(it->second.wTableID);
				if (itTable != m_mapTableInfo.end())
				{
					itTable->second.bIsSitDown[it->second.wChairID] = false;//设置没有坐下 界面展示的
					itTable->second.cbSitDownCount--;//总数--

					if (itTable->second.cbSitDownCount > GAME_PLAYER)
					{
						itTable->second.cbSitDownCount = 0;
					}

					//更新显示
					ImageView* enterBtn = (ImageView*)m_pScrollView->getChildByTag(TAG_TABLE_LIST_INFO + it->second.wTableID);
					if (enterBtn)
					{
						Sprite* text = (Sprite*)enterBtn->getChildByTag(TAG_TABLE_PLAYER_INFO + GAME_PLAYER*it->second.wTableID + it->second.wChairID);
						if (text)
						{
							text->removeFromParent();
						}
						if (itTable->second.cbSitDownCount <= 0)
						{
							enterBtn->loadTexture(SETPATH("hhsw/table/table_free.png"));
						}

						enterBtn->setTouchEnabled(true);

						//清空人物
						int tempTag = TAG_TABLE_PLAYER_INFO + GAME_PLAYER*it->second.wTableID + it->second.wChairID;
						enterBtn->removeChildByTag(tempTag);

						Text * name = (Text *)enterBtn->getChildByTag(TAG_TABLE_PLAYER_NAME + GAME_PLAYER*it->second.wTableID + it->second.wChairID);
						if (name)
						{
							name->setText("");
						}
					}

				}

				tempMap.erase(it);

			}
		}

		//玩家进入处理
		if (pBuf->UserStatus.cbUserStatus == US_SIT || pBuf->UserStatus.cbUserStatus == US_READY || pBuf->UserStatus.cbUserStatus == US_PLAYING)
		{
			std::map<DWORD, tagTableUserSitDownInfo>::iterator it = tempMap.find(pBuf->dwUserID);
			if (it != tempMap.end())
			{
				//如果存在

				if (pBuf->UserStatus.wTableID != INVALID_TABLE && pBuf->UserStatus.wChairID != INVALID_CHAIR)//如果有桌子号 椅子号
				{
					//更新信息
					tagTableUserSitDownInfo buf = it->second;
					buf.wTableID = pBuf->UserStatus.wTableID;
					buf.wChairID = pBuf->UserStatus.wChairID;
					buf.strName = strName;
					tempMap.insert(pair<DWORD, tagTableUserSitDownInfo>(pBuf->dwUserID, buf));//更新信息


					std::map<int, tagTableInfo>::iterator itTable = m_mapTableInfo.find(pBuf->UserStatus.wTableID);
					if (itTable != m_mapTableInfo.end())
					{
						itTable->second.bIsSitDown[pBuf->UserStatus.wChairID] = true;

						//更新显示
						auto enterBtn = (ImageView*)m_pScrollView->getChildByTag(TAG_TABLE_LIST_INFO + pBuf->UserStatus.wTableID);
						if (enterBtn)
						{

							//当前桌子有一个玩家的情况就显示座子在玩的画面
							if (itTable->second.cbSitDownCount >= 1)
							{
								enterBtn->loadTexture(SETPATH("hhsw/table/table_play.png"));
							}

							enterBtn->setTouchEnabled(false);

							//计算椅子上人物位置
							Vec2 pos;//位置
							int indexX = 0;//帧索引
							int indexY = 0;

							if (wFaceID < 50) //男
							{
								pos = TablePlayerPos[pBuf->UserStatus.wChairID];
								indexX = 8 + TableHeadIndex[pBuf->UserStatus.wChairID];
								indexY = rand() % 11;
							}
							else //女
							{
								pos = TablePlayerPos[pBuf->UserStatus.wChairID];
								indexX = 0 + TableHeadIndex[pBuf->UserStatus.wChairID];
								indexY = rand() % 11;
							}

							//绘画椅子上人物
							int  tempTag = TAG_TABLE_PLAYER_INFO + GAME_PLAYER*pBuf->UserStatus.wTableID + pBuf->UserStatus.wChairID;
							if (enterBtn->getChildByTag(tempTag) == nullptr)
							{
								Sprite* PlayerInfo = Sprite::create(SETPATH("hhsw/table/playerinfo.png"));
								CCSize size = CCSize(PlayerInfo->getContentSize().width / 16, PlayerInfo->getContentSize().height / 13);
								PlayerInfo->setTextureRect(CCRect(indexX*size.width, indexY*size.height, size.width, size.height));
								PlayerInfo->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
								PlayerInfo->setPosition(pos);
								PlayerInfo->setTag(tempTag);
								enterBtn->addChild(PlayerInfo);
							}


							Text * name = (Text *)enterBtn->getChildByTag(TAG_TABLE_PLAYER_NAME + GAME_PLAYER*pBuf->UserStatus.wTableID + pBuf->UserStatus.wChairID);
							if (name)
							{
								name->setText(strName.c_str());
							}

						}
					}
				}


			}
			else
			{
				//如果不存在
				if (pBuf->UserStatus.wTableID != INVALID_TABLE && pBuf->UserStatus.wChairID != INVALID_CHAIR)//如果有桌子号 椅子号
				{
					tagTableUserSitDownInfo buf;
					memset(&buf, 0, sizeof(tagTableUserSitDownInfo));
					buf.wTableID = pBuf->UserStatus.wTableID;
					buf.wChairID = pBuf->UserStatus.wChairID;
					buf.strName = strName;

					tempMap.insert(pair<DWORD, tagTableUserSitDownInfo>(pBuf->dwUserID, buf));//唯一插入坐下信息的地方

					std::map<int, tagTableInfo>::iterator itTable = m_mapTableInfo.find(pBuf->UserStatus.wTableID);
					if (itTable != m_mapTableInfo.end())
					{
						itTable->second.bIsSitDown[pBuf->UserStatus.wChairID] = true;

						itTable->second.cbSitDownCount++;//人数 ++1

						if (itTable->second.cbSitDownCount > GAME_PLAYER)
						{
							itTable->second.cbSitDownCount = GAME_PLAYER;
						}

						//更新显示
						auto enterBtn = (ImageView*)m_pScrollView->getChildByTag(TAG_TABLE_LIST_INFO + pBuf->UserStatus.wTableID);
						if (enterBtn)
						{

							//当前桌子有一个玩家的情况就显示座子在玩的画面
							if (itTable->second.cbSitDownCount >= 1)
							{
								enterBtn->loadTexture(SETPATH("hhsw/table/table_play.png"));
							}

							enterBtn->setTouchEnabled(false);

							//计算椅子上人物位置
							Vec2 pos;//位置
							int indexX = 0;//帧索引
							int indexY = 0;

							if (wFaceID < 50) //男
							{
								pos = TablePlayerPos[pBuf->UserStatus.wChairID];
								indexX = 8 + TableHeadIndex[pBuf->UserStatus.wChairID];
								indexY = rand() % 11;
							}
							else //女
							{
								pos = TablePlayerPos[pBuf->UserStatus.wChairID];
								indexX = 0 + TableHeadIndex[pBuf->UserStatus.wChairID];
								indexY = rand() % 11;
							}
							//绘画椅子上人物
							int  tempTag = TAG_TABLE_PLAYER_INFO + GAME_PLAYER*pBuf->UserStatus.wTableID + pBuf->UserStatus.wChairID;
							if (enterBtn->getChildByTag(tempTag) == nullptr)
							{
								Sprite* PlayerInfo = Sprite::create(SETPATH("hhsw/table/playerinfo.png"));
								CCSize size = CCSize(PlayerInfo->getContentSize().width / 16, PlayerInfo->getContentSize().height / 13);
								PlayerInfo->setTextureRect(CCRect(indexX*size.width, indexY*size.height, size.width, size.height));
								PlayerInfo->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
								PlayerInfo->setPosition(pos);
								PlayerInfo->setTag(tempTag);
								enterBtn->addChild(PlayerInfo);
							}

							Text * name = (Text *)enterBtn->getChildByTag(TAG_TABLE_PLAYER_NAME + GAME_PLAYER*pBuf->UserStatus.wTableID + pBuf->UserStatus.wChairID);
							if (name)
							{
								name->setText(strName.c_str());
							}

						}
					}
				}
			}
		}

	}

	void TableScene::setTableStatus(WORD wTableID, bool bIsPlaying, bool bIsLock)
	{
		std::map<int, tagTableInfo>::iterator itTable = m_mapTableInfo.find(wTableID);
		if (itTable != m_mapTableInfo.end())
		{
			itTable->second.bIsGamePlaying = bIsPlaying;
			itTable->second.bIsHavePassWord = bIsLock;

		}
	}

	void TableScene::ExecSitDownOperate(string strPassword)
	{
		if (INVALID_TABLE == m_wCurClickTableID)
		{
			//查找一个空座位
			WORD wTableID;
			WORD wChairID;
			for (wTableID = 0; wTableID < m_GameData->getTableCount(); wTableID++)
			{
				std::map<int, tagTableInfo>::iterator itTable = m_mapTableInfo.find(wTableID);
				if (itTable != m_mapTableInfo.end() && itTable->second.bIsHavePassWord == false && itTable->second.cbSitDownCount == 0)
				{
					for (wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
					{
						if (itTable->second.bIsSitDown[wChairID] == false)
						{
							SendMsg::sendSitDownReq(NetworkManager::FLAG_GAME, wTableID, wChairID, strPassword.c_str());
							return;
						}
					}
				}
			}
			PromptLayer::createObject("不存在空房间");
		}
		else
		{
			WORD wTableID = m_wCurClickTableID;
			WORD wChairID;
			std::map<int, tagTableInfo>::iterator itTable = m_mapTableInfo.find(wTableID);
			if (itTable != m_mapTableInfo.end())
			{
				//人数没满
				if (itTable->second.cbSitDownCount <= GAME_PLAYER)
				{
					for (wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
					{
						if (itTable->second.bIsSitDown[wChairID] == false)
						{
							SendMsg::sendSitDownReq(NetworkManager::FLAG_GAME, wTableID, wChairID, strPassword.c_str());
							return;
						}
					}
				}
				//人数已满
				else
				{
					PromptLayer::createObject("本桌人数已满");
				}
			}
		}
	}

	bool TableScene::getPlayIsSitDown(std::map<DWORD, tagTableUserSitDownInfo> &tempMap, DWORD dwUserid)
	{
		std::map<DWORD, tagTableUserSitDownInfo>::iterator it = tempMap.find(dwUserid);
		if (it != tempMap.end())
		{
			return true;
		}
		return false;
	}

	void TableScene::notifyFail(Ref * obj)
	{
		//隐藏
		unschedule(schedule_selector(TableScene::outTime));
		((Button*)m_Layout->getChildByTag(TAG_TABLE_BTN_QUICK_ENTER))->setTouchEnabled(true);

	}

	void TableScene::outTime(float dt)
	{
		unschedule(schedule_selector(TableScene::outTime));
		//PromptLayer::createObject("登陆超时");
		schedule(schedule_selector(TableScene::autoClose), 3.0f);
	}

	void TableScene::autoClose(float dt)
	{
		unschedule(schedule_selector(TableScene::autoClose));
		//PromptLayer::hideModalObject();
		((Button*)m_Layout->getChildByTag(TAG_TABLE_BTN_QUICK_ENTER))->setTouchEnabled(true);
	}

	void TableScene::closeSocket_noifyFial(Object* obj)
	{
		PromptLayer::createObject("与服务器断开连接，3秒后自动退出游戏");
		UserInfo::instance()->setCurrentServerID(0);
		NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_GAME);
		MainController::getInstance()->setGameState(false);

		this->runAction(Sequence::create(DelayTime::create(3), CallFunc::create(CC_CALLBACK_0(TableScene::autoExit, this)), NULL));
	}

	//发送 桌子号椅子号 给服务端
	void TableScene::sendSitDownMsg(float dt)
	{
		unschedule(schedule_selector(TableScene::sendSitDownMsg));
		SendMsg::sendSitDownReq(NetworkManager::FLAG_GAME, INVALID_TABLE, INVALID_CHAIR);
	}
}
