//
//  sendMsg.cpp
//  hello2dx
//
//

#include "SendMsg.h"
#include "base/ccUTF8.h"
#include "sdk/util/QFileUtil.h"
#include "hall/data/DataManager.h"
//#include "game/dntg/def/timer_manager.h"
#include "sdk/message/Encrypt.h"
#define TAG "SendMsg"


#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && CODE_LLVM_ENCRYPT
#pragma obfuscate on
#endif



// 发送心跳包
void SendMsg::sendHeartbeat(int flag)
{
	// 压缩过的数据,所以不需要Common_Compilation再压缩数据
    //服务端每隔10秒检测客户端是否有发送心跳包
    TCP_Buffer	heartBuffer;
    memset(&heartBuffer,0,sizeof(TCP_Buffer));
    heartBuffer.Head.CommandInfo.wMainCmdID	= MDM_KN_COMMAND;
    heartBuffer.Head.CommandInfo.wSubCmdID	= SUB_KN_DETECT_SOCKET;
    //this->socketSend((char *)&heartBuffer, sizeof(heartBuffer.Head));
    
	//char stHeartbeat[] = { 0x01, static_cast<char>(0xFF), 0x08, 0x00, 0x70, 0x70, 0x2F, 0x70 };
	NetworkManager::getInstance()->sendData(flag, MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, &heartBuffer, sizeof(heartBuffer.Head));//内核命令->验证命令
	//NYInfo(TAG,"sendHeartbeat");

}
void SendMsg::sendVerifyReq(int flag)
{
	TCP_Validate validate;
	memset(&validate, 0, sizeof(TCP_Validate));

	//网络验证数据
	Common_Compilation(validate.szValidateKey);
	
    NetworkManager::getInstance()->sendVerifyData(flag, MDM_KN_COMMAND, SUB_KN_VALIDATE_SOCKET, &validate, sizeof(TCP_Validate));//内核命令->验证命令

}



void SendMsg::sendQuickLoginReq()
{
	CMD_MB_QuickLogon_Request Databuffer;
	memset(&Databuffer, 0, sizeof(CMD_MB_QuickLogon_Request));
	
	//登陆类型
	Databuffer.dwLogonType = UserInfo::instance()->getLoginType();
	//int x = (int)Databuffer.dwLogonType;
	//NYInfo(TAG,"dwLogonType------------:%d", x);
	//称昵
	Util::createTCHARWithString(UserInfo::instance()->getNickName(), Databuffer.szNickName);//帐号
	Databuffer.wModuleID = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Databuffer.cbDeviceType = DEVICE_TYPE_ANDROID;
	Databuffer.dwPlazaVersion = VERSION_MOBILE_ANDROID;
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	Databuffer.cbDeviceType = DEVICE_TYPE_IPHONE;
	Databuffer.dwPlazaVersion = VERSION_MOBILE_IOS;
#endif
	if (Databuffer.dwLogonType == LOGIN_TYPE_MB_APPLE_WECHAT || Databuffer.dwLogonType == LOGIN_TYPE_MB_ANDROID_WECHAT)//如果是微信登陆
	{
		//Util::createTCHARWithString(WeChatLogin::getWechatLoginUnionid(), Databuffer.szUniqueIdentify);
	}
	else
	{
		Util::createTCHARWithString(Util::getIDFA(), Databuffer.szUniqueIdentify);
	}
	Util::createTCHARWithString(Util::strToMd5Str(Util::getIDFA()).c_str(), Databuffer.szMachineID);
	Util::createTCHARWithString(Util::getSystemVersion(), Databuffer.szSystemVersion);
	Databuffer.isJailBroken = Util::isJailBroken();
	Util::createTCHARWithString(Util::getPhoneModel(), Databuffer.PhoneModel);

	Util::createTCHARWithString(UserInfo::instance()->getPromotionID(), Databuffer.szPromotionID);

	//游戏标识用来标识注册的游戏来源
	//auto temp_pDict = __Dictionary::createWithContentsOfFile("popularizeInfo.xml");//读取XML
	Util::createTCHARWithString(UserInfo::instance()->getGameflagID().c_str(), Databuffer.szGameflagID);

	// 获取源封包指针(用户登陆)
	void *pSourcePacket = &Databuffer;
	// 加密后数据
	BYTE EncryptPacket[8192] = { 0 };
	// RC4变异加密
	Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_MB_QuickLogon_Request));
	// 拷贝加密内容
	memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_MB_QuickLogon_Request));

	NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL, MDM_MB_LOGON, SUB_MB_QUICKLOGON_REQUEST, &Databuffer, sizeof(CMD_MB_QuickLogon_Request));//大厅登录 帐号登录
}

