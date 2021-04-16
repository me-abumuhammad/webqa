#ifndef MUHSININPROCESS_HPP

#include "baseprocess.hpp"

class MuhsininProcess: public BaseProcess {
public:
    MuhsininProcess(const std::shared_ptr<drogon::orm::DbClient>& db): BaseProcess(db) {}
private:
    const std::shared_ptr<drogon::orm::DbClient>& m_db;
}

#endif // !MUHSININPROCESS_HPP