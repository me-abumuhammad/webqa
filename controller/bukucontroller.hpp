#ifndef BUKUCONTROLLER_HPP
#define BUKUCONTROLLER_HPP

#include <drogon/HttpController.h>
#include <drogon/orm/DbClient.h>

#include "../db/bukuprocess.hpp"
#include "basecontroller.hpp"

class BukuController: public drogon::HttpController<BukuController, false>, public BaseController
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(BukuController::add_book, "/add_book", drogon::Post);
    METHOD_ADD(BukuController::book, "book/{id_buku}", drogon::Get);
    METHOD_LIST_END
    BukuController();
    explicit BukuController(const std::shared_ptr<drogon::orm::DbClient>& db) {
        m_db = std::make_shared<BukuProcess>(db);
    }

    void add_book(const drogon::HttpRequestPtr& req,
                  std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;
    void book(const drogon::HttpRequestPtr& req,
              std::function<void(const drogon::HttpResponsePtr&)>&& callback, int id_buku) const;
private:
    std::shared_ptr<BukuProcess> m_db;
};

#endif // BUKUCONTROLLER_HPP
