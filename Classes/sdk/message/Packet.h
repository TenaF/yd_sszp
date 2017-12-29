#ifndef PACKET_HEAD_FILE
#define PACKET_HEAD_FILE

#include "Define.h"
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//端口定义

#define MAX_CONTENT					512									//并发容量
#define PORT_AUTO_SELECT			INVALID_WORD						//自动端口

//#define PORT_LOGON					8300								//登陆端口
//#define PORT_CENTER					8310								//协调端口
//#define PORT_MANAGER				8320								//管理端口

//////////////////////////////////////////////////////////////////////////////////
//网络定义

//数据类型
#define DK_MAPPED					0x01								//映射类型
#define DK_ENCRYPT					0x02								//加密类型
#define DK_COMPRESS					0x04								//压缩类型

//长度定义
#define SOCKET_TCP_BUFFER			16384								//网络缓冲
#define SOCKET_TCP_PACKET			(SOCKET_TCP_BUFFER-sizeof(TCP_Head))//网络缓冲

//长度定义
#define SOCKET_UDP_BUFFER			16384								//网络缓冲
#define SOCKET_UDP_PACKET			(SOCKET_UDP_BUFFER-sizeof(UDP_Head))//网络缓冲

//////////////////////////////////////////////////////////////////////////////////
//结构定义

//网络内核11
struct TCP_Info
{
	BYTE							cbDataKind;							//数据类型
	BYTE							cbCheckCode;						//效验字段
	WORD							wPacketSize;						//数据大小
};

//网络命令11
struct TCP_Command
{
	WORD							wMainCmdID;							//主命令码
	WORD							wSubCmdID;							//子命令码
};

//网络包头11
struct TCP_Head
{
	TCP_Info						TCPInfo;							//基础结构
	TCP_Command						CommandInfo;						//命令信息
};

//网络检查
struct TCP_Validate
{
	TCHAR							szValidateKey[64];					//验证字符
};

//网络缓冲
struct TCP_Buffer
{
	TCP_Head						Head;								//数据包头
	BYTE							cbBuffer[SOCKET_TCP_PACKET];		//数据缓冲
};

//////////////////////////////////////////////////////////////////////////////////

//网络内核
struct UDP_Info
{
	BYTE							cbDataKind;							//数据类型
	BYTE							cbCheckCode;						//效验字段
	WORD							wPacketSize;						//数据大小
	WORD							wPacketIndex;						//数据序列
	WORD							wConnectIndex;						//连接索引
};


//网络命令
struct UDP_Command
{
	WORD							wMainCmdID;							//主命令码
	WORD							wSubCmdID;							//子命令码
};


//网络包头
struct UDP_Head
{
	UDP_Info						UDPInfo;							//基础结构
	UDP_Command						CommandInfo;						//命令信息
};

//网络缓冲
struct UDP_Buffer
{
	UDP_Head						Head;								//数据包头
	BYTE							cbBuffer[SOCKET_UDP_PACKET];		//数据缓冲
};

//////////////////////////////////////////////////////////////////////////////////

//内核命令
#define MDM_KN_COMMAND				0									//内核命令
#define SUB_KN_DETECT_SOCKET		1									//检测命令
#define SUB_KN_VALIDATE_SOCKET		2									//验证命令

//////////////////////////////////////////////////////////////////////////////////
//传输数据

#define IPC_VER						1									//版本标识
#define IPC_PACKET					(10240-sizeof(IPC_Head))			//最大包长
#define IPC_BUFFER					(sizeof(IPC_Head)+IPC_PACKET)		//缓冲长度

//数据包头
struct IPC_Head
{
	WORD							wVersion;							//版本标识
	WORD							wPacketSize;						//数据大小
	WORD							wMainCmdID;							//主命令码
	WORD							wSubCmdID;							//子命令码
};

//IPC 包结构
struct IPC_Buffer
{
	IPC_Head						Head;								//数据包头
	BYTE							cbBuffer[IPC_PACKET];				//数据缓冲
};



//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif