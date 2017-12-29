/*
 * UpgradeLayer.cpp
 *
 *  Created on: 2014-10-15
 *      Author: win7
 */

#include "UpgradeLayer.h"
#include "hall/ui/MainLayer.h"
#include "sdk/util/QFileUtil.h"

#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

#define DOWNLOAD_FILE 				""
#define TAG 						"UpgradeLayer"

enum UI_UPGRADE {

	UPGRADE_PERCENT 	= 3971,
	UPGRADE_PROGRESS 	= 3973,
	UPGRADE_BJ 			= 3969,
	UPGRADE_LOAD 		= 323,
	UPGRADE_PRCESS_BJ 	= 7457390,
};

UpgradeLayer::UpgradeLayer() {

	_pathToSave = "";
	_showDownloadInfo = NULL;
}

UpgradeLayer::~UpgradeLayer() {

	AssetsManager * assetManager = getAssetManager();
	CC_SAFE_DELETE(assetManager);
}

bool UpgradeLayer::init()
{
	BaseScene::init();
	return true;
}

void UpgradeLayer::onEnter()
{

	BaseScene::onEnter();

	CCSize visibleSize = Director::getInstance()->getWinSize();

	initDownloadDir();

	getAssetManager()->setStoragePath(_pathToSave.c_str());

	vector < string > searchPaths = FileUtils::getInstance()->getSearchPaths();
	vector<string>::iterator iter = searchPaths.begin();
	searchPaths.insert(iter, _pathToSave);
	FileUtils::getInstance()->setSearchPaths(searchPaths);

	pRoot = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("version/version.json");
	this->addChild(pRoot);

	ImageView * pImage = static_cast<ImageView *>(pRoot->getChildByTag(UPGRADE_PRCESS_BJ));
	pImage->setVisible(false);

	_showDownloadInfo = static_cast<Text *>(pRoot->getChildByTag(UPGRADE_PERCENT));
	_showDownloadInfo->setText(U2F("版本检测中..."));

	LoadingBar * pProgress = static_cast<LoadingBar *>(pRoot->getChildByTag(UPGRADE_PROGRESS));
	pProgress->setPercent(0.0f);
	pProgress->setVisible(false);

	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/xiaoren/xiaoren.ExportJson");
	cocostudio::Armature * armature = cocostudio::Armature::create("xiaoren");
	armature->setVisible(false);
	armature->setZOrder(2);
	armature->setTag(UPGRADE_LOAD);
	pRoot->addChild(armature);

	NYInfo(TAG,"准备更新版本");
	getAssetManager()->update();
	NYInfo(TAG,"更新完成");
}

void UpgradeLayer::onExit() {
	this->removeAllChildrenWithCleanup(true);
	BaseScene::onExit();
	
}

void UpgradeLayer::onError(AssetsManager::ErrorCode errorCode)
{
	if (errorCode == AssetsManager::ErrorCode::NO_NEW_VERSION)
	{
		BaseScene * pLayer = new MainLayer();
		Director::getInstance()->replaceScene(BaseScene::createScene(pLayer));
		pLayer->release();

	} else if (errorCode == AssetsManager::ErrorCode::NETWORK) {
		_showDownloadInfo->setText("网络连接错误");
	} else if (errorCode == AssetsManager::ErrorCode::CREATE_FILE) {
		_showDownloadInfo->setText("创建文件错误");
	} else if(errorCode == AssetsManager::ErrorCode::UNCOMPRESS) {
		_showDownloadInfo->setText("未找到下载的apk，请重新下载");
	}
}

void UpgradeLayer::onProgress(int percent) {

	if (percent < 0) {
		return;
	}

	string strMemo = StringUtils::format("正在下载中...  %d%%", percent);
	_showDownloadInfo = static_cast<Text *>(pRoot->getChildByTag(UPGRADE_PERCENT));
	_showDownloadInfo->setText(strMemo);

	LoadingBar * pProgress = static_cast<LoadingBar *>(pRoot->getChildByTag(UPGRADE_PROGRESS));
	pProgress->setPercent((float) percent);
	pProgress->setVisible(true);

	ImageView * pImage = static_cast<ImageView *>(pRoot->getChildByTag(UPGRADE_PRCESS_BJ));
	pImage->setVisible(true);

	cocostudio::Armature * pArmature = static_cast<cocostudio::Armature *>(pRoot->getChildByTag(UPGRADE_LOAD));
	pArmature->setVisible(true);
	pArmature->getAnimation()->play("0");
	CCSize visibleSize = Director::getInstance()->getWinSize();
	pArmature->setPosition(ccp(visibleSize.width / 2 - 330+(percent*5), visibleSize.height / 2 + 40));

}


void UpgradeLayer::onSuccess() {

	_showDownloadInfo->setText("安装中..");

	string path = _pathToSave + "/" + QFileUtil::readXML("configure.xml","download_unzip_file_name");
	Util::upgrade(path);
	Director::getInstance()->end();

}

AssetsManager * UpgradeLayer::getAssetManager() {
	static AssetsManager * assetManager = NULL;
	if (!assetManager) {
		string downloadUrl = QFileUtil::readXML("configure.xml","download_url");;
		string downloadVersion = QFileUtil::readXML("configure.xml","version_url");
		assetManager = new AssetsManager(downloadUrl.c_str(), downloadVersion.c_str(), _pathToSave.c_str());

		assetManager->setDelegate(this);

		assetManager->setConnectionTimeout(10);
	}
	return assetManager;
}

void UpgradeLayer::initDownloadDir() {
	//if(Util::isExistSDcard()){
	//_pathToSave = "/mnt/sdcard/";
	//}else{
	_pathToSave = FileUtils::getInstance()->getWritablePath();
	//}
	_pathToSave += DOWNLOAD_FILE;

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	DIR *pDir = NULL;

	pDir = opendir(_pathToSave.c_str());
	if (!pDir) {
		mkdir(_pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
#else
	if ((GetFileAttributesA(_pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(_pathToSave.c_str(), 0);
	}
#endif

	NYInfo(TAG, "创建文件路径 = %s", _pathToSave.c_str());
}

void UpgradeLayer::reset(Ref * pSender) {

	NYInfo(TAG, "  恢复中... ");

	_showDownloadInfo->setString("");
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	string command = "rm -f";
	command += "\"" + _pathToSave + "\"";
	system(command.c_str());
#else
	std::string command = "rd /s /q";
	command += "\"" + _pathToSave + "\"";
	system(command.c_str());
#endif
	getAssetManager()->deleteVersion();
	initDownloadDir();
}

//void UpgradeLayer::upgrade(Ref * pSender) {
//
//	NYInfo(TAG, "  升级中... ");
//	getAssetManager()->setStoragePath(_pathToSave.c_str());
//
//	vector < string > searchPaths = FileUtils::getInstance()->getSearchPaths();
//	vector<string>::iterator iter = searchPaths.begin();
//	searchPaths.insert(iter, _pathToSave);
//	FileUtils::getInstance()->setSearchPaths(searchPaths);
//	getAssetManager()->update();
//	string version = getAssetManager()->getVersion();
//	NYInfo(TAG, "获得服务器版本 %s", version.c_str());
//}

