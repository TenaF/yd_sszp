#include "GameClientDlg_sszp.h"
//////////////////////////////////////////////////////////////////////////
namespace sszp{

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
		sszp_mainScene = nullptr;
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
								  return OnSubGameRecord(pBuffer, wDataSize);
		}
		case SUB_S_PLACE_JETTON_FAIL:
		{
										return OnSubPlaceJettonFail(pBuffer, wDataSize);
		}
		case SUB_S_ADMIN_COMMDN:
		{
								   return OnSubAdminControl(pBuffer, wDataSize);
		}
			/*case SUB_S_SCORE_RESULT:
			{
			return OnSubScoreResult(pBuffer, wDataSize);
			}
			case SUB_S_ACCOUNT_RESULT:
			{
			return OnSubAccountResult(pBuffer, wDataSize);
			}
			case SUB_S_QUERY_STORAGE:
			{
			return OnSubQueryStorage(pBuffer, wDataSize);
			}*/
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
		case GS_PLACE_JETTON:
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
		if (sszp_mainScene)
		{
			sszp_mainScene->setLabelType(pGameStart.cbTimeLeave, TimeType_sszp_PlaceJetton, pGameStart.lBankerScore);
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
		if (sszp_mainScene)
		{
			sszp_mainScene->setLabelType(pGameFree.cbTimeLeave, TimeType_sszp_Leisure);
		}
		return true;
	}

	bool CGameClientDlg::OnSubPlaceJetton(void * pBuffer, WORD wDataSize, bool bGameMes)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_PlaceJetton));
		if (wDataSize != sizeof(CMD_S_PlaceJetton)) return false;

		CMD_S_PlaceJetton pGameFree;

		memset(&pGameFree, 0, sizeof(CMD_S_PlaceJetton));
		memcpy(&pGameFree, pBuffer, sizeof(CMD_S_PlaceJetton));

		if (sszp_mainScene)
		{
			sszp_mainScene->PlaceJettonFunction(pGameFree);
		}
		return true;
	}

	bool CGameClientDlg::OnSubGameEnd(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_GameEnd));
		if (wDataSize != sizeof(CMD_S_GameEnd)) return false;

		CMD_S_GameEnd pGameEnd;
		memset(&pGameEnd, 0, sizeof(CMD_S_GameEnd));
		memcpy(&pGameEnd, pBuffer, sizeof(CMD_S_GameEnd));

		if (sszp_mainScene)
		{
			sszp_mainScene->startMoveBlock(pGameEnd);
			sszp_mainScene->setLabelType(pGameEnd.cbTimeLeave, TimeType_sszp_Start);
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


		if (sszp_mainScene)
		{
			sszp_mainScene->userApplyBanker(pApplyBanker->wApplyUser);
		}
		return true;
	}

	bool CGameClientDlg::OnSubUserCancelBanker(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_CancelBanker));
		if (wDataSize != sizeof(CMD_S_CancelBanker)) return false;

		CMD_S_CancelBanker * pCancelBanker = (CMD_S_CancelBanker *)pBuffer;
		string strName = Util::getStringWithTChar(pCancelBanker->szCancelUser);

		if (sszp_mainScene)
			sszp_mainScene->removeBanker(strName);

		return true;
	}

	bool CGameClientDlg::OnSubChangeBanker(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_ChangeBanker));
		if (wDataSize != sizeof(CMD_S_ChangeBanker)) return false;

		CMD_S_ChangeBanker pGameEnd;
		memset(&pGameEnd, 0, sizeof(CMD_S_ChangeBanker));
		memcpy(&pGameEnd, pBuffer, sizeof(CMD_S_ChangeBanker));

		if (sszp_mainScene)
		{
			sszp_mainScene->subChangeBanker(pGameEnd);
			sszp_mainScene->removeBanker(pGameEnd.wBankerUser);
		}
		return true;
	}

	bool CGameClientDlg::OnSubGameRecord(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize%sizeof(tagServerGameRecord) == 0);
		if (wDataSize%sizeof(tagServerGameRecord) != 0) return false;
		if (sszp_mainScene)
		{
			sszp_mainScene->clearRecordList();
		}
		WORD wRecordCount = wDataSize / sizeof(tagServerGameRecord);
		for (WORD wIndex = 0; wIndex < wRecordCount; wIndex++)
		{
			tagServerGameRecord * pServerGameRecord = (((tagServerGameRecord *)pBuffer) + wIndex);
			sszp_mainScene->addRecordList(pServerGameRecord->cbAnimal, true);
		}
		return true;
	}

	bool CGameClientDlg::OnSubPlaceJettonFail(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_PlaceJettonFail));
		if (wDataSize != sizeof(CMD_S_PlaceJettonFail)) return false;

		CMD_S_PlaceJettonFail * pPlaceJettonFail = (CMD_S_PlaceJettonFail *)pBuffer;
		if (sszp_mainScene)
		{
			sszp_mainScene->hideContinueJetton();
		}
		switch (pPlaceJettonFail->enJErroe)
		{
		case 200:
			PromptLayer::createObject(U2F("请稍候，未到下注时间").c_str());
			return true;
			break;
		case 201:
			PromptLayer::createObject(U2F("金币不足，下注取消").c_str());
			return true;
			break;
		case 202:
			PromptLayer::createObject(U2F("当前区域，停止下注").c_str());
			return true;
			break;
		case 203:
			PromptLayer::createObject(U2F("下注失败").c_str());
			return true;
			break;
		case 204:
			PromptLayer::createObject(U2F("已达庄家赔付上限，无法下注").c_str());
			return true;
			break;
		case 205:
			PromptLayer::createObject(U2F("下注失败").c_str());
			return true;
			break;
		case 206:
			PromptLayer::createObject(U2F("下注失败").c_str());
			return true;
			break;
		default:
			break;
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

	bool CGameClientDlg::OnSubAdminControl(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_ControlReturns));
		if (wDataSize != sizeof(CMD_S_ControlReturns)) return false;
		return true;
	}

	/*bool CGameClientDlg::OnSubScoreResult(void * pBuffer, WORD wDataSize)
	{
	CC_ASSERT(wDataSize == sizeof(CMD_S_ScoreResult));
	if (wDataSize != sizeof(CMD_S_ScoreResult)) return false;
	CMD_S_ScoreResult * pResult = (CMD_S_ScoreResult*)pBuffer;

	return true;
	}*/

	/*bool CGameClientDlg::OnSubAccountResult(void * pBuffer, WORD wDataSize)
	{
	CC_ASSERT(wDataSize == sizeof(CMD_S_AccountResult));
	if (wDataSize != sizeof(CMD_S_AccountResult)) return false;

	CMD_S_AccountResult * pResult = (CMD_S_AccountResult*)pBuffer;


	return true;
	}*/


	/*bool CGameClientDlg::OnSubQueryStorage(void * pBuffer, WORD wDataSize)
	{
	CC_ASSERT(wDataSize == sizeof(CMD_S_QueryStorage));
	if (wDataSize != sizeof(CMD_S_QueryStorage)) return false;

	CMD_S_QueryStorage * pQueryStorage = (CMD_S_QueryStorage *)pBuffer;


	return true;
	}*/

	bool CGameClientDlg::OnRepeatPlaceJetton(void * pBuffer, WORD wDataSize)
	{
		CC_ASSERT(wDataSize == sizeof(CMD_S_RepeatBet));
		if (wDataSize != sizeof(CMD_S_RepeatBet)) return false;

		CMD_S_RepeatBet pQueryStorage;
		memset(&pQueryStorage, 0, sizeof(CMD_S_RepeatBet));
		memcpy(&pQueryStorage, pBuffer, sizeof(CMD_S_RepeatBet));

		if (sszp_mainScene)
		{
			bool isMyself = (pQueryStorage.wChairID == UserInfo::instance()->getSeatId()) ? true : false;
			sszp_mainScene->userRepeatPlaceJetton(isMyself, pQueryStorage);
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
