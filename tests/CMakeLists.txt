include(FetchContent)

# installing catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

function(new_test name files)
    add_executable(${name} ${files} ${ARGN})
    target_compile_features(${name} PUBLIC cxx_std_20)
    target_link_libraries(${name} PUBLIC Catch2::Catch2WithMain)
    target_include_directories(${name} PUBLIC ../src)
    catch_discover_tests(${name})
endfunction()

# compare
new_test(compare-tests compare.cpp)
target_link_libraries(compare-tests PRIVATE screen-module)

# database-qt
new_test(database-qt-tests database_qt.cpp)
target_link_libraries(database-qt-tests PRIVATE database-module)

add_custom_command(TARGET compare-tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_LIST_DIR}/assets $<TARGET_FILE_DIR:compare-tests>/assets
)