# Numeric Range
A header-only C++ library that enables the representation of a range of values in a linear space (via the `NumericRange` class).

The linear space, of `type T`, must have a well-defined `operator<` to enable sorting and comparison of ranges (via the `NumericRangeComparator`). Hence while this library could be used for a variety of types, numeric types are the only ones with correctness guarantees for the classes and methods shipped in this version of the library.

## Structure

A numeric range is represented by a lower bound and upper bound of value `type T`, as well as booleans defining whether these bounds are inclusive or exclusive, respectively.

```c++
template<typename T>
class NumericRange
{
public:
    T lb;
    bool lb_inclusive;
    T ub;
    bool ub_inclusive;
};

NumericRange<int> zero_to_one{0, true, 1, false};
```

### Scalars

A special exception is made for a "scalar", which represents a single value in the linear space of type `T`. For a scalar, the lower bound equals the upper bound and both bounds are inclusive.

```c++
NumericRange<int> scalar_one{1, true, 1, true};
```

## Ordering

Numeric ranges can also be ordered **as long as they do not overlap**. This is implemented by the comparator class `NumericRangeComparator`. If the ranges do overlap, comparing them throws an `std::runtime_error`.

```c++
NumericRangeComparator comp;
NumericRange<int> zero_to_one{0, true, 1, false};
NumericRange<int> one_to_two{1, true, 2, false};

assert(comp(zero_to_one, one_to_two));
```

A special exception is made for scalars so that `NumericRangeComparator` can be used to determine whether a _scalar is included in a range_.

```c++
NumericRangeComparator comp;
NumericRange<int> zero_to_two{0, true, 2, false};
NumericRange<int> scalar_one{1, true, 1, true};

assert(!comp(scalar_one, zero_to_two)); // comp returns false
assert(!comp(zero_to_two, scalar_one)); // comp returns false again
```

The result of this is that scalar `1` is neither less than nor greater than `[0, 2)`. This can be useful when dealing with a container of ranges that are being indexed with scalars. This is demonstrated in the example program [`range_map.cpp`](https://github.com/amalbansode/numeric-range/blob/master/example/range_map.cpp).

## Limitations

A numeric range or a comparison of ranges must not violate these constraints:
1. In a range, the lower bound must be `<=` the upper bound.
2. If a range's lower bound `==` upper bound, both bounds must be inclusive.
3. Overlapping ranges cannot be compared.
4. This library has only been tested with the default numeric types in C++. Using other custom types may result in unexpected or undefined behavior.

## License and Contributing

This library is provided under the MIT License. Contributions are welcome, please open a relevant issue or PR.
