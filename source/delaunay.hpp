#ifndef ZDL_DELAUNAY_H_
#define ZDL_DELAUNAY_H_
#include <vector>
#include "triangle.hpp"
#include <png++/png.hpp>
class Delaunay {
public:
    Delaunay() = default;

    template<typename P, typename T>
    void triangulate(std::vector<Coordinate<T>> points, png::image<P>& img,
         const P& line_pixel);
#ifndef NDEBUG
    void debug();
#endif //!NDEBUG
private:
    template<typename P, typename T>
    void draw_edge(png::image<P>& i_img, Coordinate<T>& beg,
    const Coordinate<T>& end, const P& line_pixel) {
    Edge<T> e(beg, end);
    for (int curr_x = b.x; curr_x < a.x; ++curr_x) {
        int curr_y = lround(e.m() * curr_x + e.c());
        if (curr_y < 0 || curr_y > i_img.get_height() ||
            curr_x < 0 || curr_x > i_img.get_width())
            continue;
        i_img[curr_y][curr_x] = line_pixel;
    }
    int stepper = e.beg().y() < e.end().y() ?
         1 : -1;
    for (int curr_y = e.beg().y(); curr_y != e.end().y(); curr_y += stepper) {
        int curr_x = lround((curr_y - e.c()) / e.m());
        if (curr_y < 0 || curr_y > i_img.get_height() ||
            curr_x < 0 || curr_x > i_img.get_width())
            continue;
        i_img[curr_y][curr_x] = line_pixel;
    }
}
};

template<typename P, typename T>
    void triangulate(std::vector<Coordinate<T>> points, png::image<P> img,
         const P& line_pixel)
{

}
#endif // !ZDL_DELAUNAY_H_