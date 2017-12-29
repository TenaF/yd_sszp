//
//  PokerMessageController.h
//  hello2dx
//
//  Created by
//
//


//#include "cocos2d.h"
#ifndef __hello2dx__PokerMessageController_sicbo__
#define __hello2dx__PokerMessageController_sicbo__
#include "hall/message/GameMessageController.h"
#include "game/sicbo/ui/MainScene_sicbo.h"
#include "game/sicbo/table/TableScene_sicbo.h"
#include "game/sicbo/data/PokerData_sicbo.h"

using namespace std;
USING_NS_CC;

namespace sicbo
{
	class PokerMessageController :public GameMessageController
	{
	public:
		static PokerMessageController* instance();

		//主桌面消息处理
		virtual bool handleMainStatus(int nSubCmdId, void *buf, WORD wBufLen);
		virtual bool hanleUpdateNotify(void *buf, WORD wBufLen);


		//登录成功
		virtual bool handleLoginSuccess(void *buf, WORD wBufLen);
		//登录失败
		virtual bool handleLoginFailure(void *buf, WORD wBufLen);
		//登录完成
		virtual bool hanleLoginFinish(void *buf, WORD wBufLen);


		//列表配置
		virtual bool handleRoomList(void *buf, WORD wBufLen);
		//房间配置，桌子列表
		virtual bool handleRoomConfig(void *buf, WORD wBufLen);
		//道具列表
		virtual bool handlePropertyConfig(void *buf, WORD wBufLen);
		//房间桌子配置完成
		virtual bool handleConfigFinish(void *buf, WORD wBufLen);



		// 用户消息处理
		//玩家进入
		virtual bool handleUserEnter(void *buf, WORD wBufLen);
		//刷新分数
		virtual bool handleUserScore(void *buf, WORD wBufLen);
		//改变游戏状态
		virtual bool handleUserStatus(void *buf, WORD wBufLen);
		//用户请求失败处理
		virtual bool handleUserReqFailure(void *buf, WORD wBufLen);

		//用户状态
		virtual bool handleTableInfo(void* buf, WORD wBufLen);
		virtual bool handleTableStatus(void* buf, WORD wBufLen);

		//
		virtual bool handleMobileUserStatus(void *buf, WORD wBufLen);

		virtual bool handleGameAck(int nSubCmdId, void *buf, WORD wBufLen);
		// 框架消息处理
		virtual bool handleMainFrameGameOption(void *buf, WORD wBufLen);
		virtual bool handleMainFrameReady(void *buf, WORD wBufLen);
		virtual bool handleMainFrameChat(void *buf, WORD wBufLen);
		virtual bool handleMainFrameExpression(void *buf, WORD wBufLen);
		virtual bool handleMainFrameLookOnConfig(void *buf, WORD wBufLen);
		virtual bool handleMainFrameGameStatus(void *buf, WORD wBufLen);

		virtual bool handleMainFrameGameScene(void *buf, WORD wBufLen);

		virtual bool handleMainFrameSystemMessage(void *buf, WORD wBufLen);




		bool handleGameSceneFree(void *buf, WORD wBufLen);
		bool handleGameScenePlay(void *buf, WORD wBufLen);
		PokerPlayerInfo *getPlayerInfo(WORD seatId) { return pokerData->getPlayerInfoBySeatId(seatId); }

		void onQuitGame();
		void onTableSceneQuit();
		void clearMainScene();
		//wangbo
		void updatePlayerInfo(PokerPlayerInfo *player);
		void cacheGameSceneData();
		void removeGameSceneData();
		static const string changePath(std::string str);
		void didEnterBackground();
	public:
		CC_SYNTHESIZE(std::string, isGetOut, IsGetOut);
	private:
		PokerMessageController();
		~PokerMessageController();
		PokerData *pokerData;
		TableScene* m_TableScene;
		MainScene_sicbo * sicbo_mainScene;

		bool m_reConnectFlag;		//断线重连标记

		void updateTest(float dt);
	};
}


#endif /* defined(__hello2dx__PokerMessageController__) */
