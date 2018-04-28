#include "./delaunay.hpp"
#include "./triangle.hpp"
#include <random>
int main() {

    Delaunay d;
    d.debug();
    typedef typename Edge<int>::point_t pt;
    png::image<png::gray_pixel> debug_image(100, 100);
    Edge<int> vertical(pt(1, 1), pt(1, 98));
    Edge<int> horizontal(pt(1, 98), pt(98, 98));
    d.draw_edge(debug_image, horizontal.beg(), horizontal.end(), png::gray_pixel(255));
    d.draw_edge(debug_image, vertical.beg(), vertical.end(), png::gray_pixel(255));

    Edge<int> hb_horizontal = horizontal.perpendicular_bisector();
    Edge<int> hb_vertical = vertical.perpendicular_bisector();
    d.draw_edge(debug_image, hb_horizontal.beg(), hb_horizontal.end(), png::gray_pixel(255));
    d.draw_edge(debug_image, hb_vertical.beg(), hb_vertical.end(), png::gray_pixel(255));

    Edge<int> diagonal(pt(30, 50), pt(33,150));
    Edge<int> hb_diagonal = diagonal.perpendicular_bisector();
    d.draw_edge(debug_image, diagonal.beg(), diagonal.end(), png::gray_pixel(255));
    d.draw_edge(debug_image, hb_diagonal.beg(), hb_diagonal.end(), png::gray_pixel(255));

    for (int i = 1; i < 25; ++i) {
        std::default_random_engine e;
        std::uniform_int_distribution<> uid(0, 199);
        png::image<png::gray_pixel> img(200, 200);
        std::vector<Coordinate<double>> v_point;
        for (int j = 0; j < i; ++j) {
            v_point.push_back(Coordinate<double>(uid(e), uid(e)));
        }
        d.triangulate(v_point, img, png::gray_pixel(255));
        img.write("./debug/triangulate-" + std::to_string(i) + "-d.png");

    }
    debug_image.write("./debug/edge-perpen-d.png");
}