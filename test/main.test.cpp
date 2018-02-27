#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

TEST_CASE("test", "[test]") {
  REQUIRE( 1 == 1);
  REQUIRE( 1 == 0);
}