//
//  LoginLayer.h
//  hello2dx
//
//  Created by on 14-7-4.
//
//

#ifndef __hello2dx__LoadingLayer__
#define __hello2dx__LoadingLayer__

#include "sdk/sdk.h"

class LoadingLayer : public LayerColor
{
	private:
		static LoadingLayer * s_pInstance;

	public:

		static void createObject();
		static void  hideModal();

		LoadingLayer();
		~LoadingLayer();

		virtual bool init();
		virtual void onEnter();
		virtual void onExit();

	
		CREATE_FUNC(LoadingLayer);
};

#endif /* defined(__hello2dx__LoadingLayer__) */
