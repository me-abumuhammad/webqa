#include "donasicontroller.hpp"

#include "../models/response.hpp"
#include "../models/muhsinin.hpp"
#include "../models/pelaksana.hpp"
#include "../models/sumberdana.hpp"
#include "../models/jenistransaksi.hpp"
#include "../utils/jsonconverter.hpp"
#include "../utils/verifheader.hpp"
#include "../utils/utils.hpp"

void DonasiController::login(const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback) const {
    Akun akun;
    drogon::HttpResponsePtr resp =  create_handle_header("POST");
    Response rs {};
    if (req->method() != drogon::HttpMethod::Post) {
        rs.build_failed("Method not allowed");
        resp->setStatusCode(drogon::HttpStatusCode::k405MethodNotAllowed);
    }
    else {
        std::unordered_map<std::string, std::string> headers = req->headers();
        std::string message = "";
        bangkong::header_vec::type types {bangkong::CONTENT_TYPE_PAIR};
        drogon::HttpStatusCode status_code;
        bool auth = check_header_key(headers, std::move(types), message, status_code);
        if (auth == true) {
            std::pair<Json::Value, JSONCPP_STRING> json = bangkong::JsonConverter()
                    .set_body(req->body())
                    .build_json_from_string()
                    .data_json();
            if (json.second == "") {
                m_db->login(Akun{json.first["username"].asString(), json.first["password"].asString(), ""}, [&akun](std::optional<Akun> res) {
                    if (res.has_value()) {
                        akun = res.value();
                    }
                });
                Json::Value akun_json = akun.get_data_json();
                m_db->save_token(akun_json);
                rs.set_data_json(std::move(akun_json));
                if (akun.get_username() == "") {
                    rs.build_failed("Wrong username or password");
                }
                else {
                    rs.build_success();
                }
                resp->setStatusCode(drogon::HttpStatusCode::k200OK);
            }
            else {
                rs.build_failed("Bad request");
                resp->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
            }
        }
        else {
            rs.build_failed(message);
            resp->setStatusCode(status_code);
        }
    }
    resp->setBody(rs.get_data_json().toStyledString());
    callback(resp);
}

void DonasiController::update_rekap(const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback) const {
    Rekap rekap;
    drogon::HttpResponsePtr resp =  create_handle_header("POST");
    Response rs {};
    if (req->method() != drogon::HttpMethod::Post) {
        rs.build_failed("Method not allowed");
        resp->setStatusCode(drogon::HttpStatusCode::k405MethodNotAllowed);
    }
    else {
        std::unordered_map<std::string, std::string> headers = req->headers();
        std::string message = "";
        bangkong::header_vec::type types {bangkong::AUTHORIZATION_PAIR, bangkong::CONTENT_TYPE_PAIR};
        drogon::HttpStatusCode status_code;
        bool auth = check_header_key(headers, std::move(types), message, status_code);
        if (auth == true) {
            VerifHeader verif{m_db};
            bool token_is_valid = verif.check_authorization_token(headers.at("authorization"), message);
            if (token_is_valid == true) {
                std::pair<Json::Value, JSONCPP_STRING> json = bangkong::JsonConverter()
                        .set_body(req->body())
                        .build_json_from_string()
                        .data_json();
                if (json.second == "") {
                    std::string_view kode = "";
                    std::string_view nama = "";
                    uint64_t nominal = 0;
                    if (json.first["kode"].isNull() == false) {
                        kode = json.first["kode"].asCString();
                    }
                    if (json.first["nama"].isNull() == false) {
                        nama = json.first["nama"].asCString();
                    }
                    if (json.first["nominal"].isNull() == false) {
                        nominal = json.first["nominal"].asUInt64();
                    }
                    m_db->update_rekap(Rekap {kode,
                                              nama,
                                              nominal}, [&rekap](std::optional<Rekap> res){
                        if (res.has_value())
                            rekap = res.value();
                    });
                    rs.set_data_json(rekap.get_data_json());
                    if(rekap.get_code() == "") {
                        rs.build_failed("Data gagal disimpan");
                    }
                    else {
                        rs.build_success();
                    }

                    resp->setStatusCode(drogon::HttpStatusCode::k200OK);
                }
                else {
                    rs.build_failed("Key failed");
                    resp->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
                }
            }
            else {
                rs.build_failed(message);
                resp->setStatusCode(status_code);
            }
        }
        else {
            rs.build_failed(message);
            resp->setStatusCode(status_code);
        }
    }
    resp->setBody(rs.get_data_json().toStyledString());
    callback(resp);
}

