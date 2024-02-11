cmake_minimum_required(VERSION 3.22)
project(screen-tracker)

include(CTest)

set(CMAKE_AUTOMOC ON)
set(QT_VERSION 5)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS
    Core
    Widgets
    Sql
)

add_subdirectory(src)
add_subdirectory(tests)