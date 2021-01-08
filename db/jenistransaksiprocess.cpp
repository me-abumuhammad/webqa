#include "jenistransaksiprocess.hpp"


void JenistransaksiProcess::add(JenisTransaksi &&jenis_transaksi,
                                JenisTransaksiResult &&callback,
                                std::string &msg) {
    if (check_jenis_process(jenis_transaksi.get_nama()) == false) {
        auto res = m_db->execSqlSync("insert into donasi.jenis_transaksi(nama, jenis) values($1, $2)",
                                     jenis_transaksi.get_nama(),
                                     jenis_transaksi.get_jenis());
        if (res.affectedRows() == 0)
            callback(std::nullopt);
        else {
            std::optional<JenisTransaksi> opt = get_last_jenis_transaksi();
            callback(opt);
        }
    }
    else {
        msg = "Data sudah ada";
        callback(std::nullopt);
    }
}

bool JenistransaksiProcess::check_jenis_process(std::string_view name) {
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select count(id_jenis_transaksi) as hit from donasi.jenis_transaksi where nama = $1", name.data());
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() == 0)
            return false;
        else {
            int r = 0;
            for(auto row: f_get)
                r = row["hit"].as<int>();
            return r >= 1;
        }
    } catch (const drogon::orm::DrogonDbException&) {

    }
    return false;
}

std::optional<JenisTransaksi> JenistransaksiProcess::get_last_jenis_transaksi() const {
    std::future<drogon::orm::Result> res = m_db
            ->execSqlAsyncFuture("select * from donasi.jenis_transaksi order by id_jenis_transaksi desc limit 1");
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() == 0)
            return std::nullopt;
        else {
            for (auto row: f_get) {
                std::vector<Buku> buku = get_buku_by_id_jenis(row["id_jenis_transaksi"].as<int>());
                JenisTransaksi jns{row["id_jenis_transaksi"].as<int>(),
                                  row["nama"].as<std::string_view>(),
                                  row["status"].as<unsigned short>(),
                                  buku};
                std::optional<JenisTransaksi> opt = std::move(jns);
                return jns;
            }
        }
    } catch (const drogon::orm::DrogonDbException& e) {
    }
    return std::nullopt;
}

void JenistransaksiProcess::get_jenis_transaksi_by_id(int id, JenisTransaksiResult &&callback) {
    std::future<drogon::orm::Result> res =
            m_db
            ->execSqlAsyncFuture("select * from donasi.jenis_transaksi where id_jenis_transaksi = $", id);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        for(auto row: f_get) {
            std::vector<Buku> buku = get_buku_by_id_jenis(row["id_jenis_transaksi"].as<int>());
            JenisTransaksi jns{row["id_jenis_transaksi"].as<int>(),
                              row["nama"].as<std::string_view>(),
                              row["status"].as<unsigned short>(),
                              buku};
            std::optional<JenisTransaksi> opt = std::move(jns);
            callback(opt);
        }

    } catch (const drogon::orm::DrogonDbException& e) {

    }
    callback(std::nullopt);
}

std::vector<Buku> JenistransaksiProcess::get_buku_by_id_jenis(int id) const {
    std::vector<Buku> result;
    std::future<drogon::orm::Result> res =
            m_db
            ->execSqlAsyncFuture("select * from donasi.buku_transaksi where jenis_transaksi_id = $1", id);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() == 0)
            return result;
        else {
            for (auto row: f_get) {
                m_buku_process->get_buku_by_id(row["buku_id"].as<int>(), [&result](std::optional<Buku> res) {
                    if (!res.has_value())
                        result.emplace_back(res.value());
                });
            }
            return result;
        }
    } catch (const drogon::orm::DrogonDbException& e) {
    }
    return result;
}

void JenistransaksiProcess::update(JenisTransaksi &&jenis, JenisTransaksiResult &&callback) {
    std::optional<JenisTransaksi> res_jenis;
    get_jenis_transaksi_by_id(jenis.get_id_jenis_transaksi(), [&res_jenis](std::optional<JenisTransaksi> res){
        res_jenis = res;
    });

    if (!res_jenis.has_value()) {
        std::string msg = "";
        add(std::move(jenis), [cb = std::move(callback)](std::optional<JenisTransaksi> rs) {
            cb(rs);
        }, msg);
    }
    else {
        auto result = m_db->execSqlSync("update donasi.jenis_transaksi set nama = $1, jenis = $2 where id_jenis_transaksi = $3",
                                        jenis.get_nama(),
                                        jenis.get_jenis(),
                                        jenis.get_id_jenis_transaksi());
        if (result.affectedRows() == 0) callback(std::nullopt);
        else {
            std::optional<JenisTransaksi> opt = std::move(jenis);
            callback(opt);
        }
    }
}

std::vector<JenisTransaksi> JenistransaksiProcess::get_all_jenis_transaksi(std::string_view kode) {
    std::vector<JenisTransaksi> result;
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select * from donasi.jenis_transaksi");
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            result.reserve(f_get.size());
            for (auto row: f_get) {
                std::vector<Buku> collect_buku = get_id_buku_by_transaksi(row["id_jenis_transaksi"].as<int>(), kode);
                result.emplace_back(row["id_jenis_transaksi"].as<int>(),
                        row["nama"].as<std::string_view>(),
                        row["jenis"].as<int>(),
                        collect_buku);
            }
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return result;
}

