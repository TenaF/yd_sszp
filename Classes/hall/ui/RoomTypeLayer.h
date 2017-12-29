//
//  RoomTypeLayer.h
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#ifndef __hello2dx__RoomTypeLayer__
#define __hello2dx__RoomTypeLayer__

#include "sdk/scene/BaseScene.h"


class RoomTypeLayer : public BaseScene
{
public:
	Widget * pRoot;
	ui::ScrollView* pView;
	int m_kindID ;
	Node * m_loadingParent;

public:

	RoomTypeLayer(int kindID);
	~RoomTypeLayer();

    void initView();
    void onEnter();
    void onExit();

	void touchButton(Ref * obj ,TouchEventType eventType);

	void initRoomList();

	void notifySuccess(Ref * obj);
	void notifyServerList(Ref * obj);

	//显示loading
	void showLoading(Node * _parent);
	void hiddenLoading();

	void onHiddenLoading(float f);
};

#endif /* defined(__hello2dx__LogoScene__) */
