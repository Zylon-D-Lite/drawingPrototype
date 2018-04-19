#include "./interpolator.hpp"
#include <algorithm>

Polynomial Interpolator::newton_divided_difference(Line a_line) {
    std::sort(a_line.begin(), a_line.end(),
         [](const Coordinate& a, const Coordinate& b)->bool {return a.y < b.y; });

// prepare the divided differences
    for (int i = 0; i < a_line.size(); ++i) {
        double current_d_d = calculate_dd(a_line, 0, i);
        divided_differences.push_back(current_d_d);
    }

    Polynomial ret;
    std::vector<int> x_points;
    for (int i = 0; i < a_line.size(); ++i) {
        x_points.push_back(a_line[i].x);
    }
    for (int i = 0; i < divided_differences.size(); ++i) {
        auto temp = newton_calculate_single_term
             (std::vector<int>(x_points.begin(), x_points.begin() + i));
        //std::cout << "temp = " << temp.to_string() << "\t";
        //std::cout << "divided_differences[i] = " << divided_differences[i] << std::endl;
        //std::cout << "temp * divided_differences = " << (temp * divided_differences[i]).to_string() << std::endl;
        ret.append(temp * divided_differences[i]);
    }
    ret.prettify();
    return ret;
}
double Interpolator::calculate_dd(const Line& a_line,
     size_t x0_index, size_t xn_index)
{
    if (xn_index == x0_index)
        return (a_line[xn_index].y);
    if (xn_index - x0_index == 1)
        return ((a_line[xn_index].y - a_line[x0_index].y)
             / double(a_line[xn_index].x - a_line[x0_index].x));
    else {
        return (calculate_dd(a_line, x0_index + 1, xn_index)
             - calculate_dd(a_line, x0_index, xn_index - 1)) /
             (a_line[xn_index].x - a_line[x0_index].x);
    }
}

Polynomial Interpolator::newton_calculate_single_term(std::vector<int> points)
{
    Polynomial ret{ { {1, 0} } };//1
    for (size_t i = 0; i < points.size(); i++)
    {
        Polynomial curr_factor{ {{ 1, 1}, {-points[i], 0}}}; //(x - points[i]) in polynomial form
        ret = (ret * curr_factor);
    }
    return ret;
}