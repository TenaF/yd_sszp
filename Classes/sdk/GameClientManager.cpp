/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "GameClientManager.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include <curl/include/ios/curl/curl.h>
#include <curl/include/ios/curl/easy.h>
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <curl/include/android/curl/curl.h>
#include <curl/include/android/curl/easy.h>
#endif
#if WIN32
#include <curl/curl.h>
#endif


#include <stdio.h>
#include <vector>
#include <thread>
#include "sdk/log/NYLog.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#endif

#include "unzip/unzip.h"

using namespace cocos2d;
using namespace std;

NS_CC_EXT_BEGIN;

#define KEY_OF_VERSION   "current-version-code"
#define KEY_OF_DOWNLOADED_VERSION    "downloaded-version-code"
#define TEMP_PACKAGE_FILE_NAME    "cocos2dx-update-temp-package.zip"
#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

#define LOW_SPEED_LIMIT 1L
#define LOW_SPEED_TIME 5L

// Message type
#define GameClientManager_MESSAGE_UPDATE_SUCCEED                0
#define GameClientManager_MESSAGE_RECORD_DOWNLOADED_VERSION     1
#define GameClientManager_MESSAGE_PROGRESS                      2
#define GameClientManager_MESSAGE_ERROR                         3

#define TAG "GameClientManager"
// Some data struct for sending messages

struct ErrorMessage
{
	GameClientManager::ErrorCode code;
	GameClientManager* manager;
};

struct ProgressMessage
{
	int percent;
	GameClientManager* manager;
};

// Implementation of GameClientManager

GameClientManager::GameClientManager(const char* packageUrl/* =NULL */, const char* versionFileUrl/* =NULL */, const char* storagePath/* =NULL */)
: _storagePath(storagePath)
, _version("")
, _packageUrl(packageUrl)
, _versionFileUrl(versionFileUrl)
, _downloadedVersion("")
, _curl(nullptr)
, _connectionTimeout(0)
, _delegate(nullptr)
, _isDownloading(false)
, _shouldDeleteDelegateWhenExit(false)
{
	checkStoragePath();
}

GameClientManager::~GameClientManager()
{
	if (_shouldDeleteDelegateWhenExit)
	{
		delete _delegate;
	}
}

void GameClientManager::checkStoragePath()
{
	if (_storagePath.size() > 0 && _storagePath[_storagePath.size() - 1] != '/')
	{
		_storagePath.append("/");
	}
}

// Multiple key names
static std::string keyWithHash( const char* prefix, const std::string& url )
{
	char buf[256];
	sprintf(buf,"%s%zd",prefix,std::hash<std::string>()(url));
	return buf;
}

// hashed version
std::string GameClientManager::keyOfVersion() const
{
	return keyWithHash(KEY_OF_VERSION,_packageUrl);
}

// hashed version
std::string GameClientManager::keyOfDownloadedVersion() const
{
	return keyWithHash(KEY_OF_DOWNLOADED_VERSION,_packageUrl);
}

static size_t getVersionCode(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	string *version = (string*)userdata;
	version->append((char*)ptr, size * nmemb);

	return (size * nmemb);
}

bool GameClientManager::checkUpdate()
{
	//if (_versionFileUrl.size() == 0) return false;
	//NYInfo(TAG," checkUpdatee *****&&&&&& %s","");
	_curl = curl_easy_init();
	if (! _curl)
	{
		NYInfo(TAG,"can not init curl %s","");
		return false;
	}

	if(_versionFileUrl.size() > 0)
	{
	NYInfo(TAG," check _versionFileUrl upgrade ***** = %s",_versionFileUrl.c_str());
		_version.clear();

		CURLcode res;
		curl_easy_setopt(_curl, CURLOPT_URL, _versionFileUrl.c_str());
		curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, getVersionCode);
		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &_version);
		if (_connectionTimeout) curl_easy_setopt(_curl, CURLOPT_CONNECTTIMEOUT, _connectionTimeout);
		curl_easy_setopt(_curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_LIMIT, LOW_SPEED_LIMIT);
		curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_TIME, LOW_SPEED_TIME);
		res = curl_easy_perform(_curl);

		if (res != 0)
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this] {
						if (this->_delegate)
						this->_delegate->onError(ErrorCode::NETWORK);
					});
			NYInfo(TAG,"can not get version file content, error code is %d", res);
			curl_easy_cleanup(_curl);
			return false;
		}

		string recordedVersion = UserDefault::getInstance()->getStringForKey(keyOfVersion().c_str());

		if (recordedVersion == _version)
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this] {
						if (this->_delegate)
						this->_delegate->onError(ErrorCode::NO_NEW_VERSION);
					});
			NYInfo(TAG,"there is not new version");
			// Set resource search path.
			setSearchPath();
			return false;
		}


		NYInfo(TAG,"there is a new version: %s", _version.c_str());
	}
	return true;
}

