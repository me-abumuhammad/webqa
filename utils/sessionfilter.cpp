#include "sessionfilter.hpp"

#include "utils.hpp"
#include "../models/response.hpp"

void SessionFilter::doFilter(const drogon::HttpRequestPtr &req, drogon::FilterCallback &&cb, drogon::FilterChainCallback &&ccb) {
    trantor::Date now = trantor::Date::date();
    if (!req->session()) {
        auto resp = drogon::HttpResponse::newNotFoundResponse();
        cb(resp);
        return;
    }
    if (req->session()->find(bangkong::TIME_SESSION)) {
        auto last_date = req->session()->get<trantor::Date>(bangkong::TIME_SESSION);
        req->session()->insert(bangkong::TIME_SESSION, now);
        if (now > last_date.after(bangkong::TIME_OF_SESSION)) {
            ccb();
            return;
        }
        else {
            Response res {};
            res.build_failed("Access interval should be at least two hours");
            auto resp = drogon::HttpResponse::newHttpJsonResponse(res.get_data_json());
            cb(resp);
            return;
        }
    }
    req->session()->insert(bangkong::TIME_SESSION, now);
    ccb();
}
