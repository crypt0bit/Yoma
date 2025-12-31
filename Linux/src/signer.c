#include "signer.h"
#include <stdlib.h>
#include <stdio.h>

int sign_app(const char *app_path) {
    char cmd[512];

    snprintf(cmd, sizeof(cmd),
        "rm -rf %s/*.app/_CodeSignature", app_path);
    system(cmd);

    snprintf(cmd, sizeof(cmd),
        "resources/ldid -S %s/*.app",
        app_path);

    system(cmd);
    return 1;
}
