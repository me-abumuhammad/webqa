#include "dbprocess.hpp"

void DbProcess::login(Akun &&akun, AkunResult &&callback) {

    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select * from donasi.akun where username = $1 and password = $2",
                             akun.get_username(),
                             akun.get_password());
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() == 0) {
            callback(std::nullopt);
        }
        else {
            for (auto row: f_get) {
                Akun skn{row["username"].as<std::string_view>(),
                                        "",
                                        row["name"].as<std::string_view>()};
                std::optional<Akun> opt = std::move(skn);
                callback(opt);
            }
        }
    } catch (const drogon::orm::DrogonDbException&) {
        callback(std::nullopt);
    }
}

void DbProcess::update_rekap(Rekap &&rekap, RekapResult &&callback) {
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select count(kode) as hit from donasi.rekap where kode = $1",
                                                                    rekap.get_code());
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() == 0) {
            callback(std::nullopt);
        }
        else {
            int hit = 0;
            for (auto row: f_get) {
                hit = row["hit"].as<int>();
            }
            if (hit == 0) {

                auto result = m_db->execSqlSync("insert into donasi.rekap values($1, $2, $3)",
                                                rekap.get_code(),
                                                rekap.get_name(),
                                                rekap.get_nominal());
                if (result.affectedRows() == 0) {
                    callback(std::nullopt);
                }
                else {
                    std::optional<Rekap> opt = std::move(rekap);
                    callback(opt);
                }
            }
            else {
                auto result = m_db->execSqlSync("update donasi.rekap set name = $1, nominal = $2 where kode = $3",
                                                rekap.get_name(),
                                                rekap.get_nominal(),
                                                rekap.get_code());
                if (result.affectedRows() == 0) {
                    callback(std::nullopt);
                }
                else {
                    std::optional<Rekap> opt = std::move(rekap);
                    callback(opt);
                }
            }
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        callback(std::nullopt);
    }
}

void DbProcess::get_all_donasi(RekapAllResult &&callback) {
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select * from donasi.rekap");
    std::vector<Rekap> result;
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        result.reserve(f_get.size());
        for (auto row: f_get) {
            result.emplace_back(row["kode"].as<std::string_view>(), row["name"].as<std::string>(), row["nominal"].as<uint64_t>());
        }
        callback(result);
    } catch (const drogon::orm::DrogonDbException&) {
        callback(result);
    }
}

bool DbProcess::save_token(const Json::Value &akun) {
    std::string usename = akun["username"].asString();
    std::string token = akun["token"].asString();
    auto result = m_db->execSqlSync("update donasi.akun set token = $1 where username = $2", token, usename);
    if (result.affectedRows() == 0)
        return false;
    else
        return true;
}
