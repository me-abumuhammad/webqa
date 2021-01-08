#ifndef JENISTRANSAKSICONTROLLER_HPP
#define JENISTRANSAKSICONTROLLER_HPP

#include "basecontroller.hpp"

#include "../db/jenistransaksiprocess.hpp"

class JenisTransaksiController: public drogon::HttpController<JenisTransaksiController, false>, public BaseController
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(JenisTransaksiController::add_jenis_transaksi, "/add", drogon::Post);
    METHOD_LIST_END
    JenisTransaksiController() {}
    explicit JenisTransaksiController(const std::shared_ptr<drogon::orm::DbClient>& db) {
        m_db = std::make_shared<JenistransaksiProcess>(db);
    }

    void add_jenis_transaksi(bangkong::ReqParam::type req,
                             bangkong::RespParam::type callback) const;
private:
    std::shared_ptr<JenistransaksiProcess> m_db;
};

#endif // JENISTRANSAKSI_HPP
