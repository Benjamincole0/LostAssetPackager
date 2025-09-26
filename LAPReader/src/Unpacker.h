#pragma once
#include <string>
#include <cstdint>
#include <vector>
#include <memory>
#include <fstream>

class Unpacker
{
public:
    static uint32_t footerOffset;

    static void ListAssets(const std::string& p_packedPath);

    void SetPackedAssetPackPath(const std::string& p_path);
    static void ExtractAsset(const std::string& p_packedPath, const std::string& p_assetName, const std::string& p_outName);

    static uint32_t GetFooterOffset(const std::string& p_packedAssetPath);

    static std::vector<uint8_t> LoadAsset(const std::string& p_packedPath, const std::string& p_assetName);

private:
    std::unique_ptr<std::ofstream> _currentFileOut;
    std::string _packedAssetPath;
};