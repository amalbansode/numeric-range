#include "../src/numeric_range.hpp"

#include <iostream>
#include <map>

// A sample program that uses a NumericRange as the key for mapping to a value.
// Demonstrates insertion, error detection for overlapping ranges, indexing
// with a "scalar", and iterator usage.
int main ()
{
  // Remember to use the custom comparator
  std::map<NumericRange, double, NumericRangeComparator> range_based_map;

  // Populate the map with some sample ranges and a mix of bound types
  range_based_map.insert({{0, true, 1, false}, 0});
  range_based_map.insert({{1, false, 2, false}, 1});
  range_based_map.insert({{5, false, 6, true}, 5});

  // Inserting an overlapping range is an error because overlapping
  // ranges cannot be compared and thus sorted
  try
  {
    range_based_map.insert({{1, true, 2.5, false}, 2});
  }
  catch (std::runtime_error &e)
  {
    std::cout << "Caught insertion of overlapping range!" << std::endl;
  }

  // Index into the map with a "scalar" we expect to be contained in some range
  std::cout << range_based_map[NumericRange{0}] << std::endl;
  std::cout << range_based_map[NumericRange{1.5}] << std::endl;
  std::cout << range_based_map[NumericRange{6}] << std::endl;

  // Note that the range (1, 2) inserted above uses *exclusive* bounds,
  // so a value should not be found for 2!
  auto nonexistent = range_based_map.find(NumericRange{2});
  if (nonexistent != range_based_map.end())
    std::cout << nonexistent->second << std::endl;
  else
    std::cout << "Range for an undefined input was (successfully) not found" << std::endl;

  // Iterate through the map and print key-value pairs
  for (auto & i : range_based_map)
  {
    if (i.first.lb_inclusive)
      std::cout << "[";
    else
      std::cout << "(";

    std::cout << i.first.lb << ", " << i.first.ub;

    if (i.first.ub_inclusive)
      std::cout << "]";
    else
      std::cout << ")";

    std::cout << " is mapped to " << i.second << std::endl;
  }

  return 0;
}