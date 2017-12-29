#pragma once
#include "sdk/scene/SceneBase.h"
#include "sdk/sdk.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "game/fruit/message/CMD_Game_fruit.h"
USING_NS_CC;
using namespace std;
using namespace cocostudio;
using namespace ui;
namespace fruit
{

#define TAG "MainScene"

#define vibleSize Director::getInstance()->getVisibleSize()
	enum TimeType_fruit
	{
		TimeType_fruit_none = 0,
		TimeType_fruit_PlaceJetton,		//下注时间
		TimeType_fruit_Start,			//开牌时间
		TimeType_fruit_Leisure			//猜大小时间
	};
	enum CurrentBankerState_fruit
	{
		CurrentBanker_fruit_Already,	//已经再庄
		CurrentBanker_fruit_Ready,		//准备上庄
		CurrentBanker_fruit_None		//没有上庄
	};
	enum ButtonType_fruit
	{
		ButtonType_fruit_none = 0,
		ButtonType_fruit_automaticJ_10 = 10,				//自动下注10次
		ButtonType_fruit_automaticJ_20 = 20,				//自动下注20次
		ButtonType_fruit_banker,							//我要上庄按钮
		ButtonType_fruit_listplantL_move,					//我要上庄按钮
		ButtonType_fruit_out,								//退出
		ButtonType_fruit_help,								//帮助
		ButtonType_fruit_setting,							//设置
		ButtonType_fruit_b,									//申请上庄列表按钮
		ButtonType_fruit_p,									//在线人数；列表
		ButtonType_fruit_menu,								//widge旁边的按钮
		ButtonType_fruit_root,
		ButtonType_fruit_automaticJ_Infinite = 9999999,		//无限跟注
	};
	class MainScene_fruit : public SceneBase
	{
	public:
		static MainScene_fruit * createScene();
		static MainScene_fruit * instance;
		CREATE_FUNC(MainScene_fruit);
		virtual bool init();
		void onEnter();
		void onExit();
		virtual void onBackPressed();
		void selectedEvent(cocos2d::Ref *pSender, CheckBoxEventType type);
		void continueUpJetton(float dt);								//各个筹码下注的连住
		void continueUpJettonAll(float dt);								//全部筹码下注的连住
		void continueRightJetton(float dt);								//右按钮下注的连住
		void continueGoButton(float dt);								//右按钮下注的连住
		void continueScoreJetton(float dt);								//左边金币逐渐移到右边
		void continueScoreJettonTwo(float dt);							//左边金币逐渐移到右边第二遍
		void continueZha(float dt);										//左边金币逐渐移到右边第三遍
		void continueScoreJettonFour(float dt);							//左边金币逐渐移到右边第四遍
		void subChangeGold(CMD_S_ChangeGold& end);						//兑换金币
		void subButtonRight(CMD_S_ButtonRight& end);					//右移金币或者结束猜大小
		void subButtonLeft(CMD_S_ButtonLeft& end);						//左移金币
		void subCompare(CMD_S_Guess& end);								//比大小
		void updateScore(CMD_GR_MobileUserScore& buf);
		void setLabelType(int time, TimeType_fruit type);				//设置不同类型的时间和字
		void PlaceJettonFunction(CMD_S_PlaceJetton& jetton);			//玩家下注
		void updateShowScore(bool seatId, BYTE jettonArea, SCORE jettonScore);		//刷新下注显示和按钮状态
		virtual void initGameScene(CMD_S_StatusFree& obj);				//初始化游戏
		virtual void initGameScene(CMD_S_StatusPlay& obj);
		void startMoveBlock(CMD_S_GameEnd& gameEnd);					//开始移动方格
		void userApplyBanker(int userId);								//申请坐庄回调
		void userRepeatPlaceJetton(bool istrue, CMD_S_RepeatBet& pQueryStorage);	//复投消息处理
		
		
	private:
		TextBMFont *timeLabel_Mid;									//中间倒计时或者猜大小金币
		TextBMFont * myJettonScoreShow[8];							//自己下注分数
		TextBMFont * scoreTopRight;									//顶上总金币
		TextBMFont * scoreTopLeft;									//顶上赢得的金币
		TextBMFont * myJettonLabel[52];								//自己的下注数目
		TextBMFont * automaticJ_label;								//剩余跟注显示

		Button * anniu_left;										//2017-8-30比大小上分左按钮
		Button * anniu_right;										//2017-8-30比大小上分右按钮
		Button * anniu_1_7;											//2017-8-30比大小的小按钮
		Button * anniu_8_14;										//2017-8-30比大小的大按钮
		Button * anniu_all;											//2017-8-30按钮全部下注一次
		Button * anniu_go;											//2017-8-30按钮开始
		Button * B_CurMoney[8];										//金币下注按钮

		ImageView *timeLable;										//倒计时旁边的标签
		ImageView *blockLight[24];									//选中动物图片
		ImageView *blockSideLight;									//底上亮的图片
		ImageView *blockSideLightTwo;								//底上亮的图片
		ImageView *light_right[8];									//底上亮的图片
		ImageView *light_left[3];									//底上亮的图片
		ImageView *goFrame;
		ImageView *goFrame_1;
		ImageView * endAnimal;										//下注结束
		ImageView * BgPanel;

