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
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_subdirectory(3rdparty/drogon)
include_directories("${CMAKE_CURRENT_LIST_DIR}/3rdparty/jwt-cpp/")
set(DATE_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/3rdparty/date/include)

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
    utils/utils.cpp
    utils/utils.hpp
    models/response.cpp
    models/response.hpp
    utils/sha256.cpp
    utils/sha256.hpp
    utils/verifheader.cpp
    utils/verifheader.hpp
    controller/akuncontroller.cpp
    controller/akuncontroller.hpp
    utils/sessionfilter.cpp
    utils/sessionfilter.hpp
    controller/basecontroller.cpp
    controller/basecontroller.hpp
    models/users.cpp
    models/users.hpp
    db/akunprocess.cpp
    db/akunprocess.hpp
    models/jenistransaksi.cpp
    models/jenistransaksi.hpp
    models/buku.cpp
    models/buku.hpp
    controller/bukucontroller.cpp
    controller/bukucontroller.hpp
    db/bukuprocess.cpp
    db/bukuprocess.hpp
    db/baseprocess.cpp
    db/baseprocess.hpp
    db/interfaceprocess.hpp
    controller/jenistransaksicontroller.cpp
    controller/jenistransaksicontroller.hpp
    db/jenistransaksiprocess.cpp
    db/jenistransaksiprocess.hpp
    models/bukujenistransaksi.cpp
    models/bukujenistransaksi.hpp
    models/header.cpp
    models/header.hpp
    models/menu.cpp
    models/menu.hpp
    db/menuprocess.cpp
    db/menuprocess.hpp
    controller/menucontroller.cpp
    controller/menucontroller.hpp
    models/pelaksana.cpp
    models/pelaksana.hpp
    models/muhsinin.cpp
    models/muhsinin.hpp
    models/sumberdana.cpp
    models/sumberdana.hpp
    db/donasiprocess.cpp
    db/donasiprocess.hpp
    utils/money.hpp
    utils/mystack.hpp
    utils/mystack.cpp
    utils/corsfilter.cpp
    utils/corsfilter.hpp
    controller/transaksicontroller.cpp
    controller/transaksicontroller.hpp
    models/hitbase.cpp
    models/hitbase.hpp
    controller/muhsinincontroller.cpp
    controller/muhsinincontroller.h
    ${VIEWSRC})
add_dependencies(webqa drogon_ctl)

target_include_directories(webqa
        PUBLIC
        ${DATE_INCLUDE_DIRS}
        )

target_link_libraries(${PROJECT_NAME}
        ${CMAKE_CURR}
        drogon
        fmt::fmt
        ${OPENSSL_LIBRARIES}
        ${Boost_LIBRARIES}
        )
