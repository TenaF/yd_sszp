#ifndef DEFINE_HEAD_FILE
#define DEFINE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
#include <memory.h>
#include <string.h>
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//对应数据类型
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned int		DWORD;
typedef  int		        LONG;
typedef	long long			LONGLONG;
typedef WORD				TCHAR;
typedef long long			SCORE;
typedef unsigned int UINT;


struct SYSTEMTIME {
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
};

//数值定义
#define CopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//对应数据类型
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned int		DWORD;
typedef  long		        LONG;
typedef	long long			LONGLONG;
typedef WORD				TCHAR;
typedef long long			SCORE;
typedef unsigned int UINT;


struct SYSTEMTIME {
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
};

//数值定义
#define CopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif

//数值定义

//头像大小
#define FACE_CX						48									//头像宽度
#define FACE_CY						48									//头像高度

//长度定义
#define LEN_LESS_ACCOUNTS			6									//最短帐号
#define LEN_LESS_NICKNAME			6									//最短昵称
#define LEN_LESS_PASSWORD			6									//最短密码

//人数定义
#define MAX_CHAIR					100									//最大椅子
#define MAX_TABLE					512									//最大桌子
#define MAX_COLUMN					32									//最大列表
#define MAX_ANDROID					256									//最大机器
#define MAX_PROPERTY				128									//最大道具
#define MAX_WHISPER_USER			16									//最大私聊


//列表定义
#define MAX_KIND					128									//最大类型
#define MAX_SERVER					1024								//最大房间

//参数定义
#define INVALID_CHAIR				0xFFFF								//无效椅子
#define INVALID_TABLE				0xFFFF								//无效桌子

//税收定义
#define REVENUE_BENCHMARK			0L								    //税收起点
#define REVENUE_DENOMINATOR			1000L								//税收分母

//////////////////////////////////////////////////////////////////////////////////
//系统参数

//积分类型
#define SCORE						LONGLONG							//积分类型
#define SCORE_STRING				TEXT("%I64d")						//积分类型

//游戏状态
#define GAME_STATUS_FREE			0									//空闲状态
#define GAME_STATUS_PLAY			100									//游戏状态
#define GAME_STATUS_WAIT			200									//等待状态

//系统参数
#define LEN_USER_CHAT				128									//聊天长度
#define TIME_USER_CHAT				1L									//聊天间隔
#define TRUMPET_MAX_CHAR            128									//喇叭长度

//////////////////////////////////////////////////////////////////////////////////
//索引质数

//列表质数
#define PRIME_TYPE					11L									//种类数目
#define PRIME_KIND					53L									//类型数目
#define PRIME_NODE					101L								//节点数目
#define PRIME_PAGE					53L									//自定数目
#define PRIME_SERVER				1009L								//房间数目

//人数质数
#define PRIME_SERVER_USER			503L								//房间人数
#define PRIME_ANDROID_USER			503L								//机器人数
#define PRIME_PLATFORM_USER			100003L								//平台人数

//////////////////////////////////////////////////////////////////////////////////
//数据长度

//资料数据
#define LEN_MD5						33									//加密密码
#define LEN_USERNOTE				32									//备注长度
#define LEN_ACCOUNTS				32									//帐号长度
#define LEN_NICKNAME				32									//昵称长度
#define LEN_PASSWORD				33									//密码长度
#define LEN_GROUP_NAME				32									//社团名字
#define LEN_UNDER_WRITE				32									//个性签名

//数据长度
#define LEN_QQ						16									//Q Q 号码
#define LEN_EMAIL					33									//电子邮件
#define LEN_USER_NOTE				256									//用户备注
#define LEN_SEAT_PHONE				33									//固定电话
#define LEN_MOBILE_PHONE			12									//移动电话
#define LEN_PASS_PORT_ID			19									//证件号码
#define LEN_COMPELLATION			16									//真实名字
#define LEN_DWELLING_PLACE			128									//联系地址

//机器标识
#define LEN_NETWORK_ID				13									//网卡长度
#define LEN_MACHINE_ID				33									//序列长度

//列表数据
#define LEN_TYPE					32									//种类长度
#define LEN_KIND					32									//类型长度
#define LEN_NODE					32									//节点长度
#define LEN_PAGE					32									//定制长度
#define LEN_SERVER					32									//房间长度
#define LEN_PROCESS					32									//进程长度

//////////////////////////////////////////////////////////////////////////////////

