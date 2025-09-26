#include "Packager.h"
#include "Utility/Converter.h"
#include "AssetDataStructure.h"

namespace LAP
{
    void Packager::AddHeader(LAP::Version p_version, std::ostream& p_output)
    {
        LAPCore::Converter::WriteRaw(p_output, p_version);

        uint32_t placeholder = 0;
        LAPCore::Converter::WriteRaw(p_output, placeholder);

        headerSize = sizeof(p_version) + sizeof(placeholder);
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

    void Packager::AddSingleAsset(const std::string& p_assetPath, std::ofstream& p_output)
    {
        std::string data = LAPCore::Converter::ReadRawFile(p_assetPath);
        AddAsset(p_assetPath, p_assetPath, data, p_output);
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


    void Packager::AddFooter(std::ofstream& p_output)
    {
        const uint32_t count = static_cast<uint32_t>(storedAssets.size());

        footerOffset = static_cast<uint32_t>(p_output.tellp());
        LAPCore::Converter::WriteRaw(p_output, count);

        for (const auto& asset : storedAssets)
        {
            const uint16_t nameLength = static_cast<uint16_t>(asset.name.size());
            LAPCore::Converter::WriteRaw(p_output, nameLength);
            LAPCore::Converter::WriteRaw(p_output, asset.name);
            LAPCore::Converter::WriteRaw(p_output, asset.offset);
            LAPCore::Converter::WriteRaw(p_output, asset.size);
        }

        p_output.flush();
        p_output.seekp(sizeof(LAP::Version), std::ios::beg);
        LAPCore::Converter::WriteRaw(p_output, footerOffset);

        p_output.seekp(0, std::ios::end);
    }
}
