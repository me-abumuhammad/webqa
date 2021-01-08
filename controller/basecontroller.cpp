#include "basecontroller.hpp"

#include "../utils/utils.hpp"

BaseController::BaseController()
{

}

std::string BaseController::base_url() const {
    return bangkong::BASE_URL;
}

drogon::HttpResponsePtr BaseController::create_handle_header(std::string_view method) const {
    drogon::HttpResponsePtr resp = drogon::HttpResponse::newHttpResponse();
    resp->addHeader("Access-Control-Allow-Origin", "*");
    resp->addHeader("Access-Control-Allow-Methods", method.data());
    resp->addHeader("Access-Control-Allow-Headers",
                                   "x-requested-with,content-type");
    resp->addHeader("Access-Control-Allow-Credentials","true");
    resp->setContentTypeCode(drogon::ContentType::CT_APPLICATION_JSON);
    return resp;
}

bool BaseController::check_header_key(const std::unordered_map<std::string, std::string> &headers,
                                      std::vector<std::pair<std::string, std::string>> &&types,
                                      std::string &message,
                                      drogon::HttpStatusCode& method) const {
    for (auto &item: types) {
        auto fd = headers.find(item.first);
        if (fd == headers.end()) {
            if (item.first == "authorization") {
                message = "Authorization not found";
                method = drogon::HttpStatusCode::k401Unauthorized;
            }
            else if (item.first == "content-type") {
                message = "Content type not found";
                method = drogon::HttpStatusCode::k406NotAcceptable;
            }
            return false;
        }
        else {
            if (item.first == "content-type") {
                if (item.second != headers.at(item.first)) {
                    message = "Not acceptable";
                    return false;
                }
            }
        }
    }
    return true;
}

