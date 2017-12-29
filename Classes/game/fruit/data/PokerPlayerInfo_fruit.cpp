//
//  PokerPlayerInfo.cpp
//  hello2dx
//
//  Created by
//
//

#include "PokerPlayerInfo_fruit.h"

namespace fruit
{
	PokerPlayerInfo::PokerPlayerInfo()
	{
		init();
	}

	PokerPlayerInfo::~PokerPlayerInfo()
	{
    
	}

	void PokerPlayerInfo::init()
	{
		m_llScore = 0;
		m_llUpdateScore = 0;
		m_playStatus = 0;
		m_bNewIn = false;
		m_llUserBet = 0;
		m_llBet = 0;
		m_llTurnBet = 0;
		m_llTotalBet = 0;
		m_llLostBet = 0;
		m_bShowHand = false;
		m_bGiveUp = false;
		m_llGameTax = 0;
		m_llGameScore = 0;
    
		m_nCardsCount = 0;
		m_FaceId=0;
	}

	//
	void PokerPlayerInfo::setUserScoreInfo(tagMobileUserScore userScore)
	{
		m_llScore = m_InfoHead.lScore;
		m_llUpdateScore = abs(m_llScore - userScore.lScore);
		m_InfoHead.dwDrawCount	= userScore.dwDrawCount;
		m_InfoHead.dwWinCount	= userScore.dwExperience;
		m_InfoHead.dwFleeCount	= userScore.dwFleeCount;
		m_InfoHead.dwLostCount	= userScore.dwLostCount;
	
		m_InfoHead.lScore		= userScore.lScore;
		m_InfoHead.dwExperience = userScore.dwExperience;
	}

	void PokerPlayerInfo::setUserScore(SCORE llScore)
	{
		m_llScore = m_InfoHead.lScore;
		m_InfoHead.lScore = llScore;
		m_llUpdateScore = abs(m_llScore - m_InfoHead.lScore);
	}

	void PokerPlayerInfo::addUserScore(SCORE llAddScore)
	{
		m_InfoHead.lScore += llAddScore;
		m_llScore = m_InfoHead.lScore;
	}
}