#ifndef ZDL_TRIANGLE_H_
#define ZDL_TRIANGLE_H_
#include <iostream>
#include <array>
#include <cmath>

//!-----------------------------------------------------
//                Coordinate Class
//------------------------------------------------------
template<typename T>
class Coordinate {
    template<typename U>
    friend bool operator==(const Coordinate<U>&, const Coordinate<U>&);
    template<typename U>
    friend bool operator!=(const Coordinate<U>&, const Coordinate<U>&);
public:
    typedef T value_t;
    Coordinate() = default;
    Coordinate(const value_t& x, const value_t& y)
     : _x(x), _y(y)
    { }
    value_t x() const { return _x; }
    value_t y() const { return _y; }
    void set_x(value_t x) { _x = x; }
    void set_y(value_t y) { _y = y; }
    Coordinate transpose() const  { return Coordinate(_y, _x);}
private:
    value_t _x, _y;
};

//!-----------------------------------------------------
//                      Edge Class
//------------------------------------------------------
template<typename T>
class Edge {
    template<typename U>
    friend bool operator==(const Edge<U>&, const Edge<U>&);
    template<typename U>
    friend bool operator!=(const Edge<U>&, const Edge<U>&);
public:
    bool is_vertical() const { return std::abs(_beg.x() - _end.x()) < 0.0000005; }
    bool is_horizontal() const { return std::abs(_beg.y() - _end.y()) < 0.0000005; }
    typedef Coordinate<T> point_t;
    typedef double slope_t;
    typedef double offset_t;
    Edge() = default;
    Edge(const point_t& a, const point_t& b)
    {
        a.x() < b.x() ? (_beg = a, _end = b) : (_beg = b, _end = a);
    }
    Edge(const std::initializer_list<point_t>& il)
        : Edge(*il.begin(), *il.end()) {

    }
    point_t beg() const { return _beg; }
    point_t end() const { return _end; }

    void set_beg(point_t beg) { _beg = beg; }
    void set_end(point_t end) { _end = end; }

    slope_t m() const {
        return (slope_t)(_end.y() - _beg.y()) / (_end.x() - _beg.x());
    }
    offset_t c() const {
        return (_end.y() - (m() * _end.x()));
    }

    Edge perpendicular_bisector();
    bool debug();
    double length();
private:
    point_t _beg;
    point_t _end;
};

template<typename T> inline
Edge<T> Edge<T>::perpendicular_bisector() {
    double half_length = length() / 2;
    if (is_vertical()) {
        typename point_t::value_t half_y
        = lround((_beg.y() + _end.y()) / 2);
        point_t b(_beg.x() - lround(half_length), half_y);
        point_t e(_beg.x() + lround(half_length), half_y);
        return Edge<T>(b, e);
    } else if(is_horizontal()) {
        typename point_t::value_t half_x
        = lround((_beg.x() + _end.x()) / 2);
        point_t b(half_x, _beg.y() - lround(half_length));
        point_t e(half_x, _beg.y() + lround(half_length));
        return Edge<T>(b, e);
    } else {
        typename point_t::value_t half_x = lround((_beg.x() + _end.x()) / 2);
        typename point_t::value_t half_y = m() * half_x + c();
        double M = -1 / m();
        double C = half_y - (M * half_x);
        double D = length();
        //here, I wanted to find the Coordinates that are
        //equidistance from the mid point of the given line.
        //use the binomial formula -> [-b² ± √{b² - (4ac)}]/[2a] and
        // length² = x² + y² and
        // y = orthogonal_m * x + orthogonal_c
        // std::cout << "C = " << C << '\t' << "D = " << D << std::endl;
        // std::cout << "-2mc = " << -2*M*C << '\t'
        //           << "pow(M*D, 2) + pow(D,2) - pow(C,2) = " << pow(M*D, 2) + pow(D,2) - pow(C,2)
        //           << '\t' << "2 * pow(M,2) + 2 = " << 2 * pow(M,2) + 2 << std::endl;
        double beg_x = half_x - (D/(2 * sin(atan(M))));
        double end_x = half_x + (D/(2 * sin(atan(M))));

        point_t b(beg_x, M * beg_x + C);
        point_t e(end_x, M * end_x + C);
        return Edge<T>(b, e);
    }
}

