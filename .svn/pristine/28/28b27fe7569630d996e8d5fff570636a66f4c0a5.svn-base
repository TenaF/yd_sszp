//
//  DeviceAttr.m
//

#import "DeviceAttr.h"
#import "DeviceInfo.h"
#import "OpenUDIDIOS.h"
#import <CommonCrypto/CommonDigest.h>

@implementation DeviceAttr

- (id) init
{
    self = [super init];
    if (self)
    {
        //nothing
    }
    
    return self;
}

+ (NSString*) getMacAddress
{
    int mib[6];
    size_t len;
    char *buf;
    unsigned char *ptr;
    struct if_msghdr *ifm;
    struct sockaddr_dl *sdl;
    
    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;
    
    if ((mib[5] = if_nametoindex("en0")) == 0)
    {
        NSLog(@"Error: if_nametoindex error");
        return nil;
    }
    
    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0)
    {
        NSLog(@"Error: sysctl, take 1");
        return nil;
    }
    
    if ((buf = (char *)malloc(len)) == NULL)
    {
        NSLog(@"could not allocate momery, error");
    }
    
    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0)
    {
        NSLog(@"Error: sysctl, take 2");
    }
    
    ifm = (struct if_msghdr*)buf;
    sdl = (struct sockaddr_dl*)(ifm + 1);
    ptr = (unsigned char*)LLADDR(sdl);
    
    NSString *macAddr = [NSString stringWithFormat:@"%02x:%02x:%02x:%02x:%02x:%02x",*ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
    free(buf);
    
    return [macAddr uppercaseString];
}

+ (NSString*) get32MD5String:(NSString *)data
{
    NSString *result = nil;
    
    if (data)
    {
        unsigned char digest[16];
        NSData *temp = [data dataUsingEncoding:NSASCIIStringEncoding];
        CC_MD5([temp bytes], [temp length], digest);
        result = [NSString stringWithFormat:@"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
                    digest[0], digest[1],
                    digest[2], digest[3],
                    digest[4], digest[5],
                    digest[6], digest[7],
                    digest[8], digest[9],
                    digest[10], digest[11],
                    digest[12], digest[13],
                    digest[14], digest[15]];
        result = [result uppercaseString];
        
        NSLog(@"hashedISU result = %@",result);
    }
    
    return result;
}

+ (NSString*) get32MD5StringEx:(NSString *)data
{
    NSString *result = nil;
    
    if (data)
    {
        unsigned char digest[16];
        NSData *temp = [data dataUsingEncoding:NSASCIIStringEncoding];
        CC_MD5([temp bytes], [temp length], digest);
        result = [NSString stringWithFormat:@"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
                  digest[0], digest[1],
                  digest[2], digest[3],
                  digest[4], digest[5],
                  digest[6], digest[7],
                  digest[8], digest[9],
                  digest[10], digest[11],
                  digest[12], digest[13],
                  digest[14], digest[15]];
        //result = [result uppercaseString];
        
        NSLog(@"hashedISU result = %@",result);
    }
    
    return result;
}

+ (NSString*) getSpecialCode
{
    NSString *userName;
    NSString *mode = [DeviceInfo getDeviceModel];
    NSString *UDID = [OpenUDIDIOS value];
    userName = [NSString stringWithFormat:@"qlmios%@%@",mode,UDID];
    NSLog(@"getNoRegisterLoginId userName = %@",userName);
    
    return userName;
}

@end
