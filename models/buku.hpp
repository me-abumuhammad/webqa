#ifndef BUKU_HPP
#define BUKU_HPP

#include "absmodel.hpp"

#include <string_view>

class Buku: public AbsModel
{
public:
    Buku();
    Buku(std::string_view nama, bool status): m_nama{nama}, m_status{status} {}
    Buku(int id, std::string_view nama, bool status): m_nama{nama}, m_status{status}, m_id_buku{id} {}
    void set_nama(std::string_view nama) {
        m_nama = nama.data();
    }
    void set_status(bool status) {
        m_status = status;
    }
    void set_id_buku(int id) {
        m_id_buku = id;
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
    Json::Value get_data_json() const override {
         Json::Value json;
         json["nama"] = m_nama;
         json["status"] = m_status;
         json["id_buku"] = Json::Int(m_id_buku);
         return json;
    }
private:
    std::string m_nama;
    bool m_status;
    int m_id_buku;
};

#endif // BUKU_HPP
