#import "Signer.h"

@implementation Signer

+ (BOOL)isMachO:(NSString *)path {
    NSData *d = [NSData dataWithContentsOfFile:path];
    if (d.length < 4) return NO;
    const unsigned char *b = d.bytes;
    return (b[0]==0xFE && b[1]==0xED) || (b[0]==0xCE && b[1]==0xFA) || (b[0]==0xCF && b[1]==0xFA);
}

+ (void)signDir:(NSString *)dirPath {
    NSFileManager *fm = [NSFileManager defaultManager];
    NSArray *items = [fm contentsOfDirectoryAtPath:dirPath error:nil];
    for (NSString *item in items) {
        if ([item isEqualToString:@"."] || [item isEqualToString:@".."]) continue;
        NSString *full = [dirPath stringByAppendingPathComponent:item];
        BOOL isDir;
        [fm fileExistsAtPath:full isDirectory:&isDir];
        if (isDir) [self signDir:full];
        else if ([self isMachO:full]) {
            NSString *cmd = [NSString stringWithFormat:@"./resources/ldid -S \"%@\"", full];
            system([cmd UTF8String]);
        }
    }
}

+ (void)signPayload:(NSString *)payloadPath {
    [self signDir:payloadPath];
}

@end
