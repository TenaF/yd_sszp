//
//  MyPageView.cpp
//  hello2dx
//
//  Created by on 14-9-29.
//
//

#include "MyPageView.h"

#define TAG "MyPageView"

MyPageView::MyPageView()
{
}

bool MyPageView::onTouchBegan(Touch *touch, Event *unusedEvent)
{
	m_nCurPage = this->getCurPageIndex();
	PageView::onTouchBegan(touch, unusedEvent);
	return true;
}
void MyPageView::onTouchMoved(Touch *touch, Event *unusedEvent)
{
	PageView::onTouchMoved(touch,unusedEvent);
}
void MyPageView::onTouchEnded(Touch *touch, Event *unusedEvent)
{
	PageView::onTouchEnded(touch, unusedEvent);
	int nPageIndex = this->getCurPageIndex();

	if (m_nCurPage == nPageIndex)
	{
		//OpenGL坐标系位置
		Vec2 endVec = touch->getLocation();
		m_Listener->touchCheck(endVec);
	}
}