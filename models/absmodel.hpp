#ifndef ABSMODEL_HPP
#define ABSMODEL_HPP

#include <json/json.h>
#include <drogon/drogon.h>

namespace bangkong {
    struct DonasiData {
        int donatur;
        int jenis_donasi;
        std::string nominal;
        std::string tanggal;
        int penerima;
        int jenis_transaksi;
        std::string keterangan;
        std::string url;

        void print() {
            std::cout << donatur << " " << jenis_donasi << " " << nominal << " " << tanggal << " " << penerima << " " << jenis_transaksi << " " << keterangan << " " << url << '\n';
        }
    };
}

namespace drogon {
template<>
inline bangkong::DonasiData fromRequest(const HttpRequest &req) {
    auto json = req.getJsonObject();
    bangkong::DonasiData donasi;
    if (json) {
        donasi.donatur = (*json)["donatur"].asInt();
        donasi.jenis_donasi = (*json)["jenis_donasi"].asInt();
        donasi.nominal = (*json)["nominal"].asString();
        donasi.tanggal = (*json)["tanggal"].asString();
        donasi.penerima = (*json)["penerima"].asInt();
        donasi.jenis_transaksi = (*json)["jenis_transaksi"].asInt();
        donasi.keterangan = (*json)["keterangan"].asString();
        donasi.url = (*json)["url"].asString();
    }
    return donasi;
}
}

class AbsModel
{
public:
    AbsModel() {}
    virtual ~AbsModel() {}

    virtual Json::Value get_data_json() const = 0;
};

#endif // ABSMODEL_HPP
