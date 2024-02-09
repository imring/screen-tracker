#include <filesystem>
#include <source_location>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "screen.hpp"

TEST_CASE("image comparsion") {
    const std::filesystem::path source_file = std::source_location::current().file_name();
    const std::filesystem::path path        = source_file.parent_path();

    const QImage image1{(path / "assets" / "image1.png").string().c_str()};
    const QImage image2{(path / "assets" / "image2.png").string().c_str()};

    SECTION("same images") {
        REQUIRE(Screen::Compare(image1, image1) == Catch::Approx(1));
        REQUIRE(Screen::Compare(image2, image2) == Catch::Approx(1));
    }

    SECTION("different images") {
        REQUIRE(Screen::Compare(image1, image2) == Catch::Approx(0.5));
        REQUIRE(Screen::Compare(image2, image1) == Catch::Approx(0.5));
    }
}