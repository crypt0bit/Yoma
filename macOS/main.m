#import <Foundation/Foundation.h>
#import "ZipUtil.h"
#import "Signer.h"
#import "Installer.h"

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        if (argc < 2) {
            printf("Usage: yoma run <app.ipa>\n");
            return 0;
        }

        NSString *ipaPath = [NSString stringWithUTF8String:argv[2]];
        NSLog(@"[*] Extracting IPA...");
        [ZipUtil extractIPA:ipaPath to:@"."];
        NSLog(@"[*] Signing payload...");
        [Signer signPayload:@"Payload"];
        NSLog(@"[*] Packing IPA...");
        [ZipUtil packPayload:@"Payload" toIPA:@"signed.ipa"];
        NSLog(@"[*] Installing IPA...");
        [Installer installIPA:@"signed.ipa"];
        NSLog(@"[*] Done!");
    }
    return 0;
}
