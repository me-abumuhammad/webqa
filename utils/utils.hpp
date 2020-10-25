#ifndef UTILS_HPP
#define UTILS_HPP

#include <utility>
#include <type_traits>
#include <string_view>
#include <boost/lexical_cast.hpp>
#include <jwt.h>
#include <boost/algorithm/string.hpp>

namespace bangkong {
constexpr static const char* ID_JWT = "ID_JWT";
constexpr static const char* HAS_KEY = "HAS_KEY";
constexpr static const char* DB_CONNECTION = "DB_CONNECTION";
constexpr static const char* TIME_SESSION = "TIME_SESSION";
constexpr static int TIME_OF_SESSION = 7200;
constexpr static const char* BASE_URL = "http://localhost:17000/";

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

template <typename T>
using EnableIfString = std::enable_if_t<
                            std::is_convertible_v<T, std::string>>;

template <typename T,
          template <typename Elem,
                    typename Alloc = std::allocator<Elem>>
          class Cont = std::vector,
          typename = EnableIfString<T>>
Cont<T> split_string(const T& msg, std::string_view delim = " ") {
    Cont<T> broker;
    boost::algorithm::split(broker, msg, boost::is_any_of(delim.data()));
    return broker;
}

jwt::decoded_jwt decode_jwt_string(std::string_view token);
}
#endif // UTILS_HPP
