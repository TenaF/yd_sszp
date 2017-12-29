#include "GameClientDlg_fruit.h"
//////////////////////////////////////////////////////////////////////////
namespace fruit{
	
#define IDI_FREE					99									
#define IDI_PLACE_JETTON			100									
#define IDI_DISPATCH_CARD			301									
#define IDI_OPEN_CARD				302								   

#define IDI_ANDROID_BET				1000	


	//////////////////////////////////////////////////////////////////////////

	CGameClientDlg::CGameClientDlg()
	{
		
		m_lMeMaxScore = 0L;
		m_lAreaLimitScore = 0L;
		m_lApplyBankerCondition = 0L;
		m_GameEndTime = 0;

		m_blUsing = false;



		
		ZeroMemory(m_lUserJettonScore, sizeof(m_lUserJettonScore));

	
		m_lBankerScore = 0L;
		m_wCurrentBanker = 0L;
		m_cbLeftCardCount = 0;

	
		m_bMeApplyBanker = false;
		fruit_mainScene = nullptr;
		return;
	}

	
	CGameClientDlg::~CGameClientDlg()
	{
	}

	
	bool CGameClientDlg::OnInitGameEngine()
	{


		return true;
	}

	CGameClientDlg * CGameClientDlg::instance = nullptr;

	CGameClientDlg * CGameClientDlg::getInstance()
	{
		if (!instance)
		{
			instance = new CGameClientDlg();
		}
		return instance;
	}

	
	bool CGameClientDlg::OnResetGameEngine()
	{
		
		m_lMeMaxScore = 0L;

		
		ZeroMemory(m_lUserJettonScore, sizeof(m_lUserJettonScore));

		
		m_lBankerScore = 0L;
		m_wCurrentBanker = 0L;
		m_cbLeftCardCount = 0;

		m_bMeApplyBanker = false;

		return true;
	}

	
	void CGameClientDlg::OnGameOptionSet()
	{

		return;
	}

	
	bool CGameClientDlg::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
	{

		return true;
	}

	
	bool CGameClientDlg::OnEventLookonMode(void * pData, WORD wDataSize)
	{
		return true;
	}

	
	bool CGameClientDlg::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
	{
		log("wSubCmdID____%d", wSubCmdID);
		switch (wSubCmdID)
		{
		case SUB_S_GAME_FREE:		
		{
										return OnSubGameFree(pBuffer, wDataSize);
		}
		case SUB_S_GAME_START:		
		{
										return OnSubGameStart(pBuffer, wDataSize);
		}
		case SUB_S_PLACE_JETTON:	
		{
										return OnSubPlaceJetton(pBuffer, wDataSize, true);
		}
		case SUB_S_APPLY_BANKER:	
		{
										return OnSubUserApplyBanker(pBuffer, wDataSize);
		}
		case SUB_S_CANCEL_BANKER:	
		{
										return OnSubUserCancelBanker(pBuffer, wDataSize);
		}
		case SUB_S_CHANGE_BANKER:	
		{
										return OnSubChangeBanker(pBuffer, wDataSize);
		}
		case SUB_S_GAME_END:		
		{
										return OnSubGameEnd(pBuffer, wDataSize);
		}
		case SUB_S_SEND_RECORD:		
		{
								  return true;
										//return OnSubGameRecord(pBuffer, wDataSize);
		}
		case SUB_S_PLACE_JETTON_FAIL:	
		{
											return OnSubPlaceJettonFail(pBuffer, wDataSize);
		}
		case SUB_S_CHANGE_GOLD:   //游戏一开始就全部兑换筹码 111
		{
									  return OnSubChangeGold(pBuffer, wDataSize);
		}
		case SUB_S_GAME_COMPARE:   //比大小返回结构体 112
		{
									   return true;
									   //return OnSubCompare(pBuffer, wDataSize);
		}
		case SUB_S_GUESS:   //比大小返回结构体 114
		{
									   return OnSubCompare(pBuffer, wDataSize);
		}
		case SUB_S_BUTTON_RIGHT:   //游戏一开始就全部兑换筹码 116
		{
									   return OnSubButtonRight(pBuffer, wDataSize);
		}
		case SUB_S_BUTTON_LEFT:   //游戏一开始就全部兑换筹码 117
		{
									  return OnSubButtonLeft(pBuffer, wDataSize);
		}
		

			case SUB_S_REPEAT_PLACE_JETTON:
			{
			return OnRepeatPlaceJetton(pBuffer, wDataSize);
			}
		}


		return true;
	}

