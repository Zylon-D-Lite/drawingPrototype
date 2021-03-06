#ifndef ZDL_LINE_MANAGER_H
#define ZDL_LINE_MANAGER_H
#include "line.hpp"
#include <bitset>
#include <random>
#include <algorithm>
//! Responsible for managing and drawing Lines.
/**! LineManager class takes an image already processed into
 * edges and turns the said edges into a bunch of Line
 * classes.
 */
class LineManager {
public:
    LineManager() = default;
    LineManager(const std::vector<Line> & i_lineVector) :
        lineVector(i_lineVector) { }
    LineManager(const png::image<png::rgba_pixel>& i_image);
    LineManager(const png::image<png::gray_pixel>& i_image);
    void push_back(const Line& i_line)
        { lineVector.push_back(i_line);}
    void pop_back() { lineVector.pop_back(); }
    void output_bash_file(const std::string& i_filePath,
        unsigned topLeftX, unsigned topLeftY);
private:
//data members
    static constexpr unsigned char BOTTOM_RIGHT = 0b00000001;
    static constexpr unsigned char BOTTOM       = 0b00000010;
    static constexpr unsigned char BOTTOM_LEFT  = 0b00000100;
    static constexpr unsigned char LEFT         = 0b00001000;
    static constexpr unsigned char TOP_LEFT     = 0b00010000;
    static constexpr unsigned char TOP          = 0b00100000;
    static constexpr unsigned char TOP_RIGHT    = 0b01000000;
    static constexpr unsigned char RIGHT        = 0b10000000;
    static constexpr unsigned char ALL          = 0b11111111;
    enum {
        INDEX_BOTTOM_RIGHT,
        INDEX_BOTTOM,
        INDEX_BOTTOM_LEFT,
        INDEX_LEFT,
        INDEX_TOP_LEFT,
        INDEX_TOP,
        INDEX_TOP_RIGHT,
        INDEX_RIGHT
    };
    png::image<png::rgba_pixel> stored_image;
    png::rgba_pixel lit_pixel = png::rgba_pixel(255, 255, 255, 255);
    png::rgba_pixel unlit_pixel = png::rgba_pixel(0, 0, 0, 255);
    size_t height = 0, width = 0;
    std::vector<Line> lineVector;

//function members
    //! Checks if a pixel's value is the same as given pixel.
    bool isLit(png::rgba_pixel i_curr,
    png::rgba_pixel i_lit = Line::WHITE_PIXEL);

    /**! Checks if the neighbouring pixel's value is the same as the
     * given pixel.*/
    std::bitset<8> checkNeighbours(const png::image<png::rgba_pixel>&,
        size_t y, size_t x, unsigned char, png::rgba_pixel
        checkAgainstPixel = Line::WHITE_PIXEL);


    //! Greedy follows the 'lit' pixels, starting from a given pixel.
    void follow_path(const png::image<png::rgba_pixel>& , size_t y, size_t x);

    //! Better path follow algorithm
    std::vector<Coordinate> follow_path_version_2
         (const png::image<png::rgba_pixel>&, size_t y, size_t x);
    //! Rearrange the line data so that there is less distance between lines
    void smart_rearrange();
};
#endif //!ZDL_LINE_MANAGER_H