//用户关系
#define	CP_NORMAL					0									//未知关系
#define	CP_FRIEND					1									//好友关系
#define	CP_DETEST					2									//厌恶关系
#define CP_SHIELD					3									//屏蔽聊天

//////////////////////////////////////////////////////////////////////////////////

//性别定义
#define GENDER_FEMALE				0									//女性性别
#define GENDER_MANKIND				1									//男性性别
//////////////////////////////////////////////////////////////////////////////////
//Game 2016-5-11
//登陆模式定义
#define LOGIN_TYPE_WINDOWS_ACCOUNT      0                   //PC账号登陆
#define LOGIN_TYPE_WINDOWS_WECHAT		1                   //PC微信登陆
#define LOGIN_TYPE_MB_APPLE_QUICK		2                   //苹果快速登陆
#define LOGIN_TYPE_MB_APPLE_ACCOUNT     3                   //苹果账号登陆
#define LOGIN_TYPE_MB_APPLE_WECHAT		4                   //苹果微信登陆
#define LOGIN_TYPE_MB_ANDROID_QUICK     5                   //安卓快速登陆
#define LOGIN_TYPE_MB_ANDROID_ACCOUNT   6                   //安卓账号登陆
#define LOGIN_TYPE_MB_ANDROID_WECHAT    7                   //安卓微信登陆

//////////////////////////////////////////////////////////////////////////////////

//游戏模式
#define GAME_GENRE_GOLD				0x0001								//金币类型
#define GAME_GENRE_SCORE			0x0002								//点值类型
#define GAME_GENRE_MATCH			0x0004								//比赛类型
#define GAME_GENRE_EDUCATE			0x0008								//训练类型

//分数模式
#define SCORE_GENRE_NORMAL			0x0100								//普通模式
#define SCORE_GENRE_POSITIVE		0x0200								//非负模式

//////////////////////////////////////////////////////////////////////////////////

//用户状态
#define US_NULL						0x00								//没有状态
#define US_FREE						0x01								//站立状态
#define US_SIT						0x02								//坐下状态
#define US_READY					0x03								//同意状态
#define US_LOOKON					0x04								//旁观状态
#define US_PLAYING					0x05								//游戏状态
#define US_OFFLINE					0x06								//断线状态

//////////////////////////////////////////////////////////////////////////////////

//比赛状态
#define MS_NULL						0x00								//没有状态
#define MS_SIGNUP					0x01								//报名状态
#define MS_MATCHING					0x02								//比赛状态
#define MS_OUT						0x03								//淘汰状态

//////////////////////////////////////////////////////////////////////////////////

//房间规则
#define SRL_LOOKON					0x00000001							//旁观标志
#define SRL_OFFLINE					0x00000002							//断线标志
#define SRL_SAME_IP					0x00000004							//同网标志

//房间规则
#define SRL_ROOM_CHAT				0x00000100							//聊天标志
#define SRL_GAME_CHAT				0x00000200							//聊天标志
#define SRL_WISPER_CHAT				0x00000400							//私聊标志
#define SRL_HIDE_USER_INFO			0x00000800							//隐藏标志

//////////////////////////////////////////////////////////////////////////////////
//列表数据

//无效属性
#define UD_NULL						0									//无效子项
#define UD_IMAGE					100									//图形子项
#define UD_CUSTOM					200									//自定子项

//基本属性
#define UD_GAME_ID					1									//游戏标识
#define UD_USER_ID					2									//用户标识
#define	UD_NICKNAME					3									//用户昵称

//扩展属性
#define UD_GENDER					10									//用户性别
#define UD_GROUP_NAME				11									//社团名字
#define UD_UNDER_WRITE				12									//个性签名

//状态信息
#define UD_TABLE					20									//游戏桌号
#define UD_CHAIR					21									//椅子号码

//积分信息
#define UD_SCORE					30									//用户分数
#define UD_GRADE					31									//用户成绩
#define UD_USER_MEDAL				32									//用户经验
#define UD_EXPERIENCE				33									//用户经验
#define UD_LOVELINESS				34									//用户魅力
#define UD_WIN_COUNT				35									//胜局盘数
#define UD_LOST_COUNT				36									//输局盘数
#define UD_DRAW_COUNT				37									//和局盘数
#define UD_FLEE_COUNT				38									//逃局盘数
#define UD_PLAY_COUNT				39									//总局盘数

//积分比率
#define UD_WIN_RATE					40									//用户胜率
#define UD_LOST_RATE				41									//用户输率
#define UD_DRAW_RATE				42									//用户和率
#define UD_FLEE_RATE				43									//用户逃率
#define UD_GAME_LEVEL				44									//游戏等级

