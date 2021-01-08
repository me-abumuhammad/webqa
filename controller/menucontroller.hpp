#ifndef MENUCONTROLLER_HPP
#define MENUCONTROLLER_HPP

#include "../db/menuprocess.hpp"
#include "basecontroller.hpp"

class MenuController: public drogon::HttpController<MenuController, false>, public BaseController
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(MenuController::all_menu, "/all_menu", drogon::Get);
    METHOD_LIST_END
    MenuController();
    explicit MenuController(const std::shared_ptr<drogon::orm::DbClient>& db) {
        m_db = std::make_shared<MenuProcess>(db);
    }
    void all_menu(bangkong::ReqParam::type req,
                  bangkong::RespParam::type callback) const;
private:
    std::shared_ptr<MenuProcess> m_db;
};

#endif // MENUCONTROLLLER_HPP
