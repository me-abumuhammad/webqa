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
    METHOD_LIST_END

    explicit DonasiController() {}

    explicit DonasiController(std::shared_ptr<drogon::orm::DbClient>& db) {
        m_db = std::make_unique<DbProcess>(db);
    }

    void login(const drogon::HttpRequestPtr& req,
               std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;
private:
    std::unique_ptr<DbProcess> m_db;
};

#endif // DONASICONTROLLER_HPP
