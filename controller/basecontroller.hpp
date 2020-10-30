#ifndef BASECONTROLLER_HPP
#define BASECONTROLLER_HPP

#include <drogon/drogon.h>
#include <string>

namespace bangkong {
const static std::pair<std::string, std::string> AUTHORIZATION_PAIR = {"authorization", ""};
const static std::pair<std::string, std::string> CONTENT_TYPE_PAIR = {"content-type", "application/json"};

struct header_vec {
    using type = typename std::vector<std::pair<std::string, std::string>>;
};
}

class BaseController
{
public:
    BaseController();
    ~BaseController() {}
protected:
    std::string base_url() const;
    drogon::HttpResponsePtr create_handle_header(std::string_view method) const;
    bool check_header_key(const std::unordered_map<std::string, std::string> &,
                          std::vector<std::pair<std::string, std::string>>&&,
                          std::string&,
                          drogon::HttpStatusCode&) const;
};

#endif // BASECONTROLLER_HPP
