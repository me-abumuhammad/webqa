#include "buku.hpp"

Buku::Buku()
{

}

uint64_t Buku::count_debet(const std::weak_ptr<drogon::orm::DbClient> &db) const {
    uint64_t result = 0;
    std::future<drogon::orm::Result> res = db.lock()->execSqlAsyncFuture("SELECT SUM(nominal) as jum FROM donasi.debet WHERE buku_id = $1", m_id_buku);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            for (auto row: f_get)
                result = row["jum"].as<uint64_t>();
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return result;
}

uint64_t Buku::count_kredit(const std::weak_ptr<drogon::orm::DbClient> &db) const {
    uint64_t result = 0;
    std::future<drogon::orm::Result> res = db.lock()->execSqlAsyncFuture("SELECT SUM(nominal) as jum FROM donasi.kredit WHERE buku_id = $1", m_id_buku);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            for (auto row: f_get)
                result = row["jum"].as<uint64_t>();
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return result;
}
