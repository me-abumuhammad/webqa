#ifndef JENISTRANSAKSIPROCESS_HPP
#define JENISTRANSAKSIPROCESS_HPP

#include <functional>

#include "bukuprocess.hpp"

#include "../models/jenistransaksi.hpp"

class JenistransaksiProcess: public BaseProcess
{
public:

    using JenisTransaksiResult = std::function<void(std::optional<JenisTransaksi>)>;
    JenistransaksiProcess() = delete ;
    JenistransaksiProcess(const JenistransaksiProcess&) = delete ;
    JenistransaksiProcess(JenistransaksiProcess&&) = delete ;
    ~JenistransaksiProcess() {}
    JenistransaksiProcess(const std::shared_ptr<drogon::orm::DbClient>& db): BaseProcess(db), m_db{db} {
        m_buku_process = std::make_unique<BukuProcess>(db);
    }
    void add(JenisTransaksi&&, JenisTransaksiResult&&, std::string&);
    void get_jenis_transaksi_by_id(int, JenisTransaksiResult&&);
    void update(JenisTransaksi&&, JenisTransaksiResult&&);
    std::vector<JenisTransaksi> get_all_jenis_transaksi(std::string_view kode = "D");
private:
    bool check_jenis_process(std::string_view name);
    std::optional<JenisTransaksi> get_last_jenis_transaksi() const;
    std::vector<Buku> get_buku_by_id_jenis(int id) const;
private:
    const std::shared_ptr<drogon::orm::DbClient>& m_db;
    std::unique_ptr<BukuProcess> m_buku_process;
};

#endif // JENISTRANSAKSIPROCESS_HPP
