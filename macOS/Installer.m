#import "Installer.h"
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/installation_proxy.h>

@implementation Installer

+ (void)installIPA:(NSString *)ipaPath {
    idevice_t dev = NULL;
    lockdownd_client_t l = NULL;
    instproxy_client_t ip = NULL;

    if (idevice_new(&dev, NULL) != IDEVICE_E_SUCCESS) return;
    if (lockdownd_client_new_with_handshake(dev, &l, "yoma") != LOCKDOWN_E_SUCCESS) return;
    if (instproxy_client_start_service(dev, &ip, "yoma") != INSTPROXY_E_SUCCESS) return;

    plist_t opts = plist_new_dict();
    plist_dict_set_item(opts, "PackageType", plist_new_string("Developer"));

    instproxy_install(ip, [ipaPath UTF8String], opts, NULL, NULL);

    plist_free(opts);
    instproxy_client_free(ip);
    lockdownd_client_free(l);
    idevice_free(dev);
}

@end
