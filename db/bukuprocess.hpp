#ifndef BUKUPROCESS_HPP
#define BUKUPROCESS_HPP

#include <functional>
#include <drogon/drogon.h>

#include "../models/buku.hpp"

class BukuProcess
{
public:
    using BukuResult = std::function<void(std::optional<Buku>)>;
    BukuProcess(const std::shared_ptr<drogon::orm::DbClient>& db): m_db{db} {}
    BukuProcess(const BukuProcess&) = delete ;
    BukuProcess(BukuProcess&&) = delete ;
    BukuProcess() = delete ;
    void add(Buku&&, BukuResult&&);
    void update_buku(Buku&&, BukuResult&&);
    void get_buku_by_id(int, BukuResult&&);
private:
    std::optional<Buku> get_last_buku() const;
private:
    const std::shared_ptr<drogon::orm::DbClient>& m_db;
};

#endif // BUKUPROCESS_HPP
