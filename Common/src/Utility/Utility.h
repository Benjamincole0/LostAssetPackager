#pragma once
#include <string>
#include <cstring>
#include <filesystem>

namespace LAPUtility
{
    std::string StripExtension(const std::string& path)
    {
        std::string result = path;
        auto slashPos = result.find_last_of('/');
        auto dotPos   = result.find_last_of('.');
        if (dotPos != std::string::npos && (slashPos == std::string::npos || dotPos > slashPos))
        {
            result.erase(dotPos);
        }
        return result;
    }

    bool CheckFileExists(const std::string& path, bool stripExtension = false)
    {
        std::string newPath = stripExtension ? StripExtension(path) : path;
        return std::filesystem::exists(newPath);
    }
}
