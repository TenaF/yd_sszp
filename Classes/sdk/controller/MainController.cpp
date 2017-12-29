//
//  MainController.cpp
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#include "MainController.h"
#include "sdk/message/Common.h"
#include "hall/data/DataManager.h"
#include "sdk/util/Util.h"
#include "sdk/ui/UpgradeLayer.h"
#include "sdk/ui/PromptLayer.h"
#include "hall/ui/MainLayer.h"
#include "hall/data/ServerAddress.h"
#include "hall/ui/LoadingScene.h"
#include "hall/data/UserInfo.h"
#include "game/sszp/message/PokerMessageController_sszp.h"
#include "game/sicbo/message/PokerMessageController_sicbo.h"
#include "game/fruit/message/PokerMessageController_fruit.h"
#include "game/yxx/message/PokerMessageController_yxx.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d::network;
USING_NS_CC;

#define TAG "MainController"
static MainController* s_Instance = nullptr;


MainController* MainController::getInstance() {
	if (s_Instance == nullptr) {
		s_Instance = new (std::nothrow) MainController();
		s_Instance->init();
	}
	return s_Instance;
}

void MainController::init() {
    restartLoginSer();
	m_EnterForeground_Time = 0;
}

void MainController::restartLoginSer()
{
    mIsCheckSocket = false;
	m_firstConnectLogin = true;
	m_index = 0;
	m_SerMaxNum = 0;
	SerInfoVertor.clear();
}

void  MainController::startCheckNetIpConect(float dt)
{
	Director::getInstance()->getScheduler()->unscheduleSelector(schedule_selector(MainController::startCheckNetIpConect), this);
	mIsCheckSocket = true;
	if (UserInfo::instance()->getServerIp() != "NONE")
	{
		NYInfo(TAG,"getServerIp:%s", UserInfo::instance()->getServerIp().c_str());
		bool flag = true;
		for (auto info : SerInfoVertor)
		{
			if (UserInfo::instance()->getServerIp() == (char*)info.IP)
				flag = false;
		}

		if (flag)
		{
			NYInfo(TAG,"inset back-up ip:%s", UserInfo::instance()->getServerIp().c_str());
			SerInfo serinfo;
			strcpy(serinfo.IP, UserInfo::instance()->getServerIp().c_str());
			serinfo.nPort = (unsigned int)UserInfo::instance()->getServerPort();
			serinfo.bDistributePort = (bool)UserInfo::instance()->getDistributePort();
			SerInfoVertor.push_back(serinfo);
		}
	}
	NYInfo(TAG,"startCheckNetIpConect====ser num:%d", SerInfoVertor.size());
	Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(MainController::timeConect), this, 4.f, false);//-1  循环执行
	this->timeConect(0.0f);

	
}

void  MainController::connectHallFinish()
{
	Director::getInstance()->getScheduler()->unscheduleSelector(schedule_selector(MainController::timeConect), this);
	if ((m_index - 1) >= (int)SerInfoVertor.size())
		return;
	m_firstConnectLogin = false;
	SerInfo serInfo = SerInfoVertor.at(m_index-1);
	NYInfo(TAG,"+++=====logonIp:%s,port: %d=======+++", (char*)serInfo.IP, serInfo.nPort);
	UserInfo::instance()->setServerIp(serInfo.IP);//120.25.128.104
	UserInfo::instance()->setServerPort(serInfo.nPort);//8300
	UserInfo::instance()->setDistributePort(serInfo.bDistributePort);//0 or 1


}

void MainController::pushSerInfo(const SerInfo& info)
{
	if (!mIsCheckSocket)
	{
		SerInfoVertor.push_back(info);
		if (m_SerMaxNum == SerInfoVertor.size())
		{
			NYInfo(TAG,"all ip push finish!!!!!!!!!!!!!!!!!!!!!!");
			Director::getInstance()->getScheduler()->unscheduleSelector(schedule_selector(MainController::startCheckNetIpConect), this);
			//0.1s等待线程完全分离否则安卓崩溃
			Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(MainController::startCheckNetIpConect), this, 0.1f, false);//-1  循环执行
		}
	}
}

