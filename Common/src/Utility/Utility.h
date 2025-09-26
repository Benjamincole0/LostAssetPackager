#pragma once
#include <string>
#include <cstring>
#include <filesystem>

namespace LAPUtility
{
    std::string StripExtension(const std::string& p_path)
    {
        std::string result = p_path;
        auto slashPos = result.find_last_of('/');
        auto dotPos   = result.find_last_of('.');
        if (dotPos != std::string::npos && (slashPos == std::string::npos || dotPos > slashPos))
        {
            result.erase(dotPos);
        }
        return result;
    }

    bool CheckFileExists(const std::string& p_path, bool p_stripExtension = false)
    {
        std::string newPath = p_stripExtension ? StripExtension(p_path) : p_path;
        return std::filesystem::exists(newPath);
    }
}
