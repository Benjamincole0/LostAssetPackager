#include "Unpacker.h"
#include <iostream>
#include "Header.h"

uint32_t Unpacker::footerOffset = 0;

void Unpacker::ListAssets(const std::string& packedPath)
{
    std::ifstream in(packedPath, std::ios::binary);
    if (!in.is_open())
    {
        std::cerr << "Failed to open: " << packedPath << "\n";
        return;
    }

    if (footerOffset == 0)
        GetFooterOffset(packedPath);

    in.seekg(footerOffset, std::ios::beg);

    uint32_t assetCount = 0;
    in.read(reinterpret_cast<char*>(&assetCount), sizeof(assetCount));

    std::cout << "Asset count: " << assetCount << "\n";

    for (uint32_t i = 0; i < assetCount; ++i)
    {
        uint16_t nameLen = 0;
        in.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));

        std::string name(nameLen, '\0');
        in.read(&name[0], nameLen);

        uint32_t offset = 0;
        uint32_t size = 0;
        in.read(reinterpret_cast<char*>(&offset), sizeof(offset));
        in.read(reinterpret_cast<char*>(&size), sizeof(size));

        std::cout << " - [" << i << "] \"" << name << "\" (offset=" << offset << ", size=" << size << ")\n";
    }
}


void Unpacker::SetPackedAssetPackPath(const std::string& path)
{
    _packedAssetPath = path;
}

std::vector<uint8_t> Unpacker::LoadAsset(const std::string& packedPath, const std::string& assetName)
{
    std::ifstream in(packedPath, std::ios::binary);
    if (!in.is_open())
    {
        std::cerr << "Couldn't open asset pack: " << packedPath << std::endl;
        return {};
    }

    if (footerOffset == 0)
        GetFooterOffset(packedPath);

    in.seekg(footerOffset, std::ios::beg);

    uint32_t assetCount = 0;
    in.read(reinterpret_cast<char*>(&assetCount), sizeof(assetCount));

    for (uint32_t i = 0; i < assetCount; ++i)
    {
        uint16_t nameLen = 0;
        in.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));

        std::string name(nameLen, '\0');
        in.read(name.data(), nameLen);

        uint32_t offset = 0;
        uint32_t size = 0;
        in.read(reinterpret_cast<char*>(&offset), sizeof(offset));
        in.read(reinterpret_cast<char*>(&size), sizeof(size));

        if (name == assetName)
        {
            std::vector<uint8_t> buffer(size);
            in.seekg(offset, std::ios::beg);
            in.read(reinterpret_cast<char*>(buffer.data()), size);
            return buffer;
        }
    }

    std::cerr << "Asset \"" << assetName << "\" not found in pack." << std::endl;
    return {};
}


void Unpacker::ExtractAsset(const std::string& packedPath, const std::string& assetName, const std::string& outName)
{
    std::ofstream out(outName, std::ios::binary);
    if (!out)
    {
        std::cerr << "Couldn't write to output file: " << outName << std::endl;
        return;
    }

    std::vector<uint8_t> data = LoadAsset(packedPath, assetName);
    if (data.empty())
    {
        std::cerr << "Failed to extract asset: " << assetName << std::endl;
        return;
    }

    out.write(reinterpret_cast<const char*>(data.data()), data.size());
}

uint32_t Unpacker::GetFooterOffset(const std::string& packedAssetPath)
{
    std::ifstream in(packedAssetPath, std::ios::binary);
    if (!in.is_open())
    {
        std::cerr << "Couldn't open asset path: " << packedAssetPath << std::endl;
        return 0;
    }

    in.seekg(sizeof(LAP::Version), std::ios::beg);
    uint32_t offset = 0;
    in.read(reinterpret_cast<char*>(&offset), sizeof(offset));

    footerOffset = offset;

    return offset;
}
