#ifndef DONASICONTROLLER_HPP
#define DONASICONTROLLER_HPP

#include "../db/dbprocess.hpp"
#include "../db/donasiprocess.hpp"
#include "../db/jenistransaksiprocess.hpp"
#include "basecontroller.hpp"

class DonasiController: public drogon::HttpController<DonasiController, false>, public BaseController
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(DonasiController::login, "/login", drogon::Post);
    METHOD_ADD(DonasiController::update_rekap, "/update", drogon::Post);
    METHOD_ADD(DonasiController::all_rekap, "/rekap", drogon::Get);
    METHOD_ADD(DonasiController::rekap_for_bot, "/list", drogon::Get);
    METHOD_ADD(DonasiController::form_donasi, "/form_donasi", drogon::Get);
    METHOD_ADD(DonasiController::save_donasi, "/save_donasi", drogon::Post);
    METHOD_ADD(DonasiController::all_donasi, "/all_donasi", drogon::Get);
    METHOD_ADD(DonasiController::donasi, "/donasi/{nomor}", drogon::Get);
    METHOD_ADD(DonasiController::update_donasi, "/update_donasi", drogon::Post);
    METHOD_ADD(DonasiController::delete_donasi, "/delete_donasi", drogon::Post, drogon::Options, "CorsFilter");
    METHOD_LIST_END

    explicit DonasiController() {}

    explicit DonasiController(const std::shared_ptr<drogon::orm::DbClient>& db) {
        m_db = std::make_shared<DbProcess>(db);
        m_donasi = std::make_unique<DonasiProcess>(db);
        m_jenis_transaksi = std::make_unique<JenistransaksiProcess>(db);
    }

    void login(const drogon::HttpRequestPtr& req,
               std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;
    void update_rekap(const drogon::HttpRequestPtr& req,
                      std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;
    void all_rekap(const drogon::HttpRequestPtr& req,
                      std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;
    void rekap_for_bot(const drogon::HttpRequestPtr& req,
                       std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;
    void form_donasi(bangkong::ReqParam::type req,
                     bangkong::RespParam::type callback) const;

    void save_donasi(bangkong::ReqParam::type req,
                     bangkong::RespParam::type callback, bangkong::DonasiData&& donasi) const;

    void update_donasi(bangkong::ReqParam::type req,
                       bangkong::RespParam::type callback) const;

    void all_donasi(bangkong::ReqParam::type req,
                    bangkong::RespParam::type callback) const;

    void donasi(bangkong::ReqParam::type req,
                bangkong::RespParam::type callback, std::string nomor) const;

    void delete_donasi(bangkong::ReqParam::type req,
                       bangkong::RespParam::type callback) const;


private:
    template<typename T>
    Json::Value parse_to_json_array(std::vector<T>&& data) const;
private:
    std::shared_ptr<DbProcess> m_db;
    std::unique_ptr<DonasiProcess> m_donasi;
    std::unique_ptr<JenistransaksiProcess> m_jenis_transaksi;
};

#endif // DONASICONTROLLER_HPP
