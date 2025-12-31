#include <stdio.h>
#include "ipa.h"
#include "signer.h"
#include "installer.h"

int main() {
    char ipa[512];

    printf("Enter path to IPA: ");
    fgets(ipa, sizeof(ipa), stdin);

    ipa[strcspn(ipa, "\n")] = 0;

    const char *workdir = "/tmp/yoma";

    prepare_workspace(workdir);

    char app_path[512];
    if (!extract_ipa(ipa, workdir, app_path)) return 1;

    if (!sign_app(app_path)) return 1;

    char signed_ipa[512];
    repack_ipa(workdir, signed_ipa);

    install_ipa(signed_ipa);

    printf("Done.\n");
    return 0;
}
