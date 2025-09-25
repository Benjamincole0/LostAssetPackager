#pragma once
#include <string>
#include <fstream>
#include <type_traits>
#include <stdexcept>
#include <vector>

namespace LAPCore::Converter
{
    template <typename T>
    void WriteRaw(std::ostream& out, const T& value)
    {
        static_assert(std::is_trivially_copyable_v<T>);
        out.write(reinterpret_cast<const char*>(&value), sizeof(T));
        if (!out) throw std::runtime_error("Failed to write data");
    }

    inline void WriteRaw(std::ostream& out, const std::string& data)
    {
        out.write(data.data(), static_cast<std::streamsize>(data.size()));
        if (!out) throw std::runtime_error("Failed to write string");
    }

    template <typename T>
    T ReadRaw(std::istream& in)
    {
        static_assert(std::is_trivially_copyable_v<T>);
        T value{};
        in.read(reinterpret_cast<char*>(&value), sizeof(T));
        if (!in) throw std::runtime_error("Failed to read data");
        return value;
    }

    inline std::string ReadRawFile(const std::string& path)
    {
        std::ifstream in(path, std::ios::binary);
        if (!in)
            throw std::runtime_error("Failed to open file for reading: " + path);
        return std::string((std::istreambuf_iterator<char>(in)), {});
    }
}
