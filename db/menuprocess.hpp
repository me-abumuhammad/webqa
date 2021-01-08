#ifndef MENUPROCESS_HPP
#define MENUPROCESS_HPP

#include "baseprocess.hpp"

#include "../models/header.hpp"
#include "../models/menu.hpp"

class MenuProcess: public BaseProcess
{
public:
    using MenuResult = std::function<void(std::optional<std::vector<Header>>)>;
    MenuProcess(const std::shared_ptr<drogon::orm::DbClient>& db): BaseProcess(db), m_db{db} {}
    MenuProcess(const MenuProcess&) = delete ;
    MenuProcess(MenuProcess&&) = delete ;
    MenuProcess() = delete ;
    ~MenuProcess() {}

    void get_all_menu(MenuResult&&);
    std::vector<Menu> get_menu_byheaderid(const short);
private:
    const std::shared_ptr<drogon::orm::DbClient>& m_db;
};

#endif // MENUPROCESS_HPP
