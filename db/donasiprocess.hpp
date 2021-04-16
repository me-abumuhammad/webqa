#ifndef DONASIPROCESS_HPP
#define DONASIPROCESS_HPP

#include "baseprocess.hpp"

#include "../models/muhsinin.hpp"
#include "../models/pelaksana.hpp"
#include "../models/sumberdana.hpp"
#include "../models/bukujenistransaksi.hpp"

#include <unordered_map>

class DonasiProcess: public BaseProcess
{
public:
    DonasiProcess(const std::shared_ptr<drogon::orm::DbClient>& db): BaseProcess(db), m_db{db} {}
    DonasiProcess(const DonasiProcess&) = delete ;
    DonasiProcess(DonasiProcess&&) = delete ;
    DonasiProcess() = delete ;
    ~DonasiProcess() {}

    std::vector<Muhsinin> get_all_muhsinin();
    std::vector<Pelaksana> get_all_pelaksana();
    std::vector<SumberDana> get_all_sumberdana() const;

    int current_number(int jenis_donasi) const;
    bool update_current_number(int jenis_donasi, int no) const;

    bool save_donasi(bangkong::DonasiData&& data) const;
    bool update_donasi(Json::Value&& data) const;
    bool delete_donasi(std::string_view nomor) const;
    bool save_kredit(Json::Value&& data) const;
    bool update_kredit(Json::Value&& data) const;
    bool delete_transaksi(std::string_view nomor) const;

    Json::Value get_all_donasi() const;
    Json::Value get_donasi_by_nomor(std::string_view nomor) const;
    Json::Value get_transaksi_by_nomor(std::string_view nomor) const;
    Json::Value get_all_transaction() const;
    Json::Value get_detail_buku(int buku_id) const;
    Json::Value get_saldo_sumber_dana() const;

private:
    bool save_transaksi_donasi(std::tuple<int64_t, int, int, std::string, std::string, int, int>&&) const;
    bool update_transaksi_donasi(std::tuple<int64_t, int, int, std::string, std::string, int, int>&&) const;
    bool save_transaksi(std::tuple<int64_t, int, int, int, std::string, std::string, int, int>&&, const Json::Value& json) const;
    bool update_transaksi(std::tuple<int64_t, int, int, std::string, int, int>&&, const Json::Value& json) const;
    int get_id_donasi_by_nomor(const std::string& nomor) const;
    int get_num_transaksi_by_nomor(std::string_view nomor) const;
    int get_num_debet_by_nomor(std::string_view nomor) const;
    int current_index_kredit() const;
    int current_zero_index_kredit() const;
    std::vector<BukuJenisTransaksi> get_buku_transkasi_by_jenis(int id_jenis) const;
    int get_id_kredit_by_nomor(const std::string &nomor) const;
    uint64_t total_debet_by_buku(int buku_id) const;
    uint64_t total_kredit_by_buku(int buku_id) const;


private:
    const std::shared_ptr<drogon::orm::DbClient>& m_db;
};

#endif // DONASIPROCESS_HPP
