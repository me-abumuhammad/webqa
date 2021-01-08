#ifndef JENISTRANSAKSI_HPP
#define JENISTRANSAKSI_HPP

#include <string_view>
#include <string>

#include "absmodel.hpp"
#include "buku.hpp"

class JenisTransaksi: public AbsModel
{
public:
    JenisTransaksi();
    JenisTransaksi(std::string_view nama, unsigned short jenis, std::vector<Buku> buku)
      : m_nama{nama}, m_jenis{jenis}, m_buku{buku} {}
    JenisTransaksi(int id, std::string_view nama, unsigned short jenis, std::vector<Buku> buku)
      : m_id_jenis_transaksi{id}, m_nama{nama}, m_jenis{jenis}, m_buku{buku} {}
    ~JenisTransaksi() {}

    JenisTransaksi& operator=(const JenisTransaksi& other) {
        m_nama = other.m_nama;
        m_jenis = other.m_jenis;
        m_id_jenis_transaksi = other.m_id_jenis_transaksi;
        m_buku = other.m_buku;
        return *this;
    }

    JenisTransaksi(const JenisTransaksi& other) {
        m_nama = other.m_nama;
        m_jenis = other.m_jenis;
        m_id_jenis_transaksi = other.m_id_jenis_transaksi;
        m_buku = other.m_buku;
    }

    JenisTransaksi& operator=(JenisTransaksi&& other) {
        m_nama = std::move(other.m_nama);
        m_jenis = std::move(other.m_jenis);
        m_id_jenis_transaksi = std::move(other.m_id_jenis_transaksi);
        m_buku.erase(m_buku.begin(), m_buku.end());
        m_buku.shrink_to_fit();

        m_buku.reserve(other.m_buku.size());
        m_buku.insert(m_buku.begin(),
                      std::make_move_iterator(other.m_buku.begin()),
                      std::make_move_iterator(other.m_buku.end()));

        return *this;
    }

    JenisTransaksi(JenisTransaksi&& other) {
        m_nama = std::move(other.m_nama);
        m_jenis = std::move(other.m_jenis);
        m_id_jenis_transaksi = std::move(other.m_id_jenis_transaksi);
        m_buku.erase(m_buku.begin(), m_buku.end());
        m_buku.shrink_to_fit();

        m_buku.reserve(other.m_buku.size());
        m_buku.insert(m_buku.begin(),
                      std::make_move_iterator(other.m_buku.begin()),
                      std::make_move_iterator(other.m_buku.end()));
    }

    void set_nama(std::string_view nama) {
        m_nama = nama;
    }
    void set_jenis(unsigned short jenis) {
        m_jenis = jenis;
    }
    void set_id_jenis_transaksi(int id) {
        m_id_jenis_transaksi = id;
    }
    void set_name(std::string_view nama) {
        m_nama = nama;
    }
    void set_buku(std::vector<Buku> buku) {
        m_buku = buku;
    }

    int get_id_jenis_transaksi() const {
        return m_id_jenis_transaksi;
    }
    std::string get_nama() const {
        return m_nama;
    }
    unsigned short get_jenis() const {
        return m_jenis;
    }
    std::vector<Buku> get_buku() {
        return m_buku;
    }
    Json::Value get_data_json() const override {
        Json::Value json;
        json["nama"] = m_nama;
        json["jenis"] = Json::Int(m_jenis);
        json["id_jenis_transaksi"] = Json::Int(m_id_jenis_transaksi);
        Json::Value arr(Json::arrayValue);
        int i = 0;
        for (const Buku& b: m_buku) {
            arr[i] = b.get_data_json();
            ++i;
        }
        json["buku"] = arr;
        return json;
    }

private:
    int m_id_jenis_transaksi;
    std::string m_nama;
    unsigned short m_jenis;
    std::vector<Buku> m_buku;
};

#endif // JENISTRANSAKSI_HPP
