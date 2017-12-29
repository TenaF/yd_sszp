//
//  MyPageView.h
//  hello2dx
//
//  Created by on 14-9-29.
//
//  底分

#ifndef __hello2dx__MyPageView__
#define __hello2dx__MyPageView__

#include "sdk/sdk.h"

class MyPageViewListener
{
public:
	virtual void touchCheck(Vec2 Pos){}
};

class MyPageView : public PageView
{
public:
    MyPageView();

    // 内部响应
	virtual bool onTouchBegan(Touch *touch, Event *unusedEvent);
    virtual void onTouchMoved(Touch *touch, Event *unusedEvent);
    virtual void onTouchEnded(Touch *touch, Event *unusedEvent);

	void setListener(MyPageViewListener* pListeren){m_Listener = pListeren;}
	CREATE_FUNC(MyPageView);
private:
	int m_nCurPage;
	MyPageViewListener* m_Listener;
};

#endif /* defined(__hello2dx__MyPageView__) */
