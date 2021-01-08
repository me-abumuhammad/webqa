#include "sumberdana.hpp"

SumberDana::SumberDana()
{

}

uint64_t SumberDana::count_debet(const std::weak_ptr<drogon::orm::DbClient> &db) const {
    uint64_t result = 0;
    std::future<drogon::orm::Result> res;
    if (m_buku_id != 0)
        res = db.lock()->execSqlAsyncFuture("select sum(nominal) as jml from donasi.debet where buku_id = $1 and kode = $2", m_buku_id, m_kode);
    else
        res = db.lock()->execSqlAsyncFuture("select sum(nominal) as jml from donasi.debet where kode = $1", m_kode);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            for (auto row: f_get)
                result = row["jml"].as<uint64_t>();
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return result;
}

uint64_t SumberDana::count_kredit(const std::weak_ptr<drogon::orm::DbClient> &db) const {
    uint64_t result = 0;
    std::future<drogon::orm::Result> res;
    if (m_buku_id != 0)
        res = db.lock()->execSqlAsyncFuture("select sum(nominal) as jml from donasi.kredit where buku_id = $1 and kode = $2", m_buku_id, m_kode);
    else
        res = db.lock()->execSqlAsyncFuture("select sum(nominal) as jml from donasi.kredit where kode = $1", m_kode);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            for(auto row: f_get)
                result = row["jml"].as<uint64_t>();
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return result;
}
