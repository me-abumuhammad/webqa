#ifndef SESSIONFILTER_HPP
#define SESSIONFILTER_HPP

#include <drogon/HttpFilter.h>

class SessionFilter: public drogon::HttpFilter<SessionFilter, false>
{
public:
    SessionFilter() {}
    virtual void doFilter(const drogon::HttpRequestPtr& req,
                          drogon::FilterCallback&& cb,
                          drogon::FilterChainCallback&& ccb) override;
};

#endif // SESSIONFILTER_HPP
