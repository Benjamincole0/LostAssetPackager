#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include "Header.h"

namespace LAP
{
    class Packager
    {
    public:
        void AddHeader(LAP::Version version, std::ostream& output);
        void AddAsset(const std::string& assetName, const std::string& originalPath,
                      const std::string& assetData, std::ostream& output);
                      
        void AddSingleAsset(const std::string &assetPath, std::ofstream &output);
        void BatchAddAssets(const std::vector<std::string>& assetPaths, std::ofstream& output);
        void AddFooter(std::ofstream& output);

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
