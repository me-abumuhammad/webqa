#ifndef DBPROCESS_HPP
#define DBPROCESS_HPP

#include <functional>

#include "../models/akun.hpp"
#include "../models/rekap.hpp"

#include "baseprocess.hpp"

class DbProcess: public BaseProcess
{
public:
    using AkunResult = std::function<void(std::optional<Akun>)>;
    using RekapResult = std::function<void(std::optional<Rekap>)>;
    using RekapAllResult = std::function<void(std::optional<std::vector<Rekap>>)>;
    DbProcess(const std::shared_ptr<drogon::orm::DbClient>& db): BaseProcess(db), m_db {db} {}
    DbProcess(const DbProcess&) = delete ;
    DbProcess(DbProcess&&) = delete ;
    DbProcess() = delete ;
    ~DbProcess() {}
    void login(Akun&& akun, AkunResult&&);
    void update_rekap(Rekap&& rekap, RekapResult&&);
    void get_all_donasi(RekapAllResult&&);
    bool save_token(const Json::Value&);
private:
    const std::shared_ptr<drogon::orm::DbClient>& m_db;
};

#endif // DBPROCESS_HPP
