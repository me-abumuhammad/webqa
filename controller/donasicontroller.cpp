#include "donasicontroller.hpp"

#include "../models/response.hpp"
#include "../utils/jsonconverter.hpp"
#include "../utils/verifheader.hpp"

void DonasiController::login(const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback) const {
    Akun akun;
    drogon::HttpResponsePtr resp =  drogon::HttpResponse::newHttpResponse();
    resp->setContentTypeCode(drogon::ContentType::CT_APPLICATION_JSON);
    Response rs {};
    if (req->method() != drogon::HttpMethod::Post) {
        rs.build_failed("Method not allowed");
        resp->setStatusCode(drogon::HttpStatusCode::k405MethodNotAllowed);
    }
    else {
        std::unordered_map<std::string, std::string> headers = req->headers();
        if (headers["content-type"] == "application/json") {
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
                rs.set_data_json(akun.get_data_json());
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
            rs.build_failed("Not acceptable");
            resp->setStatusCode(drogon::HttpStatusCode::k406NotAcceptable);
        }
    }
    resp->setBody(rs.get_data_json().toStyledString());
    callback(resp);
}

void DonasiController::update_rekap(const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback) const {
    Rekap rekap;
    drogon::HttpResponsePtr resp =  drogon::HttpResponse::newHttpResponse();
    resp->setContentTypeCode(drogon::ContentType::CT_APPLICATION_JSON);
    Response rs {};
    if (req->method() != drogon::HttpMethod::Post) {
        rs.build_failed("Method not allowed");
        resp->setStatusCode(drogon::HttpStatusCode::k405MethodNotAllowed);
    }
    else {
        std::unordered_map<std::string, std::string> headers = req->headers();
        std::string message = "";
        VerifHeader verif {m_db};
        bool auth = verif.check_authorization_token(headers.at("authorization"), message);
        if (auth == true) {
            if (headers["content-type"] == "application/json") {
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
                        nama = json.first["kode"].asCString();
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
                rs.build_failed("Not acceptable");
                resp->setStatusCode(drogon::HttpStatusCode::k406NotAcceptable);
            }
        }
        else {
            rs.build_failed(message);
            resp->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);
        }
    }
    resp->setBody(rs.get_data_json().toStyledString());
    callback(resp);
}

void DonasiController::all_rekap(const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpRequestPtr &)> &&callback) const {
    Json::Value all;
    drogon::HttpResponsePtr resp =  drogon::HttpResponse::newHttpResponse();
    resp->setContentTypeCode(drogon::ContentType::CT_APPLICATION_JSON);
    Response rs {};
    if (req->method() != drogon::HttpMethod::Get) {
        rs.build_failed("Method not allowed");
        resp->setStatusCode(drogon::HttpStatusCode::k405MethodNotAllowed);
    }
    else {
        std::unordered_map<std::string, std::string> headers = req->headers();
        std::string message = "";
        VerifHeader verif {m_db};
        bool auth = verif.check_authorization_token(headers.at("authorization"), message);
        if (auth == true) {
            if (headers["content-type"] == "application/json") {

            }
        }
    }
}