void GameClientManager::downloadAndUncompress()
{
	//NYInfo(TAG, "  update4444... ");
	do
	{
		// if (_downloadedVersion != _version)
		//{
		if (! downLoad()) break;

		Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this] {
					UserDefault::getInstance()->setStringForKey(this->keyOfDownloadedVersion().c_str(),
							this->_version.c_str());
					UserDefault::getInstance()->flush();
				});
		//}

		// Uncompress zip file.
		if (! uncompress())
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this] {
						if (this->_delegate)
						this->_delegate->onError(ErrorCode::UNCOMPRESS);
					});
			break;
		}

		Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this] {

					// Record new version code.
					UserDefault::getInstance()->setStringForKey(this->keyOfVersion().c_str(), this->_version.c_str());

					// Unrecord downloaded version code.
					UserDefault::getInstance()->setStringForKey(this->keyOfDownloadedVersion().c_str(), "");
					UserDefault::getInstance()->flush();

					// Set resource search path.
					this->setSearchPath();

					// Delete unloaded zip file.
					string zipfileName = this->_storagePath + TEMP_PACKAGE_FILE_NAME;
					if (remove(zipfileName.c_str()) != 0)
					{
						NYInfo(TAG,"can not remove downloaded zip file %s", zipfileName.c_str());
					}

					if (this->_delegate) this->_delegate->onSuccess();
				});

	}while (0);

	//NYInfo(TAG, "  update5555... ");
	_isDownloading = false;
}

void GameClientManager::update()
{
	NYInfo(TAG, "  update111... ");
	if (_isDownloading) return;

	_isDownloading = true;

	// 1. Urls of package and version should be valid;
	// 2. Package should be a zip file.
	if (_packageUrl.size() == 0 || std::string::npos == _packageUrl.find(".zip"))
	{
		NYInfo(TAG,"no version file url, or no package url, or the package is not a zip file");
		_isDownloading = false;
		return;
	}

	//NYInfo(TAG, "  update222... ");
	// Check if there is a new version.
	if (! checkUpdate())
	{
		_isDownloading = false;
		return;
	}

	mFileLenth = getDownloadFileLenth(_packageUrl.c_str());
	if (mFileLenth <= 0)
	{
		return;
	}

	// Is package already downloaded?
	//_downloadedVersion = UserDefault::getInstance()->getStringForKey(keyOfDownloadedVersion().c_str());
	//NYInfo(TAG, "  update333... ");
	auto t = std::thread(&GameClientManager::downloadAndUncompress, this);
	t.detach();
}

