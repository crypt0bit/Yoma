#include <stdio.h>
#include <string.h>
#include "ziputil.h"
#include "sign.h"
#include "install.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage:\n");
        printf("  yoma run app.ipa\n");
        return 0;
    }

    if (!strcmp(argv[1], "run")) {
        ipa_extract(argv[2], ".");
        sign_payload("Payload");
        ipa_pack("Payload", "signed.ipa");
        install_ipa("signed.ipa");
    }
    return 0;
}
