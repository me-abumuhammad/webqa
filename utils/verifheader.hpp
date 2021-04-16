#ifndef VERIFHEADER_HPP
#define VERIFHEADER_HPP

#include "../db/interfaceprocess.hpp"

#include <jwt.h>
#include <functional>

class VerifHeader
{
public:
    VerifHeader(const std::shared_ptr<InterfaceProcess>& db): m_db{db} {}
    ~VerifHeader() {}
    bool check_authorization_token(std::string_view token, std::string& message);
    bool check_timeout(std::string_view token, std::string& message);
private:
    const std::shared_ptr<InterfaceProcess>& m_db;
};

#endif // VERIFHEADER_HPP
