#include "utils.hpp"

namespace bangkong {
jwt::decoded_jwt decode_jwt_string(std::string_view token) {
    std::vector<std::string> dta = split_string<std::string>(token.data(), " ");
    std::string& t = dta.at(1);
    auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256(bangkong::env<std::string>::value(bangkong::HAS_KEY)))
            .with_issuer("auth0");
    auto decoded = jwt::decode(t);
    return decoded;
}
}
