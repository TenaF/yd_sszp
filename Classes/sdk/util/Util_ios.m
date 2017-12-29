//
//  Util_ios.m
//
//  Created by
//
//
#include "Util_ios.h"
#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioServices.h> 
#import "Reachability.h"
#import "DeviceAttr.h"
#import "DeviceInfo.h"
#import "SvUDIDTools.h"
#import <AdSupport/AdSupport.h>
#import "sys/utsname.h"

#import "SAMKeychain.h"

// 获取电池电量
int getBatteryLevelIOS()
{
    int level = 0;
    [[UIDevice currentDevice] setBatteryMonitoringEnabled:YES];
    
    float percent = [[UIDevice currentDevice] batteryLevel];
    
    if ( percent < 0.0f )
    {
        return level;
    }
    
    level = percent*100;
    return level;
}

// 震动
void vibrateIOS()
{
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}

// 获取信号强度
int getSignalIOS()
{
    int level = 0;
    if ([Reachability IsEnableWIFI])
    {
        level = 70;
    }
    else if([Reachability IsEnable3G])
    {
        level = 70;
    }
    else
    {
        level = 0;
    }
    return level;
    
}

const char* getDeviceIdIos()
{
    NSString * currentDeviceUUIDStr = [SAMKeychain passwordForService:@" " account:@"uuid"];
    if (currentDeviceUUIDStr == nil || [currentDeviceUUIDStr isEqualToString:@""])
    {
        NSUUID * currentDeviceUUID  = [UIDevice currentDevice].identifierForVendor;
        currentDeviceUUIDStr = currentDeviceUUID.UUIDString;
        currentDeviceUUIDStr = [currentDeviceUUIDStr stringByReplacingOccurrencesOfString:@"-" withString:@""];
        currentDeviceUUIDStr = [currentDeviceUUIDStr lowercaseString];
        [SAMKeychain setPassword: currentDeviceUUIDStr forService:@" " account:@"uuid"];
    }
    return [currentDeviceUUIDStr UTF8String];
}


const char* getUniqueCodeIOS()
{
    return [[SvUDIDTools UDID] UTF8String];
}

const char* getMacAddressIOS()
{
    //获取 Mac
    return [[DeviceAttr getMacAddress] UTF8String];
}

const char* getIMEIIOS()
{
    //获取 IMEI
    DeviceInfo *deviceInfo_ = [[DeviceInfo alloc] init];
    
    NSString *uuid = [[deviceInfo_ uniqueId] substringFromIndex:10];
    
    [deviceInfo_ release];
    deviceInfo_ = nil;
    
    return [uuid UTF8String];
}

//获取客户端版本号
const char* getClientVersionIOS()
{
    NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"];
    return [version UTF8String];
}

//获取屏幕宽高
const char* getScreenSizeIOS()
{
    CGSize size = [[UIScreen mainScreen] bounds].size;
    return [[NSString stringWithFormat:@"%dx%d",(int)size.width,(int)size.height] UTF8String];
}

//获取设备名
const char* getDeviceModelIOS()
{
    DeviceInfo *deviceInfo_ = [[DeviceInfo alloc] init];
    
    NSString *name = [deviceInfo_ deviceName];
    
    [deviceInfo_ release];
    deviceInfo_ = nil;
    
    return [name UTF8String];
}

//获取系统版本号
const char* getSysVersionIOS()
{

    DeviceInfo *deviceInfo_ = [[DeviceInfo alloc] init];
    
    NSString *sysVersion = [deviceInfo_ sysVersion];
    
    [deviceInfo_ release];
    deviceInfo_ = nil;
    
    return [sysVersion UTF8String];
}

const char* getNetworkNameIOS()
{
    //获取网络名
    NSString *netName = nil;
    if ([Reachability IsEnableWIFI])
    {
        netName = [NSString stringWithFormat:@"wifi"];
    }
    else if([Reachability IsEnable3G])
    {
        netName = [NSString stringWithFormat:@"3G"];
    }
    else
    {
        netName = [NSString stringWithFormat:@"none net"];
    }
    
    return [netName UTF8String];
}

//获取运营商
const char* getOperatorIOS()
{
    DeviceInfo *deviceInfo_ = [[DeviceInfo alloc] init];
    NSString *carrierName = [deviceInfo_ carrierName];
    [deviceInfo_ release];
    deviceInfo_ = nil;
    carrierName == nil ? @"" : carrierName;
    return [carrierName UTF8String];
}

//手机系统版本
const char* getSystemVersionIOS()
{
	NSString * iponeM = [[UIDevice currentDevice] systemName];
	return [iponeM UTF8String];
}

//ios是否越狱--android是否root
bool isJailBrokenIOS()
{
    bool jailbroken = false;
    NSString *cydiaPath = @"/Applications/Cydia.app";
    NSString *aptPath = @"/private/var/lib/apt/";
    if ([[NSFileManager defaultManager] fileExistsAtPath:cydiaPath]) {
        jailbroken = true;
    }
    if ([[NSFileManager defaultManager] fileExistsAtPath:aptPath]) {
        jailbroken = true;
    }  
    return jailbroken; 
}

