#include <iostream>
#include "verifheader.hpp"

#include <vector>

#include <boost/algorithm/string.hpp>
#include <jwt.h>

#include "utils.hpp"
#include "jsonconverter.hpp"

bool VerifHeader::check_authorization_token(std::string_view token, std::string& message) {
    std::vector<std::string> splt_token = bangkong::split_string<std::string>(token.data(), " ");
    if (splt_token.size() > 1) {
        if (m_db->check_token(splt_token.at(1)) == false) {
            message = "Token tidak valid";
            return false;
        }
        else {
            if (check_timeout(token, message) == true) {
                auto decoded = bangkong::decode_jwt_string(token);
                bool status = false;
                std::unordered_map<std::string, jwt::claim> payloads = decoded.get_payload_claims();
                std::string_view usr = payloads["username"].as_string();
                m_db->check_username(usr, [&status](bool s){
                    status = s;
                });
                if (status == false) {
                    message = "Username tidak ada";
                    return status;
                }
                else {
                    return true;
                }
            }
        }
    }
    else {
        message = "Token tidak valid";
    }
    return false;
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

