#ifndef BASECONTROLLER_HPP
#define BASECONTROLLER_HPP

#include <string>

class BaseController
{
public:
    BaseController();
protected:
    std::string base_url() const;
};

#endif // BASECONTROLLER_HPP
