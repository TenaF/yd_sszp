#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//登录命令

#define MDM_GP_LOGON				1									//广场登录

//登录模式
#define SUB_GP_LOGON_GAMEID			1									//I D 登录
#define SUB_GP_LOGON_ACCOUNTS		2									//帐号登录
#define SUB_GP_REGISTER_ACCOUNTS	3									//注册帐号
//Game 2016-5-25 微信登录指令
#define SUB_GP_WECHAT_LOGON			4									//微信登陆

//登录结果
#define SUB_GP_LOGON_SUCCESS		100									//登录成功
#define SUB_GP_LOGON_FAILURE		101									//登录失败
#define SUB_GP_LOGON_FINISH			102									//登录完成
#define SUB_GP_VALIDATE_MBCARD      103                                 //登录失败

#define SUB_GP_WECHAT_LOGON_SUCCESS 104									//微信登陆成功

//升级提示
#define SUB_GP_UPDATE_NOTIFY		200									//升级提示

//////////////////////////////////////////////////////////////////////////////////
//

#define MB_VALIDATE_FLAGS           0x01                                //效验密保
#define LOW_VER_VALIDATE_FLAGS      0x02                                //效验低版本

//Game 2016-5-26 PC端微信快速登陆
struct CMD_GP_WeChatLogon_Request
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	//	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	DWORD							dwLogonType;						//0 PC登陆 1 PC微信快速登陆 2微信注册 3苹果一键登录 4手机普通注册、登陆 

	TCHAR							szUniqueIdentify[50];				//唯一标识(广告标识或者微信unioinid)

	//微信登陆才有用户昵称信息
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	BYTE							cbValidateFlags;			        //校验标识

	//2014-9-10 增加推广字段
	TCHAR							szPromotionID[255];					//推广ID号

	//2015-9-21 新增硬盘ID和计算机名称
	TCHAR							szHDSerial[LEN_MD5];				//硬盘ID
	TCHAR							szComputerName[LEN_MD5];			//计算机名称
};

//I D 登录
struct CMD_GP_LogonGameID
{
	DWORD							dwLogonType;						//0 PC登陆 1 PC微信快速登陆 2微信注册 3苹果一键登录 4手机普通注册、登陆 

	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP

	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//登录信息
	DWORD							dwGameID;							//游戏 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	BYTE							cbValidateFlags;			        //校验标识
};

//帐号登录
struct CMD_GP_LogonAccounts
{
	DWORD							dwLogonType;						//0 PC登陆 1 PC微信快速登陆 2微信注册 3苹果一键登录 4手机普通注册、登陆 

	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	//Game 2015-9-21 新增硬盘ID和计算机名称
	TCHAR							szHDSerial[LEN_MD5];				//硬盘ID
	TCHAR							szComputerName[LEN_MD5];			//计算机名称
	BYTE							cbValidateFlags;			        //校验标识
};

//注册帐号
struct CMD_GP_RegisterAccounts
{
	DWORD							dwLogonType;						//0 PC登陆 1 PC微信快速登陆 2微信注册 3苹果一键登录 4手机普通注册、登陆 

	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字

	TCHAR							m_szQQNumber[LEN_QQ];				//QQ号码
	TCHAR							m_szPhoneNum[LEN_MOBILE_PHONE];		//手机号码
	TCHAR							m_szUnderWrite[LEN_UNDER_WRITE];	//个性签名

	BYTE							cbValidateFlags;			        //校验标识

	//2014-9-10 增加推广字段
	TCHAR							szPromotionID[255];					//推广ID号

	//2015-9-21 新增硬盘ID和计算机名称
	TCHAR							szHDSerial[LEN_MD5];				//硬盘ID
	TCHAR							szComputerName[LEN_MD5];			//计算机名称
};

