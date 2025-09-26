#pragma once
#include <string>
#include <fstream>
#include <type_traits>
#include <stdexcept>
#include <vector>

namespace LAPCore::Converter
{
    template <typename T>
    void WriteRaw(std::ostream& p_out, const T& p_value)
    {
        static_assert(std::is_trivially_copyable_v<T>);
        p_out.write(reinterpret_cast<const char*>(&p_value), sizeof(T));
        if (!p_out) throw std::runtime_error("Failed to write data");
    }

    inline void WriteRaw(std::ostream& p_out, const std::string& p_data)
    {
        p_out.write(p_data.data(), static_cast<std::streamsize>(p_data.size()));
        if (!p_out) throw std::runtime_error("Failed to write string");
    }

    template <typename T>
    T ReadRaw(std::istream& p_in)
    {
        static_assert(std::is_trivially_copyable_v<T>);
        T value{};
        p_in.read(reinterpret_cast<char*>(&value), sizeof(T));
        if (!p_in) throw std::runtime_error("Failed to read data");
        return value;
    }

    inline std::string ReadRawFile(const std::string& p_path)
    {
        std::ifstream in(p_path, std::ios::binary);
        if (!in)
            throw std::runtime_error("Failed to open file for reading: " + p_path);
        return std::string((std::istreambuf_iterator<char>(in)), {});
    }
}