const SerInfo& MainController::getSerInfo(unsigned int index)
{
	SerInfo ser_info;
	memset(&ser_info, 0, sizeof(SerInfo));
	if (index < SerInfoVertor.size() && index >= 0)
	{
		//auto ser_info_ = ;
		memcpy(&ser_info,&SerInfoVertor.at(index), sizeof(SerInfo));
	}

	
	return ser_info;
}
void MainController::timeConect(float dt)
{
	if (m_index >= (int)SerInfoVertor.size())
	{
		Director::getInstance()->getScheduler()->unscheduleSelector(schedule_selector(MainController::timeConect), this);
		LoadingScene::hideModal();
		PromptLayer::createObject("连接服务器超时，请检查网络连接");
		return;
	}
	
	SerInfo serInfo = SerInfoVertor.at(m_index);
	NYInfo(TAG,"timeConect:%s", (char*)serInfo.IP);
	NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_HALL);
	this->startConnect(NetworkManager::FLAG_HALL, Util::getIpByHostName((char*)(serInfo.IP)), serInfo.nPort);
	cocos2d::NotificationCenter::getInstance()->postNotification(CONNECT_NETWORK_FINISH, Integer::create(m_subCmd));

	m_index++;
}

void MainController::serverRequest(int subCmd, bool bIPconnect)
{
	LoadingScene::createObject();
	m_subCmd = subCmd;

	if (bIPconnect)
	{
		NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_HALL);
		if (SerInfoVertor.empty())
		{
			PromptLayer::createObject("请检查填写的ip地址或者端口！！");
			LoadingScene::hideModal();
			return;
		}
		m_index = 1;
		connectHallFinish();
		SerInfo serInfo = SerInfoVertor.at(0);
		NYInfo(TAG, "timeConect:%s", (char*)serInfo.IP);
		NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_HALL);
		this->startConnect(NetworkManager::FLAG_HALL, Util::getIpByHostName((char*)(serInfo.IP)), serInfo.nPort);
		cocos2d::NotificationCenter::getInstance()->postNotification(CONNECT_NETWORK_FINISH, Integer::create(m_subCmd));
	}
	else
	{
		restartLoginSer();
		NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_HALL);
		Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(MainController::startCheckNetIpConect), this, 2.f, false);//-1  循环执行
		ServerAddress* serverAds = new ServerAddress();
		serverAds->httpRequest();

	}

}



void MainController::start() {

	UserInfo::instance()->setDomainName("http://ceshiqb.youxi555.net");
	UserInfo::instance()->pushDomainName("http://ceshiqb.youxi555.net");
	BaseScene * pLayer = new MainLayer();
	Director::getInstance()->runWithScene(BaseScene::createScene(pLayer));
	pLayer->release();
}


void MainController::stop() {

}
void MainController::connectSubGameNetWork()
{
	auto serverID = UserInfo::instance()->getCurrentServerID();
	auto pGameServer = DataManager::getInstance()->getGameServer(serverID);
	if (pGameServer){
		string ip = Util::getStringWithTChar(pGameServer->szServerAddr);
		//游戏内联网
		//游戏房间改用大厅端口号和ip网关防御用[zhangyang 2016/03/19]
		UserInfo::instance()->gamePort = pGameServer->wServerPort;

		//游戏房间改用大厅端口号和ip网关防御用[zhangyang 2016/03/19]
		if (UserInfo::instance()->getDistributePort())
		{
			NYInfo(TAG, "分发断网重连 %d:%s,%d", 1, Util::getIpByHostName((char*)(UserInfo::instance()->getServerIp().c_str())), UserInfo::instance()->getServerPort());
			MainController::getInstance()->startConnect(NetworkManager::FLAG_GAME, Util::getIpByHostName((char*)(UserInfo::instance()->getServerIp().c_str())), UserInfo::instance()->getServerPort());
		}
		else
		{
			NYInfo(TAG, "不分发断网重连  %d:%s,%d", 0, Util::getIpByHostName((char*)ip.c_str()), pGameServer->wServerPort);
			MainController::getInstance()->startConnect(NetworkManager::FLAG_GAME, Util::getIpByHostName((char*)ip.c_str()), pGameServer->wServerPort);
		}

		if (false == Director::getInstance()->getScheduler()->isScheduled("LoginGameRoom", this))
		{
			Director::getInstance()->getScheduler()->schedule([&](float dt){
				if (NetworkManager::getInstance()->isConnected(NetworkManager::FLAG_GAME)){
					Director::getInstance()->getScheduler()->unschedule("LoginGameRoom", this);
					UserInfo::instance()->SendType = 1;
					UserInfo::instance()->httpRequest();
				}
			}, this, 0.3f, false, "LoginGameRoom");
		}
	}
}
void MainController::closereConnectTipsPanel(Ref* ref)
{
    auto runScene = Director::getInstance()->getRunningScene();
    //游戏登陆完成消息收到后移除面板
    if (runScene && nullptr==runScene->getChildByName("ConnectGameScene")){
        NotificationCenter::getInstance()->removeObserver(this, GAME_LOGIN_FINISH);
        auto isConnectingPanel = runScene->getChildByName("ReconnectPanel");
        if (isConnectingPanel)
        {
            isConnectingPanel->removeFromParentAndCleanup(true);
        }
        
    }
}

