#include "mystack.hpp"

void MyStack::push(const std::string& elem) {
    elems.push_back(elem);
}

void MyStack::pop() {
    assert(!elems.empty());
    elems.pop_back();
}

const std::string& MyStack::top() const {
    assert(!elems.empty());
    return elems.back();
}