//登录成功
struct CMD_GP_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwGroupID;							//社团标识
	DWORD							dwCustomID;							//自定标识
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力

	//用户成绩
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//用户银行

	//用户信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号xQ
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//配置信息
	BYTE                            cbShowServerStatus;                 //显示服务器状态

	//Game 2016-9-23 填写密保资料 -- 登陆成功构造包时，新增密保资料ID
	BYTE							cbProtectID;						//密保资料ID

	//Game 2016-9-11 获取监控列表里面的间隔时间、图片总数、图片质量
	DWORD							dwMonitorFlag;						//监控标志 0-不监控，1-监控
	DWORD							dwIntervalTime;						//截图时间间隔
	DWORD							dwPicNumber;						//图片总数
	DWORD							dwPicPack;							//图片质量

	//2016-5-20 新增微信绑定字段
	bool							IsBindWeChat;						//微信是否已绑定
	//2016-6-22 新增登陆类型字段
	DWORD							dwLogonType;						//登陆类型

	//Game 2017-02-22 新增奖券字段
	DWORD							lMedal;								//用户奖券

	//Game 2017-6-12 新增三个字段
	DWORD							dwKindID;							//游戏KindID
	DWORD							dwServerID;							//房间ID
	TCHAR							IsBindPhone[12];					//手机绑定标志
};

//登录失败
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//登陆完成
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//中断时间
	WORD							wOnLineCountTime;					//更新时间
};

//登录失败
struct CMD_GP_ValidateMBCard
{
	UINT							uMBCardID;							//机器序列
};

//升级提示
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//强行升级
	BYTE							cbAdviceUpdate;						//建议升级
	DWORD							dwCurrentVersion;					//当前版本
};

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//社团信息
#define DTP_GP_MEMBER_INFO			2									//会员信息
#define	DTP_GP_UNDER_WRITE			3									//个性签名
#define DTP_GP_STATION_URL			4									//主页信息

//社团信息
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//社团索引
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
};

//会员信息
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//会员等级
	SYSTEMTIME						MemberOverDate;						//到期时间
};

//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_GP_SERVER_LIST			2									//列表信息

//获取命令
#define SUB_GP_GET_LIST				1									//获取列表
#define SUB_GP_GET_SERVER			2									//获取房间
#define SUB_GP_GET_ONLINE			3									//获取在线
#define SUB_GP_GET_COLLECTION		4									//获取收藏
//Game 2016-10-23
#define SUB_GP_GAMESUBTYPE			5									//获取游戏子类型

//列表信息
#define SUB_GP_LIST_TYPE			100									//类型列表
#define SUB_GP_LIST_KIND			101									//种类列表
#define SUB_GP_LIST_NODE			102									//节点列表
#define SUB_GP_LIST_PAGE			103									//定制列表
#define SUB_GP_LIST_SERVER			104									//房间列表
#define SUB_GP_VIDEO_OPTION			105									//视频配置
#define SUB_GP_GET_NOTICE_SYSTEMMESSAGE		107							//获取公告列表
#define SUB_GP_ANS_NOTICE_LIST		109									//返回公告列表

//Game 2016-10-23
#define SUB_GP_GAME_SUBTYPE			110									//返回游戏子类型

//完成信息
#define SUB_GP_LIST_FINISH			200									//发送完成
#define SUB_GP_SERVER_FINISH		201									//房间完成

//在线信息
#define SUB_GR_KINE_ONLINE			300									//类型在线
#define SUB_GR_SERVER_ONLINE		301									//房间在线

//////////////////////////////////////////////////////////////////////////
//2015-7-2
#define SUB_GP_GET_CONFIG			400									//请求获取RC4 KEY
#define SUB_GP_GET_CONFIG_Success	401									//获取获取RC4 KEY成功

//////////////////////////////////////////////////////////////////////////////////
//2015-7-2 重新定义获取RC4 KEY结构在列表命令下
struct CMD_GP_Get_Config
{
	//Game 2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	//Game 2016-3-21 新增封包时间戳
	DWORD							dwPackTime;							//封包时间戳
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
};

//2015-7-2 重新定义获取RC4 KEY成功结构在列表命令下
struct CMD_GP_Get_Config_Success
{
	WORD							wPubKeyLen;							//公钥长度
	unsigned char					szPubKey[64];						//公钥
	unsigned char					szEncrypt[64];						//加密的RC4 KEY
};

