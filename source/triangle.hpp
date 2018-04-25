#ifndef ZDL_TRIANGLE_H_
#define ZDL_TRIANGLE_H_
#include <array>

template<typename T>
class Coordinate {
    friend bool operator==(const Coordinate<T>&, const Coordinate<T>&);
    friend bool operator!=(const Coordinate<T>&, const Coordinate<T>&);
public:
    typedef T value_t;
    Coordinate() = default;
    Coordinate(const value_t& x, const value_t& y)
     : _x(x), _y(y)
    { }
    value_t x() { return _x; }
    value_t y() { return _y; }
    void set_x(value_t x) { _x = x; }
    void set_y(value_t y) { _y = y; }
private:
    value_t _x, _y;

};

template<typename T>
class Edge {
    friend bool operator==(const Edge<T>&, const Edge<T>&);
    friend bool operator!=(const Edge<T>&, const Edge<T>&);
public:
    typedef Coordinate<T> value_t;
    typedef double slope_t;
    typedef double offset_t;
    Edge() = default;
    Edge(const value_t& a, const value_t& b)
    {
        a.x < b.x ? (_beg = a, _end = b) : (_beg = b, _end = a);
    }
    Edge(const std::initializer_list<value_t>& il)
        : Edge(*il.begin(), *it.end()) {

    }
    value_t beg() { return _beg; }
    value_t end() { return _end; }

    void set_beg(value_t beg) { _beg = beg; }
    void set_end(value_t end) { _end = end; }

    slope_t m() {
        return (_end.y() - _beg.y()) / (_end.x() - _beg.x());
    }
    offset_t c() {
        slope_t _m = m();
        return (_end.y() - (_m * _end.x()));
    }
private:
    value_t _beg;
    value_t _end;
};

template<typename T>
class Triangle {
    friend bool operator==(const Triangle<T>&, const Triangle<T>&);
    friend bool operator!=(const Triangle<T>&, const Triangle<T>&);
public:
//typedefs
    typedef Coordinate<T> point_t;
    typedef Edge<T> edge_t;
    typedef std::array<value_t, 3> value_pack_t;

//constructors
    Triangle() = default;
    Triangle(point_t a, point_t b, point_t c)
    {
        if (a.x < b.x) {
            if (b.x < c.x) {
                _a = a; _b = b; _c = c;
            }
            else if (a.x < c.x) {
                _a = a; _b = c; _c = b;
            }
            else {
                _a = c; _b = a; _c = b;
            }
        }
        else {
            if (a.x < c.x) {
                _a = b; _b = a; _c = c;
            }
            else if (b.x < c.x) {
                _a = b; _b = c; _c = a;
            } else {
                _a = c; _b = b; _c = a;
            }
        }
    }
    Triangle(std::initializer_list<value_t> il)
        : Triangle(*il.begin(), *(il.begin() + 1), *il.end())
    { }
//setters and getters
    value_t get_triangle() const { return {_a, _b, _c}; }

    void set_a(const point_t a) { _a = a; }
    void set_b(const point_t b) { _b = b; }
    void set_c(const point_t c) { _c = c; }

    point_t a() { return _a; }
    point_t b() { return _b; }
    point_t c() { return _c; }

    edge_t ab();
    edge_t bc();
    edge_t ac();


private:
    value_t _a, _b, _c;
};

template<typename T>
bool operator==(const Coordinate<T>& left, const Coordinate<T>& right)
{
    return left._x == right._x &&
           left._y == right._y;
}

template<typename T>
bool operator!=(const Coordinator<T>& left, const Coordinator<T>& right)
{
    return left._x != right._x ||
           left._y != right._y;
}

template<typename T>
bool operator==(const Edge<T>& left, const Edge<T>& right) {
    return left._beg == right._beg &&
           left._end == left._end;
}

template<typename T>
bool operator!=(const Edge<T>& left, const Edge<T>& right) {
    return left._beg != right._beg ||
           left._end != left._end;
}
template<typename T>
bool operator==(const Triangle<T>& left, const Triangle<T>& right)
{
    return
    left.a() == right.a() &&
    left.b() == right.b() &&
    left.c() == right.c();
}

template<typename T>
bool operator!=(const Triangle<T>& left, const Triangle<T>& right)
{
    return
    left.a() != right.a() ||
    left.b() != right.b() ||
    left.c() != right.c();
}
#endif // !ZDL_TRIANGLE_H_
