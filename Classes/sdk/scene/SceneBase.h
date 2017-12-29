//
//  SceneBase.h
//  hello2dx
//
//  Created by  on 14-7-4.
//
//

#ifndef __hello2dx__SceneBase__
#define __hello2dx__SceneBase__

#include "cocos2d.h"
#include "sdk/sdk.h"


class SceneBase : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene(SceneBase* layer);
     static cocos2d::Scene* createSceneWithPhysics(SceneBase* layer);
    SceneBase();
    ~SceneBase(){};

	PhysicsWorld* physics_world;
	void setPhysicsWorld(PhysicsWorld* physicsWorld){physics_world=physicsWorld;};
    // 重载此函数，初始化界面
    virtual void initView(){};
    // 生命周期
    void onEnter();
	void onExit();
    void cleanup();
    void addListener();
    	
protected:
    
    // 返回键的响应函数
    virtual void onBackPressed();
    // 设置背景
    virtual void setBg(const char* resPath);

private:
    //time_t last_time;

protected:
    //Layer * pExit ;

};


#endif /* defined(__hello2dx__SceneBase__) */
