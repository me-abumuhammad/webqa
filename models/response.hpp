#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "absmodel.hpp"
#include <json/json.h>

class Response: public AbsModel
{
public:
    Response();
    Response(Json::Value&&);
    Response(const Response&) = delete ;
    Response(Response&&) = delete ;
    Response& set_data_json(Json::Value&&);
    Json::Value get_data_json() const override;
    Response& build_success();
    Response& build_failed(std::string_view msg = "failed");
private:
    Json::Value m_resp;

};

#endif // RESPONSE_HPP