//扩展信息
#define UD_NOTE_INFO				50									//用户备注
#define UD_LOOKON_USER				51									//旁观用户

//图像列表
#define UD_IMAGE_FLAG				(UD_IMAGE+1)						//用户标志
#define UD_IMAGE_GENDER				(UD_IMAGE+2)						//用户性别
#define UD_IMAGE_STATUS				(UD_IMAGE+3)						//用户状态

//////////////////////////////////////////////////////////////////////////////////
//数据库定义

#define DB_ERROR 					-1  								//处理失败
#define DB_SUCCESS 					0  									//处理成功
#define DB_NEEDMB 					18 									//处理失败

//////////////////////////////////////////////////////////////////////////////////
//道具标示
#define PT_USE_MARK_DOUBLE_SCORE    0x0001								//双倍积分
#define PT_USE_MARK_FOURE_SCORE     0x0002								//四倍积分
#define PT_USE_MARK_GUARDKICK_CARD  0x0010								//防踢道具
#define PT_USE_MARK_POSSESS         0x0020								//附身道具 

#define MAX_PT_MARK                 4                                   //标识数目

//有效范围
#define VALID_TIME_DOUBLE_SCORE     3600                                //有效时间
#define VALID_TIME_FOUR_SCORE       3600                                //有效时间
#define VALID_TIME_GUARDKICK_CARD   3600                                //防踢时间
#define VALID_TIME_POSSESS          3600                                //附身时间
#define VALID_TIME_KICK_BY_MANAGER  3600                                //游戏时间 

//////////////////////////////////////////////////////////////////////////////////
//设备类型
#define DEVICE_TYPE_PC              0x00                                //PC
#define DEVICE_TYPE_ANDROID         0x10                                //Android
#define DEVICE_TYPE_ITOUCH          0x20                                //iTouch
#define DEVICE_TYPE_IPHONE          0x40                                //iPhone
#define DEVICE_TYPE_IPAD            0x80                                //iPad

/////////////////////////////////////////////////////////////////////////////////
//手机定义

//视图模式
#define	VIEW_MODE_ALL				0x0001								//全部可视
#define	VIEW_MODE_PART				0x0002								//部分可视

//信息模式
#define VIEW_INFO_LEVEL_1			0x0010								//部分信息
#define VIEW_INFO_LEVEL_2			0x0020								//部分信息
#define VIEW_INFO_LEVEL_3			0x0040								//部分信息
#define VIEW_INFO_LEVEL_4			0x0080								//部分信息

//其他配置
#define RECVICE_GAME_CHAT			0x0100								//接收聊天
#define RECVICE_ROOM_CHAT			0x0200								//接收聊天
#define RECVICE_ROOM_WHISPER		0x0400								//接收私聊

//行为标识
#define BEHAVIOR_LOGON_NORMAL       0x0000                              //普通登录
#define BEHAVIOR_LOGON_IMMEDIATELY  0x1000                              //立即登录

/////////////////////////////////////////////////////////////////////////////////
//处理结果
#define RESULT_ERROR 					-1  								//处理错误
#define RESULT_SUCCESS 					0  									//处理成功
#define RESULT_FAIL 					1  									//处理失败

/////////////////////////////////////////////////////////////////////////////////
//变化原因
#define SCORE_REASON_WRITE              0                                   //写分变化
#define SCORE_REASON_INSURE             1                                   //银行变化
#define SCORE_REASON_PROPERTY           2                                   //道具变化
#define SCORE_REASON_MATCH_FEE          3                                   //比赛报名
#define SCORE_REASON_MATCH_QUIT         4                                   //比赛退赛

/////////////////////////////////////////////////////////////////////////////////

//登录房间失败原因
#define LOGON_FAIL_SERVER_INVALIDATION  200                                 //房间失效

////////////////////////////////////////////////////////////////////////////////



#define SR_GAMEMATCH_SCORE			0x01000000							//房间比赛模式
#define SR_GAMEMATCH_WASHOUT		0x02000000							//房间淘汰赛模式

//房间等级
#define GAME_LEVEL_POINTS			0x0001								//积分
#define GAME_LEVEL_LOW				0x0002								//低级
#define GAME_LEVEL_MIDDLE			0x0004								//中级
#define GAME_LEVEL_HIGH				0x0008								//高级

//回调通知 --------------------------------
#define CONNECT_SERVER_FAIL 				"CONNECT_SERVER_FAIL"			//连接服务器失败

