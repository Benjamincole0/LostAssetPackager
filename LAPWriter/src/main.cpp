#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include "Utility/Utility.h"
#include "Utility/Converter.h"
#include "Packager.h"
#include "Header.h"

int main(int argc, char* argv[])
{
    try
    {
        if (argc <= 1)
        {
            std::cerr << "No files specified.\nUsage: lapw <asset source> <output name>\n";
            return EXIT_FAILURE;
        }
        if (argc > 3)
        {
            std::cerr << "Too many arguments.\nUsage: lapw <asset source> <output name>\n";
            return EXIT_FAILURE;
        }
        if (argc != 3)
        {
            std::cerr << "Usage: lapw <asset source> <output name>\n";
            return EXIT_FAILURE;
        }

        const std::string inputPath  = argv[1];
        const std::string outputPath = argv[2];

        if (LAPUtility::CheckFileExists(outputPath))
        {
            bool overrideFile = false;
            std::string chosenValue;

            std::cout << outputPath << " already exists! Override? y/n: ";
            while (true)
            {
                if (!(std::cin >> chosenValue))
                {
                    std::cerr << "Error: failed to read from stdin.\n";
                    return EXIT_FAILURE;
                }

                if (chosenValue == "y")
                {
                    overrideFile = true;
                    break;
                }
                else if (chosenValue == "n")
                {
                    std::cout << "Aborting program.\n";
                    return EXIT_SUCCESS;
                }
                else
                {
                    std::cout << "Please enter y/n (case sensitive): ";
                }
            }
        }

        std::ofstream output("res.lpck", std::ios::binary);
        LAP::Packager pack;

        pack.AddHeader(LAP::Version(1, 0, 0), output);
        pack.AddAsset(argv[1], output);
        pack.AddFooter(output);
        output.close();

        std::cout << "Write complete: " << outputPath << "\n";
        return EXIT_SUCCESS;
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
