#ifndef JSONCONVERTER_HPP
#define JSONCONVERTER_HPP

#include <string_view>
#include <json/json.h>
#include <memory>
#include <fstream>

namespace bangkong {
class JsonConverter
{
public:
    JsonConverter() {}
    explicit JsonConverter(std::string_view body): m_body {body} {}
    JsonConverter& set_body(std::string_view body) {
        m_body = body;
        return *this;
    }
    JsonConverter& build_json_from_string() noexcept {
        Json::CharReaderBuilder builder;
        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        reader->parse(m_body.data(), m_body.data() + m_body.length(), &root, &error);
        return *this;
    }

    JsonConverter& build_json_from_file() noexcept {
        std::ifstream ifs;
        ifs.open(m_body.data());

        Json::CharReaderBuilder builder;
        builder["collectComments"] = true;
        Json::parseFromStream(builder, ifs, &root, &error);
        return *this;
    }

    std::pair<Json::Value, JSONCPP_STRING> data_json() noexcept {
        return std::make_pair<Json::Value, JSONCPP_STRING>(std::move(root), std::move(error));
    }

private:
    std::string_view m_body;
    JSONCPP_STRING error;
    Json::Value root;
};
}



#endif // JSONCONVERTER_HPP