//账号登录命令
void SendMsg::sendLoginReq(const char * strUserName, const char * strPassWord, const char * strPhoneNum )
{
	MainController::getInstance()->setGameState(false);
	CMD_MB_LogonAccounts Databuffer;
	memset(&Databuffer,0,sizeof(CMD_MB_LogonAccounts));
	
    Databuffer.wModuleID = 0;//模块标识
	Databuffer.dwIPAddress = 465484375;
	Databuffer.dwLogonType = UserInfo::instance()->getLoginType();
	int x = (int)Databuffer.dwLogonType;
	NYInfo(TAG,"dwLogonType------------:%d",x);
	Databuffer.dwPort = UserInfo::instance()->getServerPort();


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Databuffer.cbDeviceType = DEVICE_TYPE_ANDROID;
	Databuffer.dwPlazaVersion = VERSION_MOBILE_ANDROID;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	Databuffer.cbDeviceType = DEVICE_TYPE_IPHONE;
	Databuffer.dwPlazaVersion = VERSION_MOBILE_IOS;
#endif

	Util::createTCHARWithString(Util::getSystemVersion(), Databuffer.szSystemVersion);
	Databuffer.isJailBroken = Util::isJailBroken();
	Util::createTCHARWithString(Util::getPhoneModel(), Databuffer.PhoneModel);

	memset(&Databuffer.szAccounts,0,sizeof(TCHAR)*LEN_ACCOUNTS);
	memset(&Databuffer.szPassword,0,sizeof(TCHAR)*LEN_PASSWORD);
	memset(&Databuffer.szMobilePhone,0,sizeof(TCHAR)*LEN_MOBILE_PHONE);
	memset(&Databuffer.szMachineID,0,sizeof(TCHAR)*LEN_MD5);
	
	//帐号
    Util::createTCHARWithString(strUserName, Databuffer.szAccounts);//帐号
    Util::createTCHARWithString(strPassWord, Databuffer.szPassword);//密码
    Util::createTCHARWithString(strPhoneNum, Databuffer.szMobilePhone);//电话号码
	Util::createTCHARWithString(Util::strToMd5Str(Util::getIDFA()).c_str(), Databuffer.szMachineID);//唯一标识
    

	//数据加密RSA <<

	// 获取源封包指针(用户登陆)
	void *pSourcePacket = &Databuffer;
	// 加密后数据
	BYTE EncryptPacket[8192] = { 0 };
	// RC4变异加密
	Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_MB_LogonAccounts));
	// 拷贝加密内容
	memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_MB_LogonAccounts));

	//数据加密RSA >>

    NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL, MDM_MB_LOGON, SUB_MB_LOGON_ACCOUNTS, &Databuffer, sizeof(CMD_MB_LogonAccounts));//大厅登录 帐号登录


}


//注册
void SendMsg::registerAccounts(const char * accounts , const char * password ,const char * nickname , int sex ,int face,const char *  bankpassword, const char *  card )
{

	CMD_MB_RegisterAccounts msg;
	memset(&msg,0,sizeof(CMD_MB_RegisterAccounts));

	msg.dwIPAddress = 465484375;
	DWORD tempType = UserInfo::instance()->getLoginType();
	msg.dwLogonType = tempType;
	msg.dwPort = UserInfo::instance()->getServerPort();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	msg.cbDeviceType = DEVICE_TYPE_ANDROID;
	msg.dwPlazaVersion = VERSION_MOBILE_ANDROID;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	msg.cbDeviceType = DEVICE_TYPE_IPHONE;
	msg.dwPlazaVersion = VERSION_MOBILE_IOS;
#endif

    msg.wModuleID = 0;
	msg.cbGender = sex;
	msg.wFaceID = face;

	memset(&msg.szMachineID, 0, sizeof(TCHAR)*LEN_ACCOUNTS);

	memset(&msg.szLogonPass, 0, sizeof(TCHAR)*LEN_PASSWORD);
	Util::createTCHARWithString(password, msg.szLogonPass);

	memset(&msg.szInsurePass, 0,sizeof(TCHAR)*LEN_PASSWORD);
	Util::createTCHARWithString(bankpassword, msg.szInsurePass);

	memset(&msg.szAccounts, 0, sizeof(TCHAR)*LEN_ACCOUNTS);
	Util::createTCHARWithString(accounts, msg.szAccounts);

	memset(&msg.szNickName, 0, sizeof(TCHAR)*LEN_NICKNAME);
	Util::createTCHARWithString(nickname, msg.szNickName);

	//string phoneNumber = Util::getPhoneNumber();
/*	Util::createTCHARWithString(phoneNumber, msg.szMobilePhone);*/

	memset(&msg.szCard, 0, sizeof(TCHAR)*18);
	Util::createTCHARWithString(card, msg.szCard);
	//游戏标识用来标识注册的游戏来源
	//auto temp_pDict = __Dictionary::createWithContentsOfFile("popularizeInfo.xml");//读取XML
	Util::createTCHARWithString(UserInfo::instance()->getGameflagID().c_str(), msg.szGameflagID);

	Util::createTCHARWithString(Util::strToMd5Str(Util::getIDFA()).c_str(), msg.szMachineID);
	Util::createTCHARWithString(Util::getSystemVersion(), msg.szSystemVersion);
	msg.isJailBroken = Util::isJailBroken();
	Util::createTCHARWithString(Util::getPhoneModel(), msg.PhoneModel);

	Util::createTCHARWithString(UserInfo::instance()->getPromotionID(), msg.szPromotionID);
	//数据加密RSA <<

	// 获取源封包指针(用户登陆)
	void *pSourcePacket = &msg;
	// 加密后数据
	BYTE EncryptPacket[8192] = { 0 };
	// RC4变异加密
	Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_MB_RegisterAccounts));
	// 拷贝加密内容
	memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_MB_RegisterAccounts));

	//数据加密RSA >>

	NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL , MDM_MB_LOGON , SUB_MB_REGISTER_ACCOUNTS,&msg,sizeof(CMD_MB_RegisterAccounts));

}

