/*
 * ConfirmLayer.h
 *
 *  Created on: 2014-11-5
 *      Author: win7
 */

#ifndef ConfirmLayer_H_
#define ConfirmLayer_H_

#include "sdk/sdk.h"
//#include "MainLayer.h"

enum UI_EXIT {
	EXIT_CONFIRM_BTN = 1,			//
	EXIT_CANEL_BTN = 2,			//
	EXIT_CLOSE = 3,

};
class ConfirmLayer: public LayerColor {
	public:
		ConfirmLayer();
		virtual ~ConfirmLayer();

		static void createObject(Object* parent , int tag);

		bool init();
		void onEnter();
		void onExit();
		void hideModal();
		void touchButton(Ref * obj ,TouchEventType type);

		CREATE_FUNC(ConfirmLayer)
		;
	public:
		static ConfirmLayer * instance;

		Object * parent;
		int buttonTag;
};

#endif /* ConfirmLayer_H_ */
