//
//  LoginLayer.h
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#ifndef __hello2dx__LoginScene__
#define __hello2dx__LoginScene__

#include "sdk/scene/BaseScene.h"
//#include "sdk/GameClientManager.h"
#include "MyPageView.h"

class MainLayer : public BaseScene,MyPageViewListener
{
public:
	MainLayer();
	~MainLayer();

    void initView();
    void onEnter();
	void onExit();

	void touchButton(Ref * obj ,TouchEventType eventType);
	void touchHeadImage(Ref * obj, TouchEventType eventType);
	void connectNetSucceed(Ref * obj);
	//观察者通知
	void notifySuccess(Ref * object);
	//回调通知的接口
	void notifyFail(Ref * object);

	void downloadZip(Ref * obj ,TouchEventType eventType);
	void openApp(Ref * obj);
	void downloadFile(Ref * obj);
	void clickGame(Ref * obj);

	void initLoad(Object* pNode);
	void upgrade(Ref * pSender);
	void reset(Ref * pSender);

	void onUpgrade(int tag);

	virtual void onError(AssetsManager::ErrorCode errorCode);
	virtual void onProgress(int percent);
	virtual void onSuccess();

	void initDownloadDir();

	//GameClientManager * getAssetManager(std::string versionUrl , std::string url);

	//回调通知的接口
	void notifyQuestFail(Ref * object);
	void onSchedule(float t);
	void notifyMessage(Ref * object);
	void scheduleNotice(float t);
	void onRefreshPage(float f);
	void setNotice(string strNotice);
	void updateUserInfo();

	void pageViewEvent(Ref* pSender, PageViewEventType type);
	void MenuSelect(Ref* obj);

	void touchCheck(Vec2 Pos);

private:
	Widget * pRoot;

	std::map<int,std::string> download_map;
	std::map<int,std::string> open_map;

	Label * _showDownloadInfo;
	ProgressTimer* ct;
	string _pathToSave;
	string saveApk ;

	MyPageView * pView;
	bool menuStat ;

	Layer * pTrumpet;
	Layer * pSetting;
	Layer * pBank;
	//Layer * pLayer;

	Node * m_loadingParent;
	string m_strCurSelType;

	float beginTouchX ;
	float beginTouchY ;

	int m_nPageCount;

	CCMenuItemImage* m_TabGameType[4];
};

#endif /* defined(__hello2dx__LoginScene__) */