//系统消息
struct MSG_GP_SendMessage
{
	WORD							wChatLength;						//信息长度
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//系统消息
};

//Game 2016-10-23 获取游戏子类型
struct CMD_GP_GetGameSubType
{
	//Game 2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	//Game 2016-3-21 新增封包时间戳
	DWORD							dwPackTime;							//封包时间戳
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
};

//Game 2016-11-14
struct CMD_GP_GAME_MAINTYPE
{
	//Game 2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD                        dwPort;                  //房间端口
	DWORD                        dwIPAddress;             //客户端IP
	//Game 2016-3-21 新增封包时间戳
	DWORD						 dwPackTime;              //封包时间戳
	TCHAR						 szAccounts[LEN_ACCOUNTS];//登录帐号
};

//获取在线
struct CMD_GP_GetOnline
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	WORD							wServerCount;						//房间数目
	WORD							wOnLineServerID[MAX_SERVER];		//房间标识
};

//Game 2016-5-14 新增房间列表结构体
//房间列表
struct CMD_GP_GetServerList
{
	//Game 2016-5-14 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	WORD 							wKindIDCount;						//类型总数
	WORD							wKindIDItem[MAX_KIND];				//类型数目
};

//由服务器发送给客户端，不需要加IP和端口
//类型在线
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//类型数目
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//类型在线
};

//房间在线
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//房间数目
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//房间在线
};

//////////////////////////////////////////////////////////////////////////////////
//服务命令

#define MDM_GP_USER_SERVICE				3								//用户服务

//账号服务
#define SUB_GP_MODIFY_MACHINE			100								//修改机器
#define SUB_GP_MODIFY_LOGON_PASS		101								//修改密码
#define SUB_GP_MODIFY_INSURE_PASS		102								//修改密码
#define SUB_GP_MODIFY_UNDER_WRITE		103								//修改签名
#define SUB_GP_VERIFY_INSURE_PASS		104								//验证密码

//修改头像
#define SUB_GP_USER_FACE_INFO			200								//头像信息
#define SUB_GP_SYSTEM_FACE_INFO			201								//系统头像
#define SUB_GP_CUSTOM_FACE_INFO			202								//自定头像

//个人资料
#define SUB_GP_USER_INDIVIDUAL			301								//个人资料
#define	SUB_GP_QUERY_INDIVIDUAL			302								//查询信息
#define SUB_GP_MODIFY_INDIVIDUAL		303								//修改资料

//Game 2017-6-15 新增手机修改性别 
#define SUB_MB_MODIFY_INDIVIDUAL		304								//修改性别

//银行服务
#define SUB_GP_USER_SAVE_SCORE			400								//存款操作
#define SUB_GP_USER_TAKE_SCORE			401								//取款操作
#define SUB_GP_USER_TRANSFER_SCORE		402								//转账操作
#define SUB_GP_USER_INSURE_INFO			403								//银行资料
#define SUB_GP_QUERY_INSURE_INFO		404								//查询银行
#define SUB_GP_USER_INSURE_SUCCESS		405								//银行成功
#define SUB_GP_USER_INSURE_FAILURE		406								//银行失败
#define SUB_GP_QUERY_USER_INFO_REQUEST	407								//查询用户
#define SUB_GP_QUERY_USER_INFO_RESULT	408								//用户信息
#define SUB_GP_QUERY_TRANSRECORD        409                             //转账记录


//救济金
#define SUB_GP_C_REQ_RELIEF				415								//申请救济金
#define SUB_GP_S_REQ_RELIEF_SUCESS		416								//申请成功
#define SUB_GP_S_REQ_RELIEF_FAIL		417								//申请成功

//提现
#define SUB_GP_C_APPLYMONEY				650								//提现申请
#define SUB_GP_S_APPLYMONEYSUCESS		651								//申请成功等待审核
#define SUB_GP_C_GETMONEYRECORD			652								//获取提现记录
#define SUB_GP_S_SENDMONEYRECORD		653								//下发提现记录

