//
//  LoginScene.h
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#ifndef __hello2dx__LoadingScene__
#define __hello2dx__LoadingScene__

#include "sdk/sdk.h"

#define LOADING_SCENE "loadingScene"

class LoadingScene : public LayerColor
{
	private:
		static LoadingScene * s_pInstance;

	public:

		static void createObject(const std::string& strMsg = "");
		static void  hideModal();

		LoadingScene();
		~LoadingScene();

		virtual bool init();
		virtual void onEnter();
		virtual void onExit();
	
		CREATE_FUNC(LoadingScene);
};

#endif /* defined(__hello2dx__LoadingScene__) */
