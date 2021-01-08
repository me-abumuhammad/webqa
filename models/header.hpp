#ifndef HEADER_HPP
#define HEADER_HPP

#include "absmodel.hpp"
#include "menu.hpp"
#include <string_view>
#include <string>
#include <vector>
#include <iterator>

class Header: public AbsModel
{
public:
    Header();
    Header(std::string_view name, short position): m_name{name.data()}, m_position{position} {}
    Header(int id_header, std::string_view name, short position, std::vector<Menu> menu): m_id_header{id_header},
        m_name{name.data()}, m_position{position} {
        m_menus.reserve(menu.size());
        m_menus.insert(m_menus.begin(),
                       std::make_move_iterator(menu.begin()),
                       std::make_move_iterator(menu.end()));
    }
    ~Header() {}
    void set_id_header(int id) {
        m_id_header = id;
    }
    void set_name(std::string_view name) {
        m_name = name.data();
    }
    void set_position(short position) {
        m_position = position;
    }
    int get_id_header() const {
        return  m_id_header;
    }
    std::string get_name() const {
        return m_name;
    }
    short get_position() const {
        return m_position;
    }

    void set_menus(std::vector<Menu>&& menus) {
        m_menus.erase(m_menus.begin(), m_menus.end());
        m_menus.shrink_to_fit();
        m_menus.reserve(menus.size());

        m_menus.insert(m_menus.begin(),
                       std::make_move_iterator(menus.begin()),
                       std::make_move_iterator(menus.end()));
    }

    Json::Value get_data_json() const override {
        Json::Value json;
        json["id_header"] = Json::Int(m_id_header);
        json["name"] = m_name;
        json["position"] = Json::UInt(m_position);
        Json::Value arr(Json::arrayValue);
        int i = 0;
        for (const Menu& m: m_menus) {
            arr[i] = m.get_data_json();
            ++i;
        }
        json["menu"] = arr;
        return json;
    }
private:
    int m_id_header;
    std::string m_name;
    short m_position;
    std::vector<Menu> m_menus;
};

#endif // HEADER_HPP
