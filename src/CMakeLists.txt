include(FetchContent)

add_executable(screen-tracker
    gui/window.cpp

    main.cpp
    screen.cpp
)

target_compile_features(screen-tracker PRIVATE cxx_std_20)
target_link_libraries(screen-tracker PRIVATE
    Qt${QT_VERSION}::Core
    Qt${QT_VERSION}::Widgets
)