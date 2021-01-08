#ifndef SUMBERDANA_HPP
#define SUMBERDANA_HPP

#include "absmodel.hpp"
#include <string_view>
#include "hitbase.hpp"

class SumberDana: public AbsModel, public HitBase
{
public:
    SumberDana();
    explicit SumberDana(int id_sumber_dana, std::string_view nama, std::string_view kode, std::string_view bg):
    m_id_sumber_dana{id_sumber_dana}, m_nama{nama}, m_kode{kode}, m_bg{bg}, m_buku_id{0} {}

    SumberDana& operator=(const SumberDana& other) {
        m_id_sumber_dana = other.m_id_sumber_dana;
        m_nama = other.m_nama;
        m_kode = other.m_kode;
        m_bg = other.m_bg;
        return *this;
    }

    SumberDana(const SumberDana& other) {
        m_id_sumber_dana = other.m_id_sumber_dana;
        m_nama = other.m_nama;
        m_kode = other.m_kode;
        m_bg = other.m_bg;
    }

    SumberDana& operator=(SumberDana&& other) {
        m_id_sumber_dana = std::move(other.m_id_sumber_dana);
        m_nama = std::move(other.m_nama);
        m_kode = std::move(other.m_kode);
        m_bg = std::move(other.m_bg);
        return *this;
    }

    SumberDana(SumberDana&& other) {
        m_id_sumber_dana = std::move(other.m_id_sumber_dana);
        m_nama = std::move(other.m_nama);
        m_kode = std::move(other.m_kode);
        m_bg = std::move(other.m_bg);
    }

    Json::Value get_data_json() const override {
        Json::Value res;
        res["nama"] = m_nama;
        res["kode"] = m_kode;
        res["id_sumber_dana"] = Json::Int(m_id_sumber_dana);
        res["bg"] = m_bg;
        return res;
    }

    void set_buku_id(int buku_id) {
        m_buku_id = buku_id;
    }

    uint64_t count_debet(const std::weak_ptr<drogon::orm::DbClient> &db) const override;
    uint64_t count_kredit(const std::weak_ptr<drogon::orm::DbClient> &db) const override;

private:
    int m_id_sumber_dana;
    std::string m_nama;
    std::string m_kode;
    std::string m_bg;
    int m_buku_id;
};

#endif // SUMBERDANA_HPP
