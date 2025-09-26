#pragma once
#include <stdint.h>
#include <tuple>

namespace LAP
{
    struct Version
    {
        Version(uint16_t p_major, uint16_t p_minor, uint16_t p_patch)
        {
            major = p_major;
            minor = p_minor;
            patch = p_patch;
        }

        uint16_t major;
        uint16_t minor;
        uint16_t patch;

        bool operator<(const Version& p_other) const
        {
            return std::tie(major, minor, patch)
                < std::tie(p_other.major, p_other.minor, p_other.patch);
        }

    };
}