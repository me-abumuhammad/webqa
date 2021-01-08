#ifndef HITBASE_HPP
#define HITBASE_HPP

#include <cstdint>
#include <drogon/drogon.h>

class HitBase
{
public:
    HitBase();
    ~HitBase() {}
    virtual uint64_t count_debet(const std::weak_ptr<drogon::orm::DbClient>& db) const = 0;
    virtual uint64_t count_kredit(const std::weak_ptr<drogon::orm::DbClient>& db) const = 0;
};

#endif // HITBASE_HPP
