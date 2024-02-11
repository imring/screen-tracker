
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "qt/screen_qt.hpp"

TEST_CASE("image comparsion") {
    ScreenQt screen_module;

    const QImage image1{"./assets/image1.png"};
    const QImage image2{"./assets/image2.png"};

    SECTION("same images") {
        REQUIRE(screen_module.Compare(image1, image1) == Catch::Approx(1));
        REQUIRE(screen_module.Compare(image2, image2) == Catch::Approx(1));
    }

    SECTION("different images") {
        REQUIRE(screen_module.Compare(image1, image2) == Catch::Approx(0.5));
        REQUIRE(screen_module.Compare(image2, image1) == Catch::Approx(0.5));
    }
}