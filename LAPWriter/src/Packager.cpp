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

    void Packager::AddAsset(std::string assetPath, std::ofstream& output)
    {
        uint32_t start = static_cast<uint32_t>(output.tellp());

        std::string newAsset = LAPCore::Converter::ReadRaw(assetPath);

        output.write(newAsset.data(), newAsset.size());

        footerOffset = static_cast<uint32_t>(output.tellp());

        storedAssets.emplace_back(AssetEntry{
            assetPath,
            assetPath,
            start,
            static_cast<uint32_t>(newAsset.size())
        });
    }

    void Packager::AddAssetsRecursively()
    {
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

        output.flush();
        output.seekp(sizeof(LAP::Version), std::ios::beg);
        LAPCore::Converter::WriteRaw(output, footerOffset);
        output.seekp(0, std::ios::end);
    }
}
