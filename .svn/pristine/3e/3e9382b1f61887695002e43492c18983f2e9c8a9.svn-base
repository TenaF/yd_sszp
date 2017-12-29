#ifndef __hello2dx__MainScene_sszp_sszp__
#define __hello2dx__MainScene_sszp_sszp__
#include "sdk/scene/SceneBase.h"
#include "sdk/sdk.h"
#include "cocos-ext.h" 
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "sdk/ui/ProressCirCle.h"
#include "game/sszp/message/CMD_Game_sszp.h"

USING_NS_CC;
using namespace std;
using namespace cocostudio;
using namespace ui;
namespace sszp
{

#define TAG "MainScene"

#define vibleSize Director::getInstance()->getVisibleSize()
	enum TimeType_sszp
	{
		TimeType_sszp_none = 0,
		TimeType_sszp_PlaceJetton,		//下注时间
		TimeType_sszp_Start,			//开牌时间
		TimeType_sszp_Leisure			//休闲时间
	};
	enum CurrentBankerState_sszp
	{
		CurrentBanker_sszp_Already,		//已经再庄
		CurrentBanker_sszp_Ready,		//准备上庄
		CurrentBanker_sszp_None			//没有上庄
	};
	//----------图片枚举状态-------------------
	enum CHANGE_BANKER_BUTTON  //上庄按钮切换状态
	{
		CHANGE_BANKER_BUTTON_SSZP_NORMAL,				//申请上庄 正常的按钮
		CHANGE_BANKER_BUTTON_SSZP_NORMALGRAY,			//申请上庄 灰色的按钮
		CHANGE_BANKER_BUTTON_SSZP_APPLYUPNORMAL,		//申请下庄 正常的按钮
		CHANGE_BANKER_BUTTON_SSZP_MINEUPGRAY,			//我要下庄 灰色的按钮
		CHANGE_BANKER_BUTTON_SSZP_CANCELNORMAL		//取消申请 正常的按钮
	};
	enum CHANGE_VOICE_MP3  //上庄按钮切换状态
	{
		CHANGE_VOICE_MP3_SSZP_PLEASE_BET,			//请下注
		CHANGE_VOICE_MP3_SSZP_BET_END,				//停止下注
		
	};
	enum CHANGE_LABEL_PIC  //开始下注，空闲时间，游戏开始时间的三个倒计时显示图片
	{
		CHANGE_LABEL_PIC_SSZP_BEGIN,		//开始下注
		CHANGE_LABEL_PIC_SSZP_START,		//开牌
		CHANGE_LABEL_PIC_SSZP_LEISURE		//空闲

	};
	//-----------------------------------------------------------------
	enum ButtonType_sszp
	{
		ButtonType_sszp_none = 0,
		ButtonType_sszp_automaticJ_10 = 10,		//自动下注10次
		ButtonType_sszp_automaticJ_20 = 20,	    //自动下注20次
		ButtonType_sszp_banker,                 //我要上庄按钮
		ButtonType_sszp_out,                    //退出
		ButtonType_sszp_help,                   //帮助
		ButtonType_sszp_setting,                //设置
		ButtonType_sszp_settingMusic,
		ButtonType_sszp_b,				        //申请上庄列表按钮
		ButtonType_sszp_p,				        //在线人数；列表
		ButtonType_sszp_menu,                   //widge旁边的按钮
		ButtonType_sszp_root,
		ButtonType_sszp_banker_move,	//
		ButtonType_sszp_continue,		//继续
		ButtonType_sszp_automaticJ,			//自动下注
		ButtonType_sszp_repeatJ,			//重复下注
		ButtonType_sszp_repeatJSure,			//重复下注确定
		ButtonType_sszp_repeatJClose,			//重复下注关闭
		ButtonType_sszp_record,				//游戏记录按钮
		ButtonType_sszp_Jesson_cancel,    //下注筹码拉伸
		ButtonType_sszp_listplantL_move,
		ButtonType_sszp_automaticJ_Infinite = 9999999		//无限跟注
	};
	class MainScene_sszp : public SceneBase
	{
	public:
		static MainScene_sszp * createScene();
		static MainScene_sszp * instance;
		CREATE_FUNC(MainScene_sszp);
		void onEnter();
		void onExit();
		void autoExit();
		void clearPlayer(Object * player);								//删除玩家
		void setOhterPlayer(Object * player);							//添加玩家
		void updateScore(CMD_GR_MobileUserScore& buf);
		void setLabelType(int time, TimeType_sszp type, SCORE bankerScore = 5000000);		//设置不同类型的时间和字
		void PlaceJettonFunction(CMD_S_PlaceJetton& jetton);			//玩家下注
		
