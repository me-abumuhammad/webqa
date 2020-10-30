#include "bukucontroller.hpp"

#include "../models/buku.hpp"
#include "../models/response.hpp"
#include "../utils/verifheader.hpp"
#include "../utils/jsonconverter.hpp"

BukuController::BukuController()
{

}

void BukuController::add_book(const drogon::HttpRequestPtr &req,
                              std::function<void (const drogon::HttpResponsePtr &)> &&callback) const {
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
                    bool status = true;
                    if (json.first["nama"].isNull() == false) {
                        nama = json.first["nama"].asCString();
                    }
                    m_db->add(Buku {nama, status}, [&rs](std::optional<Buku> res){
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

void BukuController::book(const drogon::HttpRequestPtr &req,
                          std::function<void (const drogon::HttpResponsePtr &)> &&callback,
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
