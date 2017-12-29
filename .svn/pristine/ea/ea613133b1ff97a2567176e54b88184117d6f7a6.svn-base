//
//  InputPassword.h
//  hello2dx
//
//  Created by on 15-03-11.
//

#ifndef __hello2dx__InputPassword_yxx__
#define __hello2dx__InputPassword_yxx__

#include "sdk/sdk.h"

namespace yxx
{
	class InputPasswordListener
	{
	public:
		virtual void ExecSitDownOperate(string strPassword){}
	};

	class InputPassword : public Layout, public EditBoxDelegate
	{
	public:
		InputPassword(InputPasswordListener* listener);
		void showInputPasswordWindow(int nSign);

		// 内部响应
		void onTouch(Ref* target, Widget::TouchEventType type);

		virtual void editBoxEditingDidBegin(EditBox * editBox);
		virtual void editBoxEditingDidEnd(EditBox * editBox);
		virtual void editBoxTextChanged(EditBox * editBox);
		virtual void editBoxReturn(EditBox * editBox);
	private:
		ImageView* m_pBg;
		InputPasswordListener* m_Listener;
		EventListenerTouchOneByOne *touchlist;
	};
}

#endif