void DonasiController::all_rekap(const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback) const {
    Json::Value all;
    drogon::HttpResponsePtr resp =  create_handle_header("GET");
    Response rs {};
    if (req->method() != drogon::HttpMethod::Get) {
        rs.build_failed("Method not allowed");
        resp->setStatusCode(drogon::HttpStatusCode::k405MethodNotAllowed);
    }
    else {
        std::unordered_map<std::string, std::string> headers = req->headers();
        std::string message = "";
        bangkong::header_vec::type types {bangkong::AUTHORIZATION_PAIR, bangkong::CONTENT_TYPE_PAIR};
        drogon::HttpStatusCode status_code;
        bool auth = check_header_key(headers, std::move(types), message, status_code);
        if (auth == true) {
            VerifHeader verif{m_db};
            bool token_is_valid = verif.check_authorization_token(headers.at("authorization"), message);
            if (token_is_valid == true) {
                std::vector<Rekap> rek;
                m_db->get_all_donasi([&rek](std::optional<std::vector<Rekap>> res){
                    if (res.has_value()) {
                        rek.reserve(res.value().size());
                        rek.insert(rek.begin(),
                                   std::make_move_iterator(res.value().begin()),
                                   std::make_move_iterator(res.value().end()));
                    }
                });
                std::vector<Rekap>::const_iterator it;
                Json::Value jsn(Json::arrayValue);
                int i = 0;
                for (it = rek.begin(); it != rek.end(); ++it) {
                    jsn[i] = it->get_data_json();
                    ++i;
                }
                rs.set_data_json(std::move(jsn));
                rs.build_success();
                resp->setStatusCode(drogon::HttpStatusCode::k200OK);
            }
            else {
                rs.build_failed(message);
                resp->setStatusCode(status_code);
            }
        }
        else {
            rs.build_failed(message);
            resp->setStatusCode(status_code);
        }
    }
    resp->setBody(rs.get_data_json().toStyledString());
    callback(resp);
}

void DonasiController::rekap_for_bot(const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback) const {
    Json::Value all;
    drogon::HttpResponsePtr resp =  create_handle_header("GET");
    Response rs {};
    if (req->method() != drogon::HttpMethod::Get) {
        rs.build_failed("Method not allowed");
        resp->setStatusCode(drogon::HttpStatusCode::k405MethodNotAllowed);
    }
    else {
        std::unordered_map<std::string, std::string> headers = req->headers();
        std::string message = "";
        bangkong::header_vec::type types {bangkong::AUTHORIZATION_PAIR, bangkong::CONTENT_TYPE_PAIR};
        drogon::HttpStatusCode status_code;
        bool auth = check_header_key(headers, std::move(types), message, status_code);
        if (auth == true) {
            std::vector<Rekap> rek;
            m_db->get_all_donasi([&rek](std::optional<std::vector<Rekap>> res){
                if (res.has_value()) {
                    rek.reserve(res.value().size());
                    rek.insert(rek.begin(),
                               std::make_move_iterator(res.value().begin()),
                               std::make_move_iterator(res.value().end()));
                }
            });
            std::vector<Rekap>::const_iterator it;
            Json::Value jsn(Json::arrayValue);
            int i = 0;
            for (it = rek.begin(); it != rek.end(); ++it) {
                jsn[i] = it->get_data_json();
                ++i;
            }
            rs.set_data_json(std::move(jsn));
            rs.build_success();
            resp->setStatusCode(drogon::HttpStatusCode::k200OK);
        }
        else {
            rs.build_failed(message);
            resp->setStatusCode(status_code);
        }
    }
    resp->setBody(rs.get_data_json().toStyledString());
    callback(resp);
}

