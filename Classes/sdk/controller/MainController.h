//
//  MainController.h
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#ifndef __hello2dx__MainController__
#define __hello2dx__MainController__

#include "cocos2d.h"
#include "sdk/message/MsgBase.h"
#include "sdk/message/Define.h"


using namespace std;
USING_NS_CC;

struct SerInfo
{
	char IP[128];
	unsigned int nPort;
	bool bDistributePort;
};

class MainController : public Ref
{
public:
    static MainController* getInstance();
    void start();
    void stop();

	void startConnect(int flag, const char* ip, unsigned int port);
    void stopConnect(int flag);
    void pauseMsg(bool flag);
	void serverRequest(int subCmd, bool bIPconnect);
	void setGameState(bool gameState)
	{
	}

	bool IsCheckSocket(){
		return mIsCheckSocket;
	}
    
    void restartLoginSer();
	void gamesWillEnterBackground();
	void gamesWillEnterForeground();
	void pushSerInfo(const SerInfo& info);
	const SerInfo& getSerInfo(unsigned int index);
	void startCheckNetIpConect(float dt);
	void connectHallFinish();
	void connectSubGameNetWork();
	CC_SYNTHESIZE(unsigned short, m_SerMaxNum, SerMaxNum);
private:
	void reConnectTipsPanel();
	void timeConect(float dt);
    void closereConnectTipsPanel(Ref* ref);
    void init();
    static void msgCB(Ref* object, MsgBase* msg);
    void onMsg(MsgBase* msg);
	//大厅登陆ip地址
	std::vector<SerInfo> SerInfoVertor;
	bool mIsCheckSocket;
	bool m_firstConnectLogin;
	int m_subCmd;
	int m_index;
	DWORD m_EnterForeground_Time;
};


#endif
