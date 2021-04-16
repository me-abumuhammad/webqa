//
// Created by Abu Muhammad on 4/8/21.
//

#include "muhsinincontroller.h"
#include "../utils/verifheader.hpp"
#include "../utils/utils.hpp"

void MuhsininController::add_muhsinin(bangkong::ReqParam::type req, bangkong::RespParam::type callback) const {
    drogon::HttpResponsePtr resp = create_handle_header("POST");
    Response rs{};
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
            
        }
    }
}