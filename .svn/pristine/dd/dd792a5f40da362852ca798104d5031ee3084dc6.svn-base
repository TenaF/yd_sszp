#ifndef __hello2dx__DataManager__
#define __hello2dx__DataManager__

/*
	数据管理
*/

#include "sdk/sdk.h"
#include "hall/data/UserInfo.h"
#include "sdk/scene/BaseScene.h"
#include "sdk/data/GlobalSchedule.h"

class DataManager :public Ref
{
	private:

		static DataManager * m_instance;
		//当前的游戏id
		int currentKindID;

		bool bGameServerDatafinlsh;
		bool bGameSubTypDatafinlsh;

		std::map<int, std::list<tagGameServer *>>  m_AllGameServerMap;	//游戏房间类型列表
		//所有游戏的全部子类型
		std::map<int, std::list<tagGameSubType *>>  m_AllGameSubTypeMap;	//游戏房间类型列表
		//所有的游戏类型列表
		std::list<tagGameType *> m_GameTypeList;						

		string strMessageType;											//发送消息类型
		std::map<int, std::string> m_configGameList;
	public:

		DataManager(void);
		~DataManager(void);

		static DataManager * getInstance();
// 		void GameServerDatafinlsh(){ bGameServerDatafinlsh = true; }
// 		void GameSubTypDatafinlsh(){ bGameSubTypDatafinlsh = true; }
// 
// 		bool const IsGameServerDatafinlsh(){ return bGameServerDatafinlsh; }
// 		bool const IsgetGameSubTypDatafinlsh(){ return bGameSubTypDatafinlsh; }
		//清理服务器信息
		void clearServerList();
		//保存服务器信息
		void insertGameServer(tagGameServer * pServer);
		//保存子游戏等级类型信息
		void insertGameSubType(tagGameSubType * pServer);
		//保存游戏分类类型信息
		void insertGameType(tagGameType * pServer);
		//获得服务器信息
		tagGameServer * getGameServer(int ServerID);
		//获得游戏类型
		std::list<tagGameType *> getGameTypeList(bool isSort = true);

		////游戏服务器列表
		//list<tagGameServer *> getServerList();

		void initConfigGameList(int kindID , string params);

		string getParamsFromGameList(int kindID);
		//获得当前游戏房间所有子类型
		std::list<tagGameSubType *> getCurGameSubTypeList(bool isSort = true);
		//获得当前游戏的所有房间
		std::list<tagGameServer *> getCurGameServerList(bool isSort = true);
// 		int getTransferRatio() {
// 			return transferRatio;
// 		}
// 		;
// 		void setTransferRatio(int ratio) {
// 			transferRatio = ratio;
// 		}

		 int getCurrentKindID() {
            return currentKindID;
        }
        ;
        void setCurrentKindID(int kindid) {
            currentKindID = kindid;
        };
};

#endif

