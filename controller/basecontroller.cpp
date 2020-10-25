#include "basecontroller.hpp"

#include "../utils/utils.hpp"

BaseController::BaseController()
{

}

std::string BaseController::base_url() const {
    return bangkong::BASE_URL;
}
