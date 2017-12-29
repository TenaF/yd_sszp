#include "DataManager.h"
#include "sdk/ui/PromptLayer.h"
//#include "game/message/LordMessageController.h"
#include <assert.h>

#define TAG "DataManager"

DataManager * DataManager::m_instance = nullptr;

DataManager::DataManager(void)
// 	:bGameServerDatafinlsh(false)
// 	,bGameSubTypDatafinlsh(false)
{

}


DataManager::~DataManager(void)
{
// 	std::list<tagGameServer *>::iterator it = m_ServerList.begin();
// 
// 	for(;it != m_ServerList.end();++it)
// 	{
// 		tagGameServer * pServer = (*it);
// 		delete pServer;
// 	}
// 
// 	m_ServerList.clear();

}

//创建实例
DataManager * DataManager::getInstance(){
	if(m_instance == nullptr){
		m_instance = new DataManager();
	}
	return m_instance;
}


void DataManager::clearServerList()
{
	//m_ServerList.clear();
}

void DataManager::insertGameServer(tagGameServer * pServer)
{
	std::map<int, std::list<tagGameServer *>>::const_iterator itr = m_AllGameServerMap.find(pServer->wKindID);
	if (itr == m_AllGameServerMap.end())
	{
		std::list<tagGameServer *> gameSubTypeList;
		tagGameServer * m_pServer = new tagGameServer();
		memcpy(m_pServer, pServer, sizeof(tagGameServer));
		gameSubTypeList.push_back(m_pServer);
		m_AllGameServerMap[pServer->wKindID] = gameSubTypeList;
	}
	else
	{
		std::list<tagGameServer *>& gameSubTypeList = m_AllGameServerMap[pServer->wKindID];
		std::list<tagGameServer *>::iterator it = gameSubTypeList.begin();

		for (; it != gameSubTypeList.end(); ++it)
		{
			tagGameServer * ptServer = (*it);
			if (pServer->wSortID == ptServer->wSortID)
			{
				memcpy(ptServer, pServer, sizeof(tagGameServer));
				return;
			}
		}

		tagGameServer * m_pServer = new tagGameServer();
		memcpy(m_pServer, pServer, sizeof(tagGameServer));
		gameSubTypeList.push_back(m_pServer);
	}
}

void DataManager::insertGameSubType(tagGameSubType * _pSubType)
{
	std::map<int, std::list<tagGameSubType *>>::const_iterator itr = m_AllGameSubTypeMap.find(_pSubType->dwKindID);
	if (itr == m_AllGameSubTypeMap.end())
	{
		std::list<tagGameSubType *> gameSubTypeList;
		tagGameSubType * m_pSubType = new tagGameSubType();
		memcpy(m_pSubType, _pSubType, sizeof(tagGameSubType));
		gameSubTypeList.push_back(m_pSubType);
		m_AllGameSubTypeMap[_pSubType->dwKindID] = gameSubTypeList;
	}
	else
	{
		std::list<tagGameSubType *>& gameSubTypeList = m_AllGameSubTypeMap[_pSubType->dwKindID];
		std::list<tagGameSubType *>::iterator it = gameSubTypeList.begin();

		for (; it != gameSubTypeList.end(); ++it)
		{
			tagGameSubType * m_pSubType = (*it);
			if (m_pSubType->wSortID == _pSubType->wSortID)
			{
				memcpy(m_pSubType, _pSubType, sizeof(tagGameSubType));
				return;
			}
		}

		tagGameSubType * m_pSubType = new tagGameSubType();
		memcpy(m_pSubType, _pSubType, sizeof(tagGameSubType));
		gameSubTypeList.push_back(m_pSubType);
	}
}





std::list<tagGameSubType *> DataManager::getCurGameSubTypeList(bool isSort)
{
	std::list<tagGameSubType *> GameSubTypeList;
	map<int, std::list<tagGameSubType *>>::iterator itr = m_AllGameSubTypeMap.find(currentKindID);
	if (itr != m_AllGameSubTypeMap.end())
		GameSubTypeList = m_AllGameSubTypeMap[currentKindID];

	if (isSort)
	{
		struct SortGameSubType
		{
			//参数用大写的_X和_Y会导致安卓编译不过
			bool operator()(tagGameSubType*_x, tagGameSubType* _y) const
			{
				return (_x->wSortID < _y->wSortID);
			}
		};
		GameSubTypeList.sort(SortGameSubType());
	}

	return GameSubTypeList;
}

std::list<tagGameServer *> DataManager::getCurGameServerList(bool isSort)
{
	std::list<tagGameServer *> GameServerlist;
	map<int, std::list<tagGameServer *>>::iterator itr = m_AllGameServerMap.find(currentKindID);
	if (itr != m_AllGameServerMap.end())
		GameServerlist = m_AllGameServerMap[currentKindID];
	if (isSort)
	{

		struct SortGameServer
		{
			//参数用大写的_X和_Y会导致安卓编译不过
			bool operator()(tagGameServer*_x, tagGameServer* _y) const
			{
				return (_x->wSortID < _y->wSortID);
			}
		};
		GameServerlist.sort(SortGameServer());
	}

	return GameServerlist;
}

//插入所有游戏子类型信息
void DataManager::insertGameType(tagGameType * pServer)
{
	std::list<tagGameType *>::iterator it = m_GameTypeList.begin();

	for (; it != m_GameTypeList.end();)
	{
		tagGameType * ptServer = (*it);
		if (pServer->wSortID == ptServer->wSortID)
		{
			memcpy(ptServer, pServer, sizeof(tagGameType));
			return;
		}
		it++;
	}

	tagGameType * m_pServer = new tagGameType();
	memcpy(m_pServer, pServer, sizeof(tagGameType));
	m_GameTypeList.push_back(m_pServer);
}

// 获得房间信息
tagGameServer * DataManager::getGameServer(int ServerID){

	std::list<tagGameServer *> gameServerList = this->getCurGameServerList(false);
	std::list<tagGameServer *>::iterator it = gameServerList.begin();
	for (; it != gameServerList.end(); it++)
	{
		tagGameServer * pServer = (*it);
		if(pServer->wServerID == ServerID){
			return (*it);
		}
	}
	return nullptr;
}
std::list<tagGameType *> DataManager::getGameTypeList(bool isSort){

	if (isSort)
	{
		struct SortGameServer
		{
			//参数用大写的_X和_Y会导致安卓编译不过
			bool operator()(tagGameType*_x, tagGameType* _y) const
			{
				return (_x->wSortID < _y->wSortID);
			}
		};
		m_GameTypeList.sort(SortGameServer());
	}

	return m_GameTypeList;
}

// 游戏服务器列表
// std::list<tagGameServer *> DataManager::getServerList(){
// 	return m_ServerList;
// }

//初始化游戏列表
void DataManager::initConfigGameList(int kindID , string params)
{
	m_configGameList.insert(map<int, std::string>::value_type(kindID, params));
}

//获得游戏参数
string DataManager::getParamsFromGameList(int kindID)
{
	return m_configGameList[kindID];
}
