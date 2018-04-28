#include "./triangle.hpp"

//higher precision specialization of perpendicular_bisector
template<>
Edge<double> Edge<double>::perpendicular_bisector() {
    double half_length = length() / 2;
    if (is_vertical()) {
        typename point_t::value_t half_y
        = ((_beg.y() + _end.y()) / 2);
        point_t b(_beg.x() - (half_length), half_y);
        point_t e(_beg.x() + (half_length), half_y);
        return Edge<double>(b, e);
    } else if(is_horizontal()) {
        typename point_t::value_t half_x
        = lround((_beg.x() + _end.x()) / 2);
        point_t b(half_x, _beg.y() - (half_length));
        point_t e(half_x, _beg.y() + (half_length));
        return Edge<double>(b, e);
    } else {
        typename point_t::value_t half_x = ((_beg.x() + _end.x()) / 2);
        typename point_t::value_t half_y = m() * half_x + c();
        double M = -1 / m();
        double C = half_y - (M * half_x);
        double D = length();
        double beg_x = half_x - (D/(2 * sin(atan(M))));
        double end_x = half_x + (D/(2 * sin(atan(M))));

        point_t b(beg_x, M * beg_x + C);
        point_t e(end_x, M * end_x + C);
        return Edge<double>(b, e);
    }
}