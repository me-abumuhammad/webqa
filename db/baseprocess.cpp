#include "baseprocess.hpp"

#include "../models/buku.hpp"

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

trantor::Date BaseProcess::to_sys_days(int y, unsigned char m, unsigned char d) const {
    return trantor::Date{static_cast<unsigned int>(y),
                         static_cast<unsigned int>(m),
                         static_cast<unsigned int>(d)};
}

trantor::Date BaseProcess::current_now() const {
    const std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> now = ::date::floor<::std::chrono::microseconds>(std::chrono::system_clock::now());
    return trantor::Date{now.time_since_epoch().count()};
}

std::vector<Buku> BaseProcess::get_id_buku_by_transaksi(int id, std::string_view tipe) const {
    std::vector<Buku> result;
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture(
                "SELECT b.* FROM donasi.buku_transaksi a "
                "LEFT JOIN donasi.buku b ON a.buku_id = b.id_buku "
                "WHERE jenis_transaksi_id = $1 AND tipe = $2", id, tipe.data());
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            result.reserve(f_get.size());
            for (auto row: f_get) {
                result.emplace_back(row["id_buku"].as<int>(),
                        row["nama"].as<std::string_view>(),
                        row["status"].as<bool>(), row["bg"].as<std::string_view>());
            }
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return result;
}

std::string BaseProcess::get_code_donasi(int donasi_id) const {
    std::string result = "";
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select kode from donasi.sumber_dana where id_sumber_dana = $1", donasi_id);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            for (auto row: f_get) {
                result = row["kode"].as<std::string>();
            }
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return result;
}
