#pragma once
#include <string>

class ZipUtil {
public:
    static bool extractIPA(const std::string &ipaPath, const std::string &outputPath);
    static bool packPayload(const std::string &payloadPath, const std::string &ipaPath);
};
