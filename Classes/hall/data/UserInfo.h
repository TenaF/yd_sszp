//
//  UserInfo.h
//  hello2dx
//
//
//

#ifndef __hello2dx__UserInfo__
#define __hello2dx__UserInfo__

#include <string>
#include <map>
#include <vector>
#include "sdk/message/Common.h"
#include "cocos2d.h"
#include "network/HttpClient.h"//网络
using namespace std;

enum SwitchState{
	SWITCH_STATE_NONE,//开关无状态等待读取aspx
	SWITCH_STATE_OPEN,//开关开启
	SWITCH_STATE_CLOSE//开关关闭
};

class UserInfo : public Ref
{
public:
	UserInfo();
	~UserInfo();
	static UserInfo* instance();
	void init();
public:
	std::string getDomainVctByIndex(unsigned int idx){
		if (idx >= m_DomainVector.size())return ""; return m_DomainVector.at(idx);
	}

	bool switchDomain();

	const std::vector<std::string >& getDomainVector()const { return m_DomainVector; }

	void pushDomainName(const std::string& domain_name){
		if (domain_name.empty() || (m_DomainVector.end() != std::find(m_DomainVector.begin(), m_DomainVector.end(), domain_name)))return;
		m_DomainVector.push_back(domain_name);
	}

	//本地存储连接大厅IP
	void setServerIp(const std::string& _server_ip) {
		UserDefault::getInstance()->setStringForKey("SERVER_IP", _server_ip);
		UserDefault::getInstance()->flush();
	}
	std::string getServerIp() { 
		return UserDefault::getInstance()->getStringForKey("SERVER_IP", "NONE");
	}
	//本地存储连接大厅端口
	void setServerPort(const int server_port) {
		UserDefault::getInstance()->setIntegerForKey("SERVER_PORT", server_port);
		UserDefault::getInstance()->flush();
	}
	int getServerPort() {
		return UserDefault::getInstance()->getIntegerForKey("SERVER_PORT", 0);
	}
	//游戏房间是否分发ip地址（0不分发1分发）
	void setDistributePort(const int distribute_port) {
		UserDefault::getInstance()->setIntegerForKey("DISTRIBUTE_PORT", distribute_port);
		UserDefault::getInstance()->flush();
	}
	int getDistributePort() {
		return UserDefault::getInstance()->getIntegerForKey("DISTRIBUTE_PORT", 0);
	}
	//本地存储推广ID
	void setPromotionID(const std::string& promotionId){
		UserDefault::getInstance()->setStringForKey("PROMOTION_ID", promotionId);
		UserDefault::getInstance()->flush();
	}

	//第一次安装的日期
	void setInstalldate(const std::string& date){
		UserDefault::getInstance()->setStringForKey("INSTALL_DATE", date);
		UserDefault::getInstance()->flush();
	}
	const std::string getInstalldate(){
		return UserDefault::getInstance()->getStringForKey("INSTALL_DATE");
	}

	const std::string getPromotionID() {
		return UserDefault::getInstance()->getStringForKey("PROMOTION_ID");
	}
	//游戏标识ID
	std::string getGameflagID() {

		if (m_gameflagID.empty())
		{
			auto temp_pDict = __Dictionary::createWithContentsOfFile("popularizeInfo.xml");
			m_gameflagID = temp_pDict->valueForKey("gameflagID")->getCString();
		}
		return m_gameflagID;
	}

