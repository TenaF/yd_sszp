//
//  CWebViewImp.h
//  BaseGameHall
//
//  Created by mac on 14-3-31.
//  Copyright (c) 2014年 mac. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CoreLocation/CoreLocation.h>
#import <UIKit/UIKit.h>

class CWebView;
@interface CWebViewImp : UIView<UIWebViewDelegate,UIAlertViewDelegate>
{
	CWebView* m_pWebControl;        //!<基于cocos2d-x层指针
	UIView* m_pView;//!<视图指针
	UIWebView* m_pWebView;//!<基于OC网页视图指针
	UIImageView* background;
	UIButton * bgyinying;//阴影
	CGRect bgRect;
	UIImageView * title;
	UIButton * closeButton;
	UIImageView* buttonClose;
	UIImageView * activitiesBg;

	NSString *newAgent;
	NSString *m_webViewDefault;

	//  ImageButton *     _imageButton;

	//UIToolBar*          m_pToolBar;         //!<工具栏指针
	UIActivityIndicatorView* m_activity;

}
-( void ) setWebView:(CWebView*) pWebControl;
-(void) closeBtn1:(id)sender;

-(BOOL) openUrl:(const char*)urlString pWebNumber:(int)_pWebNumber;
-(BOOL) closeWebView;

@end
