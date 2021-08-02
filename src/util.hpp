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
}
