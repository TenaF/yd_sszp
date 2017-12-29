//
//  CWebViewImp.m
//  BaseGameHall
//
//  Created by mac on 14-3-31.
//  Copyright (c) 2014Âπ¥ mac. All rights reserved.
//
//#import "CommonData.h"
#import "WebViewImp.h"
#import "WebView.h"
#import "CCEAGLView.h"
@implementation CWebViewImp

-(void)dealloc{
	[m_pWebView setDelegate:nil];
    [m_pWebView release];
    [m_pView release];
    [super dealloc];
}

-(void) setWebView:(CWebView*) pWebControl
{
    m_pWebControl = pWebControl;
}

-(BOOL) openUrl:(const char*)urlString pWebNumber:(int)_pWebNumber
{
    assert(m_pWebControl);
    if(m_pWebControl == NULL)
        return NO;
    
    
    background = NULL;
    m_pView = NULL;            //!<视图指针
    m_pWebView = NULL;         //!<基于OC网页视图指针
    bgyinying = NULL;//阴影
   
    title = NULL;
    closeButton = NULL;
    buttonClose = NULL;
    activitiesBg = NULL;
    
    
    CGSize winsize = [[CCEAGLView sharedEGLView] surfaceSize];
    
    CGRect rect = [[UIScreen mainScreen] bounds];
    
    
	if( m_pWebView )
	{
		if([m_pWebView isLoading])
			[m_pWebView stopLoading];
        
		[m_pWebView release];
		m_pWebView = nil;
	}
    
	if(m_pView)
	{
		[m_pView release];
		m_pView = nil;
	}
    
    //清除cookies
    NSHTTPCookie *cookie;
    NSHTTPCookieStorage *storage = [NSHTTPCookieStorage sharedHTTPCookieStorage];
    for (cookie in [storage cookies])
    {
        [storage deleteCookie:cookie];
    }
    NSURLCache * cache = [NSURLCache sharedURLCache];
    [cache removeAllCachedResponses];
    [cache setDiskCapacity:0];
    [cache setMemoryCapacity:0];
    
    CCSize size = m_pWebControl->getContentSize();
    m_pView = [[UIView alloc] initWithFrame:CGRectMake( m_pWebControl->getPositionX()
                                                       , m_pWebControl->getPositionY(),size.width,size.height)];
    [self addSubview:m_pView];
    
    bgyinying = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [bgyinying setFrame:CGRectMake(bgRect.size.width* 0.92, -7,2000,2000)];
    [bgyinying setBackgroundImage:[[UIImage imageNamed:@""]stretchableImageWithLeftCapWidth:0.0 topCapHeight:0.0] forState:UIControlStateNormal];
    bgyinying.backgroundColor = [UIColor blackColor];
    bgyinying.alpha = 0.5;
    [bgyinying addTarget:self action:@selector(closeBgBtn:) forControlEvents:UIControlEventTouchUpInside];
    [[CCEAGLView sharedEGLView] addSubview:bgyinying];
    [bgyinying setAdjustsImageWhenDisabled:NO];
    bgyinying.adjustsImageWhenDisabled = NO;
    [bgyinying retain];
    
    if (_pWebNumber == 4)
    {
        background = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, rect.size.height , rect.size.width)];
        [background setImage:[UIImage imageNamed:@"moregame_bg.png"]];
        [[CCEAGLView sharedEGLView]  addSubview:background];
    }
    
    if (_pWebNumber == 1)
    {
        background = [[UIImageView alloc] initWithFrame:CGRectMake((rect.size.height- rect.size.height * 0.95)/2, (rect.size.width - rect.size.width*0.82)/2, rect.size.height * 0.95, rect.size.width*0.82)];
        [background setImage:[UIImage imageNamed:@"publish/Commone_Win_BJ.png"]];
        
        [[CCEAGLView sharedEGLView]  addSubview:background];
        
        bgRect = background.frame;
        
        if (winsize.width == 1136)
        {
            background.frame=CGRectMake((rect.size.height- rect.size.height * 0.95)/2, (rect.size.width - rect.size.width*0.82)/2- 10, rect.size.height * 0.95, rect.size.width*0.89);
        }
    }
    else if (_pWebNumber == 3)
    {
        
        activitiesBg = [[UIImageView alloc] init];
        [activitiesBg setImage:[UIImage imageNamed:@"publish/login_bg.png"]];
        [[CCEAGLView sharedEGLView] addSubview:activitiesBg];
        activitiesBg.frame = CGRectMake((rect.size.height- rect.size.height * 0.61)/2-1 ,(rect.size.width - rect.size.width*0.5)/2 + 10, rect.size.height * 0.61f, rect.size.width*0.5f);
        [activitiesBg retain];
        
        if(winsize.width == 1136)
        {
            activitiesBg.frame = CGRectMake((rect.size.height- rect.size.height * 0.61)/2-1 ,(rect.size.width - rect.size.width*0.7)/2 , rect.size.height * 0.61f, rect.size.width*0.7f);
            
        }
        else if (winsize.width == 2048)
        {
            activitiesBg.frame = CGRectMake((rect.size.height- rect.size.height * 0.71)/2-1 ,(rect.size.width - rect.size.width*0.8)/2 , rect.size.height * 0.71f, rect.size.width*0.8f);
        }
        else if (winsize.width == 960)
        {
            activitiesBg.frame = CGRectMake((rect.size.height- rect.size.height * 0.61)/2-1 ,(rect.size.width - rect.size.width*0.62)/2 , rect.size.height * 0.61f, rect.size.width*0.62f);
        }
    }
    
    
    m_pWebView = [[UIWebView alloc] initWithFrame:CGRectMake((rect.size.height- rect.size.height * 0.91)/2-1 ,(rect.size.width - rect.size.width*0.82)/2 + 20 ,rect.size.height * 0.91,rect.size.width*0.76)];
    
    
    m_pWebView.scalesPageToFit = true;
    m_pWebView.backgroundColor = [UIColor clearColor];
    m_pWebView.opaque = NO;
    m_pWebView.scrollView.scrollEnabled = NO;
    m_pWebView.delegate = self;
    
