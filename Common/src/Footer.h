#pragma once
#include <vector> 
#include "AssetDataStructure.h"

namespace LAP
{
    struct Header
    {
        std::vector<AssetDataEntry> assets;
    };
}