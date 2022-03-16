#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main()

#include "catch.hpp"
#include "../src/numeric_range.hpp"

using namespace std;
using namespace numeric_range;

TEST_CASE("NumericRange Constructor", "[numeric_range]" ) {
  // Basic Constructor should pass
  REQUIRE_NOTHROW(NumericRange<int>(0, true, 1, false));

  // LB must be <= UB
  REQUIRE_THROWS_AS(NumericRange<int>(1, true, 0, false), std::runtime_error);

  // If LB == UB, bounds must be inclusive
  REQUIRE_THROWS_AS(NumericRange<int>(0, true, 0, false), std::runtime_error);

  // Scalar Constructor should pass
  REQUIRE_NOTHROW(NumericRange<int>(0));
}

/// Range-Range Comparison Tests

TEST_CASE("Simple Range comparisons", "[numeric_range]" ) {
  NumericRangeComparator<double> comp;

  const NumericRange A = NumericRange<double>(0, true, 1, true);

  {
    NumericRange B = NumericRange<double>(0, true, 1, true);
    REQUIRE(comp(A, B) == false);
    REQUIRE(comp(B, A) == false);
  }

  {
    NumericRange B = NumericRange<double>(-0.5, true, -0.25, true);
    REQUIRE(comp(A, B) == false);
    REQUIRE(comp(B, A) == true);
  }

  {
    NumericRange B = NumericRange<double>(-0.5, true, 0, true);
    REQUIRE_THROWS_AS(comp(A, B), std::runtime_error);
    REQUIRE_THROWS_AS(comp(B, A), std::runtime_error);
  }

  {
    NumericRange B = NumericRange<double>(0, true, 0.5, true);
    REQUIRE_THROWS_AS(comp(A, B), std::runtime_error);
    REQUIRE_THROWS_AS(comp(B, A), std::runtime_error);
  }

  {
    NumericRange B = NumericRange<double>(0.25, true, 0.75, true);
    REQUIRE_THROWS_AS(comp(A, B), std::runtime_error);
    REQUIRE_THROWS_AS(comp(B, A), std::runtime_error);
  }

  {
    NumericRange B = NumericRange<double>(0.5, true, 1, true);
    REQUIRE_THROWS_AS(comp(A, B), std::runtime_error);
    REQUIRE_THROWS_AS(comp(B, A), std::runtime_error);
  }

  {
    NumericRange B = NumericRange<double>(1, true, 1.25, true);
    REQUIRE_THROWS_AS(comp(A, B), std::runtime_error);
    REQUIRE_THROWS_AS(comp(B, A), std::runtime_error);
  }

  {
    NumericRange B = NumericRange<double>(1.25, true, 1.5, true);
    REQUIRE(comp(A, B) == true);
    REQUIRE(comp(B, A) == false);
  }

}

TEST_CASE("Range A UB equals Range B LB", "[numeric_range]" ) {
  NumericRangeComparator<int> comp;

  NumericRange A = NumericRange<int>(0, true, 1, true);
  NumericRange B = NumericRange<int>(1, true, 2, true);

  REQUIRE_THROWS_AS(comp(A, B), std::runtime_error);
  REQUIRE_THROWS_AS(comp(B, A), std::runtime_error);

  A.ub_inclusive = false;
  REQUIRE(comp(A, B) == true);
  REQUIRE(comp(B, A) == false);

  B.lb_inclusive = false;
  REQUIRE(comp(A, B) == true);
  REQUIRE(comp(B, A) == false);

  A.ub_inclusive = true;
  REQUIRE(comp(A, B) == true);
  REQUIRE(comp(B, A) == false);
}

TEST_CASE("Range A LB equals Range B UB", "[numeric_range]" ) {
  NumericRangeComparator<int> comp;

  NumericRange A = NumericRange<int>(0, true, 1, true);
  NumericRange B = NumericRange<int>(-1, true, 0, true);

  REQUIRE_THROWS_AS(comp(A, B), std::runtime_error);
  REQUIRE_THROWS_AS(comp(B, A), std::runtime_error);

  A.lb_inclusive = false;
  REQUIRE(comp(A, B) == false);
  REQUIRE(comp(B, A) == true);

  B.ub_inclusive = false;
  REQUIRE(comp(A, B) == false);
  REQUIRE(comp(B, A) == true);

  A.lb_inclusive = true;
  REQUIRE(comp(A, B) == false);
  REQUIRE(comp(B, A) == true);
}

