#ifndef ZDL_DELAUNAY_H_
#define ZDL_DELAUNAY_H_
#include <vector>
#include <list>
#include "triangle.hpp"
#include "./artist.hpp"
#include <cmath>
#include <png++/png.hpp>
#include <algorithm>
class Delaunay {
public:
    Delaunay() = default;

    template<typename T>
    std::vector<Triangle<T>> triangulate(const std::vector<Coordinate<T>>&points);
private:
    template<typename T1, typename T2>
    bool is_inscribed(const Triangle<T1>& t, const Coordinate<T2>& p);

};

template<typename T1, typename T2>
    bool Delaunay::is_inscribed(const Triangle<T1>& t, const Coordinate<T2>& p)
{
    Edge<T1> orthogonal_bisector_one, orthogonal_bisector_two;
    //choose two non-horizontal lines
    double m1, mo1;
    if(!t.ab().is_horizontal()) {
        orthogonal_bisector_one = t.ab().perpendicular_bisector();
        m1 = t.ab().m();
        mo1 = orthogonal_bisector_one.m();
        if (!t.bc().is_horizontal()) {
            orthogonal_bisector_two = t.bc().perpendicular_bisector();
        } else {
            orthogonal_bisector_two = t.ac().perpendicular_bisector();
        }
    } else {
        m1 = t.bc().m();
        mo1 = orthogonal_bisector_one.m();
        orthogonal_bisector_one = t.bc().perpendicular_bisector();
        orthogonal_bisector_two = t.ac().perpendicular_bisector();
    }

    //find the center coordinates
    double center_x = (orthogonal_bisector_one.c() - orthogonal_bisector_two.c())
                    / (orthogonal_bisector_two.m() - orthogonal_bisector_one.m());

    double center_y =
         orthogonal_bisector_one.m() * center_x + orthogonal_bisector_one.c();


    double radius = sqrt(pow(t.a().x() - center_x, 2) + pow(t.a().y() - center_y, 2));

    double distance = sqrt(pow(p.x() - center_x, 2) + pow(p.y() - center_y, 2));

    if (radius > distance)
        return true;
    return false;
}


template<typename T>
std::vector<Triangle<T>> Delaunay::triangulate(const std::vector<Coordinate<T>>&points) {

    //find the boundings dimensions of the coordinates
    auto min_x_iter = std::min_element(points.begin(), points.end(), [] (const Coordinate<T>& a, const Coordinate<T>& b)
        ->bool { return a.x() < b.x(); });
    auto max_x_iter = std::max_element(points.begin(), points.end(), [] (const Coordinate<T>& a, const Coordinate<T>& b)
        ->bool { return a.x() < b.x(); });

    auto min_y_iter = std::min_element(points.begin(), points.end(), [] (const Coordinate<T>& a, const Coordinate<T>& b)
        ->bool { return a.y() < b.y(); });
    auto max_y_iter = std::max_element(points.begin(), points.end(), [] (const Coordinate<T>& a, const Coordinate<T>& b)
        ->bool { return a.y() < b.y(); });

    double width = max_x_iter->x() - min_x_iter->x();
    double height = max_y_iter->y() - min_y_iter->y();
    //create a super triangle encompassing the image
    Coordinate<T> point_a(lround(width / -2.0), 0);
    Coordinate<T> point_b(lround(width * 1.5), 0);
    Coordinate<T> point_c(lround(width / 2), height * 2);

    Triangle<T> big_triangle(point_a, point_b, point_c);

    std::list<Triangle<T>> triangle_buffer;
    triangle_buffer.push_back(big_triangle);

    for (auto p : points) {
        std::list<Edge<T>> edge_buffer;
        for (auto triangle_it = triangle_buffer.begin();
             triangle_it != triangle_buffer.end(); ++triangle_it) {
            if (is_inscribed(*triangle_it, p)) {
                //find the side
                auto ab_iter = std::find(edge_buffer.begin(), edge_buffer.end(), triangle_it->ab());
                //if found, it's a common side: remove
                if (ab_iter != edge_buffer.end())
                    edge_buffer.erase(ab_iter);
                else //if not found, push it in
                    edge_buffer.push_back(triangle_it->ab());

                auto bc_iter = std::find(edge_buffer.begin(), edge_buffer.end(), triangle_it->bc());

                if (bc_iter != edge_buffer.end())
                    edge_buffer.erase(bc_iter);
                else
                    edge_buffer.push_back(triangle_it->bc());

                auto ac_iter = std::find(edge_buffer.begin(), edge_buffer.end(), triangle_it->ac());

                if (ac_iter != edge_buffer.end())
                    edge_buffer.erase(ac_iter);
                else
                    edge_buffer.push_back(triangle_it->ac());

                triangle_it = triangle_buffer.erase(triangle_it);
                --triangle_it;//decrease so that the next triangle doesn't get skipped
            }
        }
        for (auto e : edge_buffer) {
            //make new triangles
            triangle_buffer.push_back(Triangle<T>(e.beg(), e.end(), p));
        }
    }
    std::vector<Triangle<T>> ret;
    ret.push_back(big_triangle);
    for (auto t : triangle_buffer) {
        if (have_common_point(big_triangle, t))
            continue;
        ret.push_back(t);
    }
    return ret;
}

#endif // !ZDL_DELAUNAY_H_