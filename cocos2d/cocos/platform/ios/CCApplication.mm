/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.
 
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

#import "CCApplication.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#import <UIKit/UIKit.h>

#import "CCGeometry.h"
#import "CCDirectorCaller.h"

NS_CC_BEGIN

Application* Application::sm_pSharedApplication = 0;


void Application::openURL(const char* pszUrl)
{
    NSString *iTunesLink = [NSString stringWithUTF8String:pszUrl];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:iTunesLink]];
    
}


void Application::copyByPaltform(const char* copyText)
{
   NSString *iTunesLink = [NSString stringWithUTF8String:copyText];
   UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
   pasteboard.string = iTunesLink;
   
    
  
    UIAlertView * alert = [[UIAlertView alloc]initWithTitle:@"复制成功" message:iTunesLink delegate:nil cancelButtonTitle:nil otherButtonTitles:@"确定", nil];
    [alert show];


    
}





void Application::saveUrlToCamera(const char* urlText)
{

    //?®§?®§?°÷®™???°ß°Ã®¶
    NSString *text = [NSString stringWithUTF8String:urlText];

    NSData *stringData = [text dataUsingEncoding: NSUTF8StringEncoding];

    //???®§??a????
    CIFilter *qrFilter = [CIFilter filterWithName:@"CIQRCodeGenerator"];
    [qrFilter setValue:stringData forKey:@"inputMessage"];
    [qrFilter setValue:@"M" forKey:@"inputCorrectionLevel"];

    UIColor *onColor = [UIColor redColor];
    UIColor *offColor = [UIColor blueColor];

    //???°÷®¨????£§
    CIFilter *colorFilter = [CIFilter filterWithName:@"CIFalseColor"
                                       keysAndValues:
                             @"inputImage",qrFilter.outputImage,
                             @"inputColor0",[CIColor colorWithCGColor:onColor.CGColor],
                             @"inputColor1",[CIColor colorWithCGColor:offColor.CGColor],
                             nil];

    CIImage *qrImage = colorFilter.outputImage;

    //??°Ã?°Ã°∆?®§®π
    CGSize size = CGSizeMake(300, 300);

    CGImageRef cgImage = [[CIContext contextWithOptions:nil] createCGImage:qrImage fromRect:qrImage.extent];

    UIGraphicsBeginImageContext(size);

    CGContextRef context = UIGraphicsGetCurrentContext();//???°Ã?°Ã°∆?®§????°÷®¨°÷®¨?°ß°÷®™?®™
    CGContextSetInterpolationQuality(context, kCGInterpolationNone);//?°¿?a?°Ã??®§??®™???°Ã°∆?®§???????????£§°Ã°∆???°„°Ã°«?¶Ã?a?°“?®§?
    //????????°Ã???®≤??°È?®Æ??°È??®≤????¶Ã?®™???°‰ ??®∞????®Æ?°Ã®¨?®§°‰°Ã???®≤???a°Ï????®≤???®§?????®≤??°È??????®¥?®§?CGContextScaleCTM(context, 1.0, -1.0)
    //?????®§??a??®Æ???°È??®≤??°÷®¨?°ß°Ã??°“?®§®π?? ?a°Ï???a?????®§??a?????¶Ã?®™QRCode°Ã?°Ã????°‰???°÷®¨°÷®¨?°ß?¶Ã?°ß??¶Ã???¶Ã?®™?a®§?®§?
    CGContextScaleCTM(context, 1.0, -1.0);//?®§????®Æ???°È???a??a°Ï?°“
    //CGContextGetClipBoundingBox(context)  ??°Ã®™?a?°ﬁ?¶Ã???°„°Ã???®¨°Ã???®≤??°È??°Ã?°Ã°∆?®§®π?¶Ã?®™°Ã??°“??®¨°÷®™???°÷®¨°÷®¨?°ß°÷®™?®™?¶Ã?®™°÷®™??°Ã°∆?®§???®¥°Ã®¶??°Ã???®¨?°ﬁ
    CGContextDrawImage(context, CGContextGetClipBoundingBox(context), cgImage);//??°Ã?°Ã°∆?®§®π?¶Ã?? context

    UIImage *codeImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();

    CGImageRelease(cgImage);


    //codeImage  (UIImage°Ã°«?????®≤??®∞??®¥?®§??°„°Ã?)
    // ??®≤??°È?°¿?°Í??°Ã??¶Ã?®™°Ã??°“?®§®π??
    //UIImage *img = [UIImage imageNamed:@"ImageName.png"];  
    // ?°¿?°Í??°Ã?°Ã??°“?®§®π???¶Ã??°÷®¨??°„?a°Ï?°∆°Ã°∆??®¨
    UIImageWriteToSavedPhotosAlbum(codeImage, nil, nil, nil);

}




