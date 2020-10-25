#ifndef VERIFHEADER_HPP
#define VERIFHEADER_HPP

#include "../db/dbprocess.hpp"

#include <jwt.h>

class VerifHeader
{
public:
    VerifHeader(const std::shared_ptr<DbProcess>& db): m_db{db} {}
    ~VerifHeader() {}
    bool check_authorization_token(std::string_view token, std::string& message);
    bool check_timeout(std::string_view token, std::string& message);
private:
    const std::shared_ptr<DbProcess>& m_db;
};

#endif // VERIFHEADER_HPP
