#ifndef ZDL_ARTIST_H_
#define ZDL_ARTIST_H_
#include "./triangle.hpp"
#include <png++/png.hpp>
class Artist {
public:
    Artist();
    template<typename T, typename P>
    void draw(png::image<P>& input_img,const Edge<T>& input_edge, const P& line_pixel)
    {
        const Edge<T>& e = input_edge;
        //avoid vertical lines which cannot be represented as functions
        if (!e.is_vertical())
        {
            for (int curr_x = e.beg().x(); curr_x < e.end().x(); ++curr_x) {
                int curr_y = lround(e.m() * curr_x + e.c());
                if (curr_y < 0 || curr_y >= input_img.get_height() ||
                    curr_x < 0 || curr_x >= input_img.get_width())
                    continue;
                input_img[curr_y][curr_x] = line_pixel;
            }
        }
        //transpose the edge and plot again
        Edge<T> e_prime(e.beg().transpose(), e.end().transpose());

        if (!e_prime.is_vertical())
        {
            for (int curr_x_prime = e_prime.beg().x();
                    curr_x_prime < e_prime.end().x(); ++curr_x_prime) {
                int curr_y_prime = lround(e_prime.m() * curr_x_prime + e_prime.c());
                if (curr_y_prime < 0 || curr_y_prime >= input_img.get_width() ||
                    curr_x_prime < 0 || curr_x_prime >= input_img.get_height())
                    continue;
                //swap the x and y back
                input_img[curr_x_prime][curr_y_prime] = line_pixel;
            }
        }
    }
    template<typename T, typename P>
    void color(png::image<P>& input_img, const Triangle<T>& input_triangle, const P& color_pixel)
    {

    }

    void test();
private:

};

#endif // ZDL_ARTIST_H_