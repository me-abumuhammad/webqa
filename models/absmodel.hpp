#ifndef ABSMODEL_HPP
#define ABSMODEL_HPP

#include <json/json.h>

class AbsModel
{
public:
    AbsModel() {}
    virtual ~AbsModel() {}

    virtual Json::Value get_data_json() const = 0;
};

#endif // ABSMODEL_HPP
