#ifndef BUKUPROCESS_HPP
#define BUKUPROCESS_HPP

#include <functional>

#include "../models/buku.hpp"

#include "baseprocess.hpp"

class BukuProcess: public BaseProcess
{
public:
    using BukuResult = std::function<void(std::optional<Buku>)>;
    BukuProcess(const std::shared_ptr<drogon::orm::DbClient>& db): BaseProcess(db), m_db{db} {}
    BukuProcess(const BukuProcess&) = delete ;
    BukuProcess(BukuProcess&&) = delete ;
    BukuProcess() = delete ;
    ~BukuProcess() {}
    void add(Buku&&, BukuResult&&, std::string&);
    void update_buku(Buku&&, BukuResult&&);
    void get_buku_by_id(int, BukuResult&&);
    void get_all_buku(std::function<void(std::vector<Buku>)>&&);
    bool delete_buku(int);
    bool check_book_name(std::string_view);
private:
    std::optional<Buku> get_last_buku() const;
private:
    const std::shared_ptr<drogon::orm::DbClient>& m_db;
};

#endif // BUKUPROCESS_HPP
