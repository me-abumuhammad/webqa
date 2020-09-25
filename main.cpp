#include <iostream>

#include <drogon/drogon.h>

#include <boost/lexical_cast.hpp>
#include <fstream>

#include <fmt/format.h>

#include "utils/utils.hpp"
#include "controller/donasicontroller.hpp"

#define USE_POSTGRESQL

using namespace std;

int main()
{
    auto db = drogon::orm::DbClient::newPgClient(bangkong::env<std::string>::value(bangkong::DB_CONNECTION), 1);
    auto donasi_ctrl = std::make_shared<DonasiController>(db);
    drogon::app().registerController(donasi_ctrl);

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
        auto res = drogon::HttpResponse::newHttpViewResponse("tampil.csp", data);
        callback(res);
    });

    char* port = getenv("PORT");
    int v_port = 17000;
    if (port != NULL) {
        v_port = boost::lexical_cast<int>(port);
    }

    drogon::app().setLogPath("./")
            .setLogLevel(trantor::Logger::kWarn)
            .addListener("0.0.0.0", v_port)
            .setThreadNum(16)
            .run();
}
