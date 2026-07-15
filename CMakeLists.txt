cmake_minimum_required(VERSION 3.16)
project(GitHubClient VERSION 0.0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Network Sql WebEngineWidgets)
find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)

set(SOURCES
    main.cpp
    i18n/translations.cpp
    api/github_api.cpp
    utils/browser_cookies.cpp
    gui/github_ui.cpp
)

set(HEADERS
    i18n/translations.h
    api/github_api.h
    utils/browser_cookies.h
    gui/github_ui.h
)

set(RESOURCES
    resources/resources.qrc
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    Qt5::Network
    Qt5::Sql
    Qt5::WebEngineWidgets
    OpenSSL::SSL
    OpenSSL::Crypto
    SQLite::SQLite3
)

add_executable(test_cookie_extractor
    utils/test_cookie_extractor.cpp
    utils/browser_cookies.cpp
)
target_include_directories(test_cookie_extractor PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_cookie_extractor
    Qt5::Core
    Qt5::Sql
    OpenSSL::SSL
    OpenSSL::Crypto
    SQLite::SQLite3
)
