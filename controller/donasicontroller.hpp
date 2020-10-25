#ifndef DONASICONTROLLER_HPP
#define DONASICONTROLLER_HPP

#include <drogon/HttpController.h>
#include <drogon/orm/DbClient.h>

#include "../db/dbprocess.hpp"

class DonasiController: public drogon::HttpController<DonasiController, false>
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(DonasiController::login, "/login", drogon::Post);
    METHOD_ADD(DonasiController::update_rekap, "/update", drogon::Post);
    METHOD_ADD(DonasiController::all_rekap, "/rekap", drogon::Get);
    METHOD_ADD(DonasiController::rekap_for_bot, "/list", drogon::Get);
    METHOD_LIST_END

    explicit DonasiController() {}

    explicit DonasiController(std::shared_ptr<drogon::orm::DbClient>& db) {
        m_db = std::make_shared<DbProcess>(db);
    }

    void login(const drogon::HttpRequestPtr& req,
               std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;
    void update_rekap(const drogon::HttpRequestPtr& req,
                      std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;
    void all_rekap(const drogon::HttpRequestPtr& req,
                      std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;
    void rekap_for_bot(const drogon::HttpRequestPtr& req,
                       std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;
private:
    std::shared_ptr<DbProcess> m_db;
};

#endif // DONASICONTROLLER_HPP
