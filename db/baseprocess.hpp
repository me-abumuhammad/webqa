#ifndef BASEPROCESS_HPP
#define BASEPROCESS_HPP

#include <drogon/drogon.h>

#include "interfaceprocess.hpp"

class BaseProcess: public InterfaceProcess
{
public:
    BaseProcess(const std::shared_ptr<drogon::orm::DbClient>& db): m_db {db} {}
    BaseProcess() = delete ;
    BaseProcess(const BaseProcess&) = delete ;
    BaseProcess(BaseProcess&&) = delete ;
    ~BaseProcess() {}
    virtual void check_username(std::string_view, BoolResult) override;
    virtual bool check_token(std::string_view) override;
private:
    const std::shared_ptr<drogon::orm::DbClient>& m_db;
};

#endif // BASEPROCESS_HPP
