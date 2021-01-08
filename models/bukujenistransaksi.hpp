#ifndef BUKUJENISTRANSAKSI_HPP
#define BUKUJENISTRANSAKSI_HPP

#include "absmodel.hpp"

class BukuJenisTransaksi: public AbsModel
{
public:
    BukuJenisTransaksi();
    explicit BukuJenisTransaksi(int id, int buku_id, int jenis_transaksi, std::string_view tipe):
        m_buku_id{buku_id}, m_jenis_transaksi_id{jenis_transaksi}, m_tipe{tipe.data()}, m_id_buku_transaksi{id} {}
    ~BukuJenisTransaksi() {}
    BukuJenisTransaksi& operator=(const BukuJenisTransaksi& other) {
        m_buku_id = other.m_buku_id;
        m_jenis_transaksi_id = other.m_jenis_transaksi_id;
        m_tipe = other.m_tipe;
        m_id_buku_transaksi = other.m_id_buku_transaksi;

        return *this;
    }

    BukuJenisTransaksi(const BukuJenisTransaksi& other) {
        m_buku_id = other.m_buku_id;
        m_jenis_transaksi_id = other.m_jenis_transaksi_id;
        m_tipe = other.m_tipe;
        m_id_buku_transaksi = other.m_id_buku_transaksi;
    }

    BukuJenisTransaksi& operator=(BukuJenisTransaksi&& other) {
        m_buku_id = std::move(other.m_buku_id);
        m_jenis_transaksi_id = std::move(other.m_jenis_transaksi_id);
        m_tipe = std::move(other.m_tipe);
        m_id_buku_transaksi = std::move(other.m_id_buku_transaksi);

        return *this;
    }

    BukuJenisTransaksi(BukuJenisTransaksi&& other) {
        m_buku_id = std::move(other.m_buku_id);
        m_jenis_transaksi_id = std::move(other.m_jenis_transaksi_id);
        m_tipe = std::move(other.m_tipe);
        m_id_buku_transaksi = std::move(other.m_id_buku_transaksi);
    }

    int get_buku_id() const {
        return m_buku_id;
    }

    int get_jenis_transaksi_id() const {
        return m_jenis_transaksi_id;
    }

    std::string get_tipe() const {
        return m_tipe;
    }

    int get_id_buku_transakasi() const {
        return m_id_buku_transaksi;
    }

    Json::Value get_data_json() const override {
        Json::Value json;
        return json;
    }
private:
    int m_buku_id;
    int m_jenis_transaksi_id;
    std::string m_tipe;
    int m_id_buku_transaksi;
};

#endif // BUKUJENISTRANSAKSI_HPP
