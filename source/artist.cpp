#include "./artist.hpp"

Artist::Artist() {
    /*Empty Constructor Body*/
}
void Artist::test() {

//test for draw_edge
    png::image<png::gray_pixel> debug_image(100, 100);
    Edge<int> one(Coordinate<int>(1, 1), Coordinate<int>(1, 98));
    Edge<int> two(Coordinate<int>(1, 98), Coordinate<int>(98, 98));
    Edge<int> three(Coordinate<int>(98, 98), Coordinate<int>(98, 1));
    Edge<int> four(Coordinate<int>(98, 1), Coordinate<int>(1, 1));
    Edge<int> five(Coordinate<int>(98, 98), Coordinate<int>(1, 1));
    Edge<int> six(Coordinate<int>(1, 98), Coordinate<int>(98, 1));
    Edge<int> seven(Coordinate<int>(19, 30), Coordinate<int>(21, 70));
    Edge<int> eight(Coordinate<int>(30, 19), Coordinate<int>(70, 21));

    draw(debug_image, one, png::gray_pixel(255));
    draw(debug_image, two, png::gray_pixel(255));
    draw(debug_image, three, png::gray_pixel(255));
    draw(debug_image, four, png::gray_pixel(255));
    draw(debug_image, five, png::gray_pixel(255));
    draw(debug_image, six, png::gray_pixel(255));
    draw(debug_image, seven, png::gray_pixel(255));
    draw(debug_image, eight, png::gray_pixel(255));
    debug_image.write("./debug/delaunay-draw-edge-d.png");
}