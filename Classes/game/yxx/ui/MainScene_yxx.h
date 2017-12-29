#pragma once
#include "sdk/scene/SceneBase.h"
#include "sdk/sdk.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "game/yxx/message/CMD_Game_yxx.h"
#include "game/yxx/data/PokerData_yxx.h"


USING_NS_CC;
using namespace std;
using namespace cocostudio;
using namespace ui;
namespace yxx
{

#define TAG "MainScene"

#define vibleSize Director::getInstance()->getVisibleSize()
	enum TimeType_yxx
	{
		TimeType_yxx_none = 0,
		TimeType_yxx_PlaceJetton,//下注时间
		TimeType_yxx_Start,		 //开牌时间
		TimeType_yxx_Leisure	 //休闲时间
	};
	enum CurrentBankerState_yxx
	{
		CurrentBanker_yxx_Already,//已经再庄
		CurrentBanker_yxx_Ready,  //准备上庄
		CurrentBanker_yxx_None	  //没有上庄
	};
	enum ButtonType_yxx
	{
		ButtonType_yxx_none = 0,
		ButtonType_yxx_automaticJ_10 = 10,		//自动下注10次
		ButtonType_yxx_automaticJ_20 = 20,	    //自动下注20次
		ButtonType_yxx_banker,                 //我要上庄按钮
		ButtonType_yxx_listplantL_move,                 //我要上庄按钮
		ButtonType_yxx_out,                    //退出
		ButtonType_yxx_help,                   //帮助
		ButtonType_yxx_setting,                //设置音效
		ButtonType_yxx_settingMusic,                //设置
		ButtonType_yxx_b,				        //申请上庄列表按钮
		ButtonType_yxx_p,				        //在线人数；列表
		ButtonType_yxx_menu,                   //widge旁边的按钮
		ButtonType_yxx_root,
		ButtonType_yxx_banker_move,	//
		ButtonType_yxx_continue,		//继续
		ButtonType_yxx_automaticJ,			//自动下注
		ButtonType_yxx_repeatJ,			//重复下注
		ButtonType_yxx_record,				//游戏记录按钮
		ButtonType_yxx_Jesson_cancel,    //下注筹码拉伸
		ButtonType_yxx_ludan_move,	 //路单移动
		ButtonType_yxx_ludan_Frame_move,
		ButtonType_yxx_automaticJ_Infinite = 9999999,		//无限跟注
	};
	class MainScene_yxx : public SceneBase
	{
	public:
		static MainScene_yxx * createScene();
		static MainScene_yxx * instance;
		CREATE_FUNC(MainScene_yxx);
		virtual bool init();
		void onEnter();
		void autoExit();
		void onExit();
		virtual void onBackPressed();  //屏蔽安卓退出按键
		Widget * pRoot;
		ImageView * BgPanel;
		void selectedEvent(cocos2d::Ref *pSender, CheckBoxEventType type);
		void selectedEventautomatic(cocos2d::Ref *pSender, CheckBoxEventType type);//自动跟注选择框
		void moveEndPanel(float dt);
		void soundsStartMove(float dt);
		void startDelayMove(float dt);
		void clearPlayer(PokerPlayerInfo * player);					//删除玩家
		void changeBankerButton();
		void overLayoutFunc(CMD_S_GameEnd& end);					//结算界面
		void setOtherPlayer(PokerPlayerInfo * player);				//添加玩家
		void updateScore(CMD_GR_MobileUserScore& buf);
		void updateBankerScore(SCORE itemp_bankerScore);
		void setLabelType(int time, TimeType_yxx type);				//设置不同类型的时间和字
		void createUpdate(int time);								//创建倒计时
		void removeTimeLabel();										//移除时间
		void PlaceJettonFunction(CMD_S_PlaceJetton& jetton);		//玩家下注
		int	getJettonCurType(SCORE jScore);
		void updateShowScore(bool seatId, BYTE jettonArea, SCORE jettonScore,bool isFirstBet = true);	//刷新下注显示和按钮状态
		virtual void initGameScene(CMD_S_StatusFree *obj);			 //初始化游戏
		virtual void initGameScene(CMD_S_StatusPlay *obj);
		void setJessonValue(const LONGLONG itemp_chip[]);
		void addRecordList(tagGameRecord& obj, int itemp_LaterCount = 0);		//添加记录
		void clearRecordList();
		void subChangeBanker(CMD_S_ChangeBanker user);				//切换庄家
		void automaticJCancel();									//取消全部自动跟注
		virtual void removeBankerSub(WORD id);
		void showUpeerBanker(bool isBanker);						//如果是自己上庄的显示界面处理
		virtual void removeBanker(WORD id);
		void hideContinueJetton(bool istrue);						//影藏自动跟注
		void startMoveBlock(CMD_S_GameEnd& gameEnd);				//开始移动方格
		void userApplyBanker(int userId);							//申请坐庄回调
		void userRepeatPlaceJetton(bool istrue, CMD_S_RepeatBet& pQueryStorage,bool isFirstBet = true);//复投消息处理
		Vec2 randJettonPos(int jettonArea);							//随机一个下注区域的坐标
		Vec2 randAngel(int jettonArea);								//随机其他玩家的位置区域
		Vec2 JettonTotalPlace(int jettonArea);
	private:
		MainScene_yxx();
		~MainScene_yxx();
		
		Text *nameL;											//自己的名称
		Text *usableL;											//自己的可用金币
		Text *scoreL;											//输赢
		Text *bankerUserL;										//庄家昵称
		Text *bankerTimeL;										//连庄盘数
		Text *bankerScoreL;									//庄家财富
		Text *timeLabelTop;									//顶上倒计时时间
		Text *timeLable;										//倒计时旁边的标签

