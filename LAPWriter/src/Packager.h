#pragma once
#include <vector>
#include <vector>
#include <string>
#include <fstream>
#include <ostream>
#include <cstdint>
#include "Header.h"

namespace LAP
{
    class Packager
    {
    public:
        void AddHeader(LAP::Version version, std::ostream& output);
        void AddAsset(std::string assetPath, std::ofstream& output);
        void AddAssetsRecursively();
        void AddFooter(std::ofstream& output);

public:
        uint32_t footerOffset = 0;
        uint32_t headerSize = 0;

        struct AssetEntry {
            std::string name;
            std::string path;
            uint32_t offset;
            uint32_t size;
        };

        std::vector<AssetEntry> storedAssets;
    };
}