void MainController::reConnectTipsPanel()
{
	if (DataManager::getInstance()->getCurrentKindID() == 124){
		NotificationCenter::getInstance()->postNotification(GAME_DISCONNECT, 0);
		return;
	}
	else if (DataManager::getInstance()->getCurrentKindID() == 118){
		NotificationCenter::getInstance()->postNotification(GAME_DISCONNECT, 0);
		return;
	}
	else if (DataManager::getInstance()->getCurrentKindID() == 131){
		NotificationCenter::getInstance()->postNotification(GAME_DISCONNECT, 0);
		return;
	}
	else if (DataManager::getInstance()->getCurrentKindID() == 119){
		NotificationCenter::getInstance()->postNotification(GAME_DISCONNECT, 0);
		return;
	}
	auto runScene = Director::getInstance()->getRunningScene();
	//规避<房间列表界面>进入游戏被游戏房间分数限制断开游戏联网情况，
	if (runScene && nullptr==runScene->getChildByName("ConnectGameScene"))
	{
		auto palyerState = UserInfo::instance()->getStatus();
		//游戏状态
		if (palyerState == US_NULL)return;
		auto isConnectingPanel = runScene->getChildByName("ReconnectPanel");
		if (isConnectingPanel)
		{
			//如果当前存在重连面板则更新重连状态
            NYInfo(TAG,"连接失败，重置重连状态，～～～～～");
			//isConnectingPanel->setUserObject(Bool::create(false));
		}
		else
		{
            NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(MainController::closereConnectTipsPanel),GAME_LOGIN_FINISH,nullptr);
			auto panel = Layout::create();
			panel->setTouchEnabled(true);
			panel->setContentSize(CCSize(VisibleRect::getWidth(), VisibleRect::getHeight()));
			panel->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
			panel->setBackGroundColor(Color3B::BLACK);
			panel->setBackGroundColorOpacity(50);
			//panel->setUserObject(Bool::create(true));
			auto tips_img = ImageView::create("commonresource/disconnect_Imgbg.png");
			if (tips_img)
			{
				tips_img->setPosition(Vec2(panel->getCustomSize() / 2));
				panel->addChild(tips_img);
			}
			auto remainTime = TextBMFont::create(cocos2d::Value(60).asString(), "fonts/disconnect_label.fnt");
			if (remainTime)
			{
				remainTime->setPosition(Vec2(tips_img->getPositionX() + tips_img->getCustomSize().width / 2 - 170, tips_img->getPositionY() - 5));
				panel->addChild(remainTime);
				remainTime->runAction(RepeatForever::create(Sequence::createWithTwoActions(DelayTime::create(1.0f), CallFuncN::create([&](Node* node){
					auto self = dynamic_cast<TextBMFont*>(node);
					if (self)
					{
						auto parent = self->getParent();
						if (!parent)return;
						

						auto int_value = cocos2d::Value(self->getString()).asInt();
						if (int_value > 1)
						{
							//如果正在连接状态则返回
//							auto bConnect = (Bool*)parent->getUserObject();
//							if (bConnect->getValue())return;
//							parent->setUserObject(Bool::create(true));
                            
                            //如果网连上直接移除当前重连面板
                            if (false == NetworkManager::getInstance()->isConnect(NetworkManager::FLAG_GAME))
                            {
                                connectSubGameNetWork();
                            }
							
						}
						else
						{
							parent->removeFromParentAndCleanup(true);
                            if (false == NetworkManager::getInstance()->isConnect(NetworkManager::FLAG_GAME))
                            {
                                NotificationCenter::getInstance()->postNotification(GAME_DISCONNECT, 0);
                            }
							return;
						}

						self->setString(cocos2d::Value(--int_value).asString());
					}

				}))));
			}
			runScene->addChild(panel, 1000, "ReconnectPanel");
		}

	}
}
//Home键游戏切换到后台
void MainController::gamesWillEnterBackground()
{
// 	if (false == Director::getInstance()->getScheduler()->isScheduled("LoginGameRoom111", this))
// 	{
// 		Director::getInstance()->getScheduler()->schedule([&](float dt){
// // 			if (NetworkManager::getInstance()->isConnected(NetworkManager::FLAG_GAME)){
// // 				Director::getInstance()->getScheduler()->unschedule("LoginGameRoom111", this);
// // 				UserInfo::instance()->SendType = 1;
// // 				UserInfo::instance()->httpRequest();
// // 			}
// 			NYInfo(TAG, "gamesWillEnterBackground Time:%lu", ++dd);
// 		}, this, 1.0f, false, "LoginGameRoom111");
// 	}

	NYInfo(TAG, "Home键游戏切换到后台");
// 	if (false == NetworkManager::getInstance()->isConnect(NetworkManager::FLAG_GAME))
// 		return;
	//if (DataManager::getInstance()->getCurrentKindID() != kind_qbsk)
	NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_GAME);
	m_EnterForeground_Time = Util::getTimeStamp();
}
//切换回游戏
void MainController::gamesWillEnterForeground(){
	NYInfo(TAG, "即将回到游戏进程");

	if (false == NetworkManager::getInstance()->isConnect(NetworkManager::FLAG_GAME))
		return;
	//if (DataManager::getInstance()->getCurrentKindID() != kind_qbsk)return;

	auto d_time = (float)((int)(Util::getTimeStamp() - m_EnterForeground_Time) / 1000);
	NYInfo(TAG, "m_EnterForeground_Time:%f",d_time);
	if (d_time > 10)
	{
		NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_GAME);
	}
		
}


