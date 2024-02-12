#include <algorithm>

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
        REQUIRE(database.GetCount() == 1);

        const Image &image_db = database.At(1);
        REQUIRE(image_db.IsValid());
        REQUIRE(image_db.image == image);
        REQUIRE(image_db.similarity == Catch::Approx(1.0));
    }

    SECTION("add different screen") {
        const QImage image{"assets/image2.png"};
        REQUIRE(database.AddImage(image, 0.5));
        REQUIRE(database.GetCount() == 2);

        const Image &image_db = database.At(2);
        REQUIRE(image_db.IsValid());
        REQUIRE(image_db.image == image);
        REQUIRE(image_db.similarity == Catch::Approx(0.5));
    }

    SECTION("see all screens") {
        const std::vector<Image> images = database.GetAllImages();
        REQUIRE(images.size() == 2);

        const std::vector<Image> excepted{
            database.At(1),
            database.At(2),
        };
        REQUIRE(std::equal(excepted.begin(), excepted.end(), images.begin()));
    }
}