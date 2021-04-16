#include "transaksicontroller.hpp"

#include "../models/response.hpp"
#include "../models/pelaksana.hpp"
#include "../models/sumberdana.hpp"
#include "../models/jenistransaksi.hpp"
#include "../utils/jsonconverter.hpp"
#include "../utils/verifheader.hpp"
#include "../utils/utils.hpp"


TransaksiController::TransaksiController()
{

}

void TransaksiController::form_transaksi(bangkong::ReqParam::type req, bangkong::RespParam::type callback) const {
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
            std::vector<Pelaksana> pelaksana = m_donasi->get_all_pelaksana();
            std::vector<SumberDana> sumberdana = m_donasi->get_all_sumberdana();
            std::vector<JenisTransaksi> jenis_transaksi = m_jenis_transaksi->get_all_jenis_transaksi();

            Json::Value data_json;
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

void TransaksiController::save_transaksi(bangkong::ReqParam::type req, bangkong::RespParam::type callback) const {
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
                    bool stat = m_donasi->save_kredit(std::move(json.first));
                    if (stat == true)
                        rs.build_success();
                    else
                        rs.build_failed("Data gagal disimpan");
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

void TransaksiController::all_transaction(bangkong::ReqParam::type req, bangkong::RespParam::type callback) const {
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
                Json::Value data = m_donasi->get_all_transaction();
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

void TransaksiController::transaksi(bangkong::ReqParam::type req, bangkong::RespParam::type callback, std::string nomor) const {
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
                std::vector<Pelaksana> pelaksana = m_donasi->get_all_pelaksana();
                std::vector<SumberDana> sumberdana = m_donasi->get_all_sumberdana();
                std::vector<JenisTransaksi> jenis_transaksi = m_jenis_transaksi->get_all_jenis_transaksi();

                Json::Value data_json;
                data_json["pelaksana"] = parse_to_json_array<Pelaksana>(std::move(pelaksana));
                data_json["sumber_dana"] = parse_to_json_array<SumberDana>(std::move(sumberdana));
                data_json["jenis_transaksi"] = parse_to_json_array<JenisTransaksi>(std::move(jenis_transaksi));

                Json::Value data_transaksi = m_donasi->get_transaksi_by_nomor(nomor);
                data_json["transaksi"] = std::move(data_transaksi);

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

void TransaksiController::update_transaksi(bangkong::ReqParam::type req, bangkong::RespParam::type callback) const {
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
                    bool stat = m_donasi->update_kredit(std::move(json.first));
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

void TransaksiController::delete_transaksi(bangkong::ReqParam::type req, bangkong::RespParam::type callback) const {
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
                    bool del = m_donasi->delete_transaksi(json.first["nomor"].asString());
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
Json::Value TransaksiController::parse_to_json_array(std::vector<T>&& data) const {
    Json::Value arr(Json::arrayValue);
    int i = 0;
    std::for_each(data.begin(), data.end(), [&arr, &i](const T& d) {
        arr[i] = d.get_data_json();
        ++i;
    });
    return arr;
}
