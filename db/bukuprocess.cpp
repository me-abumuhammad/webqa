#include "bukuprocess.hpp"

void BukuProcess::add(Buku &&buku, BukuResult &&callback) {
    auto res = m_db->execSqlSync("insert into donasi.buku(nama, status) values($1, $2)",
                                 buku.get_nama(),
                                 buku.get_status());
    if (res.affectedRows() == 0) {
        callback(std::nullopt);
    }
    else {
        std::optional<Buku> opt = get_last_buku();
        callback(opt);
    }
}

std::optional<Buku> BukuProcess::get_last_buku() const {
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select * from donasi.buku order by id_buku desc limit 1");
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() == 0) {
            return std::nullopt;
        }
        else {
            for(auto row: f_get) {
                Buku bk {row["id_buku"].as<int>(), row["nama"].as<std::string_view>(), row["status"].as<bool>()};
                std::optional<Buku> opt = std::move(bk);
                return bk;
            }
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        return std::nullopt;
    }
    return std::nullopt;
}

void BukuProcess::get_buku_by_id(int id, BukuResult &&callback) {
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select * from donasi.buku where id_buku = $1", id);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() == 0) {
            callback(std::nullopt);
        }
        else {
            for(auto row: f_get) {
                Buku bk {row["id_buku"].as<int>(), row["nama"].as<std::string_view>(), row["status"].as<bool>()};
                std::optional<Buku> opt = std::move(bk);
                callback(opt);
            }
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        callback(std::nullopt);
    }
}

void BukuProcess::update_buku(Buku &&buku, BukuResult &&callback) {
    std::optional<Buku> res_buku;
    get_buku_by_id(buku.get_id_buku(), [&](std::optional<Buku> res) {
        res_buku = res;
    });
    if (res_buku.has_value()) {
        add(std::move(buku), [cb = std::move(callback)](std::optional<Buku> rs) {
            cb(rs);
        });
    }
    else {
        auto result = m_db->execSqlSync("update donasi.buku set nama = $1, status = $2 where id_buku = $3",
                                        buku.get_nama(),
                                        buku.get_status(),
                                        buku.get_id_buku());
        if (result.affectedRows() == 0) {
            callback(std::nullopt);
        }
        else {
            std::optional<Buku> opt = std::move(buku);
            callback(opt);
        }
    }
}
