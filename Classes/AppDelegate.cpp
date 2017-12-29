#include "AppDelegate.h"
#include "sdk/controller/MainController.h"
#include "sdk/util/QFileUtil.h"

#include "sdk/sdk.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
	//发送起立命令
	SendMsg::sendStandUpReq(NetworkManager::FLAG_GAME, UserInfo::instance()->getTableId(), UserInfo::instance()->getSeatId(), 1, 0);
	//销毁HTTP网络实例
	cocos2d::network::HttpClient::getInstance()->destroyInstance();
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
	if(!glview) {
		//string gameName = QFileUtil::readXML("configure.xml", "title");//读取 大厅 文字
		glview = GLView::create("test-game");
		glview->setFrameSize(1366, 768);
		director->setOpenGLView(glview);
	}

	//设计分辨率
	VisibleRect::lazyInit_1280();
	//打开显示FPS 信息
	director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);

	MainController::getInstance()->start();
	
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {

	//HOME键操作逻辑
	MainController::getInstance()->gamesWillEnterBackground();
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {

    Director::getInstance()->startAnimation();
	MainController::getInstance()->gamesWillEnterForeground();
    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

}
