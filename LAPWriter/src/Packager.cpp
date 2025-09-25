#include "Packager.h"
#include "Utility/Converter.h"
#include "AssetDataStructure.h"

namespace LAP
{
    void Packager::AddHeader(LAP::Version version, std::ostream& output)
    {
        LAPCore::Converter::WriteRaw(output, version);

        uint32_t placeholder = 0;
        LAPCore::Converter::WriteRaw(output, placeholder);

        headerSize = sizeof(version) + sizeof(placeholder);
    }

    void Packager::AddAsset(const std::string& assetName, const std::string& originalPath,
                            const std::string& assetData, std::ostream& output)
    {
        uint32_t start = static_cast<uint32_t>(output.tellp());

        output.write(assetData.data(), assetData.size());
        if (!output) throw std::runtime_error("Failed to write asset data: " + assetName);

        footerOffset = static_cast<uint32_t>(output.tellp());

        storedAssets.emplace_back(AssetEntry{
            assetName,
            originalPath,
            start,
            static_cast<uint32_t>(assetData.size())
        });
    }

    void Packager::AddSingleAsset(const std::string& assetPath, std::ofstream& output)
    {
        std::string data = LAPCore::Converter::ReadRawFile(assetPath);
        AddAsset(assetPath, assetPath, data, output);
    }

    void Packager::BatchAddAssets(const std::vector<std::string>& assetPaths, std::ofstream& output)
    {
        for (const auto& path : assetPaths)
        {
            std::ifstream in(path, std::ios::binary);
            if (!in)
                throw std::runtime_error("Failed to open input file during batch: " + path);

            std::string data((std::istreambuf_iterator<char>(in)), {});
            AddAsset(path, path, data, output);
        }
    }


    void Packager::AddFooter(std::ofstream& output)
    {
        footerOffset = static_cast<uint32_t>(output.tellp());

        const uint32_t count = static_cast<uint32_t>(storedAssets.size());
        LAPCore::Converter::WriteRaw(output, count);

        for (const auto& asset : storedAssets)
        {
            const uint16_t nameLength = static_cast<uint16_t>(asset.name.size());
            LAPCore::Converter::WriteRaw(output, nameLength);
            LAPCore::Converter::WriteRaw(output, asset.name);
            LAPCore::Converter::WriteRaw(output, asset.offset);
            LAPCore::Converter::WriteRaw(output, asset.size);
        }

        // Write footer offset back into placeholder
        output.flush();
        output.seekp(sizeof(LAP::Version), std::ios::beg);
        LAPCore::Converter::WriteRaw(output, footerOffset);
        output.seekp(0, std::ios::end);
    }
}
