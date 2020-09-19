#include "donasicontroller.hpp"

#include "../models/response.hpp"
#include "../utils/jsonconverter.hpp"

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