//发送修改头像
void SendMsg::sendUpdateHeadInfo(int faceID)
{
	CMD_GP_SystemFaceInfo SystemFaceInfo;
	memset(&SystemFaceInfo,0,sizeof(SystemFaceInfo));

	SystemFaceInfo.wFaceID=faceID;
	SystemFaceInfo.dwUserID=UserInfo::instance()->getUserId();
	DWORD logintype = UserInfo::instance()->getLoginType();
	
	if (logintype == LOGIN_TYPE_WINDOWS_ACCOUNT || logintype == LOGIN_TYPE_MB_APPLE_ACCOUNT || logintype == LOGIN_TYPE_MB_ANDROID_ACCOUNT)
	{
		Util::createTCHARWithString(Util::strToMd5Str(UserInfo::instance()->getPassword()), SystemFaceInfo.szPassword);
	}
	else
	{
		Util::createTCHARWithString(UserInfo::instance()->getMD5Password(), SystemFaceInfo.szPassword);
	}
	Util::createTCHARWithString(UserInfo::instance()->getAccount(), SystemFaceInfo.szAccounts);
	Util::createTCHARWithString(Util::strToMd5Str(Util::getIDFA()).c_str(), SystemFaceInfo.szMachineID);
//	SystemFaceInfo.crc32 =  PackCrc32((char*)(&SystemFaceInfo)+sizeof(int),sizeof(SystemFaceInfo)-sizeof(int));
	//数据加密RSA <<

	// 获取源封包指针(用户登陆)
	void *pSourcePacket = &SystemFaceInfo;
	// 加密后数据
	BYTE EncryptPacket[8192] = { 0 };
	// RC4变异加密
	Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_GP_SystemFaceInfo));
	// 拷贝加密内容
	memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_GP_SystemFaceInfo));

	//数据加密RSA >>

	NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL ,MDM_GP_USER_SERVICE ,SUB_GP_SYSTEM_FACE_INFO,&SystemFaceInfo,sizeof(CMD_GP_SystemFaceInfo));

}

// 发送请求分享得金币 用RC4加密
// void SendMsg::sendShareWechat()
// {
// 	CMD_GP_SHAREWECHAT sharewechat;
// 	memset(&sharewechat, 0, sizeof(sharewechat));
// 
// 	sharewechat.dwGameID = UserInfo::instance()->getGameId();
// 	sharewechat.dwIPAddress = 465484375;
// 	sharewechat.dwPort = UserInfo::instance()->gamePort;
// 	Util::createTCHARWithString(UserInfo::instance()->getAccount(), sharewechat.szAccounts);
// 
// 	//数据加密RSA <<
// 
// 	// 获取源封包指针(用户登陆)
// 	void *pSourcePacket = &sharewechat;
// 	// 加密后数据
// 	BYTE EncryptPacket[8192] = { 0 };
// 	// RC4变异加密
// 	Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_GP_SHAREWECHAT));
// 	// 拷贝加密内容
// 	memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_GP_SHAREWECHAT));
// 
// 	//数据加密RSA >>
// 
// 	NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL, MDM_GP_USER_SERVICE, SUB_GP_C_SHAREWECHAT, &sharewechat, sizeof(CMD_GP_SHAREWECHAT));
// 
// }


// 发送请求验证码 用RC4加密
// void SendMsg::sendVerifactionCode()
// {
// 
// 	CMD_GP_WeChatCode SystemFaceInfo;
// 	memset(&SystemFaceInfo, 0, sizeof(SystemFaceInfo));
// 	CMD_GP_UserTakeScore	    takeScore;
// 	SystemFaceInfo.takeScore = takeScore;
// 	SystemFaceInfo.dwUserID = UserInfo::instance()->getUserId();
// 	SystemFaceInfo.dwIPAddress = 465484375;
// 	SystemFaceInfo.dwPort = UserInfo::instance()->gamePort;
// 	
// 	//数据加密RSA <<
// 
// 	// 获取源封包指针(用户登陆)
// 	void *pSourcePacket = &SystemFaceInfo;
// 	// 加密后数据
// 	BYTE EncryptPacket[8192] = { 0 };
// 	// RC4变异加密
// 	Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_GP_WeChatCode));
// 	// 拷贝加密内容
// 	memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_GP_WeChatCode));
// 
// 	//数据加密RSA >>
// 
// 	NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL, MDM_GP_USER_SERVICE, SUB_GP_C_WECHATCODE, &SystemFaceInfo, sizeof(CMD_GP_WeChatCode));
// 
// }


//修改密码
void SendMsg::sendUpdatePassword(const char * oldPass, const char * newPass)
{
	CMD_GP_ModifyLogonPass info ;
	memset(&info , 0,sizeof(CMD_GP_ModifyLogonPass));

	info.dwUserID = UserInfo::instance()->getUserId();
	Util::createTCHARWithString(newPass,info.szDesPassword);
	Util::createTCHARWithString(oldPass, info.szScrPassword);

	NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL , MDM_GP_USER_SERVICE,SUB_GP_MODIFY_INSURE_PASS,&info,sizeof(CMD_GP_ModifyLogonPass));


}


//查询银行
void SendMsg::getUserScore()
{
	/*
	CMD_GP_QueryGameInsureInfo info ;
	memset(&info , 0,sizeof(CMD_GP_QueryGameInsureInfo));

	info.dwUserID = UserInfo::instance()->getUserId();

	NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL , MDM_GP_USER_SERVICE,SUB_GP_QUERY_GAME_INSURE_INFO,&info,sizeof(CMD_GP_QueryGameInsureInfo));

	CCLog(U2F("%s 查询用户的金币  ").c_str(),TAG);
	*/

}