#define SUB_GP_TRANS_RECORD				700								//查询转账记录成功

//操作结果
#define SUB_GP_OPERATE_SUCCESS			900								//操作成功
#define SUB_GP_OPERATE_FAILURE			901								//操作失败


//////////////////////////////////////////////////////////////////////////////////
//提现相关结构
struct MSG_GP_C_APPLYMONEY
{
	DWORD							dwUserID;
	TCHAR							szUserName[50];
	TCHAR							szBankName[50];
	TCHAR							szBankAccount[50];
	TCHAR							szBankAddress[70];
	int								nTrueMoney;
};

struct MSG_GP_C_GetMoneyRecord
{
	DWORD							dwUserID;
};

struct MSG_GP_S_SendMoneyRecord
{
	int								nTotalRecord;
	int								nCurRecordIndex;

	int								nTrueMoney;
	TCHAR							szApplyTime[20];
	TCHAR							szDealTime[20];
	int								nAllowed;
	TCHAR							szNoteMsg[150];
};

//修改密码
struct CMD_GP_ModifyLogonPass
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	DWORD							dwUserID;							//用户 I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码

	DWORD							dwLogonType;						//登陆类型
};

//修改密码
struct CMD_GP_ModifyInsurePass
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	DWORD							dwUserID;							//用户 I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//验证密码
struct CMD_GP_VerifyInsurePass
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP

	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
};

//修改签名
struct CMD_GP_ModifyUnderWrite
{
	//Game 2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
};

//////////////////////////////////////////////////////////////////////////////////

//用户头像
struct CMD_GP_UserFaceInfo
{
	WORD							wFaceID;							//头像标识
	DWORD							dwCustomID;							//自定标识
};

//修改头像
struct CMD_GP_SystemFaceInfo
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改头像
struct CMD_GP_CustomFaceInfo
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//图片信息
};

//////////////////////////////////////////////////////////////////////////////////

//绑定机器
struct CMD_GP_ModifyMachine
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	BYTE							cbBind;								//绑定标志
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//////////////////////////////////////////////////////////////////////////////////

//个人资料
struct CMD_GP_UserIndividual
{
	DWORD							dwUserID;							//用户 I D
};

//查询信息
struct CMD_GP_QueryIndividual
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	DWORD							dwUserID;							//用户 I D
};

//修改资料
struct CMD_GP_ModifyIndividual
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
};

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_UserIndividual

#define DTP_GP_UI_NICKNAME			1									//用户昵称
#define DTP_GP_UI_USER_NOTE			2									//用户说明
#define DTP_GP_UI_UNDER_WRITE		3									//个性签名
#define DTP_GP_UI_QQ				4									//Q Q 号码
#define DTP_GP_UI_EMAIL				5									//电子邮件
#define DTP_GP_UI_SEAT_PHONE		6									//固定电话
#define DTP_GP_UI_MOBILE_PHONE		7									//移动电话
#define DTP_GP_UI_COMPELLATION		8									//真实名字
#define DTP_GP_UI_DWELLING_PLACE	9									//联系地址

//////////////////////////////////////////////////////////////////////////////////

//银行资料
struct CMD_GP_UserInsureInfo
{
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	//2017-02-22 新增奖券字段
	DWORD							lMedal;								//用户奖券
	SCORE							lTransferPrerequisite;				//转账条件
};

//存入金币
struct CMD_GP_UserSaveScore
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//玩家自己的帐号

	DWORD							dwUserID;							//用户 I D
	//2014-8-24 新增玩家帐号和登陆密码
	TCHAR							szLogonPassword[LEN_MD5];			//登陆自己的密码
	//////////////////////////////////////////////////////////////////////////////	
	SCORE							lSaveScore;							//存入金币
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//提取金币
struct CMD_GP_UserTakeScore
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//玩家自己的帐号

	DWORD							dwUserID;							//用户 I D
	//2014-8-24 新增玩家帐号和登陆密码
	TCHAR							szLogonPassword[LEN_MD5];			//登陆自己的密码
	//////////////////////////////////////////////////////////////////////////////	
	SCORE							lTakeScore;							//提取金币
	TCHAR							szPassword[LEN_MD5];				//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	//Game 2016-6-13
	bool							IsBindWeChat;						//是否已绑定微信
};