		Widget *center_point_null_picture;							//旋转动物中心
		Widget * myJettonLabelPanel[8];								//筹码里面的容器
		Widget * pRoot;

		Sprite * typeLabel;											//倒计时类型
		Layout * ove_layout;										//结算界面
		ListView * ohter_list;

		char init_ResultNumber[5];									//开奖结果
		
		int moveCircleNumber;										//移动的圈数
		int moveNumber;												//移动个数
		int jessonUpNumber;											//下注暂时记录的数据
		int rand_x;													//按键上面亮的灯控制
		int isGo_state;												//状态
		int rand_y;
		int guess_number_result;
		int itemp_number;
		int itemp_numberLightRotate;
		int itemp_guess_numberResult;
		int itemp_rotate_count;
		int itemp_rotate_count_first;
		int itemp_rotate_count_second;
		int itemp_rotate_count_third;
		int itemp_rotate_count_fourth;
		int itemp_gun_number;										//lucky打枪第一次初始位置
		int itemp_gun_second_number;								//lucky打枪第二次初始位置
		int itemp_gun_third_number;;								//lucky打枪第三次初始位置
		int itemp_gun_fourth_number;;								//lucky打枪第四次初始位置
		int wCurrentBanker;											//当前庄家userId
		int continueJetton_number;									//当前剩余下注次数
		int continue_number;										//跟注次数
		int itemp_gun_count;										//打枪次数
		int score_rank;												//房间倍率
		int isStart;												//开始游戏

		bool visible_twinkle;
		bool visible_twinkle_first;
		bool visible_twinkle_second;
		bool wCurrentIsBanker;										//当前是否为庄
		bool isMenu;												//是否显示选项按钮
		bool isContinueJetton;										//是否自动加注
		bool isGo;
		bool isSound;												//音效
		bool is_guess_visible;
		bool is_guess_useable;
		bool init_trut_or_false_Light;

		double itemp_totalTtf[52];

		SCORE my_AllJettonScore;									//下注总数
		SCORE scoreGameResult;										//游戏结束后房间的得分
		SCORE  wCurrentGameButtonRightScore;
		SCORE wCurrentJettonScore;									//当前选择的下注分数
		SCORE myJettonScore[52];									//自己的下注金额

		LONGLONG socreRecord;										//记录下自己下注前的金币
		LONGLONG  itemp_myJettonLabel[8];							//记录自己下注
		LONGLONG  myRepeatJettonLabel[8];							//重复下注

		CMD_S_GameEnd wCurrentGameEnd;								//游戏结束数据
		CMD_S_Guess  wCurrentGameGuess;
		
		TimeType_fruit wCurrentTimeType;							//当前阶段
		CurrentBankerState_fruit wCurrentBankerState;				//自己的庄家状态
		
	private:

		MainScene_fruit();
		~MainScene_fruit();
		void closeLight(float dt);									//亮图片至灰
		void initLight();											//亮图片至灰
		bool isDataNull(LONGLONG *obj, int length);					//数据是否为空
		void sideLight(float dt);									//下注时间随机亮
		int changeVoice(int voice);									//开牌水果声音
		int rotateScore(int voice);									//开牌水果声音
		void rotateLight(float dt);									//旋转函数
		void showScoreLeft(float dt);								//左边分数显示

		void rotateLightLuckyFirst(float dt);						//lucky第一次旋转函数
		void rotateLightLuckySecond(float dt);						//lucky第二次旋转函数
		void rotateLightLuckyThird(float dt);						//lucky第三次旋转函数
		void rotateLightLuckyFourth(float dt);						//lucky第四次旋转函数

		void showScoreOneSounds(float dt);							//打枪后第一次中奖信息
		void showScoreTwoSounds(float dt);							//打枪后第二次中奖信息
		void showScoreThirdSounds(float dt);						//打枪后第三次中奖信息
		void showScoreFourthSounds(float dt);						//打枪后第四次中奖信息

		void showSounds(float dt);
		void showScoreLater(float dt);
		void showSoundsTwo(float dt);
		void showSoundsThrid(float dt);
		void showSoundsThourth(float dt);

		void guessCompareone(float dt);
		void guessComparetwo(float dt);
		void guessComparethrid(float dt);
		void guessComparefinal(float dt);

		void guessWinSong(float dt);
		void lightTwinkle(float dt);								//旋转开始函数
		void rotateLightTwo(float dt);								//旋转中间函数
		void rotateLightThree(float dt);							//旋转最后函数

		void showUpJetton(bool isUp);								//自动跟注显示
		void JettonArea(int btn_number);							//下注筹码
		void JettonRepeat();										//重复下注
		void setMenuState(bool isMenu);								//设置右上顶部状态
		void closeSocket_noifyFial(Object *obj);					//与服务器断开连接
		void onTouch(Ref* target, Widget::TouchEventType type);
	};
}
