#ifndef AKUNPROCESS_HPP
#define AKUNPROCESS_HPP

#include <drogon/drogon.h>

class AkunProcess
{
public:
    AkunProcess(const std::shared_ptr<drogon::orm::DbClient>& db);
private:
    const std::shared_ptr<drogon::orm::DbClient>& m_db;
};

#endif // AKUNPROCESS_HPP
