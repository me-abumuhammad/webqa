#include "corsfilter.hpp"

CorsFilter::CorsFilter()
{

}

void CorsFilter::doFilter(const drogon::HttpRequestPtr &req,
                          drogon::FilterCallback &&fcb,
                          drogon::FilterChainCallback &&fccb) {
    if (req->method() == drogon::HttpMethod::Options) {
        drogon::HttpResponsePtr resp = drogon::HttpResponse::newHttpResponse();
        auto &origin = req->getHeader("Origin");

        resp->addHeader("Access-Control-Allow-Origin", origin);
        resp->addHeader("Access-Control-Allow-Methods", "OPTIONS,POST");
        resp->addHeader("Access-Control-Allow-Headers",
                                       "x-requested-with,content-type, authorization");
        resp->addHeader("Access-Control-Allow-Credentials","true");
        fcb(resp);
        return;
    }
    fccb();
}