Application::Application()
{
    CC_ASSERT(! sm_pSharedApplication);
    sm_pSharedApplication = this;
}

Application::~Application()
{
    CC_ASSERT(this == sm_pSharedApplication);
    sm_pSharedApplication = 0;
}

int Application::run()
{
    if (applicationDidFinishLaunching()) 
    {
        [[CCDirectorCaller sharedDirectorCaller] startMainLoop];
    }
    return 0;
}

void Application::setAnimationInterval(double interval)
{
    [[CCDirectorCaller sharedDirectorCaller] setAnimationInterval: interval ];
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////////////////////////////

Application* Application::getInstance()
{
    CC_ASSERT(sm_pSharedApplication);
    return sm_pSharedApplication;
}

// @deprecated Use getInstance() instead
Application* Application::sharedApplication()
{
    return Application::getInstance();
}

const char * Application::getCurrentLanguageCode()
{
    static char code[3]={0};
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
    NSString *currentLanguage = [languages objectAtIndex:0];
    
    // get the current language code.(such as English is "en", Chinese is "zh" and so on)
    NSDictionary* temp = [NSLocale componentsFromLocaleIdentifier:currentLanguage];
    NSString * languageCode = [temp objectForKey:NSLocaleLanguageCode];
    [languageCode getCString:code maxLength:2 encoding:NSASCIIStringEncoding];
    code[2]='\0';
    return code;
}

LanguageType Application::getCurrentLanguage()
{
    // get the current language and country config
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
    NSString *currentLanguage = [languages objectAtIndex:0];

    // get the current language code.(such as English is "en", Chinese is "zh" and so on)
    NSDictionary* temp = [NSLocale componentsFromLocaleIdentifier:currentLanguage];
    NSString * languageCode = [temp objectForKey:NSLocaleLanguageCode];

    LanguageType ret = LanguageType::ENGLISH;
    if ([languageCode isEqualToString:@"zh"])
    {
        ret = LanguageType::CHINESE;
    }
    else if ([languageCode isEqualToString:@"en"])
    {
        ret = LanguageType::ENGLISH;
    }
    else if ([languageCode isEqualToString:@"fr"]){
        ret = LanguageType::FRENCH;
    }
    else if ([languageCode isEqualToString:@"it"]){
        ret = LanguageType::ITALIAN;
    }
    else if ([languageCode isEqualToString:@"de"]){
        ret = LanguageType::GERMAN;
    }
    else if ([languageCode isEqualToString:@"es"]){
        ret = LanguageType::SPANISH;
    }
    else if ([languageCode isEqualToString:@"nl"]){
        ret = LanguageType::DUTCH;
    }
    else if ([languageCode isEqualToString:@"ru"]){
        ret = LanguageType::RUSSIAN;
    }
    else if ([languageCode isEqualToString:@"ko"]){
        ret = LanguageType::KOREAN;
    }
    else if ([languageCode isEqualToString:@"ja"]){
        ret = LanguageType::JAPANESE;
    }
    else if ([languageCode isEqualToString:@"hu"]){
        ret = LanguageType::HUNGARIAN;
    }
    else if ([languageCode isEqualToString:@"pt"]){
        ret = LanguageType::PORTUGUESE;
    }
    else if ([languageCode isEqualToString:@"ar"]){
        ret = LanguageType::ARABIC;
    }
    else if ([languageCode isEqualToString:@"nb"]){
        ret = LanguageType::NORWEGIAN;
    }
    else if ([languageCode isEqualToString:@"pl"]){
        ret = LanguageType::POLISH;
    }
    return ret;
}

Application::Platform Application::getTargetPlatform()
{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) // idiom for iOS <= 3.2, otherwise: [UIDevice userInterfaceIdiom] is faster.
    {
        return Platform::OS_IPAD;
    }
    else 
    {
        return Platform::OS_IPHONE;
    }
}

void Application::applicationScreenSizeChanged(int newWidth, int newHeight) {

}

NS_CC_END

#endif // CC_PLATFORM_IOS
