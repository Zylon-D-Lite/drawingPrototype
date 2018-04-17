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
    :height(i_image.get_height()), width(i_image.get_width()),
    stored_image(i_image) {
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

LineManager::LineManager(const png::image<png::gray_pixel>& i_image)
    : stored_image(i_image.get_width(), i_image.get_height()),
        height(i_image.get_height()), width(i_image.get_width()) {
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            stored_image[y][x].red
                = stored_image[y][x].green
                = stored_image[y][x].blue
                = i_image[y][x];
            stored_image[y][x].alpha = 255;
        }
    }
    for (size_t y = 1; y < height - 1; ++y) {
        for (size_t x = 1; x < width - 1; ++x) {
            if (isLit(stored_image[y][x]) && !checkNeighbours(stored_image, y, x,
                TOP_LEFT | TOP | TOP_RIGHT | LEFT).to_ulong()) {
                follow_path(stored_image, y, x);
            }
        }
    }
     std::cout << "Size = " << lineVector.size() << std::endl;
    smart_rearrange();
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
        if ((lastY == y && lastX == x) || y >= (height-1) || y <= 1 || x >= (width-1) || x <= 1)
            break;
        lastY = y; lastX = x;
    }
    lineVector.push_back(l);
}

std::vector<Coordinate> LineManager::follow_path_version_2
    (const png::image<png::rgba_pixel>& i_image, size_t y, size_t x) {
    size_t lastY = y, lastX= x;
    Line l;
    while (true) {
        std::bitset<8> current_bit_pattern =
             checkNeighbours(i_image, y, x,
                 RIGHT|BOTTOM_RIGHT|BOTTOM|BOTTOM_LEFT);
        // if more than one bit is on, there is a branch
        if (current_bit_pattern.count() > 1) {
            
        }
    }
}
void LineManager::smart_rearrange() {
    for (int i = 0; i < lineVector.size(); ++i) {
        if (lineVector[i].getCoordinates().empty())
            continue;
        Coordinate current_tail = lineVector[i].getCoordinates().back();
        double closest_head = sqrt(pow(height, 2) + pow(width, 2));
        double closest_tail = sqrt(pow(height, 2) + pow(width, 2));
        size_t index_closest_head = i + 1;
        size_t index_closest_tail = i + 1;
        for (int j = i + 1; j < lineVector.size(); ++j) {
            if (lineVector.at(j).getCoordinates().empty())
                continue;
            auto current_next_head = lineVector[j].getCoordinates().front();
            auto current_next_tail = lineVector[j].getCoordinates().back();
            double tail_head_distance = sqrt(pow((int)current_tail.x - (int)current_next_head.x, 2)
                                       + pow((int)current_tail.y - (int)current_next_head.y, 2));
            double tails_distance = sqrt(pow((int)current_tail.x - (int)current_next_tail.x, 2)
                                       + pow((int)current_tail.y - (int)current_next_tail.y, 2));
            if (tail_head_distance < closest_head) {
                index_closest_head = j;
                closest_head = tail_head_distance;
            }

            if (tails_distance < closest_tail) {
                index_closest_tail = j;
                closest_tail = tails_distance;
            }
        }
        if (closest_tail > 50 && closest_tail <= closest_head) {
            std::cout << "far away" << std::endl;
        }
        if (closest_head > 50 && closest_head < closest_tail) {
            std::cout << "far away head" << std::endl;
        }
        if (closest_head <= closest_tail) {
            lineVector[i + 1].getCoordinates()
                .swap(lineVector[index_closest_head].getCoordinates());

        } else {
            std::reverse(lineVector[index_closest_tail].getCoordinates().begin(),
                         lineVector[index_closest_tail].getCoordinates().end());
            lineVector[i + 1].getCoordinates()
                 .swap(lineVector[index_closest_tail].getCoordinates());
        }
    }
}