//转账金币
struct CMD_GP_UserTransferScore
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//玩家自己的帐号

	DWORD							dwUserID;							//用户 I D
	//2014-8-24 新增玩家帐号和登陆密码
	TCHAR							szLogonPassword[LEN_MD5];			//登陆自己的密码
	//////////////////////////////////////////////////////////////////////////////
	BYTE                            cbByNickName;                       //昵称赠送
	SCORE							lTransferScore;						//转账金币
	TCHAR							szPassword[LEN_MD5];				//银行密码
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	//Game 2016-6-13
	bool							IsBindWeChat;						//是否已绑定微信
};

//银行成功
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	TCHAR 							szTransferTime[32];					//转账时间
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//提取结果
struct CMD_GP_UserTakeResult
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
};

//查询银行
struct CMD_GP_QueryInsureInfo
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//玩家自己的帐号

	DWORD							dwUserID;							//用户 I D
	//2014-11-9 新增玩家帐号和登陆密码
	TCHAR							szLogonPassword[LEN_MD5];			//登陆自己的密码
};

//查询用户
struct CMD_GP_QueryUserInfoRequest
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//玩家自己的帐号

	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
	//2014-11-9 新增玩家帐号和登陆密码
	TCHAR							szLogonPassword[LEN_MD5];			//登陆自己的密码
};

//用户信息
struct CMD_GP_UserTransferUserInfo
{
	DWORD							dwTargetGameID;						//目标用户
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

//Game 2016-6-17 查询银行转账记录
struct CMD_GP_QueryInsureRecordInfo
{
	//Game 2016-6-17 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//玩家自己的帐号

	DWORD							dwUserID;							//用户 I D
	//Game 2016-6-17 新增两个转账记录查询变量
	TCHAR							szDate[255];						//选择时间
	DWORD							dwTargetGameID;						//选择收款人
	//Game 2016-6-17 新增玩家帐号和登陆密码
	TCHAR							szLogonPassword[LEN_MD5];			//登陆自己的密码
	//////////////////////////////////////////////////////////////////////////////
};

//Game 2016-6-17 修改转账记录
//转账记录
//由服务器发送给客户端，不需要加IP和端口
struct CMD_GP_RealTransRecord
{
	DWORD							dwGameID;
	DWORD                           dwSourceGameID;
	DWORD                           dwTargetGameID;
	TCHAR							szTargetAccounts[LEN_ACCOUNTS];
	LONGLONG						lSwapScore;
	//LONGLONG						lRevenue;
	TCHAR							szTime[LEN_ACCOUNTS];
	bool							bOver;
};

//////////////////////////////////////////////////////////////////////////////////

//请求领取救济金
struct CMD_GP_RequestRelief
{
	DWORD							dwUserID;							//用户 I D
};

//////////////////////////////////////////////////////////////////////////////////

//操作失败
struct CMD_GP_OperateFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//操作成功
struct CMD_GP_OperateSuccess
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//成功消息
};

//////////////////////////////////////////////////////////////////////////////////
//远程服务

#define MDM_GP_REMOTE_SERVICE		4									//远程服务

//查找服务
#define SUB_GP_C_SEARCH_DATABASE	100									//数据查找
#define SUB_GP_C_SEARCH_CORRESPOND	101									//协调查找

//查找服务
#define SUB_GP_S_SEARCH_DATABASE	200									//数据查找
#define SUB_GP_S_SEARCH_CORRESPOND	201									//协调查找

//////////////////////////////////////////////////////////////////////////////////

