#include <iostream>
#include "Unpacker.h"

int main()
{
    Unpacker::ListAssets("output.lpck");
    Unpacker::GetFooterOffset("output.lpck");
    std::cout << Unpacker::footerOffset << std::endl;
    Unpacker::ExtractAsset("output.lpck", "./res/test.txt", "output.txt");
}