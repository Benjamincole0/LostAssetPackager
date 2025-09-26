#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <filesystem>
#include "Utility/Utility.h"
#include "Utility/Converter.h"
#include "Packager.h"
#include "Header.h"
#include <algorithm>

bool confirmOverwrite(const std::string& outputPath)
{
    if (!LAPUtility::CheckFileExists(outputPath))
        return true;

    std::string chosenValue;
    std::cout << outputPath << " already exists! Override? y/n: ";

    while (true)
    {
        if (!(std::cin >> chosenValue))
        {
            std::cerr << "Error: failed to read from stdin.\n";
            return false;

        if (chosenValue == "y")
            return true;
        else if (chosenValue == "n")
        {
            std::cout << "Aborting program.\n";
            return false;
        }
        else
            std::cout << "Please enter y/n (case sensitive): ";
        }
    }
}

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            std::cerr << "No arguments provided.\n";
            std::cerr << "Usage:\n  lapw --single <asset source> <output name>\n";
            std::cerr << "       lapw --batch <some future usage>\n";
            return EXIT_FAILURE;
        }

        std::string mode = argv[1];
        if (mode == "--single")
        {
            if (argc != 4)
            {
                std::cerr << "Usage for single mode: lapw --single <asset source> <output name>\n";
                return EXIT_FAILURE;
            }

            const std::string inputPath  = argv[2];
            const std::string outputPath = std::string(argv[3]) + ".lpck";  

            if (!confirmOverwrite(outputPath))
            {
                return EXIT_SUCCESS;
            }

            std::ofstream output(outputPath, std::ios::binary);
            if (!output.is_open())
            {
                std::cerr << "Failed to open output file for writing: " << outputPath << "\n";
                return EXIT_FAILURE;
            }

            try
            {
                LAP::Packager pack;
                pack.AddHeader(LAP::Version(1, 0, 0), output);
                pack.AddSingleAsset(inputPath, output);
                pack.AddFooter(output);
                output.close();

                std::cout << "Write complete: " << outputPath <<".lpck\n";
                return EXIT_SUCCESS;
            }
            catch (...)
            {
                output.close();
                std::cerr << "Error occurred during packing. Cleaning up...\n";
                std::error_code ec;
                std::filesystem::remove(outputPath, ec);
                if (ec)
                {
                    std::cerr << "Failed to delete incomplete file: " << ec.message() << "\n";
                }
                throw;
            }
        }
        else if (mode == "--batch")
        {
            if (argc != 4)
            {
                std::cerr << "Usage for batch mode: lapw --batch <resource path> <output name>\n";
                return EXIT_FAILURE;
            }

            std::string resPath = argv[2];
            const std::string outputPath = std::string(argv[3]) + ".lpck";  

            std::vector<std::string> assets;

            auto count = std::count_if(
                std::filesystem::directory_iterator(resPath),
                std::filesystem::directory_iterator{},
                [](const std::filesystem::directory_entry& e) {
                    return std::filesystem::is_regular_file(e);
                });

            for (const auto& asset : std::filesystem::directory_iterator(resPath))
            {
                if (std::filesystem::is_regular_file(asset))
                {
                    assets.push_back(asset.path().string());
                }
            }

            if (!confirmOverwrite(outputPath))
            {
                return EXIT_SUCCESS;
            }

            std::ofstream output(outputPath, std::ios::binary);
            if (!output.is_open())
            {
                std::cerr << "Failed to open output file for writing: " << outputPath << "\n";
                return EXIT_FAILURE;
            }

            try
            {
                LAP::Packager pack;
                pack.AddHeader(LAP::Version(1, 0, 0), output);
                pack.BatchAddAssets(assets, output);
                pack.AddFooter(output);
                output.close();

                std::cout << "Write complete: " << outputPath << "\n";
                return EXIT_SUCCESS;
            }
            catch (...)
            {
                output.close();
                std::cerr << "Error occurred during packing. Cleaning up...\n";
                std::error_code ec;
                std::filesystem::remove(outputPath, ec);
                if (ec)
                {
                    std::cerr << "Failed to delete incomplete file: " << ec.message() << "\n";
                }
                throw;
            }
        }
        else
        {
            std::cerr << "Unknown flag: " << mode << "\n";
            std::cerr << "Usage:\n  lapw --single <asset source> <output name>\n";
            std::cerr << "       lapw --batch <...>\n";
            return EXIT_FAILURE;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Fatal error: unknown exception\n";
        return EXIT_FAILURE;
    }
}