void DonasiController::form_donasi(bangkong::ReqParam::type req, bangkong::RespParam::type callback) const {
    drogon::HttpResponsePtr resp = create_handle_header("GET");
    Response rs{};
    if (req->method() != drogon::HttpMethod::Get) {
        rs.build_failed("Method not allowed");
        resp->setStatusCode(drogon::HttpStatusCode::k405MethodNotAllowed);
    }
    else {
        std::unordered_map<std::string, std::string> headers = req->headers();
        std::string message = "";
        bangkong::header_vec::type types {bangkong::CONTENT_TYPE_PAIR};
        drogon::HttpStatusCode status_code;
        bool auth = check_header_key(headers, std::move(types), message, status_code);

        if (auth == true) {
            std::vector<Muhsinin> muhsinin = m_donasi->get_all_muhsinin();
            std::vector<Pelaksana> pelaksana = m_donasi->get_all_pelaksana();
            std::vector<SumberDana> sumberdana = m_donasi->get_all_sumberdana();
            std::vector<JenisTransaksi> jenis_transaksi = m_jenis_transaksi->get_all_jenis_transaksi();

            Json::Value data_json;
            data_json["muhsinin"] = parse_to_json_array<Muhsinin>(std::move(muhsinin));
            data_json["pelaksana"] = parse_to_json_array<Pelaksana>(std::move(pelaksana));
            data_json["sumber_dana"] = parse_to_json_array<SumberDana>(std::move(sumberdana));
            data_json["jenis_transaksi"] = parse_to_json_array<JenisTransaksi>(std::move(jenis_transaksi));

            rs.set_data_json(std::move(data_json));
            rs.build_success();
            resp->setStatusCode(drogon::HttpStatusCode::k200OK);
        }
        else {
            rs.build_failed(message);
            resp->setStatusCode(status_code);
        }
    }
    resp->setBody(rs.get_data_json().toStyledString());
    callback(resp);
}

void DonasiController::save_donasi(bangkong::ReqParam::type req,
                                   bangkong::RespParam::type callback,
                                   bangkong::DonasiData &&donasi) const {
    drogon::HttpResponsePtr resp = create_handle_header("POST");
    Response rs{};
    if (req->method() != drogon::HttpMethod::Post) {
        rs.build_failed("Method not allowed");
        resp->setStatusCode(drogon::HttpStatusCode::k405MethodNotAllowed);
    }
    else {
        std::unordered_map<std::string, std::string> headers = req->headers();
        std::string message = "";
        bangkong::header_vec::type types {bangkong::AUTHORIZATION_PAIR, bangkong::CONTENT_TYPE_PAIR};
        drogon::HttpStatusCode status_code;
        bool auth = check_header_key(headers, std::move(types), message, status_code);

        if (auth == true) {
            VerifHeader verif{m_db};
            bool token_is_valid = verif.check_authorization_token(headers.at("authorization"), message);
            if (token_is_valid == true) {
                bool stat = m_donasi->save_donasi(std::move(donasi));
                if (stat == true) {
                    rs.build_success();
                }
                else {
                    rs.build_failed("Data gagal disimpan");
                }
                resp->setStatusCode(drogon::HttpStatusCode::k200OK);
            }
            else {
                rs.build_failed(message);
                resp->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);
            }
        }
        else {
            rs.build_failed(message);
            resp->setStatusCode(status_code);
        }
    }
    resp->setBody(rs.get_data_json().toStyledString());
    callback(resp);
}

