#include "bukucontroller.hpp"

#include "../models/buku.hpp"
#include "../models/response.hpp"
#include "../utils/verifheader.hpp"
#include "../utils/jsonconverter.hpp"

BukuController::BukuController()
{

}

void BukuController::add_book(bangkong::ReqParam::type req,
                              bangkong::RespParam::type callback) const {
    drogon::HttpResponsePtr resp = create_handle_header("POST");
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
                    std::string_view nama = "";
                    std::string_view bg = "";
                    bool status = true;
                    if (json.first["nama"].isNull() == false) {
                        nama = json.first["nama"].asCString();
                    }
                    if (json.first["bg "].isNull() == false) {
                        bg = json.first["bg"].asCString();
                    }
                    m_db->add(Buku {nama, status, bg}, [&rs](std::optional<Buku> res){
                        if (res.has_value()) {
                            rs.set_data_json(res.value().get_data_json());
                            rs.build_success();
                        }
                        else {
                            rs.build_failed("Buku gagal disimpan");
                        }
                    }, message);
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

void BukuController::book(bangkong::ReqParam::type req,
                          bangkong::RespParam::type callback,
                          int id_buku) const {
    drogon::HttpResponsePtr resp = create_handle_header("GET");
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
                m_db->get_buku_by_id(id_buku, [&rs](std::optional<Buku> res) {
                    if (res.has_value()) {
                        rs.set_data_json(res.value().get_data_json());
                        rs.build_success();
                    }
                    else {
                        rs.build_failed("Buku tidak ada");
                    }
                });
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

void BukuController::all_book(bangkong::ReqParam::type req,
                              bangkong::RespParam::type callback) const {
    drogon::HttpResponsePtr resp = create_handle_header("GET");
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
                Json::Value vl;
                Json::Value arr =  m_db->get_data_saldo();
                vl["buku"] = arr;
                vl["sumberdana"] = m_donasi->get_saldo_sumber_dana();
                rs.set_data_json(std::move(vl));
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

void BukuController::update_book(bangkong::ReqParam::type req,
                                 bangkong::RespParam::type callback) const {
    drogon::HttpResponsePtr resp = create_handle_header("PUT");
    Response rs{};
    if (req->method() != drogon::HttpMethod::Put) {
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
                    std::string_view nama = "";
                    bool status = true;
                    int id_buku = 0;
                    std::string_view bg = "";
                    if (json.first["nama"].isNull() == false) {
                        nama = json.first["nama"].asCString();
                    }
                    if (json.first["id_buku"].isNull() == false) {
                        id_buku = json.first["id_buku"].asInt();
                    }
                    if (json.first["status"].isNull() == false) {
                        status = json.first["status"].asBool();
                    }
                    if (json.first["bg"].isNull() == false) {
                        bg = json.first["bg"].asCString();
                    }
                    m_db->update_buku(Buku {id_buku, nama, status, bg}, [&rs](std::optional<Buku> res){
                        if (res.has_value()) {
                            rs.set_data_json(res.value().get_data_json());
                            rs.build_success();
                        }
                        else {
                            rs.build_failed("Buku gagal diupdate");
                        }
                    });
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

void BukuController::delete_book(bangkong::ReqParam::type req,
                                 bangkong::RespParam::type callback, int id_buku) const {
    drogon::HttpResponsePtr resp = create_handle_header("DELETE");
    Response rs{};
    if (req->method() != drogon::HttpMethod::Delete) {
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
                if (m_db->delete_buku(id_buku)) {
                    rs.build_success();
                }
                else {
                    rs.build_failed("Data gagal dihapus");
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

void BukuController::detail_buku(bangkong::ReqParam::type req, bangkong::RespParam::type callback, int id_buku) const {
    drogon::HttpResponsePtr resp = create_handle_header("GET");
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
                Json::Value book;
                m_db->get_buku_by_id(id_buku, [&book](std::optional<Buku> res){
                    if (res.has_value()) {
                        book = res.value().get_data_json();
                    }
                });
                Json::Value data = m_donasi->get_detail_buku(id_buku);
                book["info"] = data;
                rs.set_data_json(std::move(book));
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

void BukuController::ubhar(bangkong::ReqParam::type req,
                           bangkong::RespParam::type callback) const {
    std::cout << req->getJsonObject() << std::endl;
    drogon::MultiPartParser parser;
    std::pair<Json::Value, JSONCPP_STRING> json = bangkong::JsonConverter()
            .set_body(req->body())
            .build_json_from_string()
            .data_json();
    std::cout << json.first.toStyledString() << std::endl;
    drogon::HttpResponsePtr resp = drogon::HttpResponse::newHttpResponse();
    resp->setBody("");
    callback(resp);
}