//发送转账操作请求
void SendMsg::sendTransferMoney(const char * nick, SCORE amount, const char *  password, bool isWechatBind /*= false*/)
{
	CMD_GP_UserTransferScore dataBuf;
	memset(&dataBuf, 0, sizeof(dataBuf));

	dataBuf.dwUserID = UserInfo::instance()->getUserId();
	dataBuf.cbByNickName = 0;
	dataBuf.lTransferScore = amount;
	string strNick = nick;
	Util::createTCHARWithString(strNick, dataBuf.szNickName);
	Util::createTCHARWithString(Util::strToMd5Str(Util::getIDFA()).c_str(), dataBuf.szMachineID);

	memset(&dataBuf.szPassword, 0, sizeof(TCHAR)*LEN_PASSWORD);
	string bankPw = password; 
	Util::createTCHARWithString(Util::strToMd5Str(bankPw), dataBuf.szPassword);
	DWORD logintype = UserInfo::instance()->getLoginType();

	if (logintype == LOGIN_TYPE_WINDOWS_ACCOUNT || logintype == LOGIN_TYPE_MB_APPLE_ACCOUNT || logintype == LOGIN_TYPE_MB_ANDROID_ACCOUNT)
	{
		Util::createTCHARWithString(Util::strToMd5Str(UserInfo::instance()->getPassword()), dataBuf.szLogonPassword);
	}
	else
	{
		Util::createTCHARWithString(UserInfo::instance()->getMD5Password(), dataBuf.szLogonPassword);
	}
	Util::createTCHARWithString(UserInfo::instance()->getAccount(), dataBuf.szAccounts);


	//是否绑定微信帐号
 	dataBuf.IsBindWeChat = UserInfo::instance()->getbWeChatBind();

	if (isWechatBind)
	{
		CMD_GP_WeChatCode1 code;
		ZeroMemory(&code, sizeof(code));
		memcpy(&code.transferScore, &dataBuf, sizeof(code.transferScore));
		code.dwCode = (DWORD)std::atoi(UserInfo::instance()->verifactionCode.c_str());//验证码
		//数据加密RSA <<

		// 获取源封包指针(用户登陆)
		void *pSourcePacket = &code;
		// 加密后数据
		BYTE EncryptPacket[8192] = { 0 };
		// RC4变异加密
		Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_GP_WeChatCode1));
		// 拷贝加密内容
		memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_GP_WeChatCode1));

		//数据加密RSA >>

		NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL, MDM_GP_WECHAT_CODE, SUB_GP_WECHAT_CODE_BACK1, &code, sizeof(code));
	}
	else
	{
		//数据加密RSA <<

		// 获取源封包指针(用户登陆)
		void *pSourcePacket = &dataBuf;
		// 加密后数据
		BYTE EncryptPacket[8192] = { 0 };
		// RC4变异加密
		Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_GP_UserTransferScore));
		// 拷贝加密内容
		memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_GP_UserTransferScore));

		//数据加密RSA >>

		NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL, MDM_GP_USER_SERVICE, SUB_GP_USER_TRANSFER_SCORE, &dataBuf, sizeof(dataBuf));
	}

}

//发送存款操作请求
void SendMsg::sendSaveMoney(SCORE amount)
{
	
	CMD_GP_UserSaveScore dataBuf;
	memset(&dataBuf, 0, sizeof(dataBuf));

//*((int*)cbBuffer)= n;
	dataBuf.dwUserID = UserInfo::instance()->getUserId();
	dataBuf.lSaveScore = amount;
	Util::createTCHARWithString(Util::strToMd5Str(Util::getIDFA()).c_str(), dataBuf.szMachineID);
	DWORD logintype = UserInfo::instance()->getLoginType();
	if (logintype == LOGIN_TYPE_WINDOWS_ACCOUNT || logintype == LOGIN_TYPE_MB_APPLE_ACCOUNT || logintype == LOGIN_TYPE_MB_ANDROID_ACCOUNT)
	{
		Util::createTCHARWithString(Util::strToMd5Str(UserInfo::instance()->getPassword()), dataBuf.szLogonPassword);

	}
	else
	{
		Util::createTCHARWithString(UserInfo::instance()->getMD5Password(), dataBuf.szLogonPassword);
	}
	Util::createTCHARWithString(UserInfo::instance()->getAccount(),dataBuf.szAccounts);

	//数据加密RSA <<

	// 获取源封包指针(用户登陆)
	void *pSourcePacket = &dataBuf;
	// 加密后数据
	BYTE EncryptPacket[8192] = { 0 };
	// RC4变异加密
	Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_GP_UserSaveScore));
	// 拷贝加密内容
	memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_GP_UserSaveScore));

	//数据加密RSA >>
	NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL, MDM_GP_USER_SERVICE, SUB_GP_USER_SAVE_SCORE, &dataBuf, sizeof(dataBuf));
}

