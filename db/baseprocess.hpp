#ifndef BASEPROCESS_HPP
#define BASEPROCESS_HPP

#include <drogon/drogon.h>

#include "interfaceprocess.hpp"

#include <date/date.h>

class Buku;

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

protected:
    trantor::Date to_sys_days(int y, unsigned char m, unsigned char d) const;
    trantor::Date current_now() const;
    std::vector<Buku> get_id_buku_by_transaksi(int id, std::string_view tipe = "D") const;
    std::string get_code_donasi(int donasi_id) const;
private:
    const std::shared_ptr<drogon::orm::DbClient>& m_db;
};

#endif // BASEPROCESS_HPP
