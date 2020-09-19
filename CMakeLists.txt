cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME webqa)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB SCP_LIST ${CMAKE_CURRENT_SOURCE_DIR}/views/*.csp)
foreach(cspFile ${SCP_LIST})
    message(STATUS "cspFile:" ${cspFile})
    EXEC_PROGRAM(basename ARGS "-s .csp ${cspFile}" OUTPUT_VARIABLE classname)
    message(STATUS "view classname:" ${classname})
    add_custom_command(OUTPUT ${classname}.h ${classname}.cc
        COMMAND drogon_ctl
        ARGS create view ${cspFile}
        DEPENDS ${cspFile}
        VERBATIM )
   set(VIEWSRC ${VIEWSRC} ${classname}.cc)
endforeach()

find_package(Boost COMPONENTS system date_time filesystem REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(fmt REQUIRED)

add_subdirectory(3rdparty/drogon)
include_directories("${CMAKE_CURRENT_LIST_DIR}/3rdparty/jwt-cpp/")

add_executable(webqa
    main.cpp
    config.json
    models/absmodel.cpp
    models/absmodel.hpp
    models/akun.cpp
    models/akun.hpp
    models/rekap.cpp
    models/rekap.hpp
    db/dbprocess.cpp
    db/dbprocess.hpp
    controller/donasicontroller.cpp
    controller/donasicontroller.hpp
    utils/jsonconverter.cpp
    utils/jsonconverter.hpp
    utils/utils.hpp
    models/response.cpp
    models/response.hpp
    utils/sha256.cpp
    utils/sha256.hpp
    ${VIEWSRC})
add_dependencies(webqa drogon_ctl)

target_link_libraries(${PROJECT_NAME}
        ${CMAKE_CURR}
        drogon
        fmt::fmt
        ${OPENSSL_LIBRARIES}
        ${Boost_LIBRARIES}
        )
