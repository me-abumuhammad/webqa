#ifndef DBPROCESS_HPP
#define DBPROCESS_HPP

#include <functional>

#include <drogon/drogon.h>

#include "../models/akun.hpp"


class DbProcess
{
public:
    using AkunResult = std::function<void(std::optional<Akun>)>;
    DbProcess(std::shared_ptr<drogon::orm::DbClient>& db): m_db {db} {}
    DbProcess(const DbProcess&) = delete ;
    DbProcess(DbProcess&&) = delete ;
    DbProcess() = delete ;
    void login(Akun&& akun, AkunResult&&);
private:
    std::shared_ptr<drogon::orm::DbClient>& m_db;
};

#endif // DBPROCESS_HPP