//协调查找
struct CMD_GP_C_SearchCorrespond
{
	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//玩家自己的帐号

	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//协调查找
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//用户数目
	tagUserRemoteInfo				UserRemoteInfo[16];					//用户信息
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//2016-3-28
//获取KEY命令
#define MDM_GP_GET_KEY						5							//客户端请求KEY主指令
#define SUB_GP_GET_KEY						1							//客户端请求KEY子指令
#define SUB_GP_GETKEY_SUCCESS				2							//服务端发送生成的RSA加密的RC4 KEY给客户端

#define SUB_GP_LOGIN_GETKEY					3							//客户端登陆获取RC4 KEY命令
#define SUB_GP_LOGIN_GETKEY_SUCCESS         4							//服务端发送RC4 KEY给客户端命令

#define SUB_GP_REGISTER_GETKEY				5							//客户端注册获取RC4 KEY命令
#define SUB_GP_REGISTER_GETKEY_SUCCESS      6							//服务端发送RC4 KEY给客户端命令

#define SUB_GP_GETKEY_QUERYSCORE			7							//客户端银行刷新获取RC4 KEY命令
#define SUB_GP_GETKEY_QUERYSCORE_SUCCESS    8							//服务端发送RC4 KEY给客户端命令

#define SUB_GP_GETKEY_SAVESCORE				9							//客户端银行存款获取RC4 KEY命令
#define SUB_GP_GETKEY_SAVESCORE_SUCCESS		10							//服务端发送RC4 KEY给客户端命令

#define SUB_GP_GETKEY_TAKESCORE				11							//客户端银行取款获取RC4 KEY命令
#define SUB_GP_GETKEY_TAKESCORE_SUCCESS		12							//服务端发送RC4 KEY给客户端命令

#define SUB_GP_GETKEY_TRANSFERSCORE			13							//客户端银行转账获取RC4 KEY命令
#define SUB_GP_GETKEY_TRANSFERSCORE_SUCCESS	14							//服务端发送RC4 KEY给客户端命令

#define SUB_GP_GETKEY_CHANGFACE				15							//客户端修改头像获取RC4 KEY命令
#define SUB_GP_GETKEY_CHANGFACE_SUCCESS		16							//服务端发送RC4 KEY给客户端命令

//Game 2016-5-11
#define SUB_GP_GETKEY_QUICKLOGON        	17							//客户端快速登录请求获取RC4 KEY命令
#define SUB_GP_GETKEY_QUICKLOGON_SUCCESS    18							//服务端发送RC4 KEY给客户端命令

//Game 2016-5-31
#define SUB_GP_GETKEY_WECHATSHARE			19							//客户端快速登录请求获取RC4 KEY命令
#define SUB_GP_GETKEY_WECHATSHARE_SUCCESS	20							//服务端发送RC4 KEY给客户端命令

//Game 2017-6-14
#define SUB_GP_GETKEY_MODIFYINDIVIDUAL		21							//客户端修改资料请求获取RC4 KEY命令							
#define SUB_GP_GETKEY_MODIFYINDIVIDUAL_SUCCESS	22						//服务端发送RC4 KEY给客户端命令

//客户端向服务端RC4_KEY
struct CMD_GP_Get_Key
{
	//Game 2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	//Game 2016-3-21 新增封包时间戳
	DWORD							dwPackTime;							//封包时间戳
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
};

//由服务器发送给客户端，不需要加IP和端口
//服务端发客户端发送公钥KEY
struct CMD_GP_GetKeySuccess
{
	WORD							wPubKeyLen;							//公钥长度
	unsigned char					szPubKey[64];						//公钥
	unsigned char					szEncrypt[64];						//加密的RC4 KEY
};
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//登录命令
#define MDM_MB_LOGON				100									//广场登录

//登录模式
#define SUB_MB_LOGON_GAMEID			1									//I D 登录
#define SUB_MB_LOGON_ACCOUNTS		2									//帐号登录
#define SUB_MB_REGISTER_ACCOUNTS	3									//注册帐号
//Game 2016-5-11 微信快速登陆
#define SUB_MB_QUICKLOGON_REQUEST	4									//快速登录请求(包含微信和一键登录用同一个请求命令)

//登录结果
#define SUB_MB_LOGON_SUCCESS		100									//登录成功
#define SUB_MB_LOGON_FAILURE		101									//登录失败

//Game 2016-5-11 微信快速登陆成功
#define SUB_MB_QUICKLOGON_SUCCESS	102									//快速登录成功
#define SUB_MB_QUICKLOGON_FAILURE	103									//快速登录成功

//升级提示
#define SUB_MB_UPDATE_NOTIFY		200									//升级提示

//////////////////////////////////////////////////////////////////////////////////

//Game 2016-5-11 快速登录请求
struct CMD_MB_QuickLogon_Request
{
	DWORD							dwLogonType;						//0 PC登陆 1 PC微信快速登陆 2微信注册 3苹果一键登录 4手机普通注册、登陆 
	TCHAR							szUniqueIdentify[50];				//唯一标识(广告标识或者微信unioinid)

