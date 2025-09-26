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

    static void ListAssets(const std::string& packedPath);

    void SetPackedAssetPackPath(const std::string& path);
    static void ExtractAsset(const std::string& packedPath, const std::string& assetName, const std::string& outName);

    static uint32_t GetFooterOffset(const std::string& packedAssetPath);

    static std::vector<uint8_t> LoadAsset(const std::string& packedPath, const std::string& assetName);

private:
    std::unique_ptr<std::ofstream> _currentFileOut;
    std::string _packedAssetPath;
};