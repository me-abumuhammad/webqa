#ifndef MYSTACK_HPP
#define MYSTACK_HPP

#include <deque>
#include <string>
#include <cassert>

class MyStack
{
private:
    std::deque<std::string> elems;
public:
    void push(const std::string& elem);
    void pop();
    const std::string& top() const;
    bool empty() const {
        return elems.empty();
    }
};

#endif // MYSTACK_HPP
