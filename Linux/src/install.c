#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/installation_proxy.h>
#include "install.h"

int install_ipa(const char *ipa) {
    idevice_t dev = NULL;
    lockdownd_client_t l = NULL;
    instproxy_client_t ip = NULL;

    if (idevice_new(&dev, NULL) != IDEVICE_E_SUCCESS) return -1;
    if (lockdownd_client_new_with_handshake(dev, &l, "yoma") != LOCKDOWN_E_SUCCESS) return -1;
    if (instproxy_client_start_service(dev, &ip, "yoma") != INSTPROXY_E_SUCCESS) return -1;

    plist_t opts = plist_new_dict();
    plist_dict_set_item(opts, "PackageType", plist_new_string("Developer"));

    instproxy_install(ip, ipa, opts, NULL, NULL);

    plist_free(opts);
    instproxy_client_free(ip);
    lockdownd_client_free(l);
    idevice_free(dev);
    return 0;
}
