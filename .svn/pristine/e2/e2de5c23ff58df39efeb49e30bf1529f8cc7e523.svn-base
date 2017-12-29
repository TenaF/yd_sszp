//
//  PokerData.cpp
//  hello2dx
//
//  Created by
//
//

#include "PokerData_yxx.h"
#include "hall/data/UserInfo.h"
#define TAG "PokerData"

namespace yxx
{
	PokerData* PokerData::_instance = 0;

	PokerData::PokerData()
	{
		m_pPlayerInfoMap = new map<int, PokerPlayerInfo*>();
		m_mapUserSitDownInfo.clear();

		m_dwMatchRule = 0;
		m_wTableCount=0;
		init();
	}

	void PokerData::init()
	{
		m_bQuickStart = false;
		m_byUserStatus = US_NULL;
		m_byGameStatus = GAME_STATUS_FREE;
	}

	PokerData::~PokerData()
	{
		_instance = nullptr;
	}

	PokerData* PokerData::instance()
	{
		if(!_instance)
		{
			_instance = new PokerData();
		}
		return _instance;
	}



	void PokerData::setPlayerInfo(DWORD dwUserId, PokerPlayerInfo* pInfo)
	{
		if (m_pPlayerInfoMap)
		{
			m_pPlayerInfoMap->insert(pair<int, PokerPlayerInfo*> (dwUserId, pInfo));
		}
    
	}

	PokerPlayerInfo* PokerData::getPlayerInfo(DWORD dwUserId)
	{
		if (m_pPlayerInfoMap)
		{
			map<int, PokerPlayerInfo*>::iterator it = m_pPlayerInfoMap->find(dwUserId);
			if (it != m_pPlayerInfoMap->end())
			{
				return it->second;
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	}

	PokerPlayerInfo* PokerData::getPlayerInfoBySeatId(int seatId)
	{
		if (m_pPlayerInfoMap)
		{
			map<int, PokerPlayerInfo*>::iterator it = m_pPlayerInfoMap->begin();
			while (it != m_pPlayerInfoMap->end())
			{
				if (it->second->getSeatId() == seatId && it->second->getTableId() == UserInfo::instance()->getTableId())
				{
					return it->second;
				}
				++it;
			}
		}
		return nullptr;
	}

	void PokerData::removePlayerInfo(DWORD dwUserId)
	{
		if (m_pPlayerInfoMap)
		{
			m_pPlayerInfoMap->erase(dwUserId);
		}
	}

	//根据真实座位号获得转换过的位置
	WORD PokerData::getPosBySeat(int seat) // 0-1
	{
		WORD me_chair_id = UserInfo::instance()->getSeatId();//自己的位置 0-3
		if (me_chair_id >= GAME_PLAYER)//如果出错 直接返回
			return seat;
		if (me_chair_id >= GAME_PLAYER/2) {//如果转换过视图
			seat = (seat + GAME_PLAYER / 2) % GAME_PLAYER;
			return seat;
		}
		return seat;//如果没有转换视图

	}

	

}