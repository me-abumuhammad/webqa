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
