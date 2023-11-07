add_executable(screen-tracker
    compare.cpp
    database.cpp
    main.cpp
    tracker.cpp
    window.cpp
)

target_compile_features(screen-tracker PRIVATE cxx_std_20)
target_link_libraries(screen-tracker PRIVATE
    Qt${QT_VERSION}::Widgets
    Qt${QT_VERSION}::Gui
    Qt${QT_VERSION}::Sql
    Qt${QT_VERSION}::Qml
    QtOct
)