bool GameClientManager::uncompress()
{
	// Open the zip file
	string outFileName = _storagePath + TEMP_PACKAGE_FILE_NAME;
	FILE *fp = fopen(outFileName.c_str(), "r");
	if (! fp)
	{
		NYInfo(TAG," read file is not exits");
	}
	NYInfo(TAG,"read file is exits");
	fclose(fp);

	unzFile zipfile = unzOpen(outFileName.c_str());
	if (! zipfile)
	{
		NYInfo(TAG,"can not open downloaded zip file %s", outFileName.c_str());
		return false;
	}

	// Get info about the zip file
	unz_global_info global_info;
	if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
	{
		NYInfo(TAG,"can not read file global info of %s", outFileName.c_str());
		unzClose(zipfile);
		return false;
	}

	// Buffer to hold data read from the zip file
	char readBuffer[BUFFER_SIZE];

	NYInfo(TAG,"start uncompressing");

	// Loop to extract all files.
	uLong i;
	for (i = 0; i < global_info.number_entry; ++i)
	{
		// Get info about current file.
		unz_file_info fileInfo;
		char fileName[MAX_FILENAME];
		if (unzGetCurrentFileInfo(zipfile,
						&fileInfo,
						fileName,
						MAX_FILENAME,
						NULL,
						0,
						NULL,
						0) != UNZ_OK)
		{
			NYInfo(TAG,"can not read file info");
			unzClose(zipfile);
			return false;
		}

		const string fullPath = _storagePath + fileName;

		// Check if this entry is a directory or a file.
		const size_t filenameLength = strlen(fileName);
		if (fileName[filenameLength-1] == '/')
		{
			// Entry is a direcotry, so create it.
			// If the directory exists, it will failed scilently.
			if (!createDirectory(fullPath.c_str()))
			{
				NYInfo(TAG,"can not create directory %s", fullPath.c_str());
				unzClose(zipfile);
				return false;
			}
		}
		else
		{
			//There are not directory entry in some case.
			//So we need to test whether the file directory exists when uncompressing file entry
			//, if does not exist then create directory
			const string fileNameStr(fileName);

			size_t startIndex=0;

			size_t index=fileNameStr.find("/",startIndex);

			while(index != std::string::npos)
			{
				const string dir=_storagePath+fileNameStr.substr(0,index);

				FILE *out = fopen(dir.c_str(), "r");

				if(!out)
				{
					if (!createDirectory(dir.c_str()))
					{
						NYInfo(TAG,"can not create directory %s", dir.c_str());
						unzClose(zipfile);
						return false;
					}
					else
					{
						NYInfo(TAG,"create directory %s",dir.c_str());
					}
				}
				else
				{
					fclose(out);
				}

				startIndex=index+1;

				index=fileNameStr.find("/",startIndex);

			}

			// Entry is a file, so extract it.

			// Open current file.
			if (unzOpenCurrentFile(zipfile) != UNZ_OK)
			{
				NYInfo(TAG,"can not open file %s", fileName);
				unzClose(zipfile);
				return false;
			}

			// Create a file to store current file.
			FILE *out = fopen(fullPath.c_str(), "wb+");
			if (! out)
			{
				NYInfo(TAG,"can not open destination file %s", fullPath.c_str());
				unzCloseCurrentFile(zipfile);
				unzClose(zipfile);
				return false;
			}

			// Write current file content to destinate file.
			int error = UNZ_OK;
			do
			{
				error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
				if (error < 0)
				{
					NYInfo(TAG,"can not read zip file %s, error code is %d", fileName, error);
					unzCloseCurrentFile(zipfile);
					unzClose(zipfile);
					return false;
				}

				if (error > 0)
				{
					fwrite(readBuffer, error, 1, out);
				}
			}while(error > 0);

			fclose(out);
		}

		unzCloseCurrentFile(zipfile);

		// Goto next entry listed in the zip file.
		if ((i+1) < global_info.number_entry)
		{
			if (unzGoToNextFile(zipfile) != UNZ_OK)
			{
				NYInfo(TAG,"can not read next file");
				unzClose(zipfile);
				return false;
			}
		}
	}

	NYInfo(TAG,"end uncompressing");
	unzClose(zipfile);

	return true;
}

/*
 * Create a direcotry is platform depended.
 */
bool GameClientManager::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	mode_t processMask = umask(0);
	int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
	umask(processMask);
	if (ret != 0 && (errno != EEXIST))
	{
		return false;
	}

	return true;
#else
	BOOL ret = CreateDirectoryA(path, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
	return true;
#endif
}

void GameClientManager::setSearchPath()
{
	vector<string> searchPaths = FileUtils::getInstance()->getSearchPaths();
	vector<string>::iterator iter = searchPaths.begin();
	searchPaths.insert(iter, _storagePath);
	FileUtils::getInstance()->setSearchPaths(searchPaths);
}

