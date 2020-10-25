#ifndef USERS_HPP
#define USERS_HPP

#include "akun.hpp"

class Users: public Akun
{
public:
    Users();
    ~Users();
    explicit Users(std::string_view username,
          std::string_view password,
          std::string_view name,
          bool status);
    Users(const Users& other);
    Users& operator=(const Users& other);
    Users(Users&& other);
    Users& operator=(Users&& other);

    void set_status(bool status) {
        m_status = status;
    }

    bool get_status() const {
        return m_status;
    }
private:
    bool m_status;
};

#endif // USERS_HPP
