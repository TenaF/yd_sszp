#pragma once
#include "game/yxx/ui/MainScene_yxx.h"

//////////////////////////////////////////////////////////////////////////
namespace yxx{
	//游戏对话框
	class CGameClientDlg
	{
		//友元声明
		friend class CGameClientView;

		//限制信息
	protected:
		LONGLONG						m_lMeMaxScore;						//最大下注
		LONGLONG						m_lAreaLimitScore;					//区域限制
		LONGLONG						m_lApplyBankerCondition;			//申请条件
		int                             m_GameEndTime;
		bool							m_blUsing;

		//个人下注
	protected:
		LONGLONG						m_lUserJettonScore[AREA_COUNT + 1];	//个人总注

		//庄家信息
	protected:
		LONGLONG						m_lBankerScore;						//庄家积分
		WORD							m_wCurrentBanker;					//当前庄家
		BYTE							m_cbLeftCardCount;					//扑克数目
		bool							m_bEnableSysBanker;					//系统做庄


		//状态变量
	protected:
		bool							m_bMeApplyBanker;					//申请标识

		//控件变量
	protected:

		//控件变量
	protected:

		//声音资源
	protected:


		struct SortTemp
		{
			LONGLONG a;
			LONGLONG b;
			LONGLONG c;
			LONGLONG d;
			SortTemp()
			{
				memset(this, 0, sizeof(*this));
			}
		};


		SortTemp m_TempData;

		//函数定义
	public:
		//构造函数
		CGameClientDlg();
		//析构函数
		virtual ~CGameClientDlg();
	private:
		static CGameClientDlg *instance;
	public:
		static CGameClientDlg * getInstance();
		MainScene_yxx * yxx_mainScene;
		//网络消息
		virtual bool OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
		//常规继承
	private:
		//初始函数
		virtual bool OnInitGameEngine();
		//重置框架
		virtual bool OnResetGameEngine();
		//游戏设置
		virtual void OnGameOptionSet();
		//时钟删除
		virtual bool OnEventGameClockKill(WORD wChairID);
		//时钟信息
		virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);
		//旁观状态
		virtual bool OnEventLookonMode(void * pData, WORD wDataSize);
		//游戏场景
		virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, void * pData, WORD wDataSize);
		virtual bool AllowBackGroundSound(bool bAllowSound);

		bool OnFrameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize);
		//消息处理
	protected:
		//游戏开始
		bool OnSubGameStart(void * pBuffer, WORD wDataSize);
		//游戏下注
		bool OnSubGameBet(void * pBuffer, WORD wDataSize);
		//游戏空闲
		bool OnSubGameFree(void * pBuffer, WORD wDataSize);
		//用户加注
		bool OnSubPlaceJetton(void * pBuffer, WORD wDataSize, bool bGameMes);
		//游戏结束
		bool OnSubGameEnd(void * pBuffer, WORD wDataSize);
		//申请做庄
		bool OnSubUserApplyBanker(void * pBuffer, WORD wDataSize);
		//取消做庄
		bool OnSubUserCancelBanker(void * pBuffer, WORD wDataSize);
		//切换庄家
		bool OnSubChangeBanker(void * pBuffer, WORD wDataSize);
		//游戏记录
		bool OnSubGameRecord(void * pBuffer, WORD wDataSize);
		//下注失败
		bool OnSubPlaceJettonFail(void * pBuffer, WORD wDataSize);
		//扑克牌
		bool OnSubSendCard(void * pBuffer, WORD wDataSize);

		bool OnSubCheckImageIndex(void * pBuffer, WORD wDataSize);
		//控制
		bool OnSubAdminControl(void * pBuffer, WORD wDataSize);
		bool OnSubScoreResult(void * pBuffer, WORD wDataSize);
		bool OnSubAccountResult(void * pBuffer, WORD wDataSize);
		bool OnSubChangeUserScore(void * pBuffer, WORD wDataSize);
		bool OnRepeatPlaceJetton(void * pBuffer, WORD wDataSize); //复投成功
		//赋值函数
	protected:
		//设置庄家
		void SetBankerInfo(WORD wBanker, LONGLONG lScore);
		//个人下注
		void SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);

		//功能函数
	protected:
		void ReSetBankCtrol(int nGameState);
		//更新控制
		void UpdateButtonContron();

		//消息映射
	protected:

	};
}
//////////////////////////////////////////////////////////////////////////
