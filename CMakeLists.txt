cmake_minimum_required(VERSION 3.22)
project(screen-tracker)

set(CMAKE_AUTOMOC ON)
set(QT_VERSION 5)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS
    Core
    Widgets
)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)