//higher precision specialization of perpendicular_bisector
template<> inline
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

template<typename T>
double Edge<T>::length() {
    return sqrt(pow(_beg.x() - _end.x() , 2) + pow(_beg.y() - _end.y(), 2));
}

//!-----------------------------------------------------
//                    Triangle Class
//------------------------------------------------------
template<typename T>
class Triangle {
    template<typename U>
    friend bool operator==(const Triangle<U>&, const Triangle<U>&);
    template<typename U>
    friend bool operator!=(const Triangle<U>&, const Triangle<U>&);
    template<typename U>
    friend bool have_common_point(const Triangle<U>&, const Triangle<U>&);
public:
//typedefs
    typedef Coordinate<T> point_t;
    typedef Edge<T> edge_t;
    typedef std::array<point_t, 3> value_pack_t;

//constructors
    Triangle() = default;
    Triangle(point_t a, point_t b, point_t c)
    {
        if (a.x() < b.x()) {
            if (b.x() < c.x()) {
                _a = a; _b = b; _c = c;
            }
            else if (a.x() < c.x()) {
                _a = a; _b = c; _c = b;
            }
            else {
                _a = c; _b = a; _c = b;
            }
        }
        else {
            if (a.x() < c.x()) {
                _a = b; _b = a; _c = c;
            }
            else if (b.x() < c.x()) {
                _a = b; _b = c; _c = a;
            } else {
                _a = c; _b = b; _c = a;
            }
        }
    }
    Triangle(std::initializer_list<point_t> il)
        : Triangle(*il.begin(), *(il.begin() + 1), *il.end())
    { }
//setters and getters
    value_pack_t get_triangle() const { return {_a, _b, _c}; }

    void set_a(const point_t a) { _a = a; }
    void set_b(const point_t b) { _b = b; }
    void set_c(const point_t c) { _c = c; }

    point_t a() const { return _a; }
    point_t b() const { return _b; }
    point_t c() const { return _c; }

    edge_t ab() const { return edge_t(_a, _b);}
    edge_t bc() const { return edge_t(_b, _c);}
    edge_t ac() const { return edge_t(_a, _c);}


private:
    point_t _a, _b, _c;
};
template<typename U>
bool operator==(const Coordinate<U>& left, const Coordinate<U>& right)
{
    return left._x == right._x &&
           left._y == right._y;
}

template<typename U>
bool operator!=(const Coordinate<U>& left, const Coordinate<U>& right)
{
    return left._x != right._x ||
           left._y != right._y;
}

template<typename U>
bool operator==(const Edge<U>& left, const Edge<U>& right) {
    return left._beg == right._beg &&
           left._end == right._end;
}

template<typename U>
bool operator!=(const Edge<U>& left, const Edge<U>& right) {
    return left._beg != right._beg ||
           left._end != right._end;
}
template<typename U>
bool operator==(const Triangle<U>& left, const Triangle<U>& right)
{
    return
    left.a() == right.a() &&
    left.b() == right.b() &&
    left.c() == right.c();
}

template<typename U>
bool operator!=(const Triangle<U>& left, const Triangle<U>& right)
{
    return
    left.a() != right.a() ||
    left.b() != right.b() ||
    left.c() != right.c();
}

template<typename U>
bool have_common_point(const Triangle<U>& left, const Triangle<U>& right)
{
    if (left.a() == right.a() ||
        left.a() == right.b() ||
        left.a() == right.c() ||
        left.b() == right.b() ||
        left.b() == right.a() ||
        left.b() == right.c() ||
        left.c() == right.a() ||
        left.c() == right.b() ||
        left.c() == right.c())
        return true;
    return false;
}
#endif // !ZDL_TRIANGLE_H_
