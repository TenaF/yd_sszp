/*
 * UpgradeLayer.h
 *
 *  Created on: 2014-10-15
 *      Author: win7
 */

#ifndef UpgradeLayer_H_
#define UpgradeLayer_H_

#include "sdk/sdk.h"

class UpgradeLayer: public BaseScene, public cocos2d::extension::AssetsManagerDelegateProtocol {

	public:
		UpgradeLayer();
		virtual ~UpgradeLayer();

		virtual bool init();
		void onEnter();
		//void upgrade(Ref * pSender);
		void onExit();

		void reset(Ref * pSender);

		virtual void onError(AssetsManager::ErrorCode errorCode);
		virtual void onProgress(int percent);
		virtual void onSuccess();

		void initDownloadDir();

		AssetsManager * getAssetManager();

		CREATE_FUNC(UpgradeLayer);

	private:
		Text * _showDownloadInfo;
		string _pathToSave;

	public:
		Widget * pRoot;
};

#endif /* UpgradeLayer_H_ */
