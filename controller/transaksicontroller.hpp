#ifndef TRANSAKSICONTROLLER_HPP
#define TRANSAKSICONTROLLER_HPP

#include "../db/dbprocess.hpp"
#include "../db/donasiprocess.hpp"
#include "../db/jenistransaksiprocess.hpp"
#include "basecontroller.hpp"

class TransaksiController: public drogon::HttpController<TransaksiController, false>, public BaseController
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(TransaksiController::form_transaksi, "/form_transaksi", drogon::Get);
    METHOD_ADD(TransaksiController::save_transaksi, "/save_transaksi", drogon::Post);
    METHOD_ADD(TransaksiController::all_transaction, "/all_transaksi", drogon::Get);
    METHOD_ADD(TransaksiController::transaksi, "/donasi/{nomor}", drogon::Get);
    METHOD_ADD(TransaksiController::update_transaksi, "/update_transaksi", drogon::Post);
    METHOD_ADD(TransaksiController::delete_transaksi, "/delete_transaksi", drogon::Post, drogon::Options, "CorsFilter");
    METHOD_LIST_END
    explicit TransaksiController();
    explicit TransaksiController(const std::shared_ptr<drogon::orm::DbClient>& db) {
        m_db = std::make_shared<DbProcess>(db);
        m_donasi = std::make_unique<DonasiProcess>(db);
        m_jenis_transaksi = std::make_unique<JenistransaksiProcess>(db);
    }

    void form_transaksi(bangkong::ReqParam::type req,
                        bangkong::RespParam::type callback) const;

    void save_transaksi(bangkong::ReqParam::type req,
                        bangkong::RespParam::type callback) const;

    void all_transaction(bangkong::ReqParam::type req,
                         bangkong::RespParam::type callback) const;

    void transaksi(bangkong::ReqParam::type req,
                   bangkong::RespParam::type callback, std::string nomor) const;

    void update_transaksi(bangkong::ReqParam::type req,
                          bangkong::RespParam::type callback) const;

    void delete_transaksi(bangkong::ReqParam::type req,
                       bangkong::RespParam::type callback) const;
private:
    template<typename T>
    Json::Value parse_to_json_array(std::vector<T>&& data) const;
private:
    std::shared_ptr<DbProcess> m_db;
    std::unique_ptr<DonasiProcess> m_donasi;
    std::unique_ptr<JenistransaksiProcess> m_jenis_transaksi;
};

#endif // TRANSAKSICONTROLLER_HPP
