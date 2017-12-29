#ifndef __hello2dx__TableScene_yxx__
#define __hello2dx__TableScene_yxx__

#include "sdk/scene/SceneBase.h"
#include "game/yxx/data/PokerData_yxx.h"
#include "InputPassword_yxx.h"
#include "game/yxx/message/CMD_Game_yxx.h"
namespace yxx
{
	//每张桌子的信息
	struct tagTableInfo
	{
		WORD    wTableID;
		bool    bIsGamePlaying;
		bool    bIsHavePassWord;
		bool    bIsSitDown[GAME_PLAYER];
		WORD    wSitDownFaceID[GAME_PLAYER];
		BYTE    cbSitDownCount;
	};



	class TableScene : public SceneBase, InputPasswordListener
	{
	public:
		TableScene();
		~TableScene();

		void heartbeat(float dt);
		void initView();
		void onEnter();
		void onExit();
		void bindData(PokerData* data);
		void onBackPressed();
		void onTouch(Ref* target, Widget::TouchEventType type);
		virtual void update(float dt);
		//接口函数
		void onMoblieUserStatusReset();
		void onMoblieUserStatusRefresh(std::map<DWORD, tagTableUserSitDownInfo> &tempMap);
		void onMoblieUserStatusChange(std::map<DWORD, tagTableUserSitDownInfo> &tempMap, CMD_GR_UserStatus* pBuf, WORD wFaceID, string strName);
		void setTableStatus(WORD wTableID, bool bIsPlaying, bool bIsLock);

		//接口回调
		void ExecSitDownOperate(string strPassword);

		bool getPlayIsSitDown(std::map<DWORD, tagTableUserSitDownInfo> &tempMap, DWORD dwUserid);
		void notifyFail(Ref * obj);
		void outTime(float dt);
		void autoClose(float dt);
		void autoExit();
		void closeSocket_noifyFial(Object * obj);
		void sendSitDownMsg(float dt);
		void getSitdownChairID(){ m_wSitdownTableID; }
		void addTableFlag();
		bool isTableIsFlag(WORD wtableId);
		void tableFlagClear(){ m_pRoomIdflag.clear(); }
	private:
		void ChangeTable();
		void disconnect(Ref * obj);
		Layout* m_Layout;
		cocos2d::ui::ScrollView* m_pScrollView;

		WORD m_wCurClickTableID;
		PokerData* m_GameData;
		WORD m_wSitdownTableID;
		WORD m_wSitdownChairID;
		vector<WORD> m_pRoomIdflag;//进去过的有鱼阵的房间ID

		std::map<int, tagTableInfo> m_mapTableInfo;

	};
}

#endif 
