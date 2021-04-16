#include "donasiprocess.hpp"

#include "../utils/utils.hpp"

#include <date/date.h>

#include "jenistransaksiprocess.hpp"

#include "../utils/money.hpp"

std::vector<Muhsinin> DonasiProcess::get_all_muhsinin() {
    std::vector<Muhsinin> result;
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select * from donasi.muhsinin");
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            result.reserve(f_get.size());
            for (auto row: f_get) {
                result.emplace_back(row["id_muhsinin"].as<int>(),
                        row["nama"].as<std::string_view>(),
                        row["inisial"].as<std::string_view>());
            }
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return result;
}

std::vector<Pelaksana> DonasiProcess::get_all_pelaksana() {
    std::vector<Pelaksana> result;
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select * from donasi.pelaksana");
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        result.reserve(f_get.size());
        for (auto row: f_get) {
            result.emplace_back(row["nama"].as<std::string_view>(),
                    row["id_pelaksana"].as<int>(),
                    row["inisial"].as<std::string_view>());
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return result;
}

std::vector<SumberDana> DonasiProcess::get_all_sumberdana() const {
    std::vector<SumberDana> result;
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select * from donasi.sumber_dana");
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        result.reserve(f_get.size());
        for (auto row: f_get) {
            result.emplace_back(row["id_sumber_dana"].as<int>(),
                    row["nama"].as<std::string_view>(),
                    row["kode"].as<std::string_view>(),
                    row["bg"].as<std::string_view>());
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return result;
}

int DonasiProcess::current_number(int jenis_donasi) const {
    int current = 0;
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select nomor from donasi.nomor where jenis_donasi_id = $1", jenis_donasi);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        for (auto row: f_get)
            current = row["nomor"].as<int>();
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return current;
}

int DonasiProcess::current_index_kredit() const {
    int current = 0;
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("SELECT max(index_kredit) FROM donasi.kredit");
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        for (auto row: f_get)
            current = row["max"].as<int64_t>();
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return current;
}

bool DonasiProcess::update_current_number(int jenis_donasi, int no) const {
    bool status = false;
    try {
        auto result = m_db->execSqlSync("update donasi.nomor set nomor = $1 where jenis_donasi_id = $2",
                                        static_cast<int64_t>(no),
                                        jenis_donasi);
        status = result.affectedRows() > 0;
    } catch (const drogon::orm::DrogonDbException& e) {
         std::cout << e.base().what() << std::endl;
    }
    return status;
}

bool DonasiProcess::save_donasi(bangkong::DonasiData &&data) const {
    std::string tgl = data.tanggal;
    int current_num = current_number(data.jenis_donasi);
    std::string current_tgl = current_now().toCustomedFormattedStringLocal("%Y-%m-%d");
    std::string num = bangkong::create_num(current_tgl, data.jenis_donasi, current_num + 1);
    std::tuple<int, int, int> tpl = bangkong::tgl_to_int(tgl);
    auto tanggal = to_sys_days(std::get<0>(tpl), std::get<1>(tpl), std::get<2>(tpl));
    try {
//        std::cout << num << " " << tanggal.toDbStringLocal() << " " << std::get<0>(tpl) << " " << std::get<1>(tpl) << " " << current_now().microSecondsSinceEpoch() << '\n';
        auto res = m_db->execSqlSync("insert into donasi.donasi(muhsinin_id, keterangan, nomor, pos_dana, url_kuitansi, pelaksana_id, tanggal, tahun, bulan, tgl_input)"
                                     " values($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)",
                                     data.donatur,
                                     data.keterangan,
                                     num,
                                     data.jenis_donasi,
                                     data.url,
                                     data.penerima,
                                     tanggal.toDbStringLocal(),
                                     std::get<0>(tpl),
                                     std::get<1>(tpl),
                                     current_now().microSecondsSinceEpoch()
                                     );

        if (res.affectedRows() > 0) {
            update_current_number(data.jenis_donasi, current_num + 1);
            uint64_t nominal = boost::lexical_cast<uint64_t>(data.nominal);
            int donasi_id = get_id_donasi_by_nomor(num);
            std::string kode = get_code_donasi(data.jenis_donasi);
            std::tuple<int64_t, int, int, std::string, std::string, int, int> data_transaksi = std::make_tuple(
                        nominal,
                        donasi_id,

                        data.jenis_transaksi,
                        kode,
                        num,
                        std::get<0>(tpl),
                        std::get<1>(tpl));
            bool st = save_transaksi_donasi(std::move(data_transaksi));
            return st;
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << "DonasiProcess::save_donasi: " << e.base().what() << std::endl;
    }
    return false;
}

bool DonasiProcess::update_donasi(Json::Value &&data) const {
    std::string tgl = data["tanggal"].asString();
    std::string current_tgl = current_now().toCustomedFormattedStringLocal("%Y-%m-%d");
    std::tuple<int, int, int> tpl = bangkong::tgl_to_int(tgl);
    auto tanggal = to_sys_days(std::get<0>(tpl), std::get<1>(tpl), std::get<2>(tpl));
    try {
        auto res = m_db->execSqlSync("update donasi.donasi set muhsinin_id = $1,"
                                     " keterangan = $2,"
                                     " pos_dana = $3,"
                                     " url_kuitansi = $4,"
                                     " pelaksana_id = $5,"
                                     " tanggal = $6,"
                                     " tahun = $7,"
                                     " bulan = $8,"
                                     " tgl_update = $9 where nomor = $10",
                                     data["donatur"].asInt(),
                                     data["keterangan"].asString(),
                                     data["jenis_donasi"].asInt(),
                                     data["url"].asString(),
                                     data["penerima"].asInt(),
                                     tanggal.toDbStringLocal(),
                                     std::get<0>(tpl),
                                     std::get<1>(tpl),
                                     current_now().microSecondsSinceEpoch(),
                                     data["nomor"].asString());
        if (res.affectedRows() > 0) {
            uint64_t nominal = boost::lexical_cast<uint64_t>(data["nominal"].asString());
            std::string kode = get_code_donasi(data["jenis_donasi"].asInt());
            int donasi_id = get_id_donasi_by_nomor(data["nomor"].asString());
            std::tuple<int64_t, int, int, std::string, std::string, int, int> data_transaksi = std::make_tuple(
                        nominal,
                        donasi_id,
                        data["jenis_transaksi"].asInt(),
                        kode,
                        data["nomor"].asString(),
                        std::get<0>(tpl),
                        std::get<1>(tpl));
            bool st = update_transaksi_donasi(std::move(data_transaksi));
            return st;
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return false;
}

/*
 * 0 Nominal
 * 1 Donasi ID
 * 2 Jenis Transaksi
 * 3 Kode Sumber Dana
 * 4 Nomor Donasi
 * 5 Tahun
 * 6 Bulan
*/

bool DonasiProcess::save_transaksi_donasi(std::tuple<int64_t, int, int, std::string, std::string, int, int> &&data) const {
    Buku &buku = get_id_buku_by_transaksi(std::get<2>(data)).at(0);
    try {
        auto res = m_db->execSqlSync("insert into donasi.debet(buku_id, kode, nominal, aktivitas_id, tgl_input, jenis_transaksi_id, nomor, tahun, bulan)"
                                     " values($1, $2, $3, $4, $5, $6, $7, $8, $9)",
                                     buku.get_id_buku(),
                                     std::get<3>(data),
                                     std::get<0>(data),
                                     static_cast<int64_t>(std::get<1>(data)),
                                     current_now().microSecondsSinceEpoch(),
                                     std::get<2>(data),
                                     std::get<4>(data),
                                     std::get<5>(data),
                                     std::get<6>(data)
                                     );
        return res.affectedRows() > 0;
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << "DonasiProcess::save_transaksi_donasi: " << e.base().what() << std::endl;
    }
    return false;
}

/*
 * 0 Nominal
 * 1 Buku ID
 * 2 Jenis Transaksi
 * 3 Index Kredit
 * 4 Kode Sumber Dana
 * 5 Nomor Kredit
 * 6 Tahun
 * 7 Bulan
*/

bool DonasiProcess::save_transaksi(std::tuple<int64_t, int, int, int, std::string, std::string, int, int> &&data, const Json::Value& json) const {

    try {
        auto res = m_db->execSqlSync("insert into donasi.kredit(nomor_kredit, index_kredit, buku_id, kode, nominal, jenis_transaksi_id, tgl_input, keterangan, pelaksana_id, tanggal, url_kuitansi, tahun, bulan)"
                                     " values($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13)",
                                     std::get<5>(data),
                                     static_cast<int64_t>(std::get<3>(data)),
                                     std::get<1>(data),
                                     std::get<4>(data),
                                     std::get<0>(data),
                                     std::get<2>(data),
                                     current_now().microSecondsSinceEpoch(),
                                     json["keterangan"].asString(),
                                     json["penanggung_jawab"].asInt(),
                                     json["tanggal"].asString(),
                                     json["url"].asString(),
                                     std::get<6>(data),
                                     std::get<7>(data));
        return res.affectedRows() > 0;

    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return false;
}

/*
 * 0 Nominal
 * 1 Buku ID
 * 2 Jenis Transaksi
 * 3 Kode Sumber Dana
 * 4 Tahun
 * 5 Bulan
*/

bool DonasiProcess::update_transaksi(std::tuple<int64_t, int, int, std::string, int, int> &&data, const Json::Value &json) const {
    try {
        auto res = m_db->execSqlSync("update donasi.kredit set buku_id = $1, kode = $2, nominal = $3, jenis_transaksi_id = $4, tgl_update = $5, keterangan = $6, pelaksana_id = $7, tanggal = $8, url_kuitansi = $9, tahun = $10, bulan = $11 where nomor_kredit = $12",
                                     std::get<1>(data),
                                     std::get<3>(data),
                                     std::get<0>(data),
                                     std::get<2>(data),
                                     current_now().microSecondsSinceEpoch(),
                                     json["keterangan"].asString(),
                                     json["penanggung_jawab"].asInt(),
                                     json["tanggal"].asString(),
                                     json["url"].asString(),
                                     std::get<4>(data),
                                     std::get<5>(data),
                                     json["nomor"].asString());
        return res.affectedRows() > 0;
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return false;
}

bool DonasiProcess::save_kredit(Json::Value &&data) const {
    std::vector<BukuJenisTransaksi> buku_jenis = get_buku_transkasi_by_jenis(data["jenis_transaksi"].asInt());
    if (buku_jenis.size() > 0){
        std::vector<BukuJenisTransaksi>::const_iterator it;
        for (it = buku_jenis.begin(); it != buku_jenis.end(); ++it) {
            uint64_t nominal = boost::lexical_cast<uint64_t>(data["nominal"].asString());
            std::string kode = get_code_donasi(data["sumber_dana"].asInt());
            std::string tgl = data["tanggal"].asString();
            std::string current_tgl = current_now().toCustomedFormattedStringLocal("%Y-%m-%d");
            std::tuple<int, int, int> tpl = bangkong::tgl_to_int(tgl);

            auto tanggal = to_sys_days(std::get<0>(tpl), std::get<1>(tpl), std::get<2>(tpl));
            if (it->get_tipe() == "D") {
                int current_num = 0;
                std::string num = "";
                if (buku_jenis.size() > 1) {
                    current_num = current_index_kredit();
                    num = bangkong::create_num_kredit(current_tgl, data["sumber_dana"].asInt(), current_num);

                }
                else {
                    current_num = current_zero_index_kredit();
                    num = bangkong::create_num_kredit(current_tgl, data["sumber_dana"].asInt(), current_num + 1, 1);
                }

                std::tuple<int64_t, int, int, std::string, std::string, int, int> dta =
                        std::make_tuple(
                        nominal,
                        get_id_kredit_by_nomor(num),
                        it->get_jenis_transaksi_id(),
                        kode, num, std::get<0>(tpl),
                        std::get<1>(tpl));
                save_transaksi_donasi(std::move(dta));
            }
            else {
                int current_num = current_index_kredit();
                std::string num = bangkong::create_num_kredit(current_tgl, data["sumber_dana"].asInt(), current_num + 1);
                std::tuple<int64_t, int, int, int, std::string, std::string, int, int> data_trans = std::make_tuple(
                            nominal,
                            it->get_buku_id(),
                            it->get_jenis_transaksi_id(),
                            current_num + 1,
                            kode,
                            num,
                            std::get<0>(tpl),
                            std::get<1>(tpl));
                save_transaksi(std::move(data_trans), data);
            }
        }
        return true;
    }
    return false;
}

int DonasiProcess::current_zero_index_kredit() const {
    int num = 0;
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("SELECT count(aktivitas_id) as jml FROM donasi.debet WHERE aktivitas_id = $1", static_cast<int64_t>(0));
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        for (auto row: f_get)
            num = row["jml"].as<int32_t>();
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return num;
}
/*
 * 0 Nominal
 * 1 Buku ID
 * 2 Jenis Transaksi
 * 3 Kode Sumber Dana
 * 4 Tahun
 * 5 Bulan
*/
bool DonasiProcess::update_kredit(Json::Value &&data) const {
    std::vector<BukuJenisTransaksi> buku_jenis = get_buku_transkasi_by_jenis(data["jenis_transaksi"].asInt());
    if (buku_jenis.size() > 0){
        std::vector<BukuJenisTransaksi>::const_iterator it;
        for (it = buku_jenis.begin(); it != buku_jenis.end(); ++it) {
            uint64_t nominal = boost::lexical_cast<uint64_t>(data["nominal"].asString());
            std::string kode = get_code_donasi(data["sumber_dana"].asInt());
            std::string tgl = data["tanggal"].asString();
            std::tuple<int, int, int> tpl = bangkong::tgl_to_int(tgl);

            auto tanggal = to_sys_days(std::get<0>(tpl), std::get<1>(tpl), std::get<2>(tpl));
            if (it->get_tipe() == "D") {
                std::tuple<int64_t, int, int, std::string, std::string, int, int> dta =
                        std::make_tuple(
                        nominal,
                        get_id_kredit_by_nomor(data["nomor"].asString()),
                        it->get_jenis_transaksi_id(),
                        kode, data["nomor"].asString(), std::get<0>(tpl),
                        std::get<1>(tpl));
                update_transaksi_donasi(std::move(dta));
            }
            else {
                std::tuple<int64_t, int, int, std::string, int, int> data_trans = std::make_tuple(
                            nominal,
                            it->get_buku_id(),
                            it->get_jenis_transaksi_id(),
                            kode,
                            std::get<0>(tpl),
                            std::get<1>(tpl));
                update_transaksi(std::move(data_trans), data);
            }
        }
        return true;
    }
    return false;
}

/*
 * 0 Nominal
 * 1 Donasi ID
 * 2 Jenis Transaksi
 * 3 Kode Sumber Dana
 * 4 Nomor Donasi
 * 5 Tahun
 * 6 Bulan
*/

bool DonasiProcess::update_transaksi_donasi(std::tuple<int64_t, int, int, std::string, std::string, int, int> &&data) const {
    Buku &buku = get_id_buku_by_transaksi(std::get<2>(data)).at(0);
    try {
        auto res = m_db->execSqlSync("update donasi.debet set buku_id = $1,"
                                     " kode = $2,"
                                     " nominal = $3,"
                                     " tgl_update = $4,"
                                     " jenis_transaksi_id = $5, tahun = $7, bulan = $8 where nomor = $6",
                                     buku.get_id_buku(),
                                     std::get<3>(data),
                                     std::get<0>(data),
                                     current_now().microSecondsSinceEpoch(),
                                     std::get<2>(data),
                                     std::get<4>(data),
                                     std::get<5>(data),
                                     std::get<6>(data));
        return res.affectedRows() > 0;
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return false;
}

int DonasiProcess::get_id_donasi_by_nomor(const std::string &nomor) const {
    int id = 0;
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select id_donasi from donasi.donasi where nomor = $1", nomor);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            for (auto row: f_get)
                id = row["id_donasi"].as<uint64_t>();
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return id;
}

int DonasiProcess::get_id_kredit_by_nomor(const std::string &nomor) const {
    int id = 0;
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select id_kredit from donasi.kredit where nomor_kredit = $1", nomor);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            for (auto row: f_get)
                id = row["id_kredit"].as<uint64_t>();
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return id;
}

Json::Value DonasiProcess::get_all_donasi() const {
    Json::Value result(Json::arrayValue);
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("SELECT a.nomor, a.keterangan, d.nama as nama_muhsinin, b.nominal, c.nama as donasi, a.tanggal FROM donasi.donasi a"
                                                                    " LEFT JOIN donasi.debet b on a.id_donasi = b.aktivitas_id"
                                                                    " LEFT JOIN donasi.sumber_dana c on a.pos_dana = c.id_sumber_dana"
                                                                    " LEFT JOIN donasi.muhsinin d on a.muhsinin_id = d.id_muhsinin"
                                                                    " LEFT JOIN donasi.jenis_transaksi e on b.jenis_transaksi_id = e.id_jenis_transaksi"
                                                                    " WHERE b.kode != 'SPP' and a.status_delete = $1 and aktivitas_id != 0 and e.jenis = 1 order by tanggal desc", false);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0){
            int i = 0;
            for (auto row: f_get) {
                Json::Value dta;
                dta["nomor"] = row["nomor"].as<std::string>();
                dta["keterangan"] = row["keterangan"].as<std::string>();
                dta["nama_muhsinin"] = row["nama_muhsinin"].as<std::string>();
                std::string nom = boost::lexical_cast<std::string>(row["nominal"].as<int64_t>());
                dta["nominal"] =  Money::getInstance().toMoneyFormat(nom, ".", "");
                dta["donasi"] = row["donasi"].as<std::string>();
                dta["tanggal"] = row["tanggal"].as<std::string>();
                result[i] = dta;
                ++i;
            }
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return result;
}

Json::Value DonasiProcess::get_all_transaction() const {
    Json::Value result(Json::arrayValue);
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("SELECT a.nomor_kredit, a.keterangan, a.tanggal, a.nominal, b.nama as sumber, c.nama as pelaksana, d.nama as jenis_trans FROM donasi.kredit a"
                                                                    " LEFT JOIN donasi.sumber_dana b on a.kode = b.kode"
                                                                    " LEFT JOIN donasi.pelaksana c on a.pelaksana_id = c.id_pelaksana"
                                                                    " LEFT JOIN donasi.jenis_transaksi d on a.jenis_transaksi_id = d.id_jenis_transaksi"
                                                                    " WHERE a.status_delete = $1  order by tanggal desc", false);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            int i = 0;
            for (auto row: f_get) {
                Json::Value dta;
                dta["sumber_dana"] = row["sumber"].as<std::string>();
                dta["keterangan"] = row["keterangan"].as<std::string>();
                dta["nama_pelaksana"] = row["pelaksana"].as<std::string>();
                std::string nom = boost::lexical_cast<std::string>(row["nominal"].as<int64_t>());
                dta["nominal"] = Money::getInstance().toMoneyFormat(nom, ".", "");
                dta["nomor"] = row["nomor_kredit"].as<std::string>();
                dta["tanggal"] = row["tanggal"].as<std::string>();
                dta["jenis_trans"] = row["jenis_trans"].as<std::string>();
                result[i] = dta;
                ++i;
            }
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return result;
}

Json::Value DonasiProcess::get_donasi_by_nomor(std::string_view nomor) const {
    Json::Value result;
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("SELECT a.*, d.id_muhsinin, b.nominal, c.id_sumber_dana, b.jenis_transaksi_id FROM donasi.donasi a"
                                                                    " LEFT JOIN donasi.debet b on a.id_donasi = b.aktivitas_id"
                                                                    " LEFT JOIN donasi.sumber_dana c on a.pos_dana = c.id_sumber_dana"
                                                                    " LEFT JOIN donasi.muhsinin d on a.muhsinin_id = d.id_muhsinin"
                                                                    " WHERE a.nomor = $1", nomor.data());
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            for (auto row: f_get) {
                result["donatur"] = Json::Int(row["id_muhsinin"].as<int>());
                result["jenis_donasi"] = Json::Int(row["id_sumber_dana"].as<int>());
                int64_t nominal = row["nominal"].as<int64_t>();
                std::string nominal_str = boost::lexical_cast<std::string>(nominal);
                result["nominal"] = Money::getInstance().toMoneyFormat(nominal_str, ",", "");
                result["tanggal"] = row["tanggal"].as<std::string>();
                result["penerima"] = Json::Int(row["pelaksana_id"].as<int>());
                result["jenis_transaksi"] = Json::Int(row["jenis_transaksi_id"].as<int>());
                result["keterangan"] = row["keterangan"].as<std::string>();
                result["url"] = row["url_kuitansi"].as<std::string>();
            }
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return result;
}

Json::Value DonasiProcess::get_transaksi_by_nomor(std::string_view nomor) const {
    Json::Value result;
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("SELECT a.nomor_kredit, a.keterangan, a.tanggal, b.id_sumber_dana, c.id_pelaksana, a.jenis_transaksi_id, a.url_kuitansi, a.nominal FROM donasi.kredit a"
                                                                    " LEFT JOIN donasi.sumber_dana b on a.kode = b.kode"
                                                                    " LEFT JOIN donasi.pelaksana c on a.pelaksana_id = c.id_pelaksana"
                                                                    " WHERE a.status_delete = false and a.nomor_kredit = $1", nomor.data());
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            for (auto row: f_get) {
                result["pelaksana"] = Json::Int(row["id_pelaksana"].as<int>());
                result["sumber_dana"] = Json::Int(row["id_sumber_dana"].as<int>());
                int64_t nominal = row["nominal"].as<int64_t>();
                std::string nominal_str = boost::lexical_cast<std::string>(nominal);
                result["nominal"] = Money::getInstance().toMoneyFormat(nominal_str, ",", "");
                result["tanggal"] = row["tanggal"].as<std::string>();
                result["jenis_transaksi"] = Json::Int(row["jenis_transaksi_id"].as<int>());
                result["keterangan"] = row["keterangan"].as<std::string>();
                result["url"] = row["url_kuitansi"].as<std::string>();
            }
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return result;
}

bool DonasiProcess::delete_donasi(std::string_view nomor) const {
    std::string_view tipe = nomor.substr(4, 1);
    int id_tipe = boost::lexical_cast<int>(tipe.data());
    int current_num = current_number(id_tipe);
    try {
        auto res = m_db->execSqlSync("update donasi.donasi set status_delete = $1 where nomor = $2", true, nomor.data());
        if (res.affectedRows() > 0) {
            update_current_number(id_tipe, current_num - 1);
            auto exe = m_db->execSqlSync("update donasi.debet set status_delete = $1 where nomor = $2", true, nomor.data());
            return exe.affectedRows() > 0;
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return false;
}

int DonasiProcess::get_num_transaksi_by_nomor(std::string_view nomor) const {
    int num = 0;
    auto res = m_db->execSqlAsyncFuture("select * from donasi.kredit where nomor_kredit = $1", nomor.data());
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        num = f_get.size();
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return num;
}

int DonasiProcess::get_num_debet_by_nomor(std::string_view nomor) const {
    int num = 0;
    auto res = m_db->execSqlAsyncFuture("select * from donasi.debet where nomor = $1", nomor.data());
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        num = f_get.size();
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return num;
}

bool DonasiProcess::delete_transaksi(std::string_view nomor) const {
    if (get_num_transaksi_by_nomor(nomor) > 0) {
        try {
            auto remove_kredit = m_db->execSqlSync("delete from donasi.kredit where nomor_kredit = $1", nomor.data());
            if (remove_kredit.affectedRows() > 0){
                if (get_num_debet_by_nomor(nomor) > 0) {
                    auto remove_debet = m_db->execSqlSync("delete from donasi.debet where nomor = $1", nomor.data());
                    return remove_debet.affectedRows() > 0;
                }
                return true;
            }
            return false;
        } catch (const drogon::orm::DrogonDbException& e) {
            std::cout << e.base().what() << std::endl;
        }
    }
    else {
        try {

            auto remove_debet = m_db->execSqlSync("delete from donasi.debet where nomor = $1", nomor.data());
            return remove_debet.affectedRows();
        } catch (const drogon::orm::DrogonDbException& e) {
            std::cout << e.base().what() << std::endl;
        }
    }
    return false;
}

std::vector<BukuJenisTransaksi> DonasiProcess::get_buku_transkasi_by_jenis(int id_jenis) const {
    std::vector<BukuJenisTransaksi> result;
    auto res = m_db->execSqlAsyncFuture("select * from donasi.buku_transaksi where jenis_transaksi_id = $1 order by id_buku_transaksi desc", id_jenis);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            result.reserve(f_get.size());
            for(auto row: f_get) {
                result.emplace_back(
                    row["id_buku_transaksi"].as<int>(),
                    row["buku_id"].as<int>(),
                    row["jenis_transaksi_id"].as<int>(),
                    row["tipe"].as<std::string_view>()
                );
            }
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return result;
}

uint64_t DonasiProcess::total_debet_by_buku(int buku_id) const {
    uint64_t total = 0;
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select sum(nominal) as jml from donasi.debet where buku_id = $1 and status_delete = $2", buku_id, false);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            for (auto row: f_get)
                total = row["jml"].as<uint64_t>();
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return total;
}

uint64_t DonasiProcess::total_kredit_by_buku(int buku_id) const {
    uint64_t total = 0;
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select sum(nominal) as jml from donasi.kredit where buku_id = $1 and status_delete = $2", buku_id, false);
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() > 0) {
            for (auto row: f_get)
                total = row["jml"].as<uint64_t>();
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << e.base().what() << std::endl;
    }
    return total;
}

Json::Value DonasiProcess::get_detail_buku(int buku_id) const {
    Json::Value result;
    uint64_t total_debet = total_debet_by_buku(buku_id);
    uint64_t total_kredit = total_kredit_by_buku(buku_id);
    uint64_t total_saldo = total_debet - total_kredit;
    result["total_debet"] = Money::getInstance().toMoneyFormat(boost::lexical_cast<std::string>(total_debet), ".", "");
    result["total_kredit"] = Money::getInstance().toMoneyFormat(boost::lexical_cast<std::string>(total_kredit), ".", "");
    result["total_saldo"] = Money::getInstance().toMoneyFormat(boost::lexical_cast<std::string>(total_saldo), ".", "");
    std::vector<SumberDana> sumber_dana_all = get_all_sumberdana();
    std::vector<SumberDana>::iterator it;
    Json::Value sub_result(Json::arrayValue);
    int i = 0;
    for (it = sumber_dana_all.begin(); it != sumber_dana_all.end(); ++it) {
        Json::Value sub;
        it->set_buku_id(buku_id);
        sub = it->get_data_json();
        uint64_t debet = it->count_debet(m_db);
        uint64_t kredit = it->count_kredit(m_db);
        uint64_t saldo = debet - kredit;
        sub["debet"] = Money::getInstance().toMoneyFormat(boost::lexical_cast<std::string>(debet), ".", "");
        sub["kredit"] = Money::getInstance().toMoneyFormat(boost::lexical_cast<std::string>(kredit), ".", "");
        sub["saldo"] = Money::getInstance().toMoneyFormat(boost::lexical_cast<std::string>(saldo), ".", "");
        sub_result[i] = sub;
        ++i;
    };
    result["per_pos"] = sub_result;
    return result;
}

Json::Value DonasiProcess::get_saldo_sumber_dana() const {
    Json::Value result;
    std::vector<SumberDana> sumber_dana_all = get_all_sumberdana();
    std::vector<SumberDana>::iterator it;
    Json::Value sub_result(Json::arrayValue);
    int i = 0;
    uint64_t total_debet = 0;
    uint64_t total_kredit = 0;
    uint64_t total = 0;
    for (it = sumber_dana_all.begin(); it != sumber_dana_all.end(); ++it) {
        Json::Value sub;
        sub = it->get_data_json();
        uint64_t debet = it->count_debet(m_db);
        total_debet += debet;
        uint64_t kredit = it->count_kredit(m_db);
        total_kredit += kredit;
        uint64_t saldo = debet - kredit;
        total += saldo;
        sub["debet"] = Money::getInstance().toMoneyFormat(boost::lexical_cast<std::string>(debet), ".", "");
        sub["kredit"] = Money::getInstance().toMoneyFormat(boost::lexical_cast<std::string>(kredit), ".", "");
        sub["saldo"] = Money::getInstance().toMoneyFormat(boost::lexical_cast<std::string>(saldo), ".", "");
        sub_result[i] = sub;
        ++i;
    };
    result["per_pos"] = sub_result;
    result["total_debet"] =  Money::getInstance().toMoneyFormat(boost::lexical_cast<std::string>(total_debet), ".", "");
    result["total_kredit"] = Money::getInstance().toMoneyFormat(boost::lexical_cast<std::string>(total_kredit), ".", "");
    result["total"] = Money::getInstance().toMoneyFormat(boost::lexical_cast<std::string>(total), ".", "");
    return result;
}
