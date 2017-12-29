#include "VisibleRect.h"
#include "sdk/log/NYLog.h"

cocos2d::CCRect VisibleRect::s_visibleRect;

#define TAG "VisibleRect"

static float s_scale = 0;
float VisibleRect::getScale()
{
    return s_scale;
}

void VisibleRect::lazyInit()
{
	GLView* glView = Director::getInstance()->getOpenGLView();

	s_scale = 1;
	float width = glView->getFrameSize().width;
	float height = glView->getFrameSize().height;
	// ipad
	if ((width == 1024 || width == 2048) && (height == 768 || height == 1536))
	{
		s_scale = 1.2;
	}

	if (width * 9 == height * 16)
	{
		glView->setDesignResolutionSize(854 , 480 , ResolutionPolicy::FIXED_WIDTH);

	} else if (width * 2 == height * 3)
	{
		glView->setDesignResolutionSize(854 , 480 , ResolutionPolicy::FIXED_HEIGHT);

	} else
	{
		glView->setDesignResolutionSize(854 , 480 , ResolutionPolicy::EXACT_FIT);
	}

	// 翻转需要这个支持
	Director::getInstance()->setProjection(Director::Projection::_2D);

	s_visibleRect.origin = glView->getVisibleOrigin();
	s_visibleRect.size = glView->getVisibleSize();
	
}

void VisibleRect::lazyInit_1280()
{
	
	GLView* glView = Director::getInstance()->getOpenGLView();

	//设计分辨率大小 适配规则
	glView->setDesignResolutionSize(1366, 768, ResolutionPolicy::EXACT_FIT);

	//翻转需要这个支持
	Director::getInstance()->setProjection(Director::Projection::_2D);

	s_visibleRect.origin = glView->getVisibleOrigin();
	s_visibleRect.size = glView->getVisibleSize();
	
}

void VisibleRect::lazyInit_1280_Full()
{

	GLView* glView = Director::getInstance()->getOpenGLView();

	s_scale = 1;
	float width = glView->getFrameSize().width;
	float height = glView->getFrameSize().height;
	// ipad
	if ((width == 1024 || width == 2048) && (height == 768 || height == 1536))
	{
		s_scale = 1.2;
	}
	
	glView->setDesignResolutionSize(1280 , 800 , ResolutionPolicy::EXACT_FIT);

	// 翻转需要这个支持
	Director::getInstance()->setProjection(Director::Projection::_2D);

	s_visibleRect.origin = glView->getVisibleOrigin();
	s_visibleRect.size = glView->getVisibleSize();
	
}

cocos2d::CCRect VisibleRect::getVisibleRect()
{
    return cocos2d::CCRect(s_visibleRect.origin.x, s_visibleRect.origin.y, s_visibleRect.size.width, s_visibleRect.size.height);
}

Vec2 VisibleRect::left()
{
    return Vec2(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Vec2 VisibleRect::right()
{
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Vec2 VisibleRect::top()
{
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Vec2 VisibleRect::bottom()
{
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y);
}

Vec2 VisibleRect::center()
{
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Vec2 VisibleRect::leftTop()
{
    return Vec2(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Vec2 VisibleRect::rightTop()
{
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Vec2 VisibleRect::leftBottom()
{
    return s_visibleRect.origin;
}

Vec2 VisibleRect::rightBottom()
{
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y);
}
