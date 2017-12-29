/*
 * ExitLayer.h
 *
 *  Created on: 2014-11-5
 *      Author: win7
 */

#ifndef EXITLAYER_H_
#define EXITLAYER_H_

#include "sdk/sdk.h"
//#include "MainLayer.h"

enum UI_EXIT {
	EXIT_CONFIRM_BTN = 1,			//
	EXIT_CANEL_BTN = 2,			//
	EXIT_CLOSE = 3,

};
class ExitLayer: public LayerColor {
	public:
		ExitLayer();
		virtual ~ExitLayer();

		static void createObject();

		bool init();
		void onEnter();
		void onExit();
		void hideModal();
		void touchButton(Ref * obj ,TouchEventType type);

		CREATE_FUNC(ExitLayer)
		;
	public:
		static ExitLayer * instance;
};

#endif /* EXITLAYER_H_ */
