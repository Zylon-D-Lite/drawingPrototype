#include "./delaunay.hpp"

#ifndef NDEBUG
bool Delaunay::debug() {
//test for draw_edge
    png::image<png::gray_pixel> debug_image(100, 100);
    draw_edge(debug_image, Coordinate<int>(1, 1), Coordinate<int>(1, 98), png::gray_pixel(255));
    draw_edge(debug_image, Coordinate<int>(1, 98), Coordinate<int>(98, 98), png::gray_pixel(255));
    draw_edge(debug_image, Coordinate<int>(98, 98), Coordinate<int>(98, 1), png::gray_pixel(255));
    draw_edge(debug_image, Coordinate<int>(98, 1), Coordinate<int>(1, 1), png::gray_pixel(255));
    draw_edge(debug_image, Coordinate<int>(98, 98), Coordinate<int>(1, 1), png::gray_pixel(255));
    draw_edge(debug_image, Coordinate<int>(1, 98), Coordinate<int>(98, 1), png::gray_pixel(255));
    draw_edge(debug_image, Coordinate<int>(19, 30), Coordinate<int>(21, 70), png::gray_pixel(255));
    draw_edge(debug_image, Coordinate<int>(30, 19), Coordinate<int>(70, 21), png::gray_pixel(255));
    debug_image.write("./debug/delaunay-draw-edge-d.png");
    return true;

}
#endif