//手机型号
const char* getPhoneModelIOS()
{
	struct utsname systemInfo;
    uname(&systemInfo);
    NSString *platform = [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
    NSLog(@"----设备类型---%@",platform);
    if ([platform isEqualToString:@"iPhone1,1"]) return [@"iPhone 2G" UTF8String];
    
    if ([platform isEqualToString:@"iPhone1,2"]) return [@"iPhone 3G" UTF8String];
    
    if ([platform isEqualToString:@"iPhone2,1"]) return [@"iPhone 3GS" UTF8String];
    
    if ([platform isEqualToString:@"iPhone3,1"]) return [@"iPhone 4" UTF8String];
    
    if ([platform isEqualToString:@"iPhone3,2"]) return [@"iPhone 4" UTF8String];
    
    if ([platform isEqualToString:@"iPhone3,3"]) return [@"iPhone 4" UTF8String];
    
    if ([platform isEqualToString:@"iPhone4,1"]) return [@"iPhone 4S" UTF8String];
    
    if ([platform isEqualToString:@"iPhone5,1"]) return [@"iPhone 5" UTF8String];
    
    if ([platform isEqualToString:@"iPhone5,2"]) return [@"iPhone 5" UTF8String];
    
    if ([platform isEqualToString:@"iPhone5,3"]) return [@"iPhone 5c" UTF8String];
    
    if ([platform isEqualToString:@"iPhone5,4"]) return [@"iPhone 5c" UTF8String];
    
    if ([platform isEqualToString:@"iPhone6,1"]) return [@"iPhone 5s" UTF8String];
    
    if ([platform isEqualToString:@"iPhone6,2"]) return [@"iPhone 5s" UTF8String];
    
    if ([platform isEqualToString:@"iPhone7,1"]) return [@"iPhone 6 Plus" UTF8String];
    
    if ([platform isEqualToString:@"iPhone7,2"]) return [@"iPhone 6" UTF8String];
    
    if ([platform isEqualToString:@"iPhone8,1"]) return [@"iPhone 6s" UTF8String];
    
    if ([platform isEqualToString:@"iPhone8,2"]) return [@"iPhone 6s Plus" UTF8String];
    
    if ([platform isEqualToString:@"iPhone9,1"]) return [@"iPhone 7s" UTF8String];
    
    if ([platform isEqualToString:@"iPhone9,2"]) return [@"iPhone 7s Plus" UTF8String];
    
    if ([platform isEqualToString:@"iPod1,1"])  return [@"iPod Touch 1G" UTF8String];
    
    if ([platform isEqualToString:@"iPod2,1"])  return [@"iPod Touch 2G" UTF8String];
    
    if ([platform isEqualToString:@"iPod3,1"])  return [@"iPod Touch 3G" UTF8String];
    
    if ([platform isEqualToString:@"iPod4,1"])  return [@"iPod Touch 4G" UTF8String];
    
    if ([platform isEqualToString:@"iPod5,1"])  return [@"iPod Touch 5G" UTF8String];
    
    if ([platform isEqualToString:@"iPad1,1"])  return [@"iPad 1G" UTF8String];
    
    if ([platform isEqualToString:@"iPad2,1"])  return [@"iPad 2" UTF8String];
    
    if ([platform isEqualToString:@"iPad2,2"])  return [@"iPad 2" UTF8String];
    
    if ([platform isEqualToString:@"iPad2,3"])  return [@"iPad 2" UTF8String];
    
    if ([platform isEqualToString:@"iPad2,4"])  return [@"iPad 2" UTF8String];
    
    if ([platform isEqualToString:@"iPad2,5"])  return [@"iPad Mini 1G" UTF8String];
    
    if ([platform isEqualToString:@"iPad2,6"])  return [@"iPad Mini 1G" UTF8String];
    
    if ([platform isEqualToString:@"iPad2,7"])  return [@"iPad Mini 1G" UTF8String];
    
    if ([platform isEqualToString:@"iPad3,1"])  return [@"iPad 3" UTF8String];
    
    if ([platform isEqualToString:@"iPad3,2"])  return [@"iPad 3" UTF8String];
    
    if ([platform isEqualToString:@"iPad3,3"])  return [@"iPad 3" UTF8String];
    
    if ([platform isEqualToString:@"iPad3,4"])  return [@"iPad 4" UTF8String];
    
    if ([platform isEqualToString:@"iPad3,5"])  return [@"iPad 4" UTF8String];
    
    if ([platform isEqualToString:@"iPad3,6"])  return [@"iPad 4" UTF8String];
    
    if ([platform isEqualToString:@"iPad4,1"])  return [@"iPad Air" UTF8String];
    
    if ([platform isEqualToString:@"iPad4,2"])  return [@"iPad Air" UTF8String];
    
    if ([platform isEqualToString:@"iPad4,3"])  return [@"iPad Air" UTF8String];
    
    if ([platform isEqualToString:@"iPad4,4"])  return [@"iPad Mini 2G" UTF8String];
    
    if ([platform isEqualToString:@"iPad4,5"])  return [@"iPad Mini 2G" UTF8String];
    
    if ([platform isEqualToString:@"iPad4,6"]) return [@"iPad Mini 2G" UTF8String];
    
    if ([platform isEqualToString:@"i386"]) return [@"iPhone Simulator" UTF8String];
    
    if ([platform isEqualToString:@"x86_64"]) return [@"iPhone Simulator" UTF8String];

    return [@"unknown iPhone " UTF8String];
}

