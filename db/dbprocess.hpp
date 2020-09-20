#ifndef DBPROCESS_HPP
#define DBPROCESS_HPP

#include <functional>

#include <drogon/drogon.h>

#include "../models/akun.hpp"
#include "../models/rekap.hpp"

class DbProcess
{
public:
    using AkunResult = std::function<void(std::optional<Akun>)>;
    using RekapResult = std::function<void(std::optional<Rekap>)>;
    using BoolResult = std::function<void(bool)>;
    using RekapAllResult = std::function<void(std::optional<std::vector<Rekap>>)>;
    DbProcess(std::shared_ptr<drogon::orm::DbClient>& db): m_db {db} {}
    DbProcess(const DbProcess&) = delete ;
    DbProcess(DbProcess&&) = delete ;
    DbProcess() = delete ;
    void login(Akun&& akun, AkunResult&&);
    void update_rekap(Rekap&& rekap, RekapResult&&);
    void check_username(std::string_view, BoolResult);
    void get_all_donasi(RekapAllResult&&);
private:
    std::shared_ptr<drogon::orm::DbClient>& m_db;
};

#endif // DBPROCESS_HPP
