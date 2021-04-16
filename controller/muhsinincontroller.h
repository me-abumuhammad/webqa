//
// Created by Abu Muhammad on 4/8/21.
//

#ifndef WEBQA_MUHSININCONTROLLER_H
#define WEBQA_MUHSININCONTROLLER_H

#include "../db/dbprocess.hpp"

#include "basecontroller.hpp"

class MuhsininController: public drogon::HttpController<MuhsininController, false>, public BaseController {
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(MuhsininController::add_muhsinin, "/add_muhsinin", drogon::Post);
    METHOD_LIST_END

    explicit MuhsininController() {}
    explicit MuhsininController(const std::shared_ptr<drogon::orm::DbClient>& db) {
        m_db = std::make_shared<DbProcess>(db);
    }
    void add_muhsinin(bangkong::ReqParam::type req,
                      bangkong::RespParam::type callback) const;

private:
    std::shared_ptr<DbProcess> m_db;
};


#endif //WEBQA_MUHSININCONTROLLER_H
