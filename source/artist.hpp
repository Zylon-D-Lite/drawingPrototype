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
            //the loop uses <= which means Edge whose beginning and end points
            //are the same will get drawn
            for (int curr_x = e.beg().x(); curr_x <= e.end().x(); ++curr_x) {
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
                    curr_x_prime <= e_prime.end().x(); ++curr_x_prime) {
                //the loop uses <= which means Edge whose beginning and end points
                //are the same will get drawn
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
        auto edge_ab = input_triangle.ab().all_points();
        auto edge_bc = input_triangle.bc().all_points();
        auto edge_ac = input_triangle.ac().all_points();

        T ab_x_length = input_triangle.ab().x_length();
        T bc_x_length = input_triangle.bc().x_length();
        T ac_x_length = input_triangle.ac().x_length();

        for (int i = 0; i < ab_x_length; ++i) {
            draw(input_img, Edge<T>(edge_ab[i], edge_ac[i]), color_pixel);
        }

        for (int i = 0; i < bc_x_length; ++i) {
            draw(input_img, Edge<T>(edge_bc[i], edge_ac[i + ab_x_length]), color_pixel);
        }
        this->draw(input_img, input_triangle.ab(), color_pixel);
        this->draw(input_img, input_triangle.bc(), color_pixel);
        this->draw(input_img, input_triangle.ac(), color_pixel);
    }

    void test();
private:

};

#endif // ZDL_ARTIST_H_