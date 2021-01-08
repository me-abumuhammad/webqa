#ifndef BUKUCONTROLLER_HPP
#define BUKUCONTROLLER_HPP

#include "../db/bukuprocess.hpp"
#include "../db/donasiprocess.hpp"
#include "basecontroller.hpp"

class BukuController: public drogon::HttpController<BukuController, false>, public BaseController
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(BukuController::add_book, "/add_book", drogon::Post);
    METHOD_ADD(BukuController::book, "book/{id_buku}", drogon::Get);
    METHOD_ADD(BukuController::all_book, "all_book", drogon::Get);
    METHOD_ADD(BukuController::update_book, "update_book", drogon::Put);
    METHOD_ADD(BukuController::delete_book, "delete_book/{id_buku}", drogon::Delete);
    METHOD_ADD(BukuController::detail_buku, "detail/{id_buku}", drogon::Get);
    METHOD_LIST_END
    BukuController();
    explicit BukuController(const std::shared_ptr<drogon::orm::DbClient>& db) {
        m_db = std::make_shared<BukuProcess>(db);
        m_donasi = std::make_unique<DonasiProcess>(db);
    }

    void add_book(bangkong::ReqParam::type req,
                  bangkong::RespParam::type callback) const;
    void book(bangkong::ReqParam::type req,
              bangkong::RespParam::type callback, int id_buku) const;
    void all_book(bangkong::ReqParam::type req,
                  bangkong::RespParam::type callback) const;
    void update_book(bangkong::ReqParam::type req,
                     bangkong::RespParam::type callback) const;
    void delete_book(bangkong::ReqParam::type req,
                     bangkong::RespParam::type callback, int id_buku) const;
    void ubhar(bangkong::ReqParam::type req,
               bangkong::RespParam::type callback) const;
    void detail_buku(bangkong::ReqParam::type req,
                     bangkong::RespParam::type callback, int id_buku) const;
private:
    std::shared_ptr<BukuProcess> m_db;
    std::unique_ptr<DonasiProcess> m_donasi;
};

#endif // BUKUCONTROLLER_HPP