	bool CGameClientDlg::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
	{

		switch (cbGameStation)
		{
		case GAME_STATUS_FREE:			
		{
											CC_ASSERT(wDataSize == sizeof(CMD_S_StatusFree));
											if (wDataSize != sizeof(CMD_S_StatusFree)) return false;

											
											CMD_S_StatusFree * pStatusFree = (CMD_S_StatusFree *)pBuffer;




											return true;
		}
		//case GS_PLACE_JETTON:		
		case GS_GAME_END:		
		{
								
									CC_ASSERT(wDataSize == sizeof(CMD_S_StatusPlay));
									if (wDataSize != sizeof(CMD_S_StatusPlay)) return false;

								
									CMD_S_StatusPlay * pStatusPlay = (CMD_S_StatusPlay *)pBuffer;


									return true;
		}
		}

		return false;
	}

	
	bool CGameClientDlg::OnSubGameStart(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_GameStart));
		if (wDataSize != sizeof(CMD_S_GameStart)) return false;

		CMD_S_GameStart pGameStart;
		memset(&pGameStart, 0, sizeof(CMD_S_GameStart));
		memcpy(&pGameStart, pBuffer, sizeof(CMD_S_GameStart));
		if (fruit_mainScene)
		{
			fruit_mainScene->setLabelType(pGameStart.cbTimeLeave, TimeType_fruit_PlaceJetton);
		}
		return true;
	}

	bool CGameClientDlg::OnSubGameFree(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_GameFree));
		if (wDataSize != sizeof(CMD_S_GameFree)) return false;

		CMD_S_GameFree pGameFree;

		memset(&pGameFree, 0, sizeof(CMD_S_GameFree));
		memcpy(&pGameFree, pBuffer, sizeof(CMD_S_GameFree));
		/*if (fruit_mainScene)
		{
			fruit_mainScene->setLabelType(pGameFree.cbTimeLeave, TimeType_fruit_Leisure);
		}*/
		return true;
	}

	bool CGameClientDlg::OnSubPlaceJetton(void * pBuffer, WORD wDataSize, bool bGameMes)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_PlaceJetton));
		if (wDataSize != sizeof(CMD_S_PlaceJetton)) return false;

		CMD_S_PlaceJetton pGameFree;

		memset(&pGameFree, 0, sizeof(CMD_S_PlaceJetton));
		memcpy(&pGameFree, pBuffer, sizeof(CMD_S_PlaceJetton));
		
		if (fruit_mainScene)
			fruit_mainScene->PlaceJettonFunction(pGameFree);
		return true;
	}

	bool CGameClientDlg::OnSubGameEnd(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_GameEnd));
		if (wDataSize != sizeof(CMD_S_GameEnd)) return false;

		CMD_S_GameEnd pGameEnd;
		memset(&pGameEnd, 0, sizeof(CMD_S_GameEnd));
		memcpy(&pGameEnd, pBuffer, sizeof(CMD_S_GameEnd));
		
		if (fruit_mainScene)
		{
			fruit_mainScene->setLabelType(pGameEnd.cbTimeLeave, TimeType_fruit_Start);
			fruit_mainScene->startMoveBlock(pGameEnd);
		}
		return true;
	}

	void CGameClientDlg::UpdateButtonContron()
	{

		return;
	}
	
	bool CGameClientDlg::OnSubUserApplyBanker(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_ApplyBanker));
		if (wDataSize != sizeof(CMD_S_ApplyBanker)) return false;

		CMD_S_ApplyBanker * pApplyBanker = (CMD_S_ApplyBanker *)pBuffer;

		
		if (fruit_mainScene)
		{
			fruit_mainScene->userApplyBanker(pApplyBanker->wApplyUser);
		}
		return true;
	}

	bool CGameClientDlg::OnSubUserCancelBanker(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_CancelBanker));
		if (wDataSize != sizeof(CMD_S_CancelBanker)) return false;

		CMD_S_CancelBanker * pCancelBanker = (CMD_S_CancelBanker *)pBuffer;
		//string strName = __String::createWithFormat("%d", pCancelBanker->wCancelUser)->getCString();

		/*if (fruit_mainScene)
			fruit_mainScene->removeBankerSub(pCancelBanker->wCancelUser);*/

		return true;
	}

	bool CGameClientDlg::OnSubChangeBanker(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_ChangeBanker));
		if (wDataSize != sizeof(CMD_S_ChangeBanker)) return false;

		CMD_S_ChangeBanker pGameEnd;
		memset(&pGameEnd, 0, sizeof(CMD_S_ChangeBanker));
		memcpy(&pGameEnd, pBuffer, sizeof(CMD_S_ChangeBanker));

		/*if (fruit_mainScene)
		{
			fruit_mainScene->subChangeBanker(pGameEnd);
			fruit_mainScene->removeBanker(pGameEnd.wBankerUser);
		}*/
		return true;
	}

	bool CGameClientDlg::OnSubGameRecord(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize%sizeof(tagServerGameRecord) == 0);
		if (wDataSize%sizeof(tagServerGameRecord) != 0) return false;
		WORD wRecordCount = wDataSize / sizeof(tagServerGameRecord);
		for (WORD wIndex = 0; wIndex < wRecordCount; wIndex++)
		{
			tagServerGameRecord * pServerGameRecord = (((tagServerGameRecord *)pBuffer) + wIndex);
			//***********这里显示路单信息需要修改**********
			//fruit_mainScene->addRecordList(*pServerGameRecord);
		}
		return true;
	}

	bool CGameClientDlg::OnSubPlaceJettonFail(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_PlaceJettonFail));
		if (wDataSize != sizeof(CMD_S_PlaceJettonFail)) return false;

		CMD_S_PlaceJettonFail * pPlaceJettonFail = (CMD_S_PlaceJettonFail *)pBuffer;

		if (pPlaceJettonFail->lJettonArea == 0XFF)
		{
			//PromptLayer::createObject(U2F("当前时间，停止下注").c_str());
			
			/*if (fruit_mainScene)
			{
				fruit_mainScene->hideContinueJetton();
			}*/
		}
		else if (pPlaceJettonFail->wPlaceUser == UserInfo::instance()->getSeatId())
		{
			//PromptLayer::createObject(U2F("下注失败").c_str());
		}
		return true;
	}

	bool CGameClientDlg::OnSubChangeGold(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_ChangeGold));
		if (wDataSize != sizeof(CMD_S_ChangeGold)) return false;
		CMD_S_ChangeGold pGameEnd;
		memset(&pGameEnd, 0, sizeof(CMD_S_ChangeGold));
		memcpy(&pGameEnd, pBuffer, sizeof(CMD_S_ChangeGold));

		if (fruit_mainScene)
		{
			fruit_mainScene->subChangeGold(pGameEnd);
		}
		return true;
	}

	bool CGameClientDlg::OnSubButtonLeft(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_ButtonLeft));
		if (wDataSize != sizeof(CMD_S_ButtonLeft)) return false;
		CMD_S_ButtonLeft pGameEnd;
		memset(&pGameEnd, 0, sizeof(CMD_S_ButtonLeft));
		memcpy(&pGameEnd, pBuffer, sizeof(CMD_S_ButtonLeft));

		if (fruit_mainScene)
		{
			fruit_mainScene->subButtonLeft(pGameEnd);
		}
		return true;
	}

	bool CGameClientDlg::OnSubCompare(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_Guess));
		if (wDataSize != sizeof(CMD_S_Guess)) return false;
		CMD_S_Guess pGameEnd;
		memset(&pGameEnd, 0, sizeof(CMD_S_Guess));
		memcpy(&pGameEnd, pBuffer, sizeof(CMD_S_Guess));

		if (fruit_mainScene)
		{
			fruit_mainScene->subCompare(pGameEnd);
		}
		return true;
	}
	bool CGameClientDlg::OnSubButtonRight(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_ButtonRight));
		if (wDataSize != sizeof(CMD_S_ButtonRight)) return false;
		CMD_S_ButtonRight pGameEnd;
		memset(&pGameEnd, 0, sizeof(CMD_S_ButtonRight));
		memcpy(&pGameEnd, pBuffer, sizeof(CMD_S_ButtonRight));

		if (fruit_mainScene)
		{
			fruit_mainScene->subButtonRight(pGameEnd);
		}
		return true;
	}

	void CGameClientDlg::SetBankerInfo(WORD wBanker, LONGLONG lScore)
	{
		m_wCurrentBanker = wBanker;
		m_lBankerScore = lScore;

	}

	void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
	{
		CC_ASSERT(cbViewIndex >= 1 && cbViewIndex <= AREA_COUNT);
		if (!(cbViewIndex >= 1 && cbViewIndex <= AREA_COUNT)) return;
		m_lUserJettonScore[cbViewIndex] = lJettonCount;
	}

	
	bool CGameClientDlg::OnRepeatPlaceJetton(void * pBuffer, WORD wDataSize)
	{
	CC_ASSERT(wDataSize == sizeof(CMD_S_RepeatBet));
	if (wDataSize != sizeof(CMD_S_RepeatBet)) return false;

	//CMD_S_RepeatBet * pQueryStorage = (CMD_S_RepeatBet *)pBuffer;

	CMD_S_RepeatBet pQueryStorage;
	memset(&pQueryStorage, 0, sizeof(CMD_S_RepeatBet));
	memcpy(&pQueryStorage, pBuffer, sizeof(CMD_S_RepeatBet));

	if (fruit_mainScene)
	{
		fruit_mainScene->userRepeatPlaceJetton(true, pQueryStorage);
	}

	return true;
	}

	void CGameClientDlg::ReSetBankCtrol(int nGameState)
	{



	}


	bool CGameClientDlg::OnEventGameClockKill(WORD wChairID)
	{
		return true;
	}

	bool CGameClientDlg::AllowBackGroundSound(bool bAllowSound)
	{
		return true;
	}
}
