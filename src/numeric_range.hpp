/*
 * numeric_range
 *
 * Copyright (c) 2022 Amal Bansode <https://www.amalbansode.com>.
 * Provided under the MIT License
 *
 * A header-only C++ library that enables the representation of a range of
 * values in a linear space (via the NumericRange class).
 * The linear space, of type T, must have a well-defined operator< to enable
 * sorting and comparison of ranges (via the NumericRangeComparator). Hence
 * while this library could be used for a variety of types, numeric types
 * are the only ones with correctness guarantees for the classes and methods
 * shipped in this version of the library.
 */

#ifndef NUMERIC_RANGE_HPP
#define NUMERIC_RANGE_HPP

#include <stdexcept>

namespace numeric_range {

/**
 * A Numeric Range represents a linear space with a minimum and maximum bound.
 * While the class is templated for any type T, as the name suggests, this
 * should be used for numeric types only. Other types may have undefined
 * behavior.
 * Use the NumericRangeComparator to compare NumericRange objects.
 * @tparam T Recommend a numeric type that has a well-defined operator<.
 */
template<typename T>
class NumericRange
{
public:
  T lb;
  bool lb_inclusive = true;
  T ub = 0;
  bool ub_inclusive = true;

  /**
   * Construct a Numeric Range from the lower bound, upper bound,
   * and their inclusive/exclusive attributes respectively.
   * Note that:
   * - LB <= UB
   * - if LB == UB, both bounds must be inclusive
   * If either condition is violated, a runtime_error is thrown.
   * @param _lb
   * @param _lb_inclusive
   * @param _ub
   * @param _ub_inclusive
   * @throws runtime_error If bounds are invalid
   */
  NumericRange (const T _lb, const bool _lb_inclusive,
                const T _ub, const bool _ub_inclusive) :
      lb(_lb), lb_inclusive(_lb_inclusive),
      ub(_ub), ub_inclusive(_ub_inclusive)
  {
    if (lb > ub)
    {
      throw std::runtime_error("LB cannot be greater than UB");
    }
    if (lb == ub && (!lb_inclusive || !ub_inclusive))
    {
      throw std::runtime_error("LB and UB must be inclusive when LB == UB");
    }
  }

  /**
   * Construct a "scalar", i.e., a single value with both bounds set as
   * inclusive. For instance, while [0, 1] is a range, 0.5 by itself is
   * a single number or a scalar. Defining this as the same type as a range
   * even though it may not represent one helps with comparing/ordering.
   * @param _scalar
   */
  explicit NumericRange (T _scalar) :
      lb(_scalar), lb_inclusive(true),
      ub(_scalar), ub_inclusive(true)
  {}
}; /* class NumericRange */

/**
 * This can be passed to STL containers or algorithms to aid in comparing
 * objects of type NumericRange.
 * @tparam T Recommend a numeric type that has a well-defined operator<.
 */
template<typename T>
class NumericRangeComparator
{
public:
  /**
   * Compare LHS and RHS of type NumericRange (their underlying template type
   * should also be the same) and return whether LHS < RHS.
   * Overlapping ranges cannot be ordered so comparing them will necessarily
   * result in a runtime_error being thrown.
   * If either is a scalar, a return value of false may indicate that the
   * the scalar is contained in the other range, although the caller will
   * need to check LHS > RHS to be sure of this (STL typically does this).
   * @param lhs
   * @param rhs
   * @return Whether LHS < RHS
   * @throws runtime_error When overlapping ranges are compared
   */
  bool
  operator() (const NumericRange<T> &lhs, const NumericRange<T> &rhs) const
  {
    const bool lhs_is_scalar = (lhs.lb == lhs.ub);
    const bool rhs_is_scalar = (rhs.lb == rhs.ub);

    if (lhs_is_scalar && rhs_is_scalar)
    {
      return (lhs.lb < rhs.lb);
    }
    else if (lhs_is_scalar)
    {
      return ((lhs.lb < rhs.lb) || (lhs.lb == rhs.lb && !rhs.lb_inclusive));
    }
    else if (rhs_is_scalar)
    {
      return ((lhs.ub < rhs.lb) || (lhs.ub == rhs.lb && !lhs.ub_inclusive));
    }
    else
    {
      if (lhs.lb == rhs.lb && lhs.lb_inclusive == rhs.lb_inclusive
          && lhs.ub == rhs.ub && lhs.ub_inclusive == rhs.ub_inclusive)
      {
        return false;
      }
      else if ((lhs.ub < rhs.lb) ||
               (lhs.ub == rhs.lb && !(lhs.ub_inclusive && rhs.lb_inclusive)))
      {
        return true;
      }
      else if ((lhs.lb > rhs.ub) ||
               (lhs.lb == rhs.ub && !(lhs.lb_inclusive && rhs.ub_inclusive)))
      {
        return false;
      }
      else
      {
        throw std::runtime_error(
            "Invalid comparison between overlapping ranges");
      }
    }
  } /* bool operator() */
}; /* class NumericRangeComparator */

} /* namespace numeric_range */

#endif //NUMERIC_RANGE_HPP