		void initGameScene(CMD_S_StatusFree& obj);						//初始化游戏
		void initGameScene(CMD_S_StatusPlay& obj);
		void addRecordList(int id, bool isInit);						//添加记录
		void clearRecordList();											//CGameClientDlg//记录信息
		void subChangeBanker(CMD_S_ChangeBanker& user);					//切换庄家
		void removeBanker(string name);
		void removeBanker(WORD id);
		void hideContinueJetton();										//CGameClientDlg取消掉所有状态
		void setJessonValue(const LONGLONG itemp_Chip[]);							//筹码赋值
		void userApplyBanker(int userId);											//CMD申请坐庄回调
		void userRepeatPlaceJetton(bool istrue, CMD_S_RepeatBet& pQueryStorage);	//CMD复投消息处理
		void startMoveBlock(CMD_S_GameEnd& gameEnd);				//CMD开始移动方格

	private:
		int initNumber;											//开奖结果
		int moveCircleNumber;									//移动的圈数
		int moveNumber;											//移动个数
		int wCurrentBanker;										//当前庄家userId
		int continueJetton_number;								//当前剩余下注次数
		int continue_number;									//跟注次数
		int BankerCount;										//庄家计数

		bool isSound;											//音效
		bool isSound_music;										//音效
		bool wCurrentIsBanker;									//当前是否为庄
		bool isMenu;											//是否显示选项按钮
		bool isBanerMove;										//是否显示玩家申请上庄信息
		bool isPlayerList;
		bool isContinueJetton;									//是否自动加注
		bool isUpJetton;										//是否升起筹码
		bool isTouchContinue;									//是否点击了跟注次数按钮
		bool isAllowBank;

		Sprite *S_circleLight;
		Sprite * typeLabel;										//倒计时类型
		Layout * ove_layout;                                    //结算界面 
		Text * nameL;											//自己的名称
		Text * beanL;											//自己的酒吧豆
		Text * usableL;											//自己的可用金币
		Text * scoreL;											//输赢
		Text * bankerUserL;										//庄家昵称
		Text * bankerTimeL;										//庄家成绩
		Text * bankerScoreL;									//庄家金币

		Widget * Panel_7;
		Widget * upper_plane;									//上局押注信息地板
		Widget * btn_plane;										//跟注移动plane
		Widget * pRoot;
		Widget * over_layout;									//结算界面
		Widget *center_point_null_picture;						//旋转动物中心


		TextBMFont * timeLabel;									//倒计时数字
		TextBMFont * timeLabelTop;								//顶上倒计时时间
		TextBMFont * myJettonLabel[8];							//自己的下注数目
		TextBMFont * myJettonUpMeLabel[8];						//自己的下注数目
		TextBMFont * myJettonUpTotalLabel[8];					//自己的下注数目
		TextBMFont * automaticJ_label;							//剩余跟注显示
		TextBMFont * CurMoneyLabel[6];							//金币显示的数字
		TextBMFont * CurMoneyLabelGray[6];						//金币显示的灰色数字
		Vector<TextBMFont *> jettonLabelVec;					//押注数量显示

		SCORE lAllLimitScore;									//区域限制
		SCORE lApplyBankerCondition;							//申请上庄条件
		SCORE lAreaLimitScore;									//区域限制
		SCORE lUserSingleLimitScore;							//区域限制
		SCORE my_AllJettonScore;								//下注总数
		SCORE my_AllJettonResultScore;							//结算用下注总数

		Button * repeatJ_btn;									//重复跟注按钮
		Button * automaticJ_btn;								//自动跟注按钮
		Button * Jesson_cancel;									//拉伸按钮
		Button *setting_btn[8];									//下注区域按钮
		Button * B_CurMoney[6];									//金币下注按钮
		Button * banker_Btn;									//我要上庄button按钮

		ListView * banker_list;									//上庄列表
		ListView * ohter_list;
		ListView * record_list;									//游戏记录列表

