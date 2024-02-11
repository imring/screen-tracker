#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "qt/database_qt.hpp"

TEST_CASE("database") {
    static DatabaseQt database;

    SECTION("initialize database") {
        REQUIRE(database.Open());
    }

    SECTION("add screen") {
        const QImage image{"assets/image1.png"};
        REQUIRE(database.AddImage(image, 1.0));

        const std::vector<Image> images = database.GetAllImages();
        REQUIRE(images.size() == 1);

        const Image &image_db = images.back();
        REQUIRE(image_db.IsValid());
        REQUIRE(image_db.image == image);
        REQUIRE(image_db.similarity == Catch::Approx(1.0));
    }

    SECTION("add different screen") {
        const QImage image{"assets/image2.png"};
        REQUIRE(database.AddImage(image, 0.5));

        const std::vector<Image> images = database.GetAllImages();
        REQUIRE(images.size() == 2);

        const Image &image_db = images.back();
        REQUIRE(image_db.IsValid());
        REQUIRE(image_db.image == image);
        REQUIRE(image_db.similarity == Catch::Approx(0.5));
    }
}