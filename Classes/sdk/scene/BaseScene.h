//
//  BaseScene.h
//  hello2dx
//
//  Created by  on 14-7-4.
//
//

#ifndef __hello2dx__BaseScene__
#define __hello2dx__BaseScene__

#include "cocos2d.h"
#include "sdk/sdk.h"


class BaseScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene(BaseScene* layer);
    
    BaseScene();
    ~BaseScene(){};

    // 重载此函数，初始化界面
    virtual void initView(){};
    // 生命周期
    void onEnter();
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


#endif /* defined(__hello2dx__BaseScene__) */
