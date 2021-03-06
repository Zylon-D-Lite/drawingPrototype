#include "./delaunay.hpp"
#include "./triangle.hpp"
#include "./artist.hpp"
#include <iostream>
#include <random>
int main() {

    Delaunay d;
    Artist a;
    a.test();
    typedef typename Edge<int>::point_t pt;
    png::image<png::gray_pixel> debug_image(100, 100);
    Edge<int> vertical(pt(1, 1), pt(1, 98));
    Edge<int> horizontal(pt(1, 98), pt(98, 98));
    a.draw(debug_image, vertical, png::gray_pixel(255));
    a.draw(debug_image, horizontal, png::gray_pixel(255));

    Edge<int> hb_horizontal = horizontal.perpendicular_bisector();
    Edge<int> hb_vertical = vertical.perpendicular_bisector();
    a.draw(debug_image, hb_horizontal, png::gray_pixel(255));
    a.draw(debug_image, hb_vertical, png::gray_pixel(255));

    Edge<int> diagonal(pt(30, 50), pt(33,150));
    Edge<int> hb_diagonal = diagonal.perpendicular_bisector();
    a.draw(debug_image, diagonal, png::gray_pixel(255));
    a.draw(debug_image, hb_diagonal, png::gray_pixel(255));

    for (int i = 1; i < 25; ++i) {
        std::default_random_engine e;
        std::uniform_int_distribution<> uid(0, 199);
        png::image<png::gray_pixel> img(200, 200);
        std::vector<Coordinate<double>> v_point;
        for (int j = 0; j < i; ++j) {
            v_point.push_back(Coordinate<double>(uid(e), uid(e)));
        }
        auto triangles = d.triangulate(v_point);
        std::cout << "i = " << i << ", triangle size() = " << triangles.size() << std::endl;
        for (auto t : triangles) {
            a.draw(img, t.ab(), png::gray_pixel(255));
            a.draw(img, t.bc(), png::gray_pixel(255));
            a.draw(img, t.ac(), png::gray_pixel(255));
            a.color(img, t, png::gray_pixel(255));
        }
        img.write("./debug/triangulate-" + std::to_string(i) + "-d.png");

    }
    debug_image.write("./debug/edge-perpen-d.png");
    png::image<png::rgb_pixel> one_triangle(200, 200);
    std::default_random_engine e;
    std::uniform_int_distribution<> uid(0, 199);
    Triangle<double> o(Coordinate<double>(uid(e), uid(e)),
                       Coordinate<double>(uid(e), uid(e)),
                       Coordinate<double>(uid(e), uid(e)));
    a.draw(one_triangle, o.ab(), png::rgb_pixel(255, 0, 0));
    a.draw(one_triangle, o.bc(), png::rgb_pixel(255, 0, 0));
    a.draw(one_triangle, o.ac(), png::rgb_pixel(255, 0, 0));
    a.color(one_triangle, o, png::rgb_pixel(0, 255, 0));
    one_triangle.write("./debug/color-d.png");
}