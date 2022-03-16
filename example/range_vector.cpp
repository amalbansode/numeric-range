#include "../src/numeric_range.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

// Helper function to print a numeric range using math-esque notation of
// bound inclusivity/exclusivity.
template <typename T>
void print_numeric_range (std::ostream& os, const NumericRange<T>& nr)
{
  if (nr.lb_inclusive)
    os << "[";
  else
    os << "(";

  os << nr.lb << ", " << nr.ub;

  if (nr.ub_inclusive)
    os << "]";
  else
    os << ")";
}

int main ()
{
  // Remember to use the custom comparator
  std::vector<NumericRange<double> > range_vector;

  // Populate the vector with some sample ranges and a mix of bound types
  range_vector.emplace_back(NumericRange<double>{0.0, true, 1.0, false});
  range_vector.emplace_back(NumericRange<double>{3.0, false, 3.1, false});
  range_vector.emplace_back(NumericRange<double>{-5.0, false, -1.0, true});
  range_vector.emplace_back(NumericRange<double>{1.1, true, 2.1, true});

  // Iterate through the map and print elements (unsorted)
  std::cout << "Unsorted elements:" << std::endl;
  for (auto & i : range_vector)
  {
    print_numeric_range(std::cout, i);
    std::cout << std::endl;
  }

  // Sort the vector using the comparator function in the library
  std::sort(range_vector.begin(), range_vector.end(), NumericRangeComparator<double>());

  // Iterate through the map and print elements (sorted)
  std::cout << "Sorted elements:" << std::endl;
  for (auto & i : range_vector)
  {
    print_numeric_range(std::cout, i);
    std::cout << std::endl;
  }

  return 0;
}