static size_t downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	FILE *fp = (FILE*)userdata;
	size_t written = fwrite(ptr, size, nmemb, fp);
	NYInfo(TAG," file size = %d",written);
	return written;
}
/* this is how the CURLOPT_XFERINFOFUNCTION callback works */
static int xferinfo(void *p,
	curl_off_t dltotal, curl_off_t dlnow,
	curl_off_t ultotal, curl_off_t ulnow)
{
// 	struct myprogress *myp = (struct myprogress *)p;
// 	CURL *curl = myp->curl;
// 	double curtime = 0;
// 
// 	curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &curtime);
// 
// 	/* under certain circumstances it may be desirable for certain functionality
// 	to only run every N seconds, in order to do this the transaction time can
// 	be used */
// 	if ((curtime - myp->lastruntime) >= MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL) {
// 		myp->lastruntime = curtime;
// 		fprintf(stderr, "TOTAL TIME: %f \r\n", curtime);
// 	}
// 
	fprintf(stderr, "UP: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T
		"  DOWN: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T
		"\r\n",
		ulnow, ultotal, dlnow, dltotal);
// 
// 	if (dlnow > STOP_DOWNLOAD_AFTER_THIS_MANY_BYTES)
// 		return 1;
	return 0;
}

int GameClientManagerProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
	static int percent = 0;
	int tmp = (int)(nowDownloaded / totalToDownload * 100);

	if (percent != tmp)
	{
		percent = tmp;
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
					auto manager = static_cast<GameClientManager*>(ptr);
					if (manager->_delegate)
					manager->_delegate->onProgress(percent);
				});

		NYInfo(TAG,"downloading... %d%%", percent);
	}

	return 0;
}

bool GameClientManager::downLoad()
{
	// Create a file to save package.
	const string outFileName = _storagePath + TEMP_PACKAGE_FILE_NAME;
	FILE *fp = fopen(outFileName.c_str(), "wb+");
	if (! fp)
	{
		createStoragePath();
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this] {
					if (this->_delegate)
					this->_delegate->onError(ErrorCode::CREATE_FILE);
				});
		NYInfo(TAG,"can not create file %s", outFileName.c_str());
		return false;
	}
	NYInfo(TAG," create file %s", outFileName.c_str());
	// Download pacakge
	CURLcode res;
	curl_easy_setopt(_curl, CURLOPT_URL, _packageUrl.c_str());
	curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, downLoadPackage);
	curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, getProgressValue); //下载进度回调方法
	curl_easy_setopt(_curl, CURLOPT_PROGRESSDATA, this); // 传入本类对象
	curl_easy_setopt(_curl, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_LIMIT, LOW_SPEED_LIMIT);
	curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_TIME, LOW_SPEED_TIME);
	curl_easy_setopt(_curl, CURLOPT_POST, false);

	res = curl_easy_perform(_curl);
	curl_easy_cleanup(_curl);
	if (res != 0)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this] {
					if (this->_delegate)
					this->_delegate->onError(ErrorCode::NETWORK);
				});
		NYInfo(TAG,"error when download package %d",res);
		fclose(fp);
		return false;
	}
	double downLength = 0.0;
	curl_easy_getinfo(_curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downLength);
	NYInfo(TAG,"succeed downloading package %s", _packageUrl.c_str());

	fclose(fp);
	return true;
}
double downloadFileLenth = 0;


double localFileLenth = 0;

long GameClientManager::getDownloadFileLenth(const char *url){
	//long downloadFileLenth = 0;
	_curl = curl_easy_init();
	curl_easy_setopt(_curl, CURLOPT_URL, url);
	curl_easy_setopt(_curl, CURLOPT_HEADER, 1);    //只需要header头
	curl_easy_setopt(_curl, CURLOPT_NOBODY, 1);    //不需要body
	if (curl_easy_perform(_curl) == CURLE_OK) {
		curl_easy_getinfo(_curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &mFileLenth);
	}
	else {
		mFileLenth = -1;
	}
	return mFileLenth;
}

