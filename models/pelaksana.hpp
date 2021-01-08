#ifndef PELAKSANA_HPP
#define PELAKSANA_HPP

#include "absmodel.hpp"

#include <string_view>

class Pelaksana: public AbsModel
{
public:
    Pelaksana();
    explicit Pelaksana(std::string_view nama, int id_pelaksana, std::string_view inisial):
        m_nama{nama.data()}, m_id_pelaksana{id_pelaksana}, m_inisial{inisial.data()} {}

    Pelaksana& operator=(const Pelaksana& other) {
        m_nama = other.m_nama;
        m_id_pelaksana = other.m_id_pelaksana;
        m_inisial = other.m_inisial;
        return *this;
    }

    Pelaksana(const Pelaksana& other) {
        m_nama = other.m_nama;
        m_id_pelaksana = other.m_id_pelaksana;
        m_inisial = other.m_inisial;
    }

    Pelaksana& operator=(Pelaksana&& other) {
        m_nama = std::move(other.m_nama);
        m_id_pelaksana = std::move(other.m_id_pelaksana);
        m_inisial = std::move(other.m_inisial);

        return *this;
    }

    Pelaksana(Pelaksana&& other) {
        m_nama = std::move(other.m_nama);
        m_id_pelaksana = std::move(other.m_id_pelaksana);
        m_inisial = std::move(other.m_inisial);
    }

    Json::Value get_data_json() const override {
        Json::Value res;
        res["nama"] = m_nama;
        res["id_pelaksana"] = Json::Int(m_id_pelaksana);
        res["inisial"] = m_inisial;
        return res;
    }
private:
    std::string m_nama;
    int m_id_pelaksana;
    std::string m_inisial;
};

#endif // PELAKSANA_HPP