	//微信登陆才有用户昵称信息
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE							cbDeviceType;						//设备类型

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识

	//2016-9-7 增加推广字段
	TCHAR							szPromotionID[255];					//推广ID号

	//Game 2016-9-2 新增手机注册登陆三个类型
	TCHAR							szSystemVersion[30];				//手机版本
	BYTE							isJailBroken;						//是否越狱或ROOT 0-未越狱或未ROOT 1 - 已越狱或已ROOT
	TCHAR							PhoneModel[30];						//手机类型
	//Game 2016-9-18				游戏标识用来标识注册的游戏来源
	TCHAR							szGameflagID[30];					//注册来源
};

//Game 2016-5-11 快速登录请求成功
struct CMD_MB_QuickLogon_Success
{
	TCHAR							szAccounts[LEN_ACCOUNTS];			//玩家的帐号
	TCHAR							szPassword[LEN_MD5];				//登录的密码

	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	DWORD							dwGroupID;							//社团标识
	DWORD							dwCustomID;							//自定标识
	DWORD							dwUserMedal;						//用户奖牌
	//用户信息
	BYTE							cbMoorMachine;						//锁定机器
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//用户成绩
	SCORE							lUserScore;							//用户金币
	SCORE							lBankScore;							//银行金币


	//2017-02-22 新增奖券字段
	DWORD							lMedal;								//用户奖券
	TCHAR							szMessage[300];
	bool                            bWeChatBind;                        //是否微信绑定

	//Game 2017-6-12 新增三个字段
	DWORD							dwKindID;							//游戏KindID
	DWORD							dwServerID;							//房间ID
	TCHAR							IsBindPhone[12];					//手机绑定标志

	//2016-6-22 新增登陆类型字段
	DWORD							dwLogonType;						//登陆类型
};

//Game 2016-5-11 快速登录请求失败
struct CMD_MB_QuickLogon_Failure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//I D 登录
struct CMD_MB_LogonGameID
{
	DWORD							dwLogonType;						//0 PC登陆 1 PC微信快速登陆 2微信注册 3苹果一键登录 4手机普通注册、登陆 

	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//玩家自己的帐号

	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//登录信息
	DWORD							dwGameID;							//游戏 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//Game 2016-9-2 新增手机注册登陆三个类型
	TCHAR							szSystemVersion[30];				//手机版本
	BYTE							isJailBroken;						//是否越狱或ROOT 0-未越狱或未ROOT 1 - 已越狱或已ROOT
	TCHAR							PhoneModel[30];						//手机类型
};

//帐号登录
struct CMD_MB_LogonAccounts
{
	DWORD							dwLogonType;						//0 PC登陆 1 PC微信快速登陆 2微信注册 3苹果一键登录 4手机普通注册、登陆 

	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//玩家自己的帐号

	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//Game 2016-9-2 新增手机注册登陆三个类型
	TCHAR							szSystemVersion[30];				//手机版本
	BYTE							isJailBroken;						//是否越狱或ROOT 0-未越狱或未ROOT 1 - 已越狱或已ROOT
	TCHAR							PhoneModel[30];						//手机类型
};

//注册帐号
struct CMD_MB_RegisterAccounts
{
	DWORD							dwLogonType;						//0 PC登陆 1 PC微信快速登陆 2微信注册 3苹果一键登录 4手机普通注册、登陆 

