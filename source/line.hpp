#ifndef ZDL_LINE_H
#define ZDL_LINE_H
#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <png.h>
#include <png++/png.hpp>
/**
 * \brief Line Class
 *      Line class responsible for storing coordinates.
 *
 * The Line class stores the coordinates in a
 * Note that the image is NOT stored within the class
 * since only the coordinates are necessary to draw the picture in a canvas.
 */
struct Coordinate {
    int y;
    int x;

    Coordinate& operator=(const Coordinate& other) {
        y = other.y;
        x = other.x;
        return *this;
    }
    bool operator==(const Coordinate &other)
        const { return (y == other.y && x == other.x); }
    bool operator!=(const Coordinate &other)
        const { return !(*this == other); }
private:
};


class Line {
public:
    static const png::rgba_pixel WHITE_PIXEL;
    static const png::rgba_pixel BLACK_PIXEL;
    static const png::rgba_pixel TRANSPARENT_PIXEL;
    static const png::rgba_pixel RED_PIXEL;
    static const png::rgba_pixel GREEN_PIXEL;
    static const png::rgba_pixel BLUE_PIXEL;

// Constructors
    Line() = default;
    Line(const std::vector<Coordinate>& ivec) :
        lineData(ivec) { }
    Line(const std::vector<Coordinate>::iterator& beg_it,
         const std::vector<Coordinate>::iterator& end_it)
        : lineData(beg_it, end_it) { }
//! Checks the continuity and the color value against an image
    std::pair<bool, std::string> check(
             const png::image<png::rgba_pixel>& i_image,
             png::rgba_pixel i_on_pixel = png::rgba_pixel(255, 255, 255, 255));
//! Checks if the line is continuous
    std::pair<bool, std::string> check();

// Getters and setters
    std::vector<Coordinate>& getCoordinates() { return lineData; }

// Inserting and removing elements
    void push_back(Coordinate i_dot) { lineData.push_back(i_dot); }
    void pop_back() { lineData.pop_back(); }
    void insert(size_t offset, const Coordinate& input);
    void append(const std::vector<Coordinate>::iterator& beg_it,
                const std::vector<Coordinate>::iterator& end_it);

// Iterators
    std::vector<Coordinate>::iterator begin() { return lineData.begin(); }
    std::vector<Coordinate>::iterator end() { return lineData.end(); }

    std::vector<Coordinate>::const_iterator cbegin() { return lineData.cbegin(); }
    std::vector<Coordinate>::const_iterator cend() { return lineData.cend(); }

// Operators
    Coordinate& operator[](unsigned long index) { return lineData[index]; }
const Coordinate& operator[](unsigned long index) const { return lineData[index]; }

// Size
    auto size() { return lineData.size(); }
private:
    bool isAdjacent(Coordinate i_current, Coordinate i_adjacent);
    std::vector<Coordinate> lineData;

};
#endif //!ZDL_LINE_H