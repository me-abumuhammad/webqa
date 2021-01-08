#include "menucontroller.hpp"

#include "../models/response.hpp"
#include "../utils/verifheader.hpp"

MenuController::MenuController()
{

}

void MenuController::all_menu(bangkong::ReqParam::type req, bangkong::RespParam::type callback) const {
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
                m_db->get_all_menu([&rs](std::optional<std::vector<Header>> res){
                    std::cout << res->size() << std::endl;
                    Json::Value arr(Json::arrayValue);
                    if (res.has_value()) {
                        std::vector<Header>::const_iterator it;
                        int i = 0;
                        for (it = res.value().begin(); it != res.value().end(); ++it) {
                            arr[i] = it->get_data_json();
                            ++i;
                        }
                    }
                    rs.set_data_json(std::move(arr));
                    rs.build_success();
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
