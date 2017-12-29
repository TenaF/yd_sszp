//
//  PokerData.h
//  hello2dx
//
//  Created by
//
//

#ifndef __hello2dx__PokerData_fruit__
#define __hello2dx__PokerData_fruit__

#include "sdk/message/Common.h"
#include "hall/data/UserInfo.h"
#include "PokerPlayerInfo_fruit.h"
#include "cocos2d.h"
#include <vector>
#include <list>
using namespace std;
USING_NS_CC;



namespace fruit
{

	struct tagTableUserSitDownInfo
	{
		WORD   wTableID;
		WORD   wChairID;
		string strName;
	};

	class PokerData
	{
	public:
		~PokerData();
		static PokerData* instance();
		void init();
		//设置比赛规则参数
		inline void setMatchRule(DWORD dwRule) { m_dwMatchRule = dwRule; }
		inline DWORD getMatchRule() { return m_dwMatchRule; }
		//设置是否快速启动参数
		inline void setQuickStartType(bool startType) { m_bQuickStart = startType; }
		inline bool getQuickStartType() { return m_bQuickStart; }
		//用户状态存取 US_NULL/US_FREE/US_SIT/US_READY/US_LOOKON/US_PLAYING/US_OFFLINE
		inline void setStatus(BYTE status) { m_byUserStatus = status; }
		inline BYTE getStatus() { return m_byUserStatus; }
		//游戏状态参数存取 GAME_STATUS_FREE/ GAME_STATUS_PLAY/GAME_STATUS_WAIT
		inline void setGameStatus(BYTE gameStatus) { m_byGameStatus = gameStatus; }
		inline BYTE getGameStatus() { return m_byGameStatus; }
    
		// 玩家信息存取
	
		void setPlayerInfo(DWORD dwUserId, PokerPlayerInfo* pInfo);
		PokerPlayerInfo* getPlayerInfo(DWORD dwUserId);
		PokerPlayerInfo* getPlayerInfoBySeatId(int seatId);
		void removePlayerInfo(DWORD dwUserId);

		// seatid 到 pos 的转换
		WORD getPosBySeat(int seat);

		//房间信息
		void setTableCount(WORD wTableCount) { m_wTableCount = wTableCount; }
		WORD getTableCount() { return m_wTableCount; }


		std::map<DWORD, tagTableUserSitDownInfo> m_mapUserSitDownInfo;//跟m_pPlayerInfoMap同步管理数据成员
		map<int, PokerPlayerInfo*>* m_pPlayerInfoMap;//玩家信息

	private:

		PokerData();
		static PokerData* _instance;

		// --------begain poker----------------
		DWORD m_dwMatchRule; //房间配置
		bool  m_bQuickStart;
		BYTE  m_byUserStatus; // 用户状态，坐下，空闲，准备等
		BYTE  m_byGameStatus; //游戏状态

		WORD    m_wTableCount;
	

	};
}

#endif