//    NSDictionary *dictionnary = [[NSDictionary alloc] initWithObjectsAndKeys:@" WeileApp:yes WeileSign:enterprise " , @"UserAgent", nil];
//    [dictionnary objectForKey:@"UserAgent"];
//    NSLog(@"ddd==== %@",[dictionnary objectForKey:@"UserAgent"]);
//   [[NSUserDefaults standardUserDefaults] registerDefaults:dictionnary];
    
    
    
    
    
    if (winsize.width == 1136)
    {
        m_pWebView.frame = CGRectMake((rect.size.height- rect.size.height * 0.91)/2 - 1 ,(rect.size.width - rect.size.width*0.8)/2 - 5  ,rect.size.height * 0.91,rect.size.width*0.83);
    }
    else if(winsize.width == 960)
    {
        m_pWebView.frame = CGRectMake((rect.size.height- rect.size.height * 0.91)/2 ,(rect.size.width - rect.size.width*0.8)/2  ,rect.size.height * 0.91,rect.size.width*0.78);
    }
    else if (winsize.width == 480)
    {
        m_pWebView.frame = CGRectMake((rect.size.height- rect.size.height * 0.91)/2-1 ,(rect.size.width - rect.size.width*0.82)/2+10  ,rect.size.height * 0.91,rect.size.width*0.75);
    }
    
    
    NSString* urlBase = [NSString stringWithCString:urlString encoding:NSUTF8StringEncoding];
    [m_pWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:urlBase]]];
    m_pWebView.backgroundColor = [UIColor clearColor];
    m_pView.backgroundColor = [UIColor clearColor];
    
    UIWebView *web = [[[UIWebView alloc] init] autorelease];
    NSString *oldAgent = [web stringByEvaluatingJavaScriptFromString:@"navigator.userAgent"];
    
     //add my info to the new agent  WeileSign:enterprise

    if ([[[NSUserDefaults standardUserDefaults] objectForKey:@"UserAgent"] hasSuffix:@"WeileApp:yes WeileSign:enterprise"])
    {
        newAgent = [oldAgent stringByAppendingString:@""];
    }
    else
    {
        newAgent = [oldAgent stringByAppendingString:@"WeileApp:yes WeileSign:enterprise"];
    }
    
    //regist the new agent
    NSMutableDictionary *dictionnary = [[NSMutableDictionary alloc] initWithObjectsAndKeys:newAgent, @"UserAgent", nil];
    [[NSUserDefaults standardUserDefaults] registerDefaults:dictionnary];


    [[CCEAGLView sharedEGLView]  addSubview:m_pWebView];
    [m_pWebView setScalesPageToFit:YES];
    m_pWebView.scrollView.scrollEnabled = NO;
    
    
    if (_pWebNumber == 1)
    {
        float btnX = 30;
        float btnY = 10;
        float sizeW = 0.5;
        float sizeH = 0.5;
        if (winsize.width == 1024 || winsize.width == 2048)
        {
            btnX = 50;
            btnY = 0;
            sizeH = sizeW = 1;
        }
        else if (winsize.width == 1136)
        {
            btnX = -186;
            btnY = 0;
            sizeH = sizeW = 0.6;
            
        }
        else
        {
            btnX = -200;
            btnY = 0;
            sizeW = sizeH = 0.5;
        }
        
        //        _imageButton = [[ImageButton alloc] initWithFrame:CGRectMake(100, 356 ,111, 111)];
        //        [_imageButton setImageName:@"ChuangguanRenwu_close0.png"];
        //        _imageButton.backgroundColor = [UIColor redColor];
        //        _imageButton.delegate = self;
        //        [m_pWebView addSubview:_imageButton];
        
        
        //        closeButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        //        [closeButton setFrame:CGRectMake(m_pWebControl->getContentSize().width-100, 15, 82*sizeW, 54*sizeH)];
        //        [closeButton setBackgroundImage:[[UIImage imageNamed:@"moreGameUI/tuijian_guanbi1.png"]stretchableImageWithLeftCapWidth:0.0 topCapHeight:0.0] forState:UIControlStateNormal];
        //        [closeButton addTarget:self action:@selector(closeBtn1:) forControlEvents:UIControlEventTouchUpInside];
        //        [[EAGLView sharedEGLView] addSubview:closeButton];
        
        
        
        UIButton *sampleButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        [sampleButton setFrame:CGRectMake(bgRect.size.width* 0.925, -8 , 43*sizeW, 46*sizeH)];
        [sampleButton setBackgroundImage:[[UIImage imageNamed:@"ChuangguanRenwu_close0.png"]stretchableImageWithLeftCapWidth:0.0 topCapHeight:0.0] forState:UIControlStateNormal];
        [sampleButton addTarget:self action:@selector(closeBtn1:) forControlEvents:UIControlEventTouchUpInside];
        [m_pWebView addSubview:sampleButton];
        
        
        if (winsize.width == 960)
        {
            sampleButton.frame = CGRectMake(bgRect.size.width* 0.92f, 2 , 43*sizeW, 46*sizeH);
        }
        else if (winsize.width == 1136)
        {
            sampleButton.frame = CGRectMake(bgRect.size.width* 0.925f, -2 , 43*sizeW, 46*sizeH);
        }
        
    }
    
    else if (_pWebNumber == 2 || _pWebNumber == 4)
    {
        
        m_pWebView.scrollView.scrollEnabled = YES;
        
        float sizeW = 0.5f;
        float sizeH = 0.5f;
        if (winsize.width == 1024)
        {
            
            sizeH = sizeW = 1;
            m_pWebView.frame = CGRectMake(0, 90, size.width, size.height);
            
        }
        else
        {
            sizeH = sizeW = 0.5;
            
        }
       
//        CCSize size = CCDirector::sharedDirector()->getWinSize();
//        background = UIImage
//        background->setPosition(ccp(size.width/2, size.height/2));
//        background->setRotation(90);
//        background->setScale(2);
//        addNode(background);
        
     
//
        title = [[UIImageView alloc] init];
        [title setImage:[UIImage imageNamed:@"publish/top_02.png"]];
        
        title.transform = CGAffineTransformMakeRotation(M_PI/2);
        [[CCEAGLView sharedEGLView]   addSubview:title];
        
        UIImageView * titleText = [[UIImageView alloc] init];
        [titleText setImage:[UIImage imageNamed:@"moreGameUI/tuijian_youxi.png"]];
        titleText.frame = CGRectMake(m_pWebControl->getContentSize().width/2-100,26, 141*sizeW, 36*sizeH);
        [title addSubview:titleText];
        [title retain];
        
        if (_pWebNumber == 2)
        {
            [titleText setImage:[UIImage imageNamed:@"moreGameUI/tuijian_youxi.png"]];
        }
        else if (_pWebNumber == 4)
        {
            [titleText setImage:[UIImage imageNamed:@"moreGameUI/web_yuanbaoduihuan.png"]];
        }
        
        closeButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        [closeButton setFrame:CGRectMake(m_pWebControl->getContentSize().width-100, 15, 82*sizeW, 54*sizeH)];
        [closeButton setBackgroundImage:[[UIImage imageNamed:@"moreGameUI/tuijian_guanbi1.png"]stretchableImageWithLeftCapWidth:0.0 topCapHeight:0.0] forState:UIControlStateNormal];
        [closeButton addTarget:self action:@selector(closeBtn1:) forControlEvents:UIControlEventTouchUpInside];
        [[CCEAGLView sharedEGLView] addSubview:closeButton];
        [closeButton retain];
        
        m_pWebView.transform = CGAffineTransformMakeRotation(M_PI/2);
        title.frame = CGRectMake(rect.size.height-102*sizeW+10, 0, 102*sizeW,rect.size.width);
        title.transform  = CGAffineTransformMakeRotation(M_PI+M_PI/2);
        m_pWebView.frame = CGRectMake(-102*sizeW+10, 0, rect.size.height, rect.size.width);
        closeButton.transform = CGAffineTransformMakeRotation(M_PI/2);
        closeButton.frame  = CGRectMake(rect.size.height-82*sizeW, rect.size.width-110, 54*sizeH,82*sizeW);
        titleText.transform = CGAffineTransformMakeRotation(M_PI);
        titleText.frame  = CGRectMake(rect.size.width/2-110, 20, 141*sizeW, 36*sizeH);
        if ( winsize.width == 2048)
        {
            
            title.frame = CGRectMake(rect.size.height-102*sizeW+10, 0, 102*sizeW,rect.size.width);
            m_pWebView.frame = CGRectMake(-102*sizeW+10, 0, rect.size.height, rect.size.width);
            closeButton.frame  = CGRectMake(rect.size.height-82*sizeW+5, rect.size.width-50, 54*sizeH,82*sizeW);
            titleText.frame  = CGRectMake(rect.size.width/2-50, 10, 141*sizeW, 36*sizeH);
            
            
        }
        else if (winsize.width == 1136)
        {
            title.frame = CGRectMake(rect.size.height-102*sizeW+10, 0, 102*sizeW,rect.size.width);
            closeButton.frame  = CGRectMake(rect.size.height-82*sizeW+5, rect.size.width-50, 54*sizeH,82*sizeW);
            titleText.frame  = CGRectMake(rect.size.width/2-30, 10, 141*sizeW, 36*sizeH);
            
        }
        else if (winsize.width == 1024)
        {
            title.frame = CGRectMake(rect.size.height-102*sizeW+10, 0, 102*sizeW,rect.size.width);
            m_pWebView.frame = CGRectMake(-102*sizeW+10, 0, rect.size.height, rect.size.width);
            closeButton.frame  = CGRectMake(rect.size.height-82*sizeW, rect.size.width-110, 54*sizeH,82*sizeW);
            titleText.frame  = CGRectMake(rect.size.width/2-50, 20, 141*sizeW, 36*sizeH);
        }
        else
        {
            title.frame = CGRectMake(rect.size.height-102*sizeW+10, 0, 102*sizeW,rect.size.width);
            closeButton.frame  = CGRectMake(rect.size.height-82*sizeW+5, rect.size.width-50, 54*sizeH,82*sizeW);
            titleText.frame  = CGRectMake(rect.size.width/2-30, 10, 141*sizeW, 36*sizeH);
            m_pWebView.frame = CGRectMake(-102*sizeW+10, 0, rect.size.height, rect.size.width);
            
        }
        
    }
    else if (_pWebNumber == 3)
    {
        m_pWebView.scrollView.scrollEnabled = YES;
        m_pWebView.frame = CGRectMake((rect.size.height- rect.size.height * 0.575)/2-1 ,(rect.size.width - rect.size.width*0.5)/2 + 30 ,rect.size.height * 0.575,rect.size.width*0.45);
        
        if (winsize.width == 1136)
        {
            
            m_pWebView.frame = CGRectMake((rect.size.height- rect.size.height * 0.575)/2-1 ,(rect.size.width - rect.size.width*0.675)/2 + 7  ,rect.size.height * 0.575,rect.size.width*0.63);
            
        }
        else if (winsize.width == 960)
        {
            m_pWebView.frame = CGRectMake((rect.size.height- rect.size.height * 0.575)/2-1 ,(rect.size.width - rect.size.width*0.56)/2  ,rect.size.height * 0.575,rect.size.width*0.56);
        }
        else if (winsize.width == 2048)
        {
            m_pWebView.frame = CGRectMake((rect.size.height- rect.size.height * 0.675)/2-1 ,(rect.size.width - rect.size.width*0.7)/2 - 7  ,rect.size.height * 0.675,rect.size.width*0.72);
        }
        else if (winsize.width == 480)
        {
            m_pWebView.frame = CGRectMake((rect.size.height- rect.size.height * 0.575)/2-1 ,(rect.size.width - rect.size.width*0.5)/2 + 20 ,rect.size.height * 0.575,rect.size.width*0.45);
        }
        
        
        float sizeW = 0.5f;
        float sizeH = 0.5f;
        if (winsize.width == 1024)
        {
            
            sizeH = sizeW = 1;
            
            
        }
        else
        {
            sizeH = sizeW = 0.5;
            
        }
        CGRect aiBg = activitiesBg.frame;
        
        closeButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        [closeButton setFrame:CGRectMake(aiBg.size.width* 0.89, -5 , 43*sizeW, 46*sizeH)];
        [closeButton setBackgroundImage:[[UIImage imageNamed:@"ChuangguanRenwu_close0.png"]stretchableImageWithLeftCapWidth:0.0 topCapHeight:0.0] forState:UIControlStateNormal];
        [closeButton addTarget:self action:@selector(closeBtn1:) forControlEvents:UIControlEventTouchUpInside];
        [[CCEAGLView sharedEGLView] addSubview:closeButton];
        [closeButton retain];
        
        
        if (winsize.width == 960)
        {
            closeButton.frame = CGRectMake(rect.size.height* 0.75f, rect.size.width*0.22 , 43*sizeW, 46*sizeH);
        }
        else if (winsize.width == 1136)
        {
            closeButton.frame = CGRectMake(rect.size.height* 0.745f, rect.size.width*0.18 , 43*0.7, 46*0.7);
        }
        else if (winsize.width == 2048)
        {
            closeButton.frame =CGRectMake(rect.size.height* 0.8, rect.size.width*0.13 , 43*1.2, 46*1.2);
        }
        else if (winsize.width == 1024)
        {
            closeButton.frame = CGRectMake(rect.size.height* 0.75, rect.size.width*0.28 , 43*1.2, 46*1.2);
        }
        
        
        
    }
    
    m_activity = [[UIActivityIndicatorView alloc]initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
    m_activity.center = CGPointMake(background.frame.size.width/2, background.frame.size.height/2);
    [[CCEAGLView sharedEGLView] addSubview:m_activity];
    [m_activity startAnimating];
    
    
    
    if (_pWebNumber == 2 || _pWebNumber == 4)
    {
        m_activity.center = CGPointMake( rect.size.width/2,  rect.size.height/2);
    }
    else if (_pWebNumber == 3)
    {
        m_activity.center = CGPointMake( rect.size.height/2,  rect.size.width/2);
    }
    
    
    m_activity.center = CGPointMake( rect.size.height/2,  rect.size.width/2);
    
    
    return YES;
}


