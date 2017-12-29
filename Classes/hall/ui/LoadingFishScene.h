//
//  LoadingFishScene.h
//  hello2dx
//
//  Created by on 15-11-7.
//
//

#ifndef __hello2dx__LoadingFishScene__
#define __hello2dx__LoadingFishScene__

#include "sdk/sdk.h"

class LoadingFishScene : public LayerColor
{
	private:
		static LoadingFishScene * s_pInstance;

		Layout* m_Layout;
		LoadingBar * m_pProgress;
		ImageView* m_LoadState;
		ImageView* m_spBlock;
		int m_LoadCount;
		ImageView* progessbj;
	public:
		static LoadingFishScene* instance();
		static void createObject();
		static void  hideModal();

		LoadingFishScene();
		~LoadingFishScene();

		virtual bool init();
		virtual void onEnter();
		virtual void onExit();

		void updateProgess(float dt);
		void logintip(float dt);
		void close(float dt);

		void normalclose(Ref * obj);

		void setProgress(int nPer);
	
		CREATE_FUNC(LoadingFishScene);
};

#endif /* defined(__hello2dx__LoadingScene__) */
