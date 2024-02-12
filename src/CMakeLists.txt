include(FetchContent)

add_library(screen-module qt/screen_qt.cpp)
target_compile_features(screen-module PUBLIC cxx_std_20)
target_link_libraries(screen-module PUBLIC Qt${QT_VERSION}::Gui)
target_link_directories(screen-module PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_library(database-module database.cpp qt/database_qt.cpp)
target_compile_features(database-module PUBLIC cxx_std_20)
target_link_libraries(database-module PUBLIC
    screen-module
    Qt${QT_VERSION}::Sql
    Qt${QT_VERSION}::Core
)
target_link_directories(database-module PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_library(tracker-module tracker.cpp)
target_compile_features(tracker-module PUBLIC cxx_std_20)
target_link_libraries(tracker-module PUBLIC screen-module)

add_executable(screen-tracker
    gui/flowlayout.cpp
    gui/imageview.cpp
    gui/window.cpp
    main.cpp
)

target_compile_features(screen-tracker PRIVATE cxx_std_20)
target_link_libraries(screen-tracker PRIVATE
    screen-module
    database-module
    tracker-module

    Qt${QT_VERSION}::Widgets
    Qt${QT_VERSION}::Concurrent
)
    
if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET screen-tracker PROPERTY WIN32_EXECUTABLE true)
endif()