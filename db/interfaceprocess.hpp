#ifndef INTERFACEPROCESS_HPP
#define INTERFACEPROCESS_HPP

#include <string_view>

class InterfaceProcess {
public:
    virtual ~InterfaceProcess() {}
    using BoolResult = std::function<void(bool)>;
    virtual void check_username(std::string_view, BoolResult) = 0;
    virtual bool check_token(std::string_view) = 0;
};

#endif // INTERFACEPROCESS_HPP