/// Scalar-Range and Scalar-Scalar Comparison Tests

TEST_CASE("Simple Scalar comparisons", "[numeric_range]" ) {
  NumericRangeComparator<double> comp;

  const NumericRange A = NumericRange<double>(0.5);

  {
    NumericRange B = NumericRange<double>(-0.5, true, 0, true);
    REQUIRE(comp(A, B) == false);
    REQUIRE(comp(B, A) == true);
  }

  {
    NumericRange B = NumericRange<double>(0);
    REQUIRE(comp(A, B) == false);
    REQUIRE(comp(B, A) == true);
  }

  {
    NumericRange B = NumericRange<double>(0, true, 0.5, true);
    REQUIRE(comp(A, B) == false);
    REQUIRE(comp(B, A) == false);
  }

  {
    NumericRange B = NumericRange<double>(0.5);
    REQUIRE(comp(A, B) == false);
    REQUIRE(comp(B, A) == false);
  }

  {
    NumericRange B = NumericRange<double>(0.5, true, 1, true);
    REQUIRE(comp(A, B) == false);
    REQUIRE(comp(B, A) == false);
  }

  {
    NumericRange B = NumericRange<double>(0, true, 1, true);
    REQUIRE(comp(A, B) == false);
    REQUIRE(comp(B, A) == false);
  }

  {
    NumericRange B = NumericRange<double>(1);
    REQUIRE(comp(A, B) == true);
    REQUIRE(comp(B, A) == false);
  }

  {
    NumericRange B = NumericRange<double>(1, true, 1.25, true);
    REQUIRE(comp(A, B) == true);
    REQUIRE(comp(B, A) == false);
  }
}

TEST_CASE("Scalar within Range", "[numeric_range]" ) {
  NumericRangeComparator<double> comp;

  const NumericRange scalar = NumericRange<double>(0.5);
  NumericRange range = NumericRange<double>(0, true, 1, true);

  REQUIRE(comp(scalar, range) == false);
  REQUIRE(comp(range, scalar) == false);

  range.lb_inclusive = false;
  REQUIRE(comp(scalar, range) == false);
  REQUIRE(comp(range, scalar) == false);

  range.ub_inclusive = false;
  REQUIRE(comp(scalar, range) == false);
  REQUIRE(comp(range, scalar) == false);

  range.ub_inclusive = true;
  REQUIRE(comp(scalar, range) == false);
  REQUIRE(comp(range, scalar) == false);
}

TEST_CASE("Scalar equals Range LB", "[numeric_range]" ) {
  NumericRangeComparator<int> comp;

  const NumericRange scalar = NumericRange<int>(0);
  NumericRange range = NumericRange<int>(0, true, 1, true);

  REQUIRE(comp(scalar, range) == false);
  REQUIRE(comp(range, scalar) == false);

  range.lb_inclusive = false;
  REQUIRE(comp(scalar, range) == true);
  REQUIRE(comp(range, scalar) == false);

  range.ub_inclusive = false;
  REQUIRE(comp(scalar, range) == true);
  REQUIRE(comp(range, scalar) == false);

  range.lb_inclusive = true;
  REQUIRE(comp(scalar, range) == false);
  REQUIRE(comp(range, scalar) == false);
}

TEST_CASE("Scalar equals Range UB", "[numeric_range]" ) {
  NumericRangeComparator<int> comp;

  const NumericRange scalar = NumericRange<int>(1);
  NumericRange range = NumericRange<int>(0, true, 1, true);

  REQUIRE(comp(scalar, range) == false);
  REQUIRE(comp(range, scalar) == false);

  range.lb_inclusive = false;
  REQUIRE(comp(scalar, range) == false);
  REQUIRE(comp(range, scalar) == false);

  range.ub_inclusive = false;
  REQUIRE(comp(scalar, range) == false);
  REQUIRE(comp(range, scalar) == true);

  range.lb_inclusive = true;
  REQUIRE(comp(scalar, range) == false);
  REQUIRE(comp(range, scalar) == true);
}
