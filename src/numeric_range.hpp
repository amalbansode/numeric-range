#ifndef NUMERIC_RANGE_HPP
#define NUMERIC_RANGE_HPP

#include <stdexcept>

namespace numeric_range {

template<typename T>
class NumericRange
{
public:
  T lb;
  bool lb_inclusive = true;
  T ub = 0;
  bool ub_inclusive = true;

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

  explicit NumericRange (T _scalar) :
      lb(_scalar), lb_inclusive(true),
      ub(_scalar), ub_inclusive(true)
  {}
};

// Is LHS < RHS?
template<typename T>
class NumericRangeComparator
{
public:
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
  } // bool operator()
}; // class NumericRangeComparator

} // namespace numeric_range

#endif //NUMERIC_RANGE_HPP
