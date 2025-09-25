#pragma once
#include <string>
#include <stdint.h>

namespace LAP
{
    struct AssetDataEntry
    {
        std::string name;
        std::string path;
        uint32_t offset;
        uint32_t size;
    };
}