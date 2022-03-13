#ifndef JSON_EQUATION_JSON_EQUATION_H
#define JSON_EQUATION_JSON_EQUATION_H

#include <exception>

class NumericRange
{
public:
  double lb = 0;
  bool lb_inclusive = true;
  double ub = 0;
  bool ub_inclusive = true;

  NumericRange(const double _lb, const bool _lb_inclusive,
               const double _ub, const bool _ub_inclusive) :
               lb(_lb), lb_inclusive(_lb_inclusive),
               ub(_ub), ub_inclusive(_ub_inclusive)
  {}
};

// Is LHS < RHS?
class NumericRangeComparator
{
public:
  bool operator()(const NumericRange& lhs, const NumericRange& rhs)
  {
    const bool lhs_is_scalar = (lhs.lb == lhs.ub);
    const bool rhs_is_scalar = (rhs.lb == rhs.ub);

    if (lhs_is_scalar && rhs_is_scalar)
    {
      return (lhs.lb < rhs.lb);
    }
    else if (lhs_is_scalar)
    {
      return ((lhs.lb < rhs.lb && rhs.lb_inclusive) || (lhs.lb == rhs.lb && !rhs.lb_inclusive));
    }
    else if (rhs_is_scalar)
    {
      return ((lhs.ub < rhs.lb && lhs.ub_inclusive) || (lhs.ub == rhs.lb && !lhs.ub_inclusive));
    }
    else
    {
      if (lhs.lb == rhs.lb && lhs.lb_inclusive == rhs.lb_inclusive
       && lhs.ub == rhs.ub && lhs.ub_inclusive == rhs.ub_inclusive)
      {
        return false;
      }
      else if (lhs.ub < rhs.lb)
      {
        return true;
      }
      else if (lhs.ub == rhs.lb && !(lhs.ub_inclusive && rhs.lb_inclusive))
      {
        return true;
      }
      else if (lhs.lb > rhs.ub)
      {
        return false;
      }
      else if (lhs.lb == rhs.ub && !(lhs.lb_inclusive && rhs.ub_inclusive))
      {
        return false;
      }
      else
      {
        throw std::runtime_error("Invalid comparison between overlapping ranges");
      }
    }
  } // bool operator()
}; // class NumericRangeComparator

#endif //JSON_EQUATION_JSON_EQUATION_H
