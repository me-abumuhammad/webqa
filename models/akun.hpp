#ifndef AKUN_HPP
#define AKUN_HPP

#include <string_view>
#inlude <string>

#include "../utils/sha256.hpp"
#include "../utils/utils.hpp"
#include "absmodel.hpp"
#include "absmodel.hpp"
#include <jwt.h>

class Akun: public AbsModel
{
public:
    Akun();
    ~Akun();
    explicit Akun(std::string_view username,
                  std::string_view password,
                  std::string_view name):
        m_username {username},
        m_password {sha256(password.data())},
        m_name {name} {}
    Akun(const Akun& other) {
        m_username = other.m_username;
        m_password = other.m_password;
        m_name = other.m_name;
    }
    Akun& operator=(const Akun& other) {
        m_username = other.m_username;
        m_password = other.m_password;
        m_name = other.m_name;
        return *this;
    }
    Akun(Akun&& other) {
        m_username = std::move(other.m_username);
        m_password = std::move(other.m_password);
        m_name = std::move(other.m_name);
    }
    Akun& operator=(Akun&& other) {
        m_username = std::move(other.m_username);
        m_password = std::move(other.m_password);
        m_name = std::move(other.m_name);
        return *this;
    }
    void set_username(std::string_view username) {
        m_username = username;
    }

    void set_password(std::string_view password) {
        m_password = sha256(password.data());
    }
    void set_name(std::string_view name) {
        m_name = name;
    }
    std::string get_username() const {
        return m_username;
    }
    std::string get_name() const {
        return m_name;
    }

    std::string get_password() const {
        return m_password;
    }

    std::string get_token() const {
        return m_token;
    }

    Json::Value get_data_json() const override {
        Json::Value json;
        json["username"] = m_username;
        json["name"] = m_name;
        if (m_username != "") {
            auto token = jwt::create()
                    .set_id(bangkong::env<std::string>::value(bangkong::ID_JWT))
                    .set_issuer("auth0")
                    .set_type("JWS")
                    .set_issued_at(std::chrono::system_clock::now())
                    .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{7200})
                    .set_payload_claim("username", jwt::claim(m_username))
                    .sign(jwt::algorithm::hs256{bangkong::env<std::string>::value(bangkong::HAS_KEY)});
            json["token"] = token;
        }
        return json;
    }
private:
    std::string m_username;
    std::string m_password;
    std::string m_name;
    std::string m_token;
};

#endif // AKUN_HPP
