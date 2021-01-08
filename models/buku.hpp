#ifndef BUKU_HPP
#define BUKU_HPP

#include "absmodel.hpp"
#include "hitbase.hpp"

#include <string_view>

class Buku: public AbsModel, public HitBase
{
public:
    Buku();
    explicit Buku(std::string_view nama, bool status, std::string_view bg): m_nama{nama}, m_status{status}, m_bg{bg} {}
    explicit Buku(int id, std::string_view nama, bool status, std::string_view bg): m_nama{nama}, m_status{status}, m_id_buku{id}, m_bg{bg} {}
    ~Buku() {}

    Buku& operator=(const Buku& other) {
        m_id_buku = other.m_id_buku;
        m_nama = other.m_nama;
        m_status = other.m_status;
        m_bg = other.m_bg;

        return *this;
    }

    Buku(const Buku& other) {
        m_id_buku = other.m_id_buku;
        m_nama = other.m_nama;
        m_status = other.m_status;
        m_bg = other.m_bg;
    }

    Buku& operator=(Buku&& other) {
        m_id_buku = std::move(other.m_id_buku);
        m_nama = std::move(other.m_nama);
        m_status = std::move(other.m_status);
        m_bg = other.m_bg;
        return *this;
    }

    Buku(Buku&& other) {
        m_id_buku = std::move(other.m_id_buku);
        m_nama = std::move(other.m_nama);
        m_status = std::move(other.m_status);
        m_bg = std::move(other.m_bg);
    }
    void set_nama(std::string_view nama) {
        m_nama = nama.data();
    }
    void set_status(bool status) {
        m_status = status;
    }
    void set_id_buku(int id) {
        m_id_buku = id;
    }
    void set_bg(std::string_view bg) {
        m_bg = bg.data();
    }
    std::string get_nama() const {
        return m_nama;
    }
    bool get_status() const {
        return m_status;
    }
    int get_id_buku() const {
        return m_id_buku;
    }
    std::string get_bg() const {
        return m_bg;
    }
    Json::Value get_data_json() const override {
         Json::Value json;
         json["nama"] = m_nama;
         json["status"] = m_status;
         json["id_buku"] = Json::Int(m_id_buku);
         json["bg"] = m_bg;
         return json;
    }

    uint64_t count_debet(const std::weak_ptr<drogon::orm::DbClient>& db) const override;
    uint64_t count_kredit(const std::weak_ptr<drogon::orm::DbClient>& db) const override;

private:
    std::string m_nama;
    bool m_status;
    int m_id_buku;
    std::string m_bg;

};

#endif // BUKU_HPP
