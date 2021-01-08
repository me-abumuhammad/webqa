#ifndef REKAP_HPP
#define REKAP_HPP

#include <string>
#include <string_view>
#include "absmodel.hpp"

class Rekap: public AbsModel
{
public:
    Rekap();
    explicit Rekap(std::string_view code,
                   std::string_view name,
                   uint64_t nominal):
        m_code {code},
        m_name {name},
        m_nominal {nominal} {}

    Rekap& operator=(const Rekap& other) {
        m_code = other.m_code;
        m_name = other.m_name;
        m_nominal = other.m_nominal;
        return *this;
    }
    Rekap(const Rekap& other) {
        m_code = other.m_code;
        m_name = other.m_name;
        m_nominal = other.m_nominal;
    }

    Rekap& operator=(Rekap&& other) {
        m_code = std::move(other.m_code);
        m_name = std::move(other.m_name);
        m_nominal = other.m_nominal;
        return *this;
    }

    Rekap(Rekap&& other) {
        m_code = std::move(other.m_code);
        m_name = std::move(other.m_name);
        m_nominal = other.m_nominal;
    }

    void set_code(std::string_view code) {
        m_code = code;
    }
    void set_name(std::string_view name) {
        m_name = name;
    }
    void set_code(uint64_t nominal) {
        m_nominal = nominal;
    }

    std::string get_code() const {
        return m_code;
    }

    std::string get_name() const {
        return m_name;
    }

    uint64_t get_nominal() const {
        return m_nominal;
    }

    Json::Value get_data_json() const override {
        Json::Value json;
        json["code"] = m_code;
        json["name"] = m_name;
        json["nominal"] = Json::UInt64(m_nominal);
        return json;
    }
private:
    std::string m_code;
    std::string m_name;
    uint64_t m_nominal;
};

#endif // REKAP_HPP
