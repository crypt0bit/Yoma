#include "installer.h"
#include <stdlib.h>
#include <stdio.h>

int install_ipa(const char *ipa) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd),
        "resources/ideviceinstaller -i %s",
        ipa);
    return system(cmd);
}
