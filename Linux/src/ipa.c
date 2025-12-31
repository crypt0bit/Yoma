#include "ipa.h"
#include <stdlib.h>
#include <string.h>

int prepare_workspace(const char *path) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "rm -rf %s && mkdir %s", path, path);
    return system(cmd) == 0;
}

int extract_ipa(const char *ipa, const char *path, char *out_app) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd),
        "resources/unzip \"%s\" -d %s", ipa, path);
    system(cmd);

    snprintf(out_app, 512, "%s/Payload", path);
    return 1;
}

int repack_ipa(const char *path, char *out_ipa) {
    strcpy(out_ipa, "signed.ipa");

    char cmd[512];
    snprintf(cmd, sizeof(cmd),
        "cd %s && resources/zip -qr ../%s Payload",
        path, out_ipa);

    system(cmd);
    return 1;
}
