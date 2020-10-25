#include "akuncontroller.hpp"


void AkunController::login(const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback) const {
    auto params = req->getParameters();
    std::string_view email = "";
    std::string_view password = "";
    std::string message = "";
    drogon::HttpViewData data;
    data.insert("base_url", base_url());
    if (params.find("email") != params.end()) {
        email = params.at("email");
        if (email == "") {
            message = "Email tidak boleh kosong";
            data.insert("email_error", message);
        }
    }
    else {
        message = "Parameter tidak valid";
        data.insert("email_error", message);
    }
    if (params.find("password") != params.end()) {
        password = params.at("password");
        if (password == "") {
            message = "Password tidak boleh kosong";
            data.insert("password_error", message);
        }
    }
    else {
        message = "Parameter tidak valid";
        data.insert("password_error", message);
    }

    if (message != "") {
        auto res = drogon::HttpResponse::newHttpViewResponse("tampil.csp", data);
        callback(res);
        return;
    }
    auto res = drogon::HttpResponse::newRedirectionResponse(base_url().append("akuncontroller/dashboard"), drogon::HttpStatusCode::k200OK);
    callback(res);
}

void AkunController::dashboard(const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback) const {
    auto res = drogon::HttpResponse::newHttpViewResponse("dashboard.csp");
    callback(res);
}

