#include "linemanager.hpp"

inline bool LineManager::isLit(png::rgba_pixel i_curr,
    png::rgba_pixel i_lit) {
    if (i_curr.red == i_lit.red &&
        i_curr.green == i_lit.green &&
        i_curr.blue == i_lit.blue &&
        i_curr.alpha == i_lit.alpha)
        return true;
    return false;
}

std::bitset<8> LineManager::checkNeighbours(
        const png::image<png::rgba_pixel>& i_image,
        size_t y, size_t x, unsigned char i_bitPattern,
        png::rgba_pixel checkAgainstPixel) {
    std::bitset<8> ret{ };
    if (i_bitPattern & BOTTOM_RIGHT){
        ret[0] = isLit(i_image[y + 1][x + 1], checkAgainstPixel);
    }
    if (i_bitPattern & BOTTOM) {
        ret[1] = isLit(i_image[y + 1][x], checkAgainstPixel);
    }
    if (i_bitPattern & BOTTOM_LEFT) {
        ret[2] = isLit(i_image[y + 1][x -1], checkAgainstPixel);
    }
    if (i_bitPattern & LEFT) {
            ret[3] = isLit(i_image[y][x - 1], checkAgainstPixel);
    }
    if (i_bitPattern & TOP_LEFT) {
        ret[4] = isLit(i_image[y - 1][x - 1], checkAgainstPixel);
    }
    if (i_bitPattern & TOP) {
        ret[5] = isLit(i_image[y -1][x], checkAgainstPixel);
    }
    if (i_bitPattern & TOP_RIGHT) {
        ret[6] = isLit(i_image[y - 1][x + 1], checkAgainstPixel);
    }
    if (i_bitPattern & RIGHT) {
        ret[7] = isLit(i_image[y][x + 1], checkAgainstPixel);
    }
    return ret;
}

void LineManager::follow_path(const png::image<png::rgba_pixel>& i_image, size_t y, size_t x) {
    size_t lastY = y, lastX = x;
    Line l;
    while (true) {
        if (checkNeighbours(i_image, y, x, RIGHT)[INDEX_RIGHT]) {
            l.getCoordinates().push_back(Coordinate{ y, x++});
        }
        else if (checkNeighbours(i_image, y, x, BOTTOM_RIGHT)[INDEX_BOTTOM_RIGHT]) {
            l.getCoordinates().push_back(Coordinate{y++, x++});
        }
        else if (checkNeighbours(i_image, y, x, BOTTOM)[INDEX_BOTTOM]) {
            l.getCoordinates().push_back(Coordinate{y++, x});
        }
        else if (checkNeighbours(i_image, y, x, BOTTOM_LEFT)[INDEX_BOTTOM_LEFT]) {
            l.getCoordinates().push_back(Coordinate{y++, x--});
        }
        if (lastY == y && lastX == x)
            break;
        lastY = y; lastX = x;
    }
    lineVector.push_back(l);
}

void LineManager::output_bash_file(const std::string& i_filePath,
        unsigned topLeftX, unsigned topLeftY) {
    std::ofstream file(i_filePath);
    file << "#!/bin/bash\n\n" << "xdotool search --name \"\\*\\[Un+\" windowfocus\n";
    for (auto l : lineVector) {
    bool firstPass = true;
        for (auto c : l.getCoordinates()) {
            if (firstPass) {
                firstPass = false;
                file << "xdotool mousemove " << c.x + topLeftX << " " << c.y + topLeftY << "\n";
                file << "xdotool mousedown 1\n";
            }
            else
                file << "xdotool mousemove " << c.x + topLeftX << " " << c.y + topLeftY << "\n";
        }
        file << "xdotool mouseup 1\nsleep 0.05\n";
    }
    file.close();
}
LineManager::LineManager(const png::image<png::rgba_pixel>& i_image)
    :height(i_image.get_height()), width(i_image.get_width()) {
    for (size_t y = 1; y < height - 1; ++y) {
        for (size_t x = 1; x < width - 1; ++x) {
            if (isLit(i_image[y][x]) && !checkNeighbours(i_image, y, x,
                TOP_LEFT | TOP | TOP_RIGHT | LEFT).to_ulong()) {
                follow_path(i_image, y, x);
            }
        }
    }
    std::default_random_engine dre;
    std::uniform_int_distribution<unsigned> uid;
    std::cout << "Size = " << lineVector.size() << std::endl;
    std::shuffle(lineVector.begin(), lineVector.end(), dre);
}