	//2015-1-21 新增游戏标志、游戏端口、游戏房间IP
	DWORD   	                 	dwPort;								//房间端口
	DWORD   	                 	dwIPAddress;						//客户端IP
	TCHAR							szAccounts[LEN_ACCOUNTS];			//玩家自己的帐号

	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	TCHAR							szCard[18];							//身份证

	//2016-9-7 增加推广字段
	TCHAR							szPromotionID[255];					//推广ID号

	//Game 2016-9-2 新增手机注册登陆三个类型
	TCHAR							szSystemVersion[30];				//手机版本
	BYTE							isJailBroken;						//是否越狱或ROOT 0-未越狱或未ROOT 1 - 已越狱或已ROOT
	TCHAR							PhoneModel[30];						//手机类型
	//Game 2016-9-18				游戏标识用来标识注册的游戏来源
	TCHAR							szGameflagID[30];					//注册来源
};

//登录成功
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//用户成绩
	SCORE							lUserScore;							//用户金币
	SCORE							lBankScore;
	//2017-02-22 新增奖券字段
	DWORD							lMedal;								//用户奖券

	TCHAR							szMessage[300];
	bool                            bWeChatBind;                        //是否微信绑定
	//2016-6-22 新增登陆类型字段
	DWORD							dwLogonType;						//登陆类型

	//Game 2016-9-3 新增手机登陆成功返回的其他参数
	DWORD							dwCustomID;							//自定索引
	DWORD							dwUserMedal;						//用户奖牌
	BYTE							cbMoorMachine;						//锁定机器
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
	//会员资料
	BYTE							cbMemberOrder;						//会员等级
	SYSTEMTIME						MemberOverDate;						//到期时间

	//Game 2017-6-12 新增三个字段
	DWORD							dwKindID;							//游戏KindID
	DWORD							dwServerID;							//房间ID
	TCHAR							IsBindPhone[12];					//手机绑定标志
};

//登录失败
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//升级提示
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//强行升级
	BYTE							cbAdviceUpdate;						//建议升级
	DWORD							dwCurrentVersion;					//当前版本
};

//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_MB_SERVER_LIST			101									//列表信息

//列表信息
#define SUB_MB_LIST_KIND			100									//种类列表
#define SUB_MB_LIST_SERVER			101									//房间列表
#define SUB_MB_LIST_FINISH			200									//列表完成

//////////////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* 账单查询                                                                     */
/************************************************************************/
struct MSG_GR_QUERY_GameBillInfo_Data
{
	DWORD								dwRecordCount;
	DWORD								dwCurRecord;
	tagGetBillInfo						BillData;
};

//////////////////////////////////////////////////////////////////////////////////
//Game 2016-6-13 微信验证码-查询微信信息
#define MDM_GP_WECHAT_CODE			200									//微信验证

#define SUB_GP_WECHAT_CODE				1								//取款接收验证码
#define SUB_GP_WECHAT_CODE_FAILURE		2								//验证码失败
#define SUB_GP_WECHAT_CODE1				3								//转账接收验证码
#define SUB_GP_WECHAT_CODE_BACK			100								//取款发送验证码及取款
#define SUB_GP_WECHAT_CODE_BACK1		101								//转账发送验证及转账

struct CMD_GP_WeChatCode
{
	CMD_GP_UserTakeScore	    takeScore;
	DWORD								dwCode;
};
struct CMD_GP_WeChatCodeFailure
{
	TCHAR							szDescribeString[128];				//描述消息
};
struct CMD_GP_WeChatCode1
{
	CMD_GP_UserTransferScore	    transferScore;
	DWORD								dwCode;
};
//////////////////////////////////////////////////////////////////////////////////
#pragma pack()

#endif