int GameClientManager::getProgressValue(const char* flag, double dt, double dn, double ult, double uln){

	GameClientManager* curlDown = (GameClientManager*)flag;
	if (!curlDown || curlDown->mFileLenth == 0 || dn == 0) return 0;

	double showTotal, showNow;
	if (downloadFileLenth == 0){
		downloadFileLenth = curlDown->mFileLenth;
	}
	showTotal = downloadFileLenth;
	if (localFileLenth == 0){
		localFileLenth = 1;// getLocalFileLenth(filePath);
	}
	showNow = localFileLenth + dn;
	//然后就可以调用你自己的进度显示函数了, 这里假设已经有一个进度函数, 那么只需要传递当前下载值和总下载值即可.
	NYInfo(TAG,"showNow:%lf, showTotal:%lf", showNow, showTotal);
	//showProgressValue(showNow, showTotal);
}

const char* GameClientManager::getPackageUrl() const
{
	return _packageUrl.c_str();
}

void GameClientManager::setPackageUrl(const char *packageUrl)
{
	_packageUrl = packageUrl;
}

const char* GameClientManager::getStoragePath() const
{
	return _storagePath.c_str();
}

void GameClientManager::setStoragePath(const char *storagePath)
{
	_storagePath = storagePath;
	checkStoragePath();
	createDirectory(storagePath);
// 	//const string outFileName = _storagePath + TEMP_PACKAGE_FILE_NAME;
// 	FILE *fp = fopen(_storagePath.c_str(), "wb+");
// 	if (!fp)
// 	{
// 		createStoragePath();
// 		return;
// 	}
// 	fclose(fp);
}

const char* GameClientManager::getVersionFileUrl() const
{
	return _versionFileUrl.c_str();
}

void GameClientManager::setVersionFileUrl(const char *versionFileUrl)
{
	_versionFileUrl = versionFileUrl;
}

string GameClientManager::getVersion()
{
	return UserDefault::getInstance()->getStringForKey(keyOfVersion().c_str());
}

void GameClientManager::deleteVersion()
{
	UserDefault::getInstance()->setStringForKey(keyOfVersion().c_str(), "");
}


void GameClientManager::setDelegate(GameClientManagerDelegateProtocol *delegate)
{
	_delegate = delegate;
}

void GameClientManager::setConnectionTimeout(unsigned int timeout)
{
	_connectionTimeout = timeout;
}

unsigned int GameClientManager::getConnectionTimeout()
{
	return _connectionTimeout;
}

GameClientManager* GameClientManager::create(const char* packageUrl, const char* versionFileUrl, const char* storagePath, ErrorCallback errorCallback, ProgressCallback progressCallback, SuccessCallback successCallback )
{
	class DelegateProtocolImpl : public GameClientManagerDelegateProtocol
	{
		public :
		DelegateProtocolImpl(ErrorCallback aErrorCallback, ProgressCallback aProgressCallback, SuccessCallback aSuccessCallback)
		: errorCallback(aErrorCallback), progressCallback(aProgressCallback), successCallback(aSuccessCallback)
		{}

		virtual void onError(GameClientManager::ErrorCode errorCode) {errorCallback(int(errorCode));}
		virtual void onProgress(int percent) {progressCallback(percent);}
		virtual void onSuccess() {successCallback();}

		private :
		ErrorCallback errorCallback;
		ProgressCallback progressCallback;
		SuccessCallback successCallback;
	};

	auto* manager = new GameClientManager(packageUrl,versionFileUrl,storagePath);
	auto* delegate = new DelegateProtocolImpl(errorCallback,progressCallback,successCallback);
	manager->setDelegate(delegate);
	manager->_shouldDeleteDelegateWhenExit = true;
	manager->autorelease();
	return manager;
}

void GameClientManager::createStoragePath()
{
	// Remove downloaded files
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	DIR *dir = NULL;

	dir = opendir (_storagePath.c_str());
	if (!dir)
	{
		mkdir(_storagePath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
#else    
	if ((GetFileAttributesA(_storagePath.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(_storagePath.c_str(), 0);
	}
#endif
}

void GameClientManager::destroyStoragePath()
{
	// Delete recorded version codes.
	deleteVersion();

	// Remove downloaded files
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	string command = "rm -r ";
	// Path may include space.
	command += "\"" + _storagePath + "\"";
	system(command.c_str());
#else
	string command = "rd /s /q ";
	// Path may include space.
	command += "\"" + _storagePath + "\"";
	system(command.c_str());
#endif
}

NS_CC_EXT_END;
