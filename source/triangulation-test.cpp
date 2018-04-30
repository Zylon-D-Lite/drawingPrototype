#include "./delaunay.hpp"
#include "./artist.hpp"
#include <random>
#include <png++/png.hpp>


int main() {
    png::image<png::rgb_pixel> img(1280, 720);
    double blue_scale = 256 / 1280.0;
    for (int y = 0; y < img.get_height(); ++y) {
        for (int x = 0; x < img.get_width(); ++x) {
            img[y][x] = png::rgb_pixel(250, 250, lround(x * blue_scale));
        }
    }
    Delaunay d;
    std::default_random_engine e(2);
    std::uniform_int_distribution<> ui1280(0, 1279);
    std::uniform_int_distribution<> ui720(0, 719);
    std::vector<Coordinate<double>> v_points;
    for (int i = 0; i < 200; ++i) {
        v_points.emplace_back(ui1280(e), ui720(e));
    }
    auto triangles = d.triangulate(v_points);
    Artist a;
    for (auto t : triangles) {

        a.draw(img, t.ab(), png::rgb_pixel(0, 0, 0));
        a.draw(img, t.bc(), png::rgb_pixel(0, 0, 0));
        a.draw(img, t.ac(), png::rgb_pixel(0, 0, 0));
    }
    a.draw(img, triangles.front().ab(), png::rgb_pixel(255, 0, 0));
    a.draw(img, triangles.front().bc(), png::rgb_pixel(255, 0, 0));
    a.draw(img, triangles.front().ac(), png::rgb_pixel(255, 0, 0));
    img.write("./delaunay-wallpaper.png");
}