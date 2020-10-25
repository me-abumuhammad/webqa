#include <iostream>
#include "verifheader.hpp"

#include <vector>

#include <boost/algorithm/string.hpp>
#include <jwt.h>

#include "utils.hpp"
#include "jsonconverter.hpp"

bool VerifHeader::check_authorization_token(std::string_view token, std::string& message) {
    auto decoded = bangkong::decode_jwt_string(token);
    auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256(bangkong::env<std::string>::value(bangkong::HAS_KEY)))
            .with_issuer("auth0");
    try {
        verifier.verify(decoded);
        std::unordered_map<std::string, jwt::claim> payloads = decoded.get_payload_claims();
        bool status = false;
        std::string_view usr = payloads["username"].as_string();
        m_db->check_username(usr, [&status](bool s){
            status = s;
        });
        return status;
    } catch (const jwt::token_verification_exception& e) {
        message = e.what();
        return false;
    }

    return true;
}

bool VerifHeader::check_timeout(std::string_view token, std::string &message) {
    auto decoded = bangkong::decode_jwt_string(token);
    auto expire = decoded.get_expires_at();
    auto diff = expire - std::chrono::system_clock::now();
    if (diff.count() < bangkong::TIME_OF_SESSION) {
        message = "Session timeout";
        return false;
    }
    else {
        return true;
    }
}

