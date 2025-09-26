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
        void AddHeader(LAP::Version p_version, std::ostream& p_output);
        void AddAsset(const std::string& p_assetName, const std::string& p_originalPath,
                      const std::string& p_assetData, std::ostream& p_output);
                      
        void AddSingleAsset(const std::string &p_assetPath, std::ofstream &p_output);
        void BatchAddAssets(const std::vector<std::string>& p_assetPaths, std::ofstream& p_output);
        void AddFooter(std::ofstream& p_output);

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
