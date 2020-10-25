#include <iostream>

#include <drogon/drogon.h>

#include <boost/lexical_cast.hpp>
#include <fstream>

#include <fmt/format.h>

#include "utils/utils.hpp"
#include "controller/donasicontroller.hpp"
#include "controller/akuncontroller.hpp"
#include "utils/sessionfilter.hpp"

#define USE_POSTGRESQL

using namespace std;

int main()
{
    auto db = drogon::orm::DbClient::newPgClient(bangkong::env<std::string>::value(bangkong::DB_CONNECTION), 1);
    auto donasi_ctrl = std::make_shared<DonasiController>(db);
    drogon::app().registerController(donasi_ctrl);

    auto akun_ctrl = std::make_shared<AkunController>(db);
    drogon::app().registerController(akun_ctrl);

    auto session_filter = std::make_shared<SessionFilter>();
    drogon::app().registerFilter(session_filter);

    drogon::app().registerHandler("/scripts/{path}", [](const drogon::HttpRequestPtr&,
                                  std::function<void(const drogon::HttpResponsePtr&)>&& callback, const std::string& path){
        std::string pth = fmt::format("../scripts/{}", path);
        auto res = drogon::HttpResponse::newFileResponse(pth);
        res->addHeader("Content-Type", "text/javascript");
        callback(res);
    });

    drogon::app().registerHandler("/styles/{path}", [](const drogon::HttpRequestPtr&,
                                  std::function<void(const drogon::HttpResponsePtr&)>&& callback, const std::string& path){
        std::string pth = fmt::format("../styles/{}", path);
        auto res = drogon::HttpResponse::newFileResponse(pth);
        res->addHeader("Content-Type", "text/css");
        callback(res);
    });

    drogon::app().registerHandler("/", [](const drogon::HttpRequestPtr&,
                                  std::function<void(const drogon::HttpResponsePtr&)>&& callback){
        drogon::HttpViewData data;
        data.insert("title", "QA");
        data.insert("base_url", bangkong::BASE_URL);
        auto res = drogon::HttpResponse::newHttpViewResponse("tampil.csp", data);
        callback(res);
    });

    drogon::HttpAppFramework::instance().enableSession(bangkong::TIME_OF_SESSION);

    int port = bangkong::env<int>::value("PORT");
    if (port == 0) {
        port = 17000;
    }

    drogon::app().setLogPath("./")
            .setLogLevel(trantor::Logger::kWarn)
            .addListener("0.0.0.0", port)
            .setThreadNum(16)
            .run();
}
