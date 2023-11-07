cmake_minimum_required(VERSION 2.20)
project(screen-tracker)

set(CMAKE_AUTOMOC ON)
set(QT_VERSION 5)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS Widgets Gui Sql Qml)
find_package(OpenCV REQUIRED)

add_subdirectory(external)
add_subdirectory(src)