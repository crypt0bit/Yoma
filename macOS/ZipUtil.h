#import <Foundation/Foundation.h>

@interface ZipUtil : NSObject

+ (BOOL)extractIPA:(NSString *)ipaPath to:(NSString *)outputPath;
+ (BOOL)packPayload:(NSString *)payloadPath toIPA:(NSString *)ipaPath;

@end