//联网 跟网络管理器类通信
void MainController::startConnect(int flag, const char* ip, unsigned int port) {
	NetworkManager::getInstance()->startConnect(this, msgCB, flag, ip, port);
}

void MainController::stopConnect(int flag) {
	NetworkManager::getInstance()->disconnect(flag);
}

void MainController::pauseMsg(bool flag) {
	//NYInfo(TAG, "pauseMsg, flag=%d", flag);
	NetworkManager::getInstance()->pause(flag);
}

void MainController::msgCB(Ref* object ,MsgBase* msg) {
	static_cast<MainController*>(object)->onMsg(msg);
}

void MainController::onMsg(MsgBase* msg) {

	//log("MainController::onMsg --- type=%d", msg->getType());
	if (msg->getType() == MSG_TYPE::MSG_TYPE_NETWORK_STATE) {
		NetworkStateMsg *stateMsg = static_cast<NetworkStateMsg *>(msg);
		switch(stateMsg->getState()) {
			case NET_STATE_TYPE::NET_STATE_START_CONNECT :
				CCLOG("%s =========kaishilianjie=========", TAG);//开始连接
				break;

			case NET_STATE_TYPE::NET_STATE_FAILED : {
				CCLOG("%s =========lianjieshibai=========", TAG);//连接失败

				string strError = U2F("连接登录服务器失败");
				NotificationCenter::getInstance()->postNotification(CONNECT_SERVER_FAIL, (Ref *) strError.c_str());
			}
				break;

			case NET_STATE_TYPE::NET_STATE_CONNECTED : {

				//SendMsg::sendVerifyReq(msg->getFlag());

				if (msg->getFlag() == NetworkManager::FLAG_HALL) {
					NYInfo(TAG, "NetworkManager::FLAG_HALL 连接成功！");
					if (m_firstConnectLogin)
						connectHallFinish();
				}
				else if (msg->getFlag() == NetworkManager::FLAG_GAME) {
					//发送8字节心跳包到游戏服务器内（请求游戏服务器Rc4Key成功后）
					NYInfo(TAG, "NetworkManager::FLAG_GAME 连接成功！");
				}
				NetworkManager::getInstance()->setConnected(msg->getFlag(), true);
			}
				break;

			case NET_STATE_TYPE::NET_STATE_DISCONNECT : {
				if (msg->getFlag() == NetworkManager::FLAG_HALL) {
                    NYInfo(TAG,"=============NET_STATE_TYPE::NET_STATE_DISCONNECT FLAG_HALL");
					//大厅断网
					//断开连接，包括主动和被动
					this->startConnect(NetworkManager::FLAG_HALL, Util::getIpByHostName((char*)(UserInfo::instance()->getServerIp().c_str())), UserInfo::instance()->getServerPort());
				} else {
					//游戏断网
					//断开连接，包括主动和被动
					NYInfo(TAG,"=============NET_STATE_TYPE::NET_STATE_DISCONNECT FLAG_GAME");
					string strError = U2F("与游戏服务器连接断开");
					//PromptLayer::createObject(strError.c_str());
                    //NYInfo(TAG,strError.c_str());
					//NotificationCenter::getInstance()->postNotification(GAME_DISCONNECT, (Ref *)strError.c_str());
						reConnectTipsPanel();
				}
			}
				break;

			case NET_STATE_TYPE::NET_STATE_CLOSE :        //主动断开
			{
				if (msg->getFlag() == NetworkManager::FLAG_HALL) {
					NYInfo(TAG,"=============NET_STATE_TYPE::NET_STATE_CLOSE FLAG_HALL");
				} else {
					NYInfo(TAG,"=============NET_STATE_TYPE::NET_STATE_CLOSE FLAG_GAME");
					string strError = U2F("与游戏服务器连接断开");
					//PromptLayer::createObject(strError.c_str());
					//NYInfo(TAG,strError.c_str());
						reConnectTipsPanel();
					//NotificationCenter::getInstance()->postNotification(GAME_DISCONNECT, (Ref *)strError.c_str());
				}

			}
				break;

			default :
				break;
		}
	} else if (msg->getType() == MSG_TYPE::MSG_TYPE_NETWORK) {
		NetworkMsg* networkmsg = static_cast<NetworkMsg*>(msg);
		if (networkmsg) {
			NYInfo(TAG, "系统接受到的消息 main=%d   sub=%d", networkmsg->getMainId(), networkmsg->getSubId());
			if (msg->getFlag() == NetworkManager::FLAG_HALL) {
				HallMessageController::instance()->handleMessage(networkmsg->getMainId(), networkmsg->getSubId(), networkmsg->getBuffer(),networkmsg->getLen());
			} else {
				int kindid = DataManager::getInstance()->getCurrentKindID();
     		switch (kindid) {
			case 124:
				sszp::PokerMessageController::instance()->handleMessage(networkmsg->getMainId(), networkmsg->getSubId(), networkmsg->getBuffer(), networkmsg->getLen());
				break;
			case 118:
				sicbo::PokerMessageController::instance()->handleMessage(networkmsg->getMainId(), networkmsg->getSubId(), networkmsg->getBuffer(), networkmsg->getLen());
				break;
			case 119:
				yxx::PokerMessageController::instance()->handleMessage(networkmsg->getMainId(), networkmsg->getSubId(), networkmsg->getBuffer(), networkmsg->getLen());
				break;
			case 131:
				fruit::PokerMessageController::instance()->handleMessage(networkmsg->getMainId(), networkmsg->getSubId(), networkmsg->getBuffer(), networkmsg->getLen());
				break;
					 default:
						 break;
                }
				//GameMessageController::instance()->handleMessage(networkmsg->getMainId(), networkmsg->getSubId(), networkmsg->getBuffer(), networkmsg->getLen());
			}
		}
	}
}

