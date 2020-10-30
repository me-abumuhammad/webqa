#include "baseprocess.hpp"

void BaseProcess::check_username(std::string_view username, BoolResult callback) {
    if (username == "") callback(false);
    else {
        std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select count(username) as hit from donasi.akun where username = $1",
                                                                        username.data());
        try {
            res.wait();
            drogon::orm::Result f_get = res.get();
            if (f_get.size() == 0) {
                callback(false);
            }
            else {
                int hit = 0;
                for (auto row: f_get) {
                    hit = row["hit"].as<int>();
                }
                callback(hit == 0 ? false: true);
            }
        } catch (const drogon::orm::DrogonDbException&) {
            callback(false);
        }
    }
}

bool BaseProcess::check_token(std::string_view token) {
    if (token == "") return false;
    else {
        std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select count(token) as hit from donasi.akun where token = $1", token.data());
        try {
            res.wait();
            drogon::orm::Result f_get = res.get();
            if (f_get.size() == 0) {
                return false;
            }
            else {
                int hit = 0;
                for (auto row: f_get) {
                    hit = row["hit"].as<int>();
                }
                return hit == 1;
            }
        } catch (const drogon::orm::DrogonDbException&) {

        }
        return false;
    }
}