//发送取款操作请求
void SendMsg::sendTakeMoney(SCORE amount, const char *  password,bool isWechatBind /*= false*/)
{
	CMD_GP_UserTakeScore dataBuf;
	memset(&dataBuf, 0, sizeof(dataBuf));

	dataBuf.dwPort = UserInfo::instance()->gamePort;
	dataBuf.dwIPAddress = 465484375;
	Util::createTCHARWithString(UserInfo::instance()->getAccount(), dataBuf.szAccounts);
	dataBuf.dwUserID = UserInfo::instance()->getUserId();

	//登陆自己的密码
	DWORD logintype = UserInfo::instance()->getLoginType();

	if (logintype == LOGIN_TYPE_WINDOWS_ACCOUNT || logintype == LOGIN_TYPE_MB_APPLE_ACCOUNT || logintype == LOGIN_TYPE_MB_ANDROID_ACCOUNT)
	{
		string passW = Util::strToMd5Str(UserInfo::instance()->getPassword());
		Util::createTCHARWithString(passW, dataBuf.szLogonPassword);
	}
	else
	{
		Util::createTCHARWithString(UserInfo::instance()->getMD5Password(), dataBuf.szLogonPassword);
	}
	dataBuf.lTakeScore = amount;
	string passW = password;
	Util::createTCHARWithString(Util::strToMd5Str(passW), dataBuf.szPassword);

	Util::createTCHARWithString(Util::strToMd5Str(Util::getIDFA()).c_str(), dataBuf.szMachineID);

	//是否绑定微信帐号
	dataBuf.IsBindWeChat = UserInfo::instance()->getbWeChatBind();


	if (isWechatBind)
	{
		CMD_GP_WeChatCode code;
		ZeroMemory(&code, sizeof(code));
		memcpy(&code.takeScore, &dataBuf, sizeof(code.takeScore));
		code.dwCode = (DWORD)std::atoi(UserInfo::instance()->verifactionCode.c_str());//验证码
		//数据加密RSA <<

		// 获取源封包指针(用户登陆)
		void *pSourcePacket = &code;
		// 加密后数据
		BYTE EncryptPacket[8192] = { 0 };
		// RC4变异加密
		Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_GP_WeChatCode));
		// 拷贝加密内容
		memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_GP_WeChatCode));

		NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL, MDM_GP_WECHAT_CODE, SUB_GP_WECHAT_CODE_BACK, &code, sizeof(code));
	}
	else
	{
		//数据加密RSA <<

		// 获取源封包指针(用户登陆)
		void *pSourcePacket = &dataBuf;
		// 加密后数据
		BYTE EncryptPacket[8192] = { 0 };
		// RC4变异加密
		Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_GP_UserTakeScore));
		// 拷贝加密内容
		memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_GP_UserTakeScore));
		//如果未绑定微信>数据加密RSA>
		NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL, MDM_GP_USER_SERVICE, SUB_GP_USER_TAKE_SCORE, &dataBuf, sizeof(dataBuf));
	}
}

//查询转账记录
void SendMsg::sendQueryBill(){

 	DWORD dwUserid = UserInfo::instance()->getUserId();
	NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL, MDM_GP_USER_SERVICE, SUB_GP_QUERY_TRANSRECORD, &dwUserid, sizeof(DWORD));
	
}

int SendMsg::PackCrc32(const char* p, int l)
{
	int r = 0x6523ca60,v;
	while (l > 4)
	{
		v = * ((int*)p);
		r = r ^ v;
		l -= 4;
		p += 4;
	}
	if(l > 0)
	{
		v = 0;
		memmove(&v, p , l);
		r = r ^ v;
	}
	return ~r;
}

//游戏登陆相关消息
// 房间登录命令
void SendMsg::sendRoomLoginReq(DWORD dwUserId)
{

    MainController::getInstance()->setGameState(true);

    CMD_GR_LogonMobile dataBuf;
    memset(&dataBuf, 0, sizeof(CMD_GR_LogonMobile));


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	dataBuf.cbDeviceType = DEVICE_TYPE_ANDROID;
	dataBuf.dwProcessVersion = VERSION_MOBILE_ANDROID;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	dataBuf.cbDeviceType = DEVICE_TYPE_IPHONE;
	dataBuf.dwProcessVersion = VERSION_MOBILE_IOS;
#endif

	dataBuf.wGameID = 0;// DataManager::getInstance()->getCurrentKindID();
	switch (dataBuf.wGameID)
	{
	default:
		dataBuf.dwProcessVersion = VERSION_MOBILE_ANDROID;
		break;
	}
	dataBuf.dwIPAddress = 465484375;
	dataBuf.dwPort = UserInfo::instance()->gamePort;
	dataBuf.wBehaviorFlags = 1;
    dataBuf.wPageTableCount = 6;
    dataBuf.dwUserID = UserInfo::instance()->getUserId();
	
	Util::createTCHARWithString(Util::strToMd5Str(Util::getIDFA()).c_str(), dataBuf.szMachineID);

	memset(dataBuf.szPassword , 0 , LEN_MD5 * sizeof(TCHAR));
	DWORD logintype = UserInfo::instance()->getLoginType();

	if (logintype == LOGIN_TYPE_WINDOWS_ACCOUNT || logintype == LOGIN_TYPE_MB_APPLE_ACCOUNT || logintype == LOGIN_TYPE_MB_ANDROID_ACCOUNT)
	{
		Util::createTCHARWithString(Util::strToMd5Str(UserInfo::instance()->getPassword()), dataBuf.szPassword);

	}
	else
	{
		Util::createTCHARWithString(UserInfo::instance()->getMD5Password(), dataBuf.szPassword);
	}

	memset(dataBuf.szAccounts, 0, LEN_MD5 * sizeof(TCHAR));
	string act = UserInfo::instance()->getAccount();
	Util::createTCHARWithString(act, dataBuf.szAccounts);

	//数据加密RSA <<

	// 获取源封包指针(用户登陆)
	void *pSourcePacket = &dataBuf;
	// 加密后数据
	BYTE EncryptPacket[8192] = { 0 };
	// RC4变异加密
	Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_GR_LogonMobile));
	// 拷贝加密内容
	memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_GR_LogonMobile));

	//NYInfo(TAG,"========================5");

	

    NetworkManager::getInstance()->sendData(NetworkManager::FLAG_GAME, MDM_GR_LOGON, SUB_GR_LOGON_MOBILE, &dataBuf, sizeof(CMD_GR_LogonMobile));//登录信息  手机登录
    
   // CCLog(U2F("%s 房间登录命令 dwUserId  = %d").c_str(),TAG,dwUserId);

}

