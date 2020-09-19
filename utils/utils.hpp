#ifndef UTILS_HPP
#define UTILS_HPP

#include <string_view>
#include <boost/lexical_cast.hpp>

namespace bangkong {
constexpr static const char* ID_JWT = "ID_JWT";
constexpr static const char* HAS_KEY = "HAS_KEY";
constexpr static const char* DB_CONNECTION = "DB_CONNECTION";

template <typename T>
struct env {
    static T value(std::string_view key) {
        char* val = std::getenv(key.data());
        if (val == NULL)
            return boost::lexical_cast<T>(0);
        else {
            return boost::lexical_cast<T>(val);
        }
    }
};
}
#endif // UTILS_HPP
