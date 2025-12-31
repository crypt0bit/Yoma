#include "sign.h"
#include <filesystem>
#include <iostream>

static bool isMachO(const std::string &path) {
    FILE *f = fopen(path.c_str(), "rb");
    if (!f) return false;
    unsigned char buf[4];
    fread(buf, 1, 4, f);
    fclose(f);
    return (buf[0]==0xFE && buf[1]==0xED) || (buf[0]==0xCE && buf[1]==0xFA) || (buf[0]==0xCF && buf[1]==0xFA);
}

static void signDir(const std::string &path) {
    for (auto &p: std::filesystem::directory_iterator(path)) {
        if (p.is_directory()) signDir(p.path().string());
        else if (isMachO(p.path().string())) {
            std::string cmd = "resources\\ldid.exe -S \"" + p.path().string() + "\"";
            system(cmd.c_str());
        }
    }
}

void Signer::signPayload(const std::string &payloadPath) {
    signDir(payloadPath);
}