	int SendType;//发送消息类型
	int mainCmd;//
	int subCmd;//
	//游戏房间端口
	int gamePort;
	void httpRequest();//HTTP请求
	//Http响应  回调函数
	//返回数据回来
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	//Http响应  回调函数 支付配置请求
	void httpRequest(const char *param, const char* tag);//HTTP请求
	//返回数据回来
	void onHttpRequestMerchandise(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	std::string  verifactionCode;//验证码
	std::string pswTemp;//
	std::string nickTemp;//
	std::string pswTempConfirm;//

	int checkValue;//
	int wFaceID;//
	string strBankPass;//
	string strCard;//



	WORD m_wSitdownTableID;//
	WORD m_wSitdownChairID;//
	int selectFace;//
	static const string setPath(string path);

	// self userid存取
	inline void setbWeChatBind(bool  _bWeChatBind) { m_bWeChatBind = _bWeChatBind; }
	inline bool getbWeChatBind() { return m_bWeChatBind; }

    // self userid存取
    inline void setUserId(DWORD dwUserId) { m_dwUserId = dwUserId; }
    inline DWORD getUserId() { return m_dwUserId; }
    // 游戏id存取
    inline void setGameId(DWORD dwGameId) { m_dwGameId = dwGameId; }
    inline DWORD getGameId() { return m_dwGameId; }
    // 游戏类型存取
    inline void setKindId(DWORD dwKind) { m_dwKindId = dwKind; }
    inline DWORD getKindId() { return m_dwKindId; }
    // 用户头像存取
    inline void setFaceId(WORD wFaceId) { m_wFaceId = wFaceId; }
    inline WORD getFaceId() { return m_wFaceId; }
    // deviceid 存取
    inline void setDeviceId(TCHAR device[])
    {
        
    }
    // 设备类型存取
    inline void setDeviceType(BYTE byDeviceType) { m_byDeviceType = byDeviceType; }
    inline BYTE getDeviceType() { return m_byDeviceType; }
    // self玩家桌子存取
    inline void setTableId(WORD wTableId) { m_wTableId = wTableId; }
    inline WORD getTableId() {return m_wTableId;}

    // self玩家座位存取
    inline void setSeatId(WORD wSeatId) { m_wSeatId = wSeatId; }
    inline WORD getSeatId() { return m_wSeatId; }

    // self玩家座位存取
    inline void setStatus(BYTE status) { m_byStatus = status; }
    inline BYTE getStatus() { return m_byStatus; }
    // self昵称存取
    inline void setNickName(string nickName) { m_strUserName = nickName; }
    inline string getNickName() { return m_strUserName; }
    // self账号存取
	inline void setAccount(string account) { m_strAccount = account; }
	inline string getAccount() { return m_strAccount; }
	// self经验存取
	inline void setExprience(WORD exp) { m_dwExprience = exp; }
    inline DWORD getExprience() { return m_dwExprience; }
	// self性别存取
    inline void setSex(BYTE sex) { m_bySix = sex; }
    inline BYTE getSex() { return m_bySix; }
	 // self密码存取
    inline void setPassword(string pass) { m_strPassWord = pass; }
    inline string getPassword() { return m_strPassWord; }
    // 临时的密码存取
    inline void setTempPassword(string pass) { m_strTempPassWord = pass; }
    inline string getTempPassword() { return m_strTempPassWord; }
	//登陆模式存取
	inline void setLoginType(DWORD loginType) { m_loginType = loginType; }
	inline DWORD getLoginType() { return m_loginType; }
	//会员等级存取
	inline void setCbMemberOrder(BYTE cbMemberOrder){ m_cbMemberOrder = cbMemberOrder; }
	inline int getCbMemberOrder(){return (int)m_cbMemberOrder; }
	//会员到期时间获取
	inline void setMemberOverDate(SYSTEMTIME MemberOverDate){ m_MemberOverDate = MemberOverDate; }
	inline SYSTEMTIME getMemberOverDate(){ return m_MemberOverDate; }

	//微信和苹果登陆才有的MD5密码由服务端传回来
	inline void setMD5Password(string pass) { m_strMD5PassWord = pass; }
	inline string getMD5Password() { return m_strMD5PassWord; }

	//奖券
	inline void setMedal(DWORD medal) { m_dwMedal = medal; }
	inline DWORD getMedal() { return m_dwMedal; }

	 //游戏币
	inline void setScore(SCORE score) { m_lScore = score; }
    inline SCORE getScore() { return m_lScore; }

	// self银行存取
	inline void setInsureScore(SCORE score) { m_lInsure = score; }
	inline SCORE getInsureScore() { return m_lInsure; }

	 // self机器标识存取
    inline void setMachineID(string machineID) { m_strMachineID = machineID; }
    inline string getMachineID() { return m_strMachineID; }

	 // self昵称存取
    inline void setStartMessage(string strMessage) { m_strStartMessage = strMessage; }
    inline string getStartMessage() { return m_strStartMessage; }

	void setCurrentServerID(DWORD serverID){m_currentServerID = serverID;};
	DWORD getCurrentServerID(){return m_currentServerID;};

	//临时存钱
	inline void setAmountTemp(SCORE account) { amountTemp = account; }
	inline SCORE getAmountTemp() { return amountTemp; }


	inline void setServerName(string strName) {
		m_strServerName = strName;
	}
	inline string getServerName() {
		return m_strServerName;
	}

	inline void setDomainName(string strName) {
		m_strDomainName = strName;
	}
	inline bool DomainIsEmpty() {
		return m_strDomainName.empty();
	}


	// 获得请求地址
	std::string getDomainUrlByName(const char* nameUrl, bool isKeyName = true);


	CC_SYNTHESIZE(bool, m_isChangeTable, IsChangeTable);//是否换桌
    CC_SYNTHESIZE(std::string, m_PlazaNetVer, PlazaNetVersion);//是否换桌
	CC_SYNTHESIZE_READONLY(SwitchState, m_nStoreSwitch, StoreSwitch);		//商城开关
	CC_SYNTHESIZE_READONLY(SwitchState, m_nBillSwitch, BillSwitch);			//账单开关
	CC_SYNTHESIZE_READONLY(SwitchState, m_nGiveSwitch, GiveSwitch);			//赠送开关
	CC_SYNTHESIZE_READONLY(SwitchState, m_nExcShareSwitch, ExcShareSwitch);	//兑换分享开关
	CC_SYNTHESIZE_READONLY(std::string, m_strServerTips, ServerTips);	//兑换分享开关
    
	//CC_SYNTHESIZE_READONLY(std::string, m_gameflagID, ServerTips);	//兑换分享开关
	
	const std::vector<int>& getPayPlatforms(){ return mPayPlatforms; }
private:

    static UserInfo* _instance;

	bool  m_bWeChatBind;
	SCORE amountTemp;//
	//用户信息
	DWORD m_dwUserId;   				//用户I D
	DWORD m_dwGameId;   				//游戏I D
	DWORD m_dwKindId;   				//类型I D
	DWORD m_dwExprience;   				//经验
	WORD  m_wFaceId;   					//用户头像
	BYTE  m_bySix;						//用户性别
	DWORD m_loginType;					//登陆模式

	BYTE  m_cbMemberOrder;				//会员等级
	SYSTEMTIME m_MemberOverDate;			//到期时间

	//TCHAR m_strMachineID[LEN_MD5];		//机器标识

	BYTE m_byDeviceType;				//设备类型
	WORD m_wTableId;					//桌子号
	WORD m_wSeatId;						//椅子号
	BYTE m_byStatus;					//用户状态

    string m_strUserName;    			//用户帐号
    string m_strPassWord;    			//用户密码
    string m_strTempPassWord;    		//备份用户密码
    string m_strBankWord;    			//银行密码
    string m_strNickName;    			//用户昵称
    string m_strPhoneNum;    			//手机号码
    string m_strAccount;
    string m_strMachineID;				//机器标识
	string m_postId;					//唯一标识
	string m_strMD5PassWord;    		//md5密码

	string m_strStartMessage;           //启动消息

	DWORD m_dwMedal;					//奖券
	SCORE m_lScore;						//金币
	SCORE m_lInsure;					//银行
	SCORE m_lCoin;						//通宝

	DWORD m_outMatchID;					//报名淘汰赛ID

	DWORD m_currentServerID;		 	//当前进入的房间ID

	string m_strServerName;

	string m_strDomainName;

	string m_gameflagID;
	//备选域名地址
	std::vector<std::string > m_DomainVector;

	//全局使用的
	std::vector<int> mPayPlatforms;

	unsigned int m_nDomainIndex;
};


#endif
