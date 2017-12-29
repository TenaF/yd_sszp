//
//  PokerPlayerInfo.h
//  hello2dx
//
//  Created by
//
//

#ifndef __hello2dx__PokerPlayerInfo_yxx__
#define __hello2dx__PokerPlayerInfo_yxx__

#include "sdk/message/Common.h"
#include "sdk/message/Struct.h"
#include "game/yxx/message/CMD_Game_yxx.h"

using namespace std;
USING_NS_CC;

namespace yxx
{
	class PokerPlayerInfo
	{
	public:
		PokerPlayerInfo();
		~PokerPlayerInfo();
    
	public:

		void init();
		//玩家座位存取
		inline void setSeatId(WORD wSeatId) { m_InfoHead.wChairID = wSeatId; }
		inline WORD getSeatId() { return m_InfoHead.wChairID; }
		//桌子存取
		inline void setTableId(WORD wTableId) { m_InfoHead.wTableID = wTableId; }
		inline WORD getTableId() { return m_InfoHead.wTableID; }
		//玩家状态存取
		inline void setUserStatus(BYTE byStatus) { m_InfoHead.cbUserStatus = byStatus; }
		inline BYTE getUserStatus() { return m_InfoHead.cbUserStatus; }
    
		inline void setUserBet(long long llBet) { m_llUserBet = llBet; }
		inline void addUserBet(long long llBet) { m_llUserBet += llBet; }
		inline void reduceUserBet(long long llBet) { m_llUserBet -= llBet; }
		inline long long getUserBet() { return m_llUserBet; }
    
		// 当前下注
		inline void setThisBet(long long llBet) { m_llBet = llBet; }
		inline long long getThisBet() { return m_llBet; }
    
		//下注数目
		inline void setTurnBet(long long llBet) { m_llTurnBet = llBet; }
		inline void addTurnBet(long long llBet) { m_llTurnBet += llBet; }
		inline long long getTurnBet() { return m_llTurnBet; }
		//累计下注
		inline void setTotalBet(long long llBet) { m_llTotalBet = llBet;}
		inline void addTotalBet(long long llBet) { m_llTotalBet += llBet;}
		inline long long getTotoalBet() { return m_llTotalBet; }
		// 输的筹码
		inline void setLostBet(long long llBet) { m_llLostBet = llBet;}
		inline long long getLostBet() { return m_llLostBet; }
		//是否显示牌
		inline void setShowHandCard(bool flag) { m_bShowHand = flag; }
		inline bool isShowHandCard() { return m_bShowHand; }
		//是否弃牌
		inline void setGiveUp(bool flag) { m_bGiveUp = flag; }
		inline bool isGiveUp() { return m_bGiveUp; }
		//游戏税收
		inline void setTax(long long llBet) { m_llGameTax = llBet; }
		inline long long getTax() { return m_llGameTax; }
		//游戏得分
		inline void setResultScore(long long llBet){ m_llGameScore = llBet;}
		inline long long getResultScore() { return m_llGameScore; }
		//玩家 UserInfoHead 信息存取
		inline void setInfo(tagMobileUserInfoHead infoHead) { m_InfoHead = infoHead;}
		inline tagMobileUserInfoHead getInfo() { return m_InfoHead; }
		// 玩家tagMobileUserScore 信息存取，平局，分数，经验等
		void setUserScoreInfo(tagMobileUserScore userScore);
		// 设置玩家分数
		void setUserScore(SCORE llScore);
		// 增加玩家分数
		void addUserScore(SCORE llAddScore);
		// 设置玩家分数
		inline void setScore(SCORE llScore) { m_llScore = llScore; }
		inline SCORE getScore() { return m_llScore; }
    
		// 设置玩家更新分数
		inline void setUpdateScore(SCORE llScore) { m_llUpdateScore = llScore; }
		inline SCORE getUpdateScore(SCORE llScore) { return m_llUpdateScore; }
		inline void setNickName(const std::string& nickName) { m_NickName = nickName; }
		inline const std::string& getNickName() { return m_NickName; }
		inline int getSex() { return m_InfoHead.cbGender; }
		inline BYTE getPlayStatus() { return m_playStatus ;}
		inline void setPlayStatus(BYTE status) { m_playStatus = status; }
		inline bool isNewIn() { return m_bNewIn; }
		inline void setNewIn(bool bNew) { m_bNewIn = bNew; }
		inline WORD getFaceId() {	return m_FaceId;}
		inline void setFaceId(WORD wFaceId) {	m_FaceId = wFaceId;		}
		int getCardsCount() { return m_nCardsCount; }
    

	private:
		tagMobileUserInfoHead m_InfoHead;//用户信息
		SCORE   m_llScore;			//当前得分
		SCORE   m_llUpdateScore;	//分数更新
		int m_nCardsCount; // 玩家手牌数
		std::string m_NickName;
		WORD m_FaceId;
    
		// --------------------begain poker -----------------
		BYTE m_playStatus;              //游戏状态
	//    BYTE m_handCards[MAX_COUNT];    //用户扑克
		bool m_bNewIn;                  //新手盲注
		long long m_llUserBet;            //用户筹码,玩家身上显示筹码
		long long m_llBet;      //下注数目
		long long m_llTurnBet; // 本轮累计下注
		long long m_llTotalBet; //累计下注
		long long m_llLostBet; //输掉的钱
		bool m_bShowHand; //是否显示牌
		bool m_bGiveUp;  //是否弃牌
		long long m_llGameTax; //游戏税收
		long long m_llGameScore; //游戏得分
	};
}

#endif /* defined(__hello2dx__PokerPlayerInfo__) */