-(BOOL) closeWebView
{
    
    [m_pView removeFromSuperview];
    
    
    if (activitiesBg)
    {
        [activitiesBg release];
        [activitiesBg removeFromSuperview];
        activitiesBg = NULL;
    }
    
    if (background)
    {
        
        [background removeFromSuperview];
        background = NULL;
        
       
    }
    
    if (bgyinying)
    {
        [bgyinying release];
        [bgyinying removeFromSuperview];
        bgyinying = NULL;
       
    }
    
    if (title)
    {
       
        [title removeFromSuperview];
        title = NULL;
        
    }
    
    if (closeButton)
    {
        [closeButton release];
        [closeButton removeFromSuperview];
        closeButton = NULL;
    }
    
    if (m_activity)
    {
        [m_activity stopAnimating];
        [m_activity setHidesWhenStopped:YES];
    }
    
    [self removeFromLayer];
    
    return false;
}

-(void) closeBgBtn:(UIButton *)sender
{
    
}

-(void) closeBtn1:(UIButton*)sender
{
    [m_pView removeFromSuperview];
    [background removeFromSuperview];
    [bgyinying removeFromSuperview];
    
    
    if (activitiesBg)
    {
        [activitiesBg release];
        [activitiesBg removeFromSuperview];
        activitiesBg = NULL;
    }
    
    if (title)
    {
        [title release];
        [title removeFromSuperview];
        title = NULL;
    }
    if (closeButton)
    {
        //        [closeButton release];
        [closeButton removeFromSuperview];
        closeButton = NULL;
    }
    
    if (m_activity)
    {
        [m_activity stopAnimating];
        [m_activity setHidesWhenStopped:YES];
    }
    [self removeFromLayer];
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("closeBtn");
}
-(void)removeFromLayer
{
    [m_pWebView removeFromSuperview];
  
}
-(void)webViewDidStartLoad:(UIWebView *)webView
{
	if(webView == m_pWebView)
		NSLog(@"webViewDidStartLoad");
}

-(void)webViewDidFinishLoad:(UIWebView *)webView
{
	if(webView == m_pWebView)
    {
        [m_activity stopAnimating];
        [m_activity setHidesWhenStopped:YES];
        m_pWebControl->onNotifyOpenUrlComplete();
    }
}

-(void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
	if(webView == m_pWebView)
    {
        [m_activity stopAnimating];
        [m_activity setHidesWhenStopped:YES];
		NSString* pError = [error domain];
		m_pWebControl->onNotifyOpenUrlError([pError UTF8String]);
	}
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
	if(webView == m_pWebView)
    {
		NSString* pUrl = [[request URL] absoluteString];
		bool bRet =  m_pWebControl->onNotifyOpenUrlStart([pUrl UTF8String]);
        //	[pUrl release];
		return bRet?YES:NO;
	}
	else
		return NO;
}

-(void) close
{
    NSLog(@"jie");
    NSLog(@"jie");
    NSLog(@"jie");
    NSLog(@"jie");
    
}
//-(void) buttonClick:(ImageButton *)button
//{
//    NSLog(@"sdfsf");
//}

-(void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"");
}

-(void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"");
}

-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"");
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"");
}

@end
