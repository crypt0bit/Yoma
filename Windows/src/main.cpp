#include <iostream>
#include "ziputil.h"
#include "sign.h"
#include "install.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: yoma run <app.ipa>\n";
        return 0;
    }

    std::string ipaPath = argv[2];
    std::cout << "[*] Extracting IPA...\n";
    ZipUtil::extractIPA(ipaPath, ".");
    std::cout << "[*] Signing payload...\n";
    Signer::signPayload("Payload");
    std::cout << "[*] Packing IPA...\n";
    ZipUtil::packPayload("Payload", "signed.ipa");
    std::cout << "[*] Installing IPA...\n";
    Installer::installIPA("signed.ipa");
    std::cout << "[*] Done!\n";

    return 0;
}
