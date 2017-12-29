#pragma once
#include "sdk/scene/SceneBase.h"
#include "sdk/sdk.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "game/sicbo/message/CMD_Game_sicbo.h"
USING_NS_CC;
using namespace std;
using namespace cocostudio;
using namespace ui;
namespace sicbo
{

#define TAG "MainScene"

#define vibleSize Director::getInstance()->getVisibleSize()
	enum TimeType_sicbo
	{
		TimeType_sicbo_none = 0,
		TimeType_sicbo_PlaceJetton,//下注时间
		TimeType_sicbo_Start,		 //开牌时间
		TimeType_sicbo_Leisure	 //休闲时间
	};
	enum CurrentBankerState_sicbo
	{
		CurrentBanker_sicbo_Already,//已经再庄
		CurrentBanker_sicbo_Ready,  //准备上庄
		CurrentBanker_sicbo_None	  //没有上庄
	};
	enum ButtonType_sicbo
	{
		ButtonType_sicbo_none = 0,
		ButtonType_sicbo_automaticJ_10 = 10,		//自动下注10次
		ButtonType_sicbo_automaticJ_20 = 20,	    //自动下注20次
		ButtonType_sicbo_banker,				    //我要上庄按钮
		ButtonType_sicbo_listplantL_move,           //我要上庄按钮
		ButtonType_sicbo_out,                       //退出
		ButtonType_sicbo_help,						//帮助
		ButtonType_sicbo_setting,					//设置音效
		ButtonType_sicbo_settingMusic,              //设置音乐
		ButtonType_sicbo_b,							//申请上庄列表按钮
		ButtonType_sicbo_p,							//在线人数；列表
		ButtonType_sicbo_menu,						//widge旁边的按钮
		ButtonType_sicbo_root,
		ButtonType_sicbo_banker_move,				//
		ButtonType_sicbo_continue,					//继续
		ButtonType_sicbo_automaticJ,				//自动下注
		ButtonType_sicbo_repeatJ,					//重复下注
		ButtonType_sicbo_record,					//游戏记录按钮
		ButtonType_sicbo_Jesson_cancel,				//下注筹码拉伸
		ButtonType_sicbo_ludan_move,				//路单移动
		ButtonType_sicbo_ludan_Frame_move,
		ButtonType_sicbo_automaticJ_Infinite = 9999999,		//无限跟注
	};
	class MainScene_sicbo : public SceneBase
	{
	public:
		static MainScene_sicbo * createScene();
		static MainScene_sicbo * instance;
		CREATE_FUNC(MainScene_sicbo);
		virtual bool init();
		void onEnter();
		void onExit();
		void autoExit();
		void selectedEvent(cocos2d::Ref *pSender, CheckBoxEventType type);
		void selectedEventautomatic(cocos2d::Ref *pSender, CheckBoxEventType type);//自动跟注选择框
		void moveEndPanel(float dt);
		void soundsStartMove(float dt);
		void clearPlayer(Object * player);							//删除玩家
		void changeBankerButton();
		void overLayoutFunc(CMD_S_GameEnd& end);					//结算界面
		void setOtherPlayer(Object * player);						//添加玩家
		void updateScore(CMD_GR_MobileUserScore& buf);
		void updateBankerScore(SCORE itemp_bankerScore);
		void setLabelType(int time, TimeType_sicbo type);			//设置不同类型的时间和字
		void createUpdate(int time);								//创建倒计时
		void timeUpdate(float dt);									//倒计时定时器
		void removeTimeLabel();										//移除时间
		void PlaceJettonFunction(CMD_S_PlaceJetton& jetton);		//玩家下注
		int	getJettonCurType(SCORE jScore);
		void updateShowScore(bool seatId, BYTE jettonArea, SCORE jettonScore, bool isFirstBet = true);	//刷新下注显示和按钮状态
		void setJessonValue(const LONGLONG itemp_Chip[]);
		virtual void initGameScene(CMD_S_StatusFree *obj);			//初始化游戏
		virtual void initGameScene(CMD_S_StatusPlay *obj);
		virtual void removeBanker(WORD id);
		virtual void removeBankerSub(WORD id);
		void addRecordList(tagGameRecord& obj, int itemp_LaterCount = 0);	//添加记录
		void clearRecordList();
		void subChangeBanker(CMD_S_ChangeBanker user);				//切换庄家
		void showUpeerBanker(bool isBanker);						//如果是自己上庄的显示界面处理
		void automaticJCancel();									//取消全部自动跟注
		void hideContinueJetton(bool istrue);						//影藏自动跟注
		void startDelayMove(float dt);
		void startMoveBlock(CMD_S_GameEnd& gameEnd);				//开始移动方格
		void userApplyBanker(int userId);							//申请坐庄回调
		void userRepeatPlaceJetton(bool istrue, CMD_S_RepeatBet& pQueryStorage, bool isFirstBet = true);//复投消息处理
		SCORE getJettonNumber(int jType);                            //初始化筹码区域
		Vec2 randJettonPos(int jettonArea);							//随机一个下注区域的坐标
		Vec2 JettonTotalPlace(int jettonArea);
		Vec2 randAngel(int jettonArea);								//随机其他玩家的位置区域

	private:
		Text * nameL;											//自己的名称
		Text * usableL;											//自己的可用金币
		Text * scoreL;											//输赢
		Text * bankerUserL;										//庄家昵称
		Text * bankerTimeL;										//连庄盘数
		Text * bankerScoreL;									//庄家财富
		Text * timeLabelTop;									//顶上倒计时时间
		Text *timeLable;										//倒计时旁边的标签

