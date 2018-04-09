#include "line.hpp"

const png::rgba_pixel Line::WHITE_PIXEL = png::rgba_pixel(255, 255, 255, 255);
const png::rgba_pixel Line::BLACK_PIXEL = png::rgba_pixel(0, 0, 0, 255);
const png::rgba_pixel Line::TRANSPARENT_PIXEL = png::rgba_pixel(0, 0, 0, 0);
const png::rgba_pixel Line::RED_PIXEL = png::rgba_pixel(255, 0, 0, 255);
const png::rgba_pixel Line::GREEN_PIXEL = png::rgba_pixel(0, 255, 0, 255);
const png::rgba_pixel Line::BLUE_PIXEL = png::rgba_pixel(0, 0, 255, 255);

bool Line::isAdjacent(Coordinate i_curr, Coordinate i_adjacent) {
    if (abs(i_curr.x - i_adjacent.x) <= 1 &&
        abs(i_curr.y - i_adjacent.y) <= 1) {
        return true;
    }
    return false;
}

std::pair<bool, std::string> Line::check(const png::image<png::rgba_pixel>& i_image,
             png::rgba_pixel i_on_pixel) {
    auto firstCheck = check();
    bool noProblems = true;
    if (firstCheck.first == true) {
        return firstCheck;
    }
    std::string returnMessage{};
    for (auto coordinate : lineData) {
        if (i_image[coordinate.y][coordinate.x].red != i_on_pixel.red ||
            i_image[coordinate.y][coordinate.x].green != i_on_pixel.green ||
            i_image[coordinate.y][coordinate.x].blue != i_on_pixel.blue) {
            returnMessage += "Pixel value mismatch at: x = "
                + std::to_string(coordinate.x) + ", y = "
                + std::to_string(coordinate.y) + "\n";
            noProblems = false;
        }
    }
    if (!noProblems)
        return std::pair<bool, std::string>(false, firstCheck.second + returnMessage);
    return std::pair<bool, std::string>(true, "No problems detected");
}

std::pair<bool, std::string> Line::check() {
    bool noProblems = true;
    std::string returnMessage{};
    for (size_t i = 0; i < lineData.size() - 1; ++i) {
        if (!isAdjacent(lineData[i], lineData[i+1])) {
            returnMessage += "Discontinued line at: x1 = "
            + std::to_string(lineData[i].x) + ", y1 = "
            + std::to_string(lineData[i].y) + ", x2 = "
            + std::to_string(lineData[i + 1].x) + ", y2 = "
            + std::to_string(lineData[i + 1].y) + "\n";
            noProblems = false;
        }
    }
    if (!noProblems)
        return std::pair<bool, std::string>(false, returnMessage);
    return std::pair<bool, std::string>(true, "No problems detected");
}