void DonasiController::update_donasi(bangkong::ReqParam::type req, bangkong::RespParam::type callback) const {
    drogon::HttpResponsePtr resp = create_handle_header("POST");
    Response rs{};
    if (req->method() != drogon::HttpMethod::Post) {
        rs.build_failed("Method not allowed");
        resp->setStatusCode(drogon::HttpStatusCode::k405MethodNotAllowed);
    }
    else {
        std::unordered_map<std::string, std::string> headers = req->headers();
        std::string message = "";
        bangkong::header_vec::type types {bangkong::AUTHORIZATION_PAIR, bangkong::CONTENT_TYPE_PAIR};
        drogon::HttpStatusCode status_code;
        bool auth = check_header_key(headers, std::move(types), message, status_code);

        if (auth == true) {
            VerifHeader verif{m_db};
            bool token_is_valid = verif.check_authorization_token(headers.at("authorization"), message);
            if (token_is_valid == true) {
                std::pair<Json::Value, JSONCPP_STRING> json = bangkong::JsonConverter()
                        .set_body(req->body())
                        .build_json_from_string()
                        .data_json();
                if (json.second == "") {
                    bool stat = m_donasi->update_donasi(std::move(json.first));
                    if (stat == true) {
                        rs.build_success();
                    }
                    else {
                        rs.build_failed("Data gagal diupdate");
                    }
                    resp->setStatusCode(drogon::HttpStatusCode::k200OK);
                }
                else {
                    rs.build_failed("Key failed");
                    resp->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
                }
            }
            else {
                rs.build_failed(message);
                resp->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);
            }
        }
        else {
            rs.build_failed(message);
            resp->setStatusCode(status_code);
        }
    }
    resp->setBody(rs.get_data_json().toStyledString());
    callback(resp);
}

void DonasiController::all_donasi(bangkong::ReqParam::type req, bangkong::RespParam::type callback) const {
    drogon::HttpResponsePtr resp = create_handle_header("GET");
    Response rs{};
    if (req->method() != drogon::HttpMethod::Get) {
        rs.build_failed("Method not allowed");
        resp->setStatusCode(drogon::HttpStatusCode::k405MethodNotAllowed);
    }
    else {
        std::unordered_map<std::string, std::string> headers = req->headers();
        std::string message = "";
        bangkong::header_vec::type types {bangkong::AUTHORIZATION_PAIR, bangkong::CONTENT_TYPE_PAIR};
        drogon::HttpStatusCode status_code;
        bool auth = check_header_key(headers, std::move(types), message, status_code);

        if (auth == true) {
            VerifHeader verif{m_db};
            bool token_is_valid = verif.check_authorization_token(headers.at("authorization"), message);
            if (token_is_valid == true) {
                Json::Value data = m_donasi->get_all_donasi();
                rs.set_data_json(std::move(data));
                rs.build_success();
                resp->setStatusCode(drogon::HttpStatusCode::k200OK);
            }
            else {
                rs.build_failed(message);
                resp->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);
            }
        }
        else {
            rs.build_failed(message);
            resp->setStatusCode(status_code);
        }
    }
    resp->setBody(rs.get_data_json().toStyledString());
    callback(resp);
}

