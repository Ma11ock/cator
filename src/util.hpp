#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

namespace cator
{
    /* Casting interface for snprintf (below). */
    namespace cast
    {
        template<typename T>
        inline T cast(T v)
        {
            return v;
        }

        inline const char *cast(const std::string &str)
        {
            return str.c_str();
        }

        inline const char *cast(std::string_view str)
        {
            return std::string(str).c_str();
        }

        inline const char *cast(const std::filesystem::path &path)
        {
            return path.generic_string().c_str();
        }
    }

    /* Printf but returns a C++ string. A temporary measure until gcc and
       clang support C++20 fmt. */
    template <typename... Args>
    inline std::string sprintf(const std::string &formatString, Args&&... args)
    {
        std::size_t strlen = static_cast<std::size_t>(
            std::snprintf(nullptr, 0, formatString.c_str(),
                          cator::cast::cast(std::forward<Args>(args))...));

        auto cString = std::make_unique<char[]>(++strlen);
        std::snprintf(cString.get(), strlen, formatString.c_str(),
                      cator::cast::cast(std::forward<Args>(args))...);
        return std::string(cString.get());
    }
    /* Traditional snprintf. Print to char *result. Return number of
       chars written. */
    template <typename... Args>
    inline std::size_t snprintf(char *result, std::size_t size,
                                const std::string &formatString, Args&&... args)
    {
        return std::snprintf(result, size, formatString.c_str(),
                             cator::cast::cast(std::forward<Args>(args))...);
    }

    /* ASPrint implementation. */
    template <typename... Args>
    inline std::size_t asprintf(char *&result, const std::string &formatString,
                                Args&&... args)
    {
        std::size_t strlen = static_cast<std::size_t>(
            std::snprintf(nullptr, 0, formatString.c_str(),
                          cator::cast::cast(std::forward<Args>(args))...));

        if(strlen == 0)
            return 0;

        result = new char[++strlen];

        return std::snprintf(result, strlen, formatString.c_str(),
                             cator::cast::cast(std::forward<Args>(args))...);
    }

    /* Returns true if the string starts with prefix, false if not. */
    inline bool startsWith(std::string_view str, std::string_view prefix)
    {
        return str.rfind(prefix, 0) == 0;
    }

    inline std::string tail(std::string_view source, std::size_t length)
    {
        if(length >= source.size())
            return std::string(source);
        return std::string(source.substr(source.size() - length));
    }

    /* Get a string form the last char until char c.
       If c is not found in the string, return str. */
    inline std::string tailUntil(std::string_view source, char c)
    {
        auto occurance = source.find_last_of(c);
        if(occurance == std::string_view::npos)
            return std::string(source);

        return cator::tail(source, source.size() - occurance - 1);
    }

    inline std::string substrUntil(std::string_view str,
                                   std::string_view::size_type n, char c)
    {
        auto sub = str.substr(n);
        return std::string(sub.substr(0, sub.find_first_of(' ')));
    }

    /* Errno and errstr. */
    inline std::string errStr()
    {
        return cator::sprintf("errno: %d, errstr: %s", errno, std::strerror(errno));
    }

     /* error_code and errstr. */
    inline std::string errStr(std::error_code errc)
    {
        return cator::sprintf("Error code: %d, string: \"%s\"",
                              errc.value(), errc.message());
    }

}
