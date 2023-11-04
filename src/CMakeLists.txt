add_executable(screen-tracker
    main.cpp
    tracker.cpp
)

target_compile_features(screen-tracker PRIVATE cxx_std_20)
target_link_libraries(screen-tracker PRIVATE
    Qt${QT_VERSION}::Widgets
    Qt${QT_VERSION}::Gui
)