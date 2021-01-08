#ifndef CORSFILTER_HPP
#define CORSFILTER_HPP

#include <drogon/HttpFilter.h>

class CorsFilter: public drogon::HttpFilter<CorsFilter, false>
{
public:
    CorsFilter();
    virtual void doFilter(const drogon::HttpRequestPtr& req,
                          drogon::FilterCallback &&fb,
                          drogon::FilterChainCallback &&fcb);
};

#endif // CORSFILTER_HPP
