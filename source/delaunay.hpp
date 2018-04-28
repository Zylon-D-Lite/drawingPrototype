#ifndef ZDL_DELAUNAY_H_
#define ZDL_DELAUNAY_H_
#include <vector>
#include <list>
#include "triangle.hpp"
#include <cmath>
#include <png++/png.hpp>
#include <algorithm>
class Delaunay {
public:
    Delaunay() = default;

    template<typename T, typename P, typename P2>
    void triangulate(const std::vector<Coordinate<T>>&points, png::image<P>& img,
         const P2& line_pixel);

#ifndef NDEBUG
    bool debug();
#endif //!NDEBUG
    template<typename P, typename T>
    void draw_edge(png::image<P>& i_img, const Coordinate<T>& beg,
        const Coordinate<T>& end, const P& line_pixel);
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
        //std::cout << "one is ab\n";
        if (!t.bc().is_horizontal()) {
          //  std::cout << "two is bc\n";
            orthogonal_bisector_two = t.bc().perpendicular_bisector();
        } else {
            //std::cout << "two is ac\n";
            orthogonal_bisector_two = t.ac().perpendicular_bisector();
        }
    } else {
        //std::cout << "one is bc\n" << "two is ac\n";
        m1 = t.bc().m();
        mo1 = orthogonal_bisector_one.m();
        orthogonal_bisector_one = t.bc().perpendicular_bisector();
        orthogonal_bisector_two = t.ac().perpendicular_bisector();
    }
    double center_x = (orthogonal_bisector_one.c() - orthogonal_bisector_two.c())
                    / (orthogonal_bisector_two.m() - orthogonal_bisector_one.m());

    double center_y =
         orthogonal_bisector_one.m() * center_x + orthogonal_bisector_one.c();
    double radius = sqrt(pow(t.a().x() - center_x, 2) + pow(t.a().y() - center_y, 2));
    double radius2 = sqrt(pow(t.b().x() - center_x, 2) + pow(t.b().y() - center_y, 2));
    double distance = sqrt(pow(p.x() - center_x, 2) + pow(p.y() - center_y, 2));

    if (radius > distance)
        return true;
    return false;
}

template<typename P, typename T>
void Delaunay::draw_edge(png::image<P>& i_img, const Coordinate<T>& beg,
    const Coordinate<T>& end, const P& line_pixel) {
    Edge<T> e(beg, end);
//avoid vertical lines which are not linear functions
    if (!e.is_vertical())
    {
        for (int curr_x = e.beg().x(); curr_x < e.end().x(); ++curr_x) {
            int curr_y = lround(e.m() * curr_x + e.c());
            if (curr_y < 0 || curr_y >= i_img.get_height() ||
                curr_x < 0 || curr_x >= i_img.get_width())
                continue;
            i_img[curr_y][curr_x] = line_pixel;
        }
    }
//transpose the edge and plot again
    Edge<T> e_prime(beg.transpose(), end.transpose());
//avoid vertical lines which are not linear functions
    if (!e_prime.is_vertical())
    {
        for (int curr_x_prime = e_prime.beg().x();
                curr_x_prime < e_prime.end().x(); ++curr_x_prime) {
            int curr_y_prime = lround(e_prime.m() * curr_x_prime + e_prime.c());
            if (curr_y_prime < 0 || curr_y_prime >= i_img.get_width() ||
                curr_x_prime < 0 || curr_x_prime >= i_img.get_height())
                continue;
            //swap the x and y back
            i_img[curr_x_prime][curr_y_prime] = line_pixel;
        }
    }
    //std::cout << "Draw edge complete." << std::endl;
}

template<typename T, typename P, typename P2>
    void Delaunay::triangulate(const std::vector<Coordinate<T>>&points, png::image<P>& img,
         const P2& line_pixel) {
    //create a super triangle encompassing the image
    Coordinate<T> point_a(lround(img.get_width() / -2.0), 0);
    Coordinate<T> point_b(lround(img.get_width() * 1.5), 0);
    Coordinate<T> point_c(lround(img.get_width() / 2), img.get_height() * 2);
    Triangle<T> big_triangle(point_a, point_b, point_c);
    // std::cout << "big_triangle.a = " << big_triangle.a().x() << ", " << big_triangle.a().y() << std::endl;
    // std::cout << "big_triangle.b = " << big_triangle.b().x() << ", " << big_triangle.b().y() << std::endl;
    // std::cout << "big_triangle.c = " << big_triangle.c().x() << ", " << big_triangle.c().y() << std::endl;
    std::list<Triangle<T>> triangle_buffer;
    triangle_buffer.push_back(big_triangle);
    //std::cout << points.size() << "is the size.\n";
    for (auto p : points) {
        //std::cout << "p : " << p.x() << ", " << p.y() << std::endl;
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
        //std::cout << "edge_buffer size = " << edge_buffer.size() << std::endl;
        for (auto e : edge_buffer) {
            //make new triangles
            triangle_buffer.push_back(Triangle<T>(e.beg(), e.end(), p));
        }
    }
    //std::cout << triangle_buffer.size() << std::endl;
    for (auto t : triangle_buffer) {
        if (have_common_point(t, big_triangle))
            continue;
        // std::cout << "t.a = " << t.a().x() << ", " << t.a().y() << std::endl;
        // std::cout << "t.b = " << t.b().x() << ", " << t.b().y() << std::endl;
        // std::cout << "t.c = " << t.c().x() << ", " << t.c().y() << std::endl;
        //std::cout << std::endl;
        draw_edge(img, t.a(), t.b(), line_pixel);
        draw_edge(img, t.a(), t.c(), line_pixel);
        draw_edge(img, t.b(), t.c(), line_pixel);
    }
}

#endif // !ZDL_DELAUNAY_H_