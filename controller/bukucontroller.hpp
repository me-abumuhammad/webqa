#ifndef BUKUCONTROLLER_HPP
#define BUKUCONTROLLER_HPP

#include <drogon/HttpController.h>
#include <drogon/orm/DbClient.h>

class BukuController: public drogon::HttpController<BukuController, false>
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(BukuController::add_book, "/add_book", drogon::Post);
    METHOD_LIST_END
    BukuController();

    void add_book(const drogon::HttpRequestPtr& req,
                  std::function<void(const drogon::HttpResponsePtr&)>&& callback) const;
};

#endif // BUKUCONTROLLER_HPP
