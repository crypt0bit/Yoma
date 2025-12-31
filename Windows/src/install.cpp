#include "install.h"
#include <iostream>

void Installer::installIPA(const std::string &ipaPath) {
    std::string cmd = "resources\\ideviceinstaller.exe -i \"" + ipaPath + "\"";
    std::cout << "[*] Installing IPA via USB...\n";
    system(cmd.c_str());
    std::cout << "[*] Done!\n";
}