//发送坐下请求
void SendMsg::sendSitDownReq(int flag, WORD wTableId, WORD wSeatId)
{

	//MainController::getInstance()->pauseMsg(true);//暂停消息派发 只有这个地方用了

    CMD_GR_UserSitDown dataBuf;
    memset(&dataBuf, 0, sizeof(CMD_GR_UserSitDown));
    dataBuf.wTableID = wTableId;
    dataBuf.wChairID = wSeatId;
	//NYInfo(TAG,"=================sendSitDownReq");
    NetworkManager::getInstance()->sendData(flag, MDM_GR_USER, SUB_GR_USER_SITDOWN, &dataBuf, sizeof(CMD_GR_UserSitDown));

}

//请求本桌的用户
void SendMsg::sendGetSeatUserInfoReq(int flag, WORD wTableId, WORD wSeatId)
{
    CMD_GR_ChairUserInfoReq dataBuf;
    memset(&dataBuf, 0, sizeof(CMD_GR_ChairUserInfoReq));
    dataBuf.wTableID = wTableId;
    dataBuf.wChairID = wSeatId;
	//NYInfo(TAG,"=================sendGetSeatUserInfoReq");
    NetworkManager::getInstance()->sendData(flag, MDM_GR_USER, SUB_GR_USER_CHAIR_INFO_REQ, &dataBuf, sizeof(CMD_GR_ChairUserInfoReq));
}

// 发送用户规则请求
void SendMsg::sendUserRuleReq(int flag)
{
	CMD_GR_UserRule dataBuf;
	memset(&dataBuf,0,sizeof(CMD_GR_UserRule));
	//NYInfo(TAG,"=================sendUserRuleReq");
    NetworkManager::getInstance()->sendData(flag, MDM_GR_USER, SUB_GR_USER_RULE, &dataBuf, sizeof(CMD_GR_UserRule));
}

//发送游戏规则请求
void SendMsg::sendGameRuleReq(int flag, BYTE byAllowLookOn, DWORD dwFrameVersion, DWORD dwClientVersion)
{
    CMD_GF_GameOption dataBuf;
    memset(&dataBuf, 0, sizeof(CMD_GF_GameOption));
    dataBuf.cbAllowLookon = byAllowLookOn; //false
    dataBuf.dwFrameVersion = dwFrameVersion; //INVALID_DWORD
    dataBuf.dwClientVersion = dwClientVersion; //INVALID_DWORD
    NetworkManager::getInstance()->sendData(flag, MDM_GF_FRAME, SUB_GF_GAME_OPTION, &dataBuf, sizeof(CMD_GF_GameOption));
}

//发送站起请求
void SendMsg::sendStandUpReq(int flag, WORD wTableId, WORD wSeatId, BYTE byForceLeave, BYTE byExitTable)
{
   
    CMD_GR_UserStandUp dataBuf;
    memset(&dataBuf, 0, sizeof(CMD_GR_UserStandUp));
    dataBuf.wTableID = wTableId;
    dataBuf.wChairID = wSeatId;
    dataBuf.cbForceLeave = byForceLeave;
	//NYInfo(TAG,"=================sendStandUpReq");
    NetworkManager::getInstance()->sendData(flag, MDM_GR_USER, SUB_GR_USER_STANDUP, &dataBuf, sizeof(CMD_GR_UserStandUp));
}

//发送快速坐下换桌请求
void SendMsg::sendQuickSitDown(int flag)
{
   
	//NYInfo(TAG,"=================sendQuickSitDown");
    NetworkManager::getInstance()->sendData(flag, MDM_GR_USER, SUB_GR_USER_CHAIR_REQ, NULL, 0);
}

//发送用户准备
void SendMsg::sendGetUserReadyReq(int flag, DWORD dwUserId, WORD wTableId, WORD wSeatId, BYTE byUserStatus)
{
  
    CMD_GR_UserStatus dataBuf;
    memset(&dataBuf, 0, sizeof(CMD_GR_UserStatus));
    dataBuf.dwUserID = dwUserId;
    dataBuf.UserStatus.wChairID = wSeatId;
    dataBuf.UserStatus.wTableID = wTableId;
    dataBuf.UserStatus.cbUserStatus = byUserStatus;
    
    NetworkManager::getInstance()->sendData(flag, MDM_GF_FRAME, SUB_GF_USER_READY, &dataBuf, sizeof(CMD_GR_UserStatus));
    
}


void SendMsg::sendUserReadyReq(int flag, DWORD dwUserId, WORD wTableId, WORD wSeatId, BYTE byUserStatus)
{
   
//    CMD_GR_UserStatus dataBuf;
//    memset(&dataBuf, 0, sizeof(CMD_GR_UserStatus));
//    dataBuf.dwUserID = dwUserId;
//    dataBuf.UserStatus.wChairID = wSeatId;
//    dataBuf.UserStatus.wTableID = wTableId;
//    dataBuf.UserStatus.cbUserStatus = byUserStatus;
//
//    NetworkManager::getInstance()->sendData(flag, MDM_GF_GAME, SUB_C_CONTINUE_SYSDISTRIBUTE, nullptr, 0);

}

