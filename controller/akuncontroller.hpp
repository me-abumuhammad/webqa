#ifndef AKUNCONTROLLER_HPP
#define AKUNCONTROLLER_HPP

#include <drogon/HttpController.h>
#include <drogon/orm/DbClient.h>

#include "basecontroller.hpp"

#include "../db/akunprocess.hpp"

class AkunController: public drogon::HttpController<AkunController, false>, public BaseController
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(AkunController::login, "/login", drogon::Post);
    METHOD_ADD(AkunController::dashboard, "/dashboard", drogon::Get);
    METHOD_LIST_END
    explicit AkunController() {}

    explicit AkunController(const std::shared_ptr<drogon::orm::DbClient>& db) {
        m_db = std::make_shared<AkunProcess>(db);
    }

    void login(const drogon::HttpRequestPtr& req,
               std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;
    void dashboard(const drogon::HttpRequestPtr& req,
                   std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;

private:
    std::shared_ptr<AkunProcess> m_db;
};

#endif // AKUNCONTROLLER_HPP
