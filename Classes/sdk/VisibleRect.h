#ifndef __VISIBLERECT_H__
#define __VISIBLERECT_H__

#include "cocos2d.h"
USING_NS_CC;

class VisibleRect
{
public:
    static void lazyInit();
	static void lazyInit_1280();
	static void lazyInit_1280_Full();
    static cocos2d::CCRect getVisibleRect();

    static Vec2 left();
    static Vec2 right();
    static Vec2 top();
    static Vec2 bottom();
    static Vec2 center();
    static Vec2 leftTop();
    static Vec2 rightTop();
    static Vec2 leftBottom();
    static Vec2 rightBottom();
    static float getWidth() { return s_visibleRect.size.width; }
    static float getHeight() { return s_visibleRect.size.height; }
    
    static float getScale();
private:
    
    static cocos2d::CCRect s_visibleRect;
};

#endif /* __VISIBLERECT_H__ */