//请求用户信息
void SendMsg::sendGetUserInfoReq(int flag, DWORD dwUserId, WORD wTableId)
{

    CMD_GR_UserInfoReq dataBuf;
    memset(&dataBuf, 0, sizeof(dataBuf));
    dataBuf.dwUserIDReq = dwUserId;
    dataBuf.wTablePos = wTableId;
    NetworkManager::getInstance()->sendData(flag, MDM_GR_USER, SUB_GR_USER_INFO_REQ, &dataBuf, sizeof(CMD_GR_UserInfoReq));

}

//发送坐下请求
void SendMsg::sendSitDownReq(int flag, WORD wTableId, WORD wSeatId, const char * password)
{
   
    CMD_GR_UserSitDown dataBuf;
    memset(&dataBuf, 0, sizeof(CMD_GR_UserSitDown));
    dataBuf.wTableID = wTableId;
    dataBuf.wChairID = wSeatId;
	Util::createTCHARWithString(password, dataBuf.szPassword);
	//NYInfo(TAG,"=================sendSitDownReq password");
    NetworkManager::getInstance()->sendData(flag, MDM_GR_USER, SUB_GR_USER_SITDOWN, &dataBuf, sizeof(CMD_GR_UserSitDown));
}

//刷新个人信息
void SendMsg::refreshUserScore(){

	CMD_GP_QueryInsureInfo QueryUserInfoRequest;
	memset(&QueryUserInfoRequest , 0,sizeof(CMD_GP_QueryInsureInfo));
	//设置变量
	QueryUserInfoRequest.dwUserID=UserInfo::instance()->getUserId();

	DWORD logintype = UserInfo::instance()->getLoginType();

	if (logintype == LOGIN_TYPE_WINDOWS_ACCOUNT || logintype == LOGIN_TYPE_MB_APPLE_ACCOUNT || logintype == LOGIN_TYPE_MB_ANDROID_ACCOUNT)
	{
		Util::createTCHARWithString(Util::strToMd5Str(UserInfo::instance()->getPassword()), QueryUserInfoRequest.szLogonPassword);

	}
	else
	{
		Util::createTCHARWithString(UserInfo::instance()->getMD5Password(), QueryUserInfoRequest.szLogonPassword);
	}

	Util::createTCHARWithString(UserInfo::instance()->getAccount(), QueryUserInfoRequest.szAccounts);


	//数据加密RSA <<

	// 获取源封包指针(用户登陆)
	void *pSourcePacket = &QueryUserInfoRequest;
	// 加密后数据
	BYTE EncryptPacket[8192] = { 0 };
	// RC4变异加密
	Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_GP_QueryInsureInfo));
	// 拷贝加密内容
	memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_GP_QueryInsureInfo));

	//数据加密RSA >>

	NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL , MDM_GP_USER_SERVICE,SUB_GP_QUERY_INSURE_INFO,&QueryUserInfoRequest,sizeof(CMD_GP_QueryInsureInfo));

}




bool SendMsg::SendGetRc4KeyWithSubCmdID(int mainCmd, int subCmd, DWORD currentTime)
{

	
	CMD_GP_Get_Key pGetKeyMsg;
	memset(&pGetKeyMsg, 0, sizeof(CMD_GP_Get_Key));

	//2016-3-21 获取从1970年1月1日0.0.0到当前时间增加到封包内
	//pGetKeyMsg.dwPackTime = dntg::TimerManager::getInstance()->GetCurTick()/1000;  //DWORD  单位秒

	pGetKeyMsg.dwPackTime = currentTime;
	//初始化IP为标识(775*775*775=465484375 )
	pGetKeyMsg.dwIPAddress = 465484375;

	pGetKeyMsg.dwPort = 8300;// UserInfo::instance()->getServerPort();

	Util::createTCHARWithString(UserInfo::instance()->getAccount(), pGetKeyMsg.szAccounts);

// 	//2014-9-25 新增加密数据包功能
// 
	//清空原始大厅RC4 KEY，申请新的RC4 KEY
	ZeroPlazaKey();

	// 获取源封包指针(用户登陆)
	void *pSourcePacket = &pGetKeyMsg;
	// 加密后数据
	BYTE EncryptPacket[8192] = { 0 };
	// RC4变异加密
	Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_GP_Get_Key));
	// 拷贝加密内容
	memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_GP_Get_Key));

	NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL, mainCmd, subCmd, &pGetKeyMsg, sizeof(CMD_GP_Get_Key));


	return true;

}

// 获得子房间类型列表
void SendMsg::sendGameSubType()
{
	CMD_GP_GetGameSubType GetGameSubTypeList;
	memset(&GetGameSubTypeList, 0, sizeof(CMD_GP_GetGameSubType));
	//Game 2016-9-13 初始化IP为标识(775*775*775=465484375 )
	GetGameSubTypeList.dwIPAddress = 465484375;
	//Game 2016-9-13 如果连接服务器成功，则获取当前登陆的服务器端口

	GetGameSubTypeList.dwPort = UserInfo::instance()->getServerPort();
	// 传入用户帐号
	Util::createTCHARWithString(UserInfo::instance()->getAccount(), GetGameSubTypeList.szAccounts);

	//变量定义
	// 	// 获取源封包指针(用户登陆)
	ZeroPlazaKey();

	void *pSourcePacket = &GetGameSubTypeList;
	// 加密后数据
	BYTE EncryptPacket[8192] = { 0 };
	// RC4变异加密
	Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_GP_GetGameSubType));
	// 拷贝加密内容
	memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_GP_GetGameSubType));

	NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL, MDM_GP_SERVER_LIST,SUB_GP_GAMESUBTYPE, &GetGameSubTypeList, sizeof(CMD_GP_GetGameSubType));//列表信息 获取房间 

}