//游戏服务器
#define GAME_OUT_MATCH_APPLY_SUCCESS 		"GAME_OUT_MATCH_APPLY_SUCCESS"			//淘汰赛报名成功
#define GAME_OUT_MATCH_QUIT_SUCCESS 		"GAME_OUT_MATCH_QUIT_SUCCESS"			//淘汰赛退赛成功
#define GAME_OUT_MATCH_ERROR 				"GAME_OUT_MATCH_ERROR"					//淘汰赛错误
#define GAME_LOGON_FAILURE 					"GAME_LOGON_FAILURE"					//游戏服务器登录失败
#define GAME_OUT_MATCH_APPLY 				"GAME_OUT_MATCH_APPLY"					//淘汰赛报名
#define GAME_OUT_MATCH_QUIT 				"GAME_OUT_MATCH_QUIT"					//淘汰赛退出
#define GAME_REQUEST_FAILURE 				"GAME_REQUEST_FAILURE"					//请求失败
#define GAME_LOGIN_FINISH 					"GAME_LOGIN_FINISH"						//登录房间完成
#define GAME_WAIT_DISTRIBUTE				"GAME_WAIT_DISTRIBUTE"					//等待分配座椅
#define GAME_SITDOWN_FINISH 				"GAME_SITDOWN_FINISH"					//椅子坐下完成
#define GAME_USER_ENTER 					"GAME_USER_ENTER"						//用户进入房间
#define GAME_CONNECT_SERVER_FAIL 			"GAME_CONNECT_SERVER_FAIL"				//连接游戏服务器失败
#define GAME_DISCONNECT_SERVER 				"GAME_DISCONNECT_SERVER"				//与游戏服务器连接中断
#define GAME_OUT_MATCH_APPLY_COUNT 			"GAME_OUT_MATCH_APPLY_COUNT"			//比赛报名人数
#define GAME_OUT_MATCH_WAITING 				"GAME_OUT_MATCH_WAITING"				//比赛等待中
#define GAME_OUT_MATCH_START 				"GAME_OUT_MATCH_START"					//淘汰赛开始
#define GAME_DISCONNECT				"GAME_DISCONNECT"					//连接服务器断开

#define WEBCHAT_LOGIN_SUCCESS       "WEBCHAT_LOGIN_SUCCESS"             //微信登录成功
#define WEBCHAT_SHARE_SUCCESS       "WEBCHAT_SHARE_SUCCESS"             //微信登录成功
#define WEBCHAT_SHARE_FAILURE       "WEBCHAT_SHARE_FAILURE"             //微信登录成功
#define SHAREWECHAT_SUCCESS					"SHAREWECHAT_SUCCESS"				//微信分享成功
#define SHAREWECHAT_FAILURE					"SHAREWECHAT_FAILURE"				//微信分享失败
#define SHAREEXCHANGE_SUCCESS				"SHAREEXCHANGE_SUCCESS"				//分享兑换成功
#define PAY_RESULT_NOTIFY					"PAY_RESULT_NOTIFY"				//支付完成通知
#define DOWN_PICTURE_NOTIFY					"DOWN_PICTURE_NOTIFY"			//下载图片完成
#define DOWN_SHOP_NOTIFY			"DOWN_SHOP_NOTIFY"		//商城图片下载完成
#define OPEN_SHOP_NOTIFY			"OPEN_SHOP_NOTIFY"		//打开商城
#define SELECT_PAYMENTTYPE_NOTIFY			"SELECT_PAYMENTTYPE_NOTIFY"     //选着商品通知
#define MAINLAYER_GAME_TYPE_NOTIFY			"MAINLAYER_GAME_TYPE_NOTIFY"	//游戏类型列表返回成功
//登录服务器
#define LOGON_BILL_INFO 					"LOGON_BILL_INFO"					//账单信息
#define LOGON_FACE_INFO 					"LOGON_FACE_INFO"					//头像
#define LOGON_OPERATE_SUCCESS 				"LOGON_OPERATE_SUCCESS"				//操作成功
#define LOGON_OPERATE_FAILURE				"LOGON_OPERATE_FAILURE"				//操作失败
#define LOGON_OUT_MATCH_DATA				"LOGON_OUT_MATCH_DATA"				//淘汰赛信息
#define LOGON_LOGON_SUCCESS					"LOGON_LOGON_SUCCESS"				//登录成功
#define LOGON_LOGON_FAILURE					"LOGON_LOGON_FAILURE"				//登录失败
#define CONNECT_NETWORK_FINISH				"CONNECT_NETWORK_FINISH"			//联网
#define LOGON_PACKAGE_INFO					"LOGON_PACKAGE_INFO"				//背包
#define LOGON_REFRESH_USER_INFO				"LOGON_REFRESH_USER_INFO"			//刷新用户信息
#define LOGON_SERVER_LIST 					"LOGON_SERVER_LIST"					//服务器列表

