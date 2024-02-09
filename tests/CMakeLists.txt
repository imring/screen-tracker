include(FetchContent)

# installing catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2 # or a later release
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

new_test(compare-tests
    compare.cpp
    ../src/screen.cpp
)
target_link_libraries(compare-tests PRIVATE Qt${QT_VERSION}::Gui)