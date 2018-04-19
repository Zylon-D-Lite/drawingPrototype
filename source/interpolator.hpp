#ifndef ZDL_INTERPOLATOR_H_
#define ZDL_INTERPOLATOR_H_

/** Class Interpolator -- Approximates a given set of
 *  Coordinates into closed form polynomial function.
 */
#include <vector>
#include "./line.hpp"
#include "./polynomial.hpp"

class Interpolator {
public:
    Interpolator() = default;
    Polynomial
         newton_divided_difference(Line a_line);
private:
    Polynomial newton_calculate_single_term(std::vector<int>);
    double calculate_dd(const Line& a_line, size_t x0_index, size_t xn_index);
    std::vector<double> divided_differences;
};

#endif