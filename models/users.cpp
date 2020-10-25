#include "users.hpp"

Users::Users()
{

}

Users::~Users() {

}

Users::Users(std::string_view username,
             std::string_view password,
             std::string_view name,
             bool status): Akun(username, password, name), m_status {status} {

}

Users::Users(const Users& other): Akun(other) {
    m_status = other.m_status;
}

Users& Users::operator=(const Users& other) {
    Akun::operator=(other);
    m_status = other.m_status;
    return *this;
}

Users::Users(Users&& other): Akun(other) {
    m_status = other.m_status;
}

Users& Users::operator=(Users&& other) {
    Akun::operator=(other);
    m_status = other.m_status;
    return *this;
}
