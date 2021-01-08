#ifndef MUHSININ_HPP
#define MUHSININ_HPP

#include "absmodel.hpp"

#include <string_view>

class Muhsinin: public AbsModel
{
public:
    Muhsinin();
    explicit Muhsinin(int id_muhsinin, std::string_view nama, std::string_view inisial)
        :m_id_muhsinin{id_muhsinin}, m_nama{nama}, m_inisial{inisial} {}

    Muhsinin& operator=(const Muhsinin& other) {
        m_id_muhsinin = other.m_id_muhsinin;
        m_nama = other.m_nama;
        m_inisial = other.m_inisial;
        return *this;
    }

    Muhsinin(const Muhsinin& other) {
        m_id_muhsinin = other.m_id_muhsinin;
        m_nama = other.m_nama;
        m_inisial = other.m_inisial;
    }

    Muhsinin& operator=(Muhsinin&& other) {
        m_id_muhsinin = std::move(other.m_id_muhsinin);
        m_nama = std::move(other.m_nama);
        m_inisial = std::move(other.m_inisial);
        return *this;
    }

    Muhsinin(Muhsinin&& other) {
        m_id_muhsinin = std::move(other.m_id_muhsinin);
        m_nama = std::move(other.m_nama);
        m_inisial = std::move(other.m_inisial);
    }

    Json::Value get_data_json() const override {
        Json::Value res;
        res["nama"] = m_nama;
        res["alias"] = m_inisial;
        res["id_muhsinin"] = m_id_muhsinin;

        return res;
    }
private:
    int m_id_muhsinin;
    std::string m_nama;
    std::string m_inisial;
};

#endif // MUHSININ_HPP