// 获得服务器列表
void SendMsg::sendServerList()
{
	CMD_GP_GetServerList GetServerList;
	memset(&GetServerList, 0, sizeof(CMD_GP_GetServerList));
	//Game 2016-9-13 初始化IP为标识(775*775*775=465484375 )
	GetServerList.dwIPAddress = 465484375;
	//Game 2016-9-13 如果连接服务器成功，则获取当前登陆的服务器端口

	GetServerList.dwPort = UserInfo::instance()->getServerPort();
	// 传入用户帐号
	Util::createTCHARWithString(UserInfo::instance()->getAccount(), GetServerList.szAccounts);

	//变量定义
// 	// 获取源封包指针(用户登陆)
	ZeroPlazaKey();

	void *pSourcePacket = &GetServerList;
	// 加密后数据
	BYTE EncryptPacket[8192] = { 0 };
	// RC4变异加密
	Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_GP_GetServerList));
	// 拷贝加密内容
	memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_GP_GetServerList));

	NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL, MDM_GP_SERVER_LIST, SUB_GP_GET_LIST, &GetServerList, sizeof(CMD_GP_GetServerList));//列表信息 获取房间 

}

//登录
bool SendMsg::SendGetRc4KeyServerList(DWORD currentTime)
{

	CMD_GP_Get_Config pGetKeyMsg;
	memset(&pGetKeyMsg, 0, sizeof(CMD_GP_Get_Config));

	//2016-3-21 获取从1970年1月1日0.0.0到当前时间增加到封包内
	pGetKeyMsg.dwPackTime = currentTime;

	//初始化IP为标识(775*775*775=465484375 )
	pGetKeyMsg.dwIPAddress = 465484375;

	pGetKeyMsg.dwPort = UserInfo::instance()->getServerPort();

	Util::createTCHARWithString(UserInfo::instance()->getAccount(), pGetKeyMsg.szAccounts);

	ZeroPlazaKey();

	// 获取源封包指针(用户登陆)
	void *pSourcePacket = &pGetKeyMsg;
	// 加密后数据
	BYTE EncryptPacket[8192] = { 0 };
	// RC4变异加密
	Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_GP_Get_Config));
	// 拷贝加密内容
	memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_GP_Get_Config));

	NetworkManager::getInstance()->sendData(NetworkManager::FLAG_HALL, MDM_GP_SERVER_LIST, SUB_GP_GET_CONFIG, &pGetKeyMsg, sizeof(CMD_GP_Get_Config));

	return true;
}


//登录
bool SendMsg::SendGetRc4KeyLogin(DWORD currentTime)
{
	
	CMD_GR_Get_Key pGetKeyMsg;
	memset(&pGetKeyMsg, 0, sizeof(CMD_GR_Get_Key));

	//2016-3-21 获取从1970年1月1日0.0.0到当前时间增加到封包内
	pGetKeyMsg.dwPackTime = currentTime;

	//初始化IP为标识(775*775*775=465484375 )
	pGetKeyMsg.dwIPAddress = 465484375;
	//游戏房间的登陆采用游戏房间的端口
	//NYInfo(TAG,"login game room port :%d", UserInfo::instance()->gamePort);
	pGetKeyMsg.dwPort = UserInfo::instance()->gamePort;

	Util::createTCHARWithString(UserInfo::instance()->getAccount(), pGetKeyMsg.szAccounts);

	// 	//2014-9-25 新增加密数据包功能
	// 
	//清空原始大厅RC4 KEY，申请新的RC4 KEY
	ZeroPlazaKey();


	// 获取源封包指针(用户登陆)
	void *pSourcePacket = &pGetKeyMsg;
	// 加密后数据
	BYTE EncryptPacket[8192] = { 0 };
	// RC4变异加密
	Vt_RC4Encrypt((BYTE *)pSourcePacket, EncryptPacket, dwPackKey, sizeof(CMD_GR_Get_Key));
	// 拷贝加密内容
	memcpy(pSourcePacket, EncryptPacket, sizeof(CMD_GR_Get_Key));

	NetworkManager::getInstance()->sendData(NetworkManager::FLAG_GAME, MDM_GR_LOGON, SUB_GR_LOGONROOM_GETKEY, &pGetKeyMsg, sizeof(CMD_GR_Get_Key));

	return true;
}

void SendMsg::sendFaceId()
{
	int sex = UserInfo::instance()->getSex();
	int faceId = UserInfo::instance()->getFaceId();
	if ((faceId <0 || faceId >3) && (faceId <50 || faceId >54))//如果faceId不在我们头像的区间，初始化faceId
	{
		faceId = 0;
		UserInfo::instance()->setFaceId(faceId);
	}
	UserInfo::instance()->httpRequest();
	UserInfo::instance()->SendType = 0;
	UserInfo::instance()->mainCmd = MDM_GP_GET_KEY;
	UserInfo::instance()->subCmd = SUB_GP_GETKEY_CHANGFACE;
	UserInfo::instance()->selectFace = faceId;
}

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && CODE_LLVM_ENCRYPT
#pragma obfuscate off
#endif