		Sprite *typeLabel;										//倒计时类型
		Sprite *S_circleLight;
		Sprite *jettonSprite;

		SCORE lApplyBankerCondition;							//申请上庄条件
		SCORE lUserSingleLimitScore;							//区域限制
		SCORE lAreaLimitScore;									//区域限制
		SCORE lAllLimitScore;									//所有限制
		SCORE my_AllJettonScore;								//下注总数

		Button *repeatJ_btn;									//重复跟注按钮
		Button *automaticJ_btn;								//自动跟注按钮
		Button *Jesson_cancel;									//拉伸按钮
		Button *banker_Btn;									//我要上庄button按钮
		Button *B_CurMoney[6];									//金币下注按钮
		Button *record_btn;										//路单上面的按钮

		Widget *upper_plane;									//上局押注信息地板
		Widget *btn_plane;										//跟注移动plane
		Widget *myJettonLabelPanel[8];							//筹码里面的容器
		Widget *center_point_null_picture;						//旋转动物中心
		Widget *over_layout;									//结算界面

		ListView *banker_list;									//上庄列表
		ListView *ohter_list;
		ListView *record_list;									//游戏记录列表

		ImageView *pic_bg;										//玩家列表亮框
		ImageView *list_planeL;									//上庄，好友列表框
		ImageView *ludan_total;									//路单
		ImageView *jessonPanelBlack;							//下注灰色
		ImageView *show_img;									//开始下注时显示的图片
		ImageView *Image_66;									//顶上下注时间
		ImageView *imageViewBetEnd;							//下注结束
		ImageView *endAnimalBg;									//顶上下注时间
		ImageView *point_Animal;								//顶上下注时间
		ImageView *endAnimal;									//下注结束
		ImageView *blockLight[52];                              //选中动物图片

		CheckBox *checkBox;										//复选框按钮
		
		CCProgressTo *progressTo;
		CCProgressTimer *heart_;
		CCSprite *psSprite1;
		CCSprite *psSprite2;
		
		
		cocostudio::Armature *armatureBird;						//摇动筛子和开始下注动画
		cocostudio::Armature *armatureBirdStop;					//停止下注
		cocostudio::Armature *armatureBirdBegin;				//停止下注

		char init_ResultNumber[3];								//开奖结果

		int m_LaterCount;
		int moveCircleNumber;									//移动的圈数
		int StartMovesound;										//移动的圈数
		int moveNumber;											//移动个数
		int wCurrentBanker;										//当前庄家userId
		int continueJetton_number;								//当前剩余下注次数
		int continue_number;									//跟注次数
		int jettonProbability[6];
		int BankerCount;										//庄家计数

		bool is_trueCheckbox;									//复选框筹码显示
		bool wCurrentIsBanker;									//当前是否为庄
		bool isMenu;											//是否显示选项按钮
		bool isBanerMove;										//是否显示玩家申请上庄信息
		bool isLudanMove;										// 路单移动
		bool isPlayerList;
		bool isContinueJetton;									//是否自动加注
		bool isTouchContinue;									//是否点击了跟注次数按钮
		bool isSound;											//音效
		bool isSound_music;										//音效
		bool isAllowBank;
		bool isWelcomeSound;
		
		TextBMFont *myJettonLabel[22];							//自己的下注数目
		TextBMFont *automaticJ_label;							//剩余跟注显示
		TextBMFont *CurMoneyLabel[6];							//金币显示的数字
		TextBMFont *CurMoneyLabelGray[6];						//金币显示的灰色数字

		LONGLONG socreRecord;									//记录下自己下注前的金币
		LONGLONG  itemp_myJettonLabel[22];
		LONGLONG itemp_totalTtf[22];
		
		SCORE myJettonScore[22];								//自己的下注金额
		SCORE wCurrentJettonScore;								//当前选择的下注分数
		SCORE tagChange[6];										//服务器下发下来的分数

		CMD_S_GameEnd wCurrentGameEnd;							//游戏结束数据
		Vector<TextBMFont *> jettonLabelVec;					//押注数量显示
		TimeType_yxx wCurrentTimeType;							//当前阶段
		CurrentBankerState_yxx wCurrentBankerState;				//自己的庄家状态

		//------------------------方法---------------------------------
		//*************************************************************

		void closeSocket_noifyFial(Object *obj);					//与服务器断开连接
		void timeUpdate(float dt);									//倒计时定时器
		void closeLight(float dt);									//亮图片至灰
		void initLight();											//亮图片至灰
		void closeBlock();											//自家下注的所有地方至灰
		bool checkAutoMatic();										//检测是否满足自动下注的条件
		void showContinueJetton();									//自动跟注显示
		void showUpJetton(bool isUp);								//自动跟注显示
		void JettonArea(Button *btn);								//下注筹码
		void setMenuState(bool isMenu);								//设置右上顶部状态
		void menuButtonMask(bool m_isTrue);							//menu上的蒙版
		void setListState(bool isMenu);								//设置左边玩家信息框状态
		void setludanState(bool isludan);							//路单状态
		bool PlaceJettonLimit(int type);							//押注限制
		void PlaceJettonEffect(int Area, int JessonNumber, bool isMySealt, bool isFirstBet); //押注限制
		void onTouch(Ref* target, Widget::TouchEventType type);
		void onJessonTouch(Ref* target, Widget::TouchEventType type);
		void repaetButtonNoifyFial();								//重复下注回调方法
		bool continueJetton();										//自动下注
		int getCurrentRate(int type);								//获取倍率
		int ButtonNameChangeScore(string currentName);
		SCORE getJettonNumber(int jType);                           //初始化筹码区域
		string CurrentButtonName(int currentScore);
	};
}
