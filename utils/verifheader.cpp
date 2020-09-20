#include <iostream>
#include "verifheader.hpp"

#include <vector>

#include <boost/algorithm/string.hpp>
#include <jwt.h>

#include "utils.hpp"
#include "jsonconverter.hpp"

bool VerifHeader::check_authorization_token(std::string_view token, std::string& message) {
    std::vector<std::string> broker;
    std::string tkn = token.data();
    boost::algorithm::split(broker, tkn, boost::is_any_of(" "));

    std::string& t = broker.at(1);
    auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256(bangkong::env<std::string>::value(bangkong::HAS_KEY)))
            .with_issuer("auth0");
    auto decoded = jwt::decode(t);
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
