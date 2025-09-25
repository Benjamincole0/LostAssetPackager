#pragma once
#include <string>
#include <fstream>
#include <type_traits>
#include <stdexcept>
#include <vector>

namespace LAPCore::Converter
{
    namespace detail
    {
        inline std::string AppendLpckIfNeeded(const std::string& path, bool addLpck)
        {
            constexpr std::string_view ext = ".lpck";
            if (!addLpck)
                return path;
            if (path.size() >= ext.size() &&
                path.compare(path.size() - ext.size(), ext.size(), ext) == 0)
                return path;
            return path + std::string(ext);
        }

        inline std::ofstream OpenOutFile(const std::string& path)
        {
            std::ofstream out(path, std::ios::binary | std::ios::trunc);
            if (!out.is_open())
                throw std::runtime_error("Failed to open file for writing: " + path);
            return out;
        }

        inline std::ifstream OpenInFile(const std::string& path)
        {
            std::ifstream in(path, std::ios::binary);
            if (!in.is_open())
                throw std::runtime_error("Failed to open file for reading: " + path);
            return in;
        }
    }

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

    inline std::string ReadRaw(const std::string& path)
    {
        std::ifstream in(path, std::ios::binary);
        if (!in)
            throw std::runtime_error("Failed to open file for reading: " + path);
        return std::string((std::istreambuf_iterator<char>(in)),
                        std::istreambuf_iterator<char>());
    }

    inline std::string ReadRawString(std::istream& in)
    {
        in.seekg(0, std::ios::end);
        std::streamsize size = in.tellg();
        in.seekg(0, std::ios::beg);

        std::string buffer(static_cast<size_t>(size), '\0');
        if (!in.read(buffer.data(), size))
            throw std::runtime_error("Failed to read string");
        return buffer;
    }

    template <typename T>
    void WriteRawToFile(const std::string& filePath, const T& value, bool addLpck = true)
    {
        auto path = detail::AppendLpckIfNeeded(filePath, addLpck);
        auto out = detail::OpenOutFile(path);
        WriteRaw(out, value);
    }

    inline void WriteRawToFile(const std::string& filePath, const std::string& data, bool addLpck = true)
    {
        auto path = detail::AppendLpckIfNeeded(filePath, addLpck);
        auto out = detail::OpenOutFile(path);
        WriteRaw(out, data);
    }

    template <typename T>
    T ReadRawFromFile(const std::string& filePath, bool addLpck = true)
    {
        auto path = detail::AppendLpckIfNeeded(filePath, addLpck);
        auto in = detail::OpenInFile(path);
        return ReadRaw<T>(in);
    }

    inline std::string ReadRawStringFromFile(const std::string& filePath, bool addLpck = true)
    {
        auto path = detail::AppendLpckIfNeeded(filePath, addLpck);
        auto in = detail::OpenInFile(path);
        return ReadRawString(in);
    }

    inline void BatchWrite(const std::string& outPath, const std::vector<std::string>& filepaths, bool addLpck = true)
    {
        auto path = detail::AppendLpckIfNeeded(outPath, addLpck);
        auto out = detail::OpenOutFile(path);

        for (const auto& assetPath : filepaths)
        {
            std::ifstream in(assetPath, std::ios::binary);
            if (!in) throw std::runtime_error("Failed to open input file: " + assetPath);

            out << in.rdbuf();
        }
    }
}