		ImageView *jessonPanelBlack;							//下注灰色
		ImageView *pic_bg;										//玩家列表亮框
		ImageView *list_planeL;
		ImageView *block[16];									//选中动物图片
		ImageView * myJettonLabelPanel[8];						//筹码里面的容器
		ImageView * BgPanel;
		ImageView * show_img;									//开始下注时显示的图片
		ImageView *label_bet;									//顶上下注时间
		ImageView * imageViewBetEnd;							//下注结束
		ImageView *endAnimalBg;									//顶上下注时间
		ImageView *point_Animal;								//顶上下注时间
		ImageView * endAnimal;									//下注结束

		LONGLONG  itemp_myJettonLabel[8];
		LONGLONG itemp_totalTtf[8];

		SCORE tagChange[6];										//服务器下发下来的分数
		SCORE myJettonScore[8];									//自己的下注金额
		SCORE wCurrentJettonScore;								//当前选择的下注分数

		CMD_S_GameEnd wCurrentGameEnd;							//游戏结束数据
		extension::Scale9Sprite *blockLight[8];                 //金币框
		TimeType_sszp wCurrentTimeType;							//当前阶段
		CurrentBankerState_sszp wCurrentBankerState;			//自己的庄家状态
		ProressCirCle *circle;									//倒计时圆圈

		const int jessonUpCount = 74;
		const int jessonDownCount = 58;
		//-------------------------------------------------------------------------------
		//***************************方法***********************************
		//-------------------------------------------------------------------------------
		MainScene_sszp();
		~MainScene_sszp();
		virtual bool init();
		virtual void onBackPressed();

		void closeSocket_noifyFial(Object *obj);					//与服务器断开连接
		void moveSlowLight(float dt);
		void moveEndAnimalPanel(float dt);
		void changeBankerButton();
		void overLayoutFunc(int type, CMD_S_GameEnd& end);           //结算界面
		void isJessonGray(bool m_bIsGray);							//筹码是否变灰
		void JessonUpOrDown(bool itemp_bIsUpOrDown, int itemp_wCurrentJettonScore);					//筹码升起还是降落
		void openSmallJesson();										//默认选择最小的一个筹码
		void automaticJCancel();									//自动跟注勾全部取消掉
		void createUpdate(int time);								//创建倒计时
		void timeUpdate(float dt);									//倒计时定时器
		void removeTimeLabel();										//移除时间
		void showUpeerBanker(bool isBanker);						//如果是自己上庄的显示界面处理
		void updateShowScore(bool isMySeatId, BYTE jettonArea, SCORE jettonScore);		//刷新下注显示和按钮状态
		void changeBankerButtonState(CHANGE_BANKER_BUTTON m_eState);			//改变按钮状态
		void changeVoiceMp3(CHANGE_VOICE_MP3 m_eState);			//改变按钮状态
		void changeLabelTime(CHANGE_LABEL_PIC m_eState);			//改变按钮状态
	
		bool continueJetton();										//自动下注
		bool PlaceJettonLimit(int type);							//押注限制
		bool checkAutoMatic();										//检测是否满足自动下注的条件

		int changeEndNumber(int itemp_endNumber);
		int	getJettonType(SCORE jScore);
		int	getJettonCurType(SCORE jScore);
		int getCurrentRate(int type);								//获取倍率
		int ButtonNameChangeScore(string currentName);

		SCORE getJettonNumber(int jType);							//初始化筹码区域
		Vec2 randAngel(int jettonArea);								//随机其他玩家的位置区域
		Vec2 randJettonPos(int jettonArea);							//随机一个下注区域的坐标
		string CurrentButtonName(int currentScore);
		
		
		void repaetButtonNoifyFial();								//重复下注回调方法
		void showContinueJetton();									//自动跟注显示
		void JettonArea(Button *btn);								//下注筹码
		void selectedEventautomatic(cocos2d::Ref *pSender, CheckBoxEventType type);//自动跟注选择框
		void setMenuState(bool isMenu);								//设置右上顶部状态
		void menuButtonMask(bool m_isTrue, int m_iTouchTag = 0);							//menu上的蒙版状态
		void setListState(bool isMenu);								//设置左边玩家信息框状态
		void PlaceJettonEffect(int Area, int JessonNumber, bool isMySealt);			//筹码效果
		void onTouch(Ref* target, Widget::TouchEventType type);
		void onJessonTouch(Ref* target, Widget::TouchEventType type);
	};
}
#endif