#define LOGON_COMPOSE_ITEM_RESULT 			"LOGON_COMPOSE_ITEM_RESULT"			//道具合成结果
#define LOGON_DISCONNECT_SERVER 			"LOGON_DISCONNECT_SERVER"			//与登录服务器连接中断
#define LOGIN_SERVER_TIME					"LOGIN_SERVER_TIME"					//系统时间
#define LOGON_USER_INSURE_SUCCESS			"LOGON_USER_INSURE_SUCCESS"					//转账成功
#define LOGON_USER_INSURE_FAILURE			"LOGON_USER_INSURE_FAILURE"					//转账失败
#define LOGON_CONNECT_FAIL					"LOGON_CONNECT_FAIL"						//连接服务器失败
#define LOGON_DISCONNECT					"LOGON_DISCONNECT"							//连接服务器断开
#define LOGON_TRANSFER_SUCCESS				"LOGON_TRANSFER_SUCCESS"					//转账完成

#define LOGON_CUSTOMER_QUESTION_FINISH		"LOGON_CUSTOMER_QUESTION_FINISH"			//客服提问返回结果
#define LOGON_CUSTOMER_QUESTION_LIST		"LOGON_CUSTOMER_QUESTION_LIST"				//客服提问完成
#define LOGON_CUSTOMER_QUESTION_DELETE		"LOGON_CUSTOMER_QUESTION_DELETE"			//删除客服提问

/////////////////////////////////////////////////////////////////////////////////
#define HOMEKEY_PRESSED                 "HOME_KEY_PRESSED"
#define HOMEKEY_RETURNGAME				"HOMEKEY_RETURNGAME"


// #if CC_TARGET_PLATFORM ==  CC_PLATFORM_WIN32
 	//#define		LOGIN_SERVER_IP  	"119.29.23.252"
// #else
	//#define		LOGIN_SERVER_IP		"123.57.47.186"
#define		LOGIN_SERVER_IP		"139.199.159.57"
/*#endif*/
#define  LOGIN_SERVER_PORT  8300

const DWORD dwPackKey = 7;
//////////////////////////////////////////////////////////////////////////////////
//版本比较

//产品版本
#define BULID_VER					0									//授权版本
#define PRODUCT_VER					6									//产品版本

//接口版本
#define INTERFACE_VERSION(cbMainVer,cbSubVer)							\
	(DWORD)(														\
	(((BYTE)(PRODUCT_VER))<<24)+									\
	(((BYTE)(cbMainVer))<<16)+										\
	((BYTE)(cbSubVer)<<8))+											\
	((BYTE)(BULID_VER))

//模块版本
#define PROCESS_VERSION(cbMainVer,cbSubVer,cbBuildVer)					\
	(DWORD)(														\
	(((BYTE)(PRODUCT_VER))<<24)+									\
	(((BYTE)(cbMainVer))<<16)+										\
	((BYTE)(cbSubVer)<<8)+											\
	(BYTE)(cbBuildVer))

/////////////////////////////////////////////////////////////////////////////////

//程序版本
#define VERSION_FRAME				PROCESS_VERSION(6,0,3)				//框架版本
#define VERSION_PLAZA				PROCESS_VERSION(6,0,3)				//大厅版本
#define VERSION_MOBILE_ANDROID	    PROCESS_VERSION(6,0,3)				//手机版本
#define VERSION_MOBILE_IOS			PROCESS_VERSION(6,0,3)				//手机版本

/////////////////////////////////////////////////////////////////////////////////
//版本定义
#define VERSION_EFFICACY			0									//效验版本
#define VERSION_FRAME_SDK			INTERFACE_VERSION(6,3)				//框架版本

/////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//常用常量

//无效数值
#define INVALID_BYTE				((BYTE)(0xFF))						//无效数值
#define INVALID_WORD				((WORD)(0xFFFF))					//无效数值
#define INVALID_DWORD				((DWORD)(0xFFFFFFFF))				//无效数值

//////////////////////////////////////////////////////////////////////////////////
//数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

#endif