		Sprite *S_circleLight;
		Sprite * typeLabel;										//倒计时类型
		Sprite *jettonSprite;
		
		Widget *center_point_null_picture;						//旋转动物中心
		Widget * over_layout;                                   //结算界面
		Widget * pRoot;
		Widget * upper_plane;									//上局押注信息地板
		Widget * btn_plane;										//跟注移动plane
		Widget * myJettonLabelPanel[8];							//筹码里面的容器

		SCORE lApplyBankerCondition;							//申请上庄条件
		SCORE lAreaLimitScore;									//区域限制
		SCORE lUserSingleLimitScore;							//区域限制
		SCORE lAllLimitScore;									//所有限制
		SCORE my_AllJettonScore;								//下注总数
		SCORE myJettonScore[52];								//自己的下注金额
		SCORE wCurrentJettonScore;								//当前选择的下注分数
		SCORE tagChange[6];										//服务器下发下来的分数
		double  itemp_myJettonLabel[52];
		double itemp_totalTtf[52];

		Button * B_CurMoney[6];									//金币下注按钮
		Button * banker_Btn;									//我要上庄button按钮
		Button *record_btn;										//路单上面的按钮
		Button * repeatJ_btn;									//重复跟注按钮
		Button * automaticJ_btn;								//自动跟注按钮
		Button * Jesson_cancel;									//拉伸按钮

		ListView * banker_list;									//上庄列表
		ListView * ohter_list;
		ListView * record_list;									//游戏记录列表

		ImageView * BgPanel;
		ImageView *blockLight[52];                              //选中动物图片
		ImageView *pic_bg;										//玩家列表亮框
		ImageView *list_planeL;									//上庄，好友列表框
		ImageView *ludan_total;									//路单
		ImageView *jessonPanelBlack;							//下注灰色
		ImageView * show_img;									//开始下注时显示的图片
		ImageView *Image_66;									//顶上下注时间
		ImageView * imageViewBetEnd;							//下注结束
		ImageView *endAnimalBg;									//顶上下注时间
		ImageView *point_Animal;								//顶上下注时间
		ImageView * endAnimal;									//下注结束

		CheckBox *checkBox;										//复选框按钮
		
		char init_ResultNumber[3];								//开奖结果
		
		int BankerCount;										//是否升起筹码
		int m_LaterCount;
		int moveCircleNumber;									//移动的圈数
		int moveNumber;											//移动个数
		int wCurrentBanker;										//当前庄家userId
		int continueJetton_number;								//当前剩余下注次数
		int continue_number;									//跟注次数
		int  jettonProbability[6];
		int StartMovesound;

		bool is_trueCheckbox;									//复选框筹码显示
		bool wCurrentIsBanker;									//当前是否为庄
		bool isMenu;											//是否显示选项按钮
		bool isBanerMove;										//是否显示玩家申请上庄信息
		bool isLudanMove;										// 路单移动
		bool isPlayerList;
		bool isContinueJetton;									//是否自动加注
		bool isSound;											//音效
		bool isSound_music;										//音效
		bool isTouchContinue;									//是否点击了跟注次数按钮
		bool isAllowBank;

		LONGLONG socreRecord;									//记录下自己下注前的金币
		TextBMFont * automaticJ_label;							//剩余跟注显示
		TextBMFont * myJettonLabel[52];							//自己的下注数目
		TextBMFont * CurMoneyLabel[6];							//金币显示的数字
		TextBMFont * CurMoneyLabelGray[6];						//金币显示的灰色数字

		
		cocostudio::Armature *armatureBird;
		CMD_S_GameEnd wCurrentGameEnd;							//游戏结束数据
		Vector<TextBMFont *> jettonLabelVec;					//押注数量显示
		TimeType_sicbo wCurrentTimeType;						//当前阶段
		CurrentBankerState_sicbo wCurrentBankerState;			//自己的庄家状态
		
		//-----------------------------------------------------------------------------
		//****************************方法*********************************************
		MainScene_sicbo();
		~MainScene_sicbo();
		string CurrentButtonName(int currentScore);
		int ButtonNameChangeScore(string currentName);
		int getCurrentRate(int type);								//获取倍率
		void closeSocket_noifyFial(Object *obj);					//与服务器断开连接
		void repaetButtonNoifyFial();								//重复下注回调方法
		bool continueJetton();										//自动下注
		void closeLight(float dt);									//亮图片至灰
		void initLight();											//亮图片至灰
		void closeBlock();											//自家下注的所有地方至灰
		void LightPart();											//亮一部分
		bool checkAutoMatic();										//检测是否满足自动下注的条件
		void showContinueJetton();									//自动跟注显示
		void showUpJetton(bool isUp);									//自动跟注显示
		void JettonArea(Button *btn);											//下注筹码
		void setMenuState(bool isMenu);								//设置右上顶部状态
		void menuButtonMask(bool m_isTrue);							//menu上蒙版的状态
		void setListState(bool isMenu);								//设置左边玩家信息框状态
		void setludanState(bool isludan);							//路单状态
		bool PlaceJettonLimit(int type);							//押注限制
		virtual void onBackPressed();
		void PlaceJettonEffect(int Area, int JessonNumber, bool isMySealt, bool isFirstBet);							//押注限制
		void onTouch(Ref* target, Widget::TouchEventType type);
		void onJessonTouch(Ref* target, Widget::TouchEventType type);
		
	};
}
