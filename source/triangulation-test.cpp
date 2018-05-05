#include "./delaunay.hpp"
#include "./artist.hpp"
#include <random>
#include <png++/png.hpp>


int main() {
    //create a buffer image slighly bigger than the final image
    png::image<png::rgb_pixel> buffer_img(2200, 1300);
    Delaunay d;

    //randomly create points within buffer image bounds
    std::default_random_engine e(2);
    std::uniform_int_distribution<> ui1280(0, 2200);
    std::uniform_int_distribution<> ui720(0, 1300);
    std::vector<Coordinate<double>> v_points;
    for (int i = 0; i < 450; ++i) {
        v_points.emplace_back(ui1280(e), ui720(e));
    }

    //triangulate points according to delaunay triangulation
    auto triangles = d.triangulate(v_points);
    Artist a;
    int de = 0;
    std::uniform_int_distribution<> uiColor(0, 255);

    //color in the triangles
    for (auto t : triangles) {
        a.color(buffer_img, t, png::rgb_pixel(uiColor(e), uiColor(e), uiColor(e)));
    }

    //final image
    png::image<png::rgb_pixel> out_img(1980, 1080);
    for (int y = 0; y < 1080; ++y) {
        for (int x = 0; x < 1980; ++x) {
            out_img[y][x] = buffer_img[y + 109][x + 109];
        }
    }

    //write the image
    out_img.write("./delaunay-wallpaper-color-fullHD.png");
}