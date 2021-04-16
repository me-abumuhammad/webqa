#include "utils.hpp"

#include <fmt/format.h>
#include <tuple>

namespace bangkong {
jwt::decoded_jwt decode_jwt_string(std::string_view token) {
    std::vector<std::string> dta = split_string<std::string>(token.data(), " ");
    std::string& t = dta.at(1);
    auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256(bangkong::env<std::string>::value(bangkong::HAS_KEY)))
            .with_issuer("auth0");
    auto decoded = jwt::decode(t);
    return decoded;
}

std::string create_num(const std::string& tgl, int id_donasi, int current_number) {
    std::string thn = tgl.substr(2, 2);
    std::string bln = tgl.substr(5, 2);
    std::string donasi = boost::lexical_cast<std::string>(id_donasi);
    std::string current = boost::lexical_cast<std::string>(current_number);
    int num_loop = 4 - current.length();
    std::string nomor = "";

    for (int i = 0; i < num_loop; ++i) {
        nomor.append("0");
    }

    std::string result = fmt::format("{}{}{}{}{}", thn, bln, donasi, nomor, current);
    return result;
}

std::string create_num_kredit(const std::string& tgl, int id_donasi, int current_number, int tipe) {
    std::string thn = tgl.substr(2, 2);
    std::string bln = tgl.substr(5, 2);
    std::string donasi = boost::lexical_cast<std::string>(id_donasi);
    std::string current = boost::lexical_cast<std::string>(current_number);
    int num_loop = 4 - current.length();
    std::string nomor = "";

    for (int i = 0; i < num_loop; ++i) {
        nomor.append("0");
    }
    std::string result = fmt::format("{}{}{}{}{}{}", thn, bln, donasi, tipe, nomor, current);
    return result;
}

std::tuple<int, int, int> tgl_to_int(const std::string& tgl) {
    int tahun = boost::lexical_cast<int>(tgl.substr(0, 4));
    int bulan = boost::lexical_cast<int>(tgl.substr(5, 2));
    int hari = boost::lexical_cast<int>(tgl.substr(8, 2));
    std::tuple<int, int, int> tpl = std::make_tuple(tahun, bulan, hari);
    return tpl;
}
}