void DonasiController::donasi(bangkong::ReqParam::type req, bangkong::RespParam::type callback, std::string nomor) const {
    drogon::HttpResponsePtr resp = create_handle_header("GET");
    Response rs{};
    if (req->method() != drogon::HttpMethod::Get) {
        rs.build_failed("Method not allowed");
        resp->setStatusCode(drogon::HttpStatusCode::k405MethodNotAllowed);
    }
    else {
        std::unordered_map<std::string, std::string> headers = req->headers();
        std::string message = "";
        bangkong::header_vec::type types{bangkong::AUTHORIZATION_PAIR, bangkong::CONTENT_TYPE_PAIR};
        drogon::HttpStatusCode status_code;
        bool auth = check_header_key(headers, std::move(types), message, status_code);

        if (auth == true) {
            VerifHeader verif{m_db};
            bool token_is_valid = verif.check_authorization_token(headers.at("authorization"), message);
            if (token_is_valid == true) {
                std::vector<Muhsinin> muhsinin = m_donasi->get_all_muhsinin();
                std::vector<Pelaksana> pelaksana = m_donasi->get_all_pelaksana();
                std::vector<SumberDana> sumberdana = m_donasi->get_all_sumberdana();
                std::vector<JenisTransaksi> jenis_transaksi = m_jenis_transaksi->get_all_jenis_transaksi();

                Json::Value data_json;
                data_json["muhsinin"] = parse_to_json_array<Muhsinin>(std::move(muhsinin));
                data_json["pelaksana"] = parse_to_json_array<Pelaksana>(std::move(pelaksana));
                data_json["sumber_dana"] = parse_to_json_array<SumberDana>(std::move(sumberdana));
                data_json["jenis_transaksi"] = parse_to_json_array<JenisTransaksi>(std::move(jenis_transaksi));

                Json::Value data_donasi = m_donasi->get_donasi_by_nomor(nomor);
                data_json["donasi"] = std::move(data_donasi);

                rs.set_data_json(std::move(data_json));
                rs.build_success();
                resp->setStatusCode(drogon::HttpStatusCode::k200OK);
            }
            else {
                rs.build_failed(message);
                resp->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);
            }
        }
        else {
            rs.build_failed(message);
            resp->setStatusCode(status_code);
        }
    }
    resp->setBody(rs.get_data_json().toStyledString());
    callback(resp);
}

void DonasiController::delete_donasi(bangkong::ReqParam::type req, bangkong::RespParam::type callback) const {
    drogon::HttpResponsePtr resp = create_handle_header("POST");
    Response rs{};
    if (req->method() != drogon::HttpMethod::Post) {
        rs.build_failed("Method not allowed");
        resp->setStatusCode(drogon::HttpStatusCode::k405MethodNotAllowed);
    }
    else {
        std::unordered_map<std::string, std::string> headers = req->headers();
        std::string message = "";
        bangkong::header_vec::type types {bangkong::AUTHORIZATION_PAIR, bangkong::CONTENT_TYPE_PAIR};
        drogon::HttpStatusCode status_code;
        bool auth = check_header_key(headers, std::move(types), message, status_code);

        if (auth == true) {

            std::pair<Json::Value, JSONCPP_STRING> json = bangkong::JsonConverter()
                    .set_body(req->body())
                    .build_json_from_string()
                    .data_json();
            if (json.second == "") {
                VerifHeader verif{m_db};
                bool token_is_valid = verif.check_authorization_token(headers.at("authorization"), message);
                if (token_is_valid == true) {
                    bool del = m_donasi->delete_donasi(json.first["nomor"].asString());
                    if (del == true) {
                        rs.build_success();
                    }
                    else
                        rs.build_failed("Data gagal dihapus");
                    resp->setStatusCode(drogon::HttpStatusCode::k200OK);
                }
                else {
                    rs.build_failed(message);
                    resp->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);
                }
            }
            else {
                rs.build_failed("Key failed");
                resp->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
            }
        }
        else {
            rs.build_failed(message);
            resp->setStatusCode(status_code);
        }
    }
    resp->setBody(rs.get_data_json().toStyledString());
    callback(resp);
}


template <typename T>
Json::Value DonasiController::parse_to_json_array(std::vector<T>&& data) const {
    Json::Value arr(Json::arrayValue);
    int i = 0;
    std::for_each(data.begin(), data.end(), [&arr, &i](const T& d) {
        arr[i] = d.get_data_json();
        ++i;
    });
    return arr;
}
