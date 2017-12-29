//
//  DeviceInfo.h
//

#import <Foundation/Foundation.h>
#import <UIKit/UIDevice.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <CoreTelephony/CTCarrier.h>


typedef enum
{
	DeviceIPhone,
	DeviceIPad,
	DeviceUnknow
}DeviceType;

@interface DeviceInfo : NSObject 
{
@private
	
	UIDevice *curDevice;
	CTTelephonyNetworkInfo *networkInfo;
	CTCarrier *carrier;
}

-(NSString*) deviceType;
//是否支持多任务 
-(BOOL) isMultitaskSupport;
-(NSString*) deviceName;
-(NSString*) sysName;
-(NSString*) sysVersion;
-(NSString*) uniqueId;
- (NSString*) model;
- (NSString*) deviceUDID;

//供应商网络编号
-(NSString*) mobileNetworkCode;
//sim供应商名称
-(NSString*) carrierName;
//所在国家编号
-(NSString*) mobileCountryCode;
//是有允许VOIP
-(BOOL)		allowsVOIP;
+ (NSString*) getDeviceModel;
@end
