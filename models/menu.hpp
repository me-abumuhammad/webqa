#ifndef MENU_HPP
#define MENU_HPP

#include "absmodel.hpp"
#include <string_view>
#include <string>

class Menu: public AbsModel
{
public:
    Menu();
    explicit Menu(int id_menu, int header_id, std::string_view name, std::string_view link, short position):
        m_id_menu{id_menu}, m_header_id{header_id}, m_name{name.data()}, m_link{link.data()}, m_position{position} {}

    Menu& operator=(const Menu& other) {
        m_id_menu = other.m_id_menu;
        m_header_id = other.m_header_id;
        m_name = other.m_name;
        m_link = other.m_link;
        m_position = other.m_position;
        return *this;
    }

    Menu(const Menu& other) {
        m_id_menu = other.m_id_menu;
        m_header_id = other.m_header_id;
        m_name = other.m_name;
        m_link = other.m_link;
        m_position = other.m_position;
    }

    Menu& operator=(Menu&& other) {
        m_id_menu = std::move(other.m_id_menu);
        m_header_id = std::move(other.m_header_id);
        m_name = std::move(other.m_name);
        m_link = std::move(other.m_link);
        m_position = std::move(other.m_position);
        return *this;
    }

    Menu(Menu&& other) {
        m_id_menu = std::move(other.m_id_menu);
        m_header_id = std::move(other.m_header_id);
        m_name = std::move(other.m_name);
        m_link = std::move(other.m_link);
        m_position = std::move(other.m_position);
    }

    Json::Value get_data_json() const override {
        Json::Value json;
        json["id_menu"] = Json::Int(m_id_menu);
        json["header_id"] = Json::Int(m_header_id);
        json["name"] = m_name;
        json["link"] = m_link;
        json["position"] = Json::Int(m_position);
        return json;
    }
private:
    int m_id_menu;
    int m_header_id;
    std::string m_name;
    std::string m_link;
    short m_position;
};

#endif // MENU_HPP
