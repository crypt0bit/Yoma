#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "sign.h"

static void sign_dir(const char *path) {
    DIR *d = opendir(path);
    if (!d) return;

    struct dirent *e;
    char full[1024];

    while ((e = readdir(d))) {
        if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;
        snprintf(full, sizeof(full), "%s/%s", path, e->d_name);

        if (e->d_type == DT_DIR) sign_dir(full);
        else if (strstr(e->d_name, ".dylib") || !strchr(e->d_name, '.')) {
            char cmd[1200];
            snprintf(cmd, sizeof(cmd), "./resources/ldid -S \"%s\"", full);
            system(cmd);
        }
    }
    closedir(d);
}

int sign_payload(const char *payload) {
    sign_dir(payload);
    return 0;
}
