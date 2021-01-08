#include "menuprocess.hpp"


void MenuProcess::get_all_menu(MenuResult &&callback) {
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select * from donasi.headers order by position asc");
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() == 0) {
            callback(std::nullopt);
        }
        else {
            std::vector<Header> headers;
            headers.reserve(f_get.size());
            for(auto row: f_get) {
                std::vector<Menu> d = get_menu_byheaderid(row["id_header"].as<short>());
                headers.emplace_back(row["id_header"].as<short>(),
                        row["nama_header"].as<std::string_view>(),
                        row["position"].as<short>(),
                        d);
            }
            callback(headers);
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << "MenuProcess: Line 25 => " << e.base().what() << std::endl;
        callback(std::nullopt);
    }
}

std::vector<Menu> MenuProcess::get_menu_byheaderid(const short header_id) {
    std::vector<Menu> result;
    std::future<drogon::orm::Result> res = m_db->execSqlAsyncFuture("select * from donasi.menus where header_id = $1", static_cast<int>(header_id));
    try {
        res.wait();
        drogon::orm::Result f_get = res.get();
        if (f_get.size() == 0) {
            return result;
        }
        else {
            result.reserve(f_get.size());
            for(auto row: f_get) {
                result.emplace_back(row["id_menu"].as<int>(),
                        row["header_id"].as<int>(),
                        row["nama_menu"].as<std::string_view>(),
                        row["link"].as<std::string_view>(),
                        row["position"].as<short>());
            }
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        std::cout << "MenuProcess: Line 50 => " << e.base().what() << std::endl;
    }
    return result;
}
