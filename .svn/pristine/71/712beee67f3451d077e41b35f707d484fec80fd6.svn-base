//
//  DeviceInfo.m
//

#import "DeviceInfo.h"
#import "OpenUDIDIOS.h"


@implementation DeviceInfo

-(id) init
{
    self = [super init];
    if (self)
	{
		curDevice = [UIDevice currentDevice];
		networkInfo = [[CTTelephonyNetworkInfo alloc] init];
		carrier = networkInfo.subscriberCellularProvider;
	}
    
	return self;
}


-(BOOL) isMultitaskSupport
{
	return [curDevice isMultitaskingSupported];
}

-(NSString*) deviceName
{
	return [curDevice name];
}

-(NSString*) sysName
{
    return [curDevice systemName];
}

-(NSString*) sysVersion
{
	return [curDevice systemVersion];
}

-(NSString*) uniqueId
{
	return [OpenUDIDIOS value];
}

- (NSString*) model
{
    return [curDevice model];
}

- (NSString*) deviceUDID
{
    return [OpenUDIDIOS value];
}

//供应商网络编号
-(NSString*) mobileNetworkCode
{
	return carrier.mobileNetworkCode;
}

//sim供应商名称
-(NSString*) carrierName
{
	return carrier.carrierName;
}

//所在国家编号
-(NSString*) mobileCountryCode
{
	return carrier.mobileCountryCode;
}

//是有允许VOIP
-(BOOL)		allowsVOIP
{
	return carrier.allowsVOIP;
}

-(NSString*) deviceType
{
    UIUserInterfaceIdiom type = [curDevice userInterfaceIdiom];
    
    if (type == UIUserInterfaceIdiomPhone)
    {
        return @"iphone";
    }
    else if (type == UIUserInterfaceIdiomPad)
    {
        return @"ipad";
    }
    else
    {
        return @"unknow";
    }
}


-(void) dealloc
{
	[networkInfo release];
	networkInfo = nil;
    
	[super dealloc];
}

+ (NSString*) getDeviceModel
{
    UIUserInterfaceIdiom type = [[UIDevice currentDevice] userInterfaceIdiom];
    
    if (type == UIUserInterfaceIdiomPhone)
    {
        return @"iphone";
    }
    else if (type == UIUserInterfaceIdiomPad)
    {
        return @"ipad";
    }
    else
    {
        return @"unknow";
    }
}

@end
