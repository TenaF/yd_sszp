//
//  ConnectGameScene.h
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#ifndef __hello2dx__ConnectGameScene__
#define __hello2dx__ConnectGameScene__

#include "sdk/scene/SceneBase.h"
#include "MyPageView.h"

class ConnectGameScene : public SceneBase,MyPageViewListener
{

public:
	Widget * pRoot;
	MyPageView * pScollView ;
//	Button * pRoom1 ;
//	Button * pRoom2 ;
	int m_nPageCount;
	int m_nCurPage;

public:

	ConnectGameScene();
	~ConnectGameScene();

    void initView();
    void onEnter();
    void onExit();

    void initRoomList(int roomType);
	void touchButton(Ref * obj ,TouchEventType eventType);
	//void touchScrollViewEvent(Ref * ref , ScrollviewEventType type);
	void  createRoomButton(Vec2 pos ,void * pServer,Layout * _parent);

	void memuCallback(Ref* target, Widget::TouchEventType type);
	void notifySuccess(Ref * obj);
	void notifyFail(Ref * obj);
	void notifyServerList(Ref * obj);
	//void notifyOutMatchCount(Ref * obj);

	void scheduleLogin(float t);
	void pageViewEvent(Ref* pSender, PageViewEventType type);
	void pageViewLEvent(Ref* pSender, PageView::EventType type);

	void touchCheck(Vec2 Pos);

	void outTime(float dt);
	void autoClose(float dt);
	void waitDistributeOutTime(float dt);
	void notifyWaitDistribute(Ref * obj);
};

#endif /* defined(__hello2dx__LogoScene__) */
