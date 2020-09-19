#include "response.hpp"

Response::Response() {
    m_resp["data"] = Json::Value();
}

Response::Response(Json::Value&& data)
{
    m_resp["data"] = data;
}

Response& Response::set_data_json(Json::Value &&json) {
    m_resp["data"] = json;
    return *this;
}

Response& Response::build_success() {
    m_resp["status"] = "Ok";
    m_resp["message"] = "success";
    return *this;
}

Response& Response::build_failed(std::string_view msg) {
    m_resp["status"] = "No";
    m_resp["message"] = msg.data();
    return *this;
}

Json::Value Response::get_data_json() const {
    return m_resp;
}
