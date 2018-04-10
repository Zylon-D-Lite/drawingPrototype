#include <png.h>
#include <iostream>
#include <png++/png.hpp>
#include <fstream>
#include <string>
#include <exception>
#include <array>
#include <vector>
#include <utility>
#include <tuple>
#include <cmath>
#include "linemanager.hpp"
typedef png::image<png::rgba_pixel> ZImage;
struct pixelGradient {
    double red = 0.0;
    double green = 0.0;
    double blue = 0.0;
    double alpha = 0.0;
};
struct pixelGradientInt {
    int red = 0;
    int green = 0;
    int blue = 0;
    int alpha = 0;
};
void gaussianFilter(png::image<png::rgba_pixel>& input_image,
         const std::string& output_filepath) {
    std::array<std::array<int, 5>, 5> coefficient = {{
        { 2, 4, 5, 4, 2 },
        { 4, 9, 12, 9, 4 },
        { 5, 12, 15, 12, 5 },
        { 4, 9, 12, 9, 4 },
        { 2, 4, 5, 4, 2 }
    }};
    for (size_t y = 2; y < input_image.get_height() - 2; ++y) {
        for (size_t x = 2; x < input_image.get_width() - 2; ++x) {
            double sumRed = 0, sumGreen = 0, sumBlue = 0, sumAlpha = 0;
            for (int matX = -2; matX < 3; ++matX) {
                for (int matY = -2; matY < 3; ++matY) {
                    sumRed += coefficient[matY + 2][matX + 2]
                            * input_image[y + matY][x + matX].red;
                    sumGreen += coefficient[matY + 2][matX + 2]
                            * input_image[y + matY][x + matX].green;
                    sumBlue += coefficient[matY + 2][matX + 2]
                            * input_image[y + matY][x + matX].blue;
                    sumAlpha += coefficient[matY + 2][matX + 2]
                            * input_image[y + matY][x + matX].alpha;
                }
            }
            input_image[y][x].red = sumRed / 159;
            input_image[y][x].green = sumGreen / 159;
            input_image[y][x].blue = sumBlue / 159;
            input_image[y][x].alpha = sumAlpha / 159;
        }
    }
    //input_image.write(output_filepath);
}

std::vector<std::vector<std::pair<pixelGradient, pixelGradient>>>
    sobelGradient(ZImage& input_image) {
    size_t height = input_image.get_height(),
           width  = input_image.get_width();
    std::array<std::array<int, 3>, 3> xCoefficient{
      { { 1, 0, -1},
        { 1, 0, -1},
        { 1, 0, -1}}
    };


    std::array<std::array<int, 3>, 3> yCoefficient{
      { { 1, 1, 1 },
        { 0, 0, 0 },
        { -1, -1, -1} }
    };
    std::vector<std::vector<std::pair<pixelGradient, pixelGradient>>> ret{};
    for (size_t y = 1; y < height -1; ++y) {
        std::vector<std::pair<pixelGradient, pixelGradient>> row;
        for (size_t x = 1; x < width - 1; ++x) {
            double gXr = 0.0, gYr = 0.0,
                   gXg = 0.0, gYg = 0.0,
                   gXb = 0.0, gYb = 0.0,
                   gXa = 0.0, gYa = 0.0;

            for (int newX = -1; newX < 2; ++newX) {
                for (int newY = -1; newY < 2; ++newY) {
                    auto currentPforGdb = input_image[y + newY][x + newX];
                    int r = currentPforGdb.red;
                    int g = currentPforGdb.green;
                    int b = currentPforGdb.blue;
                    int a = currentPforGdb.alpha;

                    int xc = xCoefficient.at(1 + newY).at(1 + newX);
                    int yc = yCoefficient.at(1 + newY).at(1 + newX);

                    gXr += xc * r;
                    gXg += xc * g;
                    gXb += xc * b;
                    gXa += xc * a;

                    gYr += yc * r;
                    gYg += yc * g;
                    gYb += yc * b;
                    gYa += yc * a;
                }
            }
            pixelGradient currentXgradient{gXr, gXg, gXb, gXa};
            pixelGradient currentYgradient{gYr, gYg, gYb, gYa};
            row.push_back(
                std::pair<pixelGradient, pixelGradient>(currentXgradient,currentYgradient));
        }
        ret.push_back(row);
    }
    return ret;
}

inline png::rgba_pixel getAdjacent(const png::image<png::rgba_pixel>& input_image, size_t y, size_t x,
    int ori) {
    switch (ori) {
        case 0:
            return input_image[y + 1][x + 1];
        case 1:
            return input_image[y + 1][x];
        case 2:
            return input_image[y + 1][x - 1];
        case 3:
            return input_image[y][x - 1];
        case 4:
            return input_image[y - 1][x - 1];
        case 5:
            return input_image[y - 1][x];
        case 6:
            return input_image[y - 1][x + 1];
        case 7:
            return input_image[y][x + 1];
    }
}
std::pair<int, int> getAdjacentCoordinates() { }
inline bool firstIsMax(const pixelGradient& one,
    const pixelGradient& two, const pixelGradient& three)
{
    double firstValue = one.red * one.green * one.blue;
    double secondValue = two.red * two.green * two.blue;
    double thirdValue = three.red * three.green * three.blue;

    if (firstValue > secondValue && firstValue > thirdValue)
        return true;
    return false;
}
std::vector<std::vector<std::pair<pixelGradient, pixelGradient>>>
    gradientExtract(const std::vector<std::vector<std::pair<pixelGradient, pixelGradient>>> &i_mat)
{
    std::vector<std::vector<std::pair<pixelGradient, pixelGradient>>> ret{ };
    for (auto row : i_mat) {
        std::vector<std::pair<pixelGradient, pixelGradient>> retRow;
        for (auto pairs : row) {
            double gR = sqrt((pow(pairs.first.red, 2) + pow(pairs.second.red, 2)));
            double gG = sqrt((pow(pairs.first.green, 2) + pow(pairs.second.green, 2)));
            double gB = sqrt((pow(pairs.first.blue, 2) + pow(pairs.second.blue, 2)));
            double gA = sqrt((pow(pairs.first.alpha, 2) + pow(pairs.second.alpha, 2)));

            double thetaR = atan(pairs.second.red / pairs.second.red);
            double thetaG = atan(pairs.second.green / pairs.second.green);
            double thetaB = atan(pairs.second.blue / pairs.second.blue);
            double thetaA = atan(pairs.second.alpha / pairs.second.alpha);
            pixelGradient theBigG = {gR, gG, gB, gA};
            pixelGradient theBigTheta = { thetaR, thetaG, thetaB, thetaA };
            retRow.push_back(std::pair<pixelGradient, pixelGradient>(theBigG, theBigTheta));
        }
        ret.push_back(retRow);
    }
    return ret;
}
inline int checkOrientation(const pixelGradient& ip_man) {
    double avgTheta = (ip_man.red + ip_man.green + ip_man.blue + ip_man.alpha ) / 4;
    return lround(avgTheta * ( 4 / M_PI)) % 8;
}
std::pair<int, int> getIndicesForGradientVector(int orientation) {
    switch (orientation) {
         case 0:
            return std::pair<int, int>(1, 1);
        case 1:
            return std::pair<int, int>(1, 0);
        case 2:
            return std::pair<int, int>(1, -1);
        case 3:
            return std::pair<int, int>(0, -1);
        case 4:
            return std::pair<int, int>(-1, -1);
        case 5:
            return std::pair<int, int>(-1, 0);
        case 6:
            return std::pair<int, int>(-1, 1);
        case 7:
            return std::pair<int, int>(0, 1);
    }
}
void nonMaximumSupress(png::image<png::rgba_pixel>& input_image,
  const std::vector<std::vector<std::pair<pixelGradient, pixelGradient>>>& referenceGradients,
  const std::string& filePath) {
    for (size_t y = 1; y < input_image.get_height()- 1; ++y) {
        for (size_t x = 1; x < input_image.get_width() - 1; ++x) {
            double mag = sqrt(pow(referenceGradients[y - 1][x - 1].first.red, 2) +
                          pow(referenceGradients[y - 1][x - 1].first.green, 2) +
                          pow(referenceGradients[y - 1][x - 1].first.blue, 2)) / 3;

            int pixelToCheckOne = checkOrientation(referenceGradients[y - 1][x - 1].second);
            int pixelToCheckTwo = (pixelToCheckOne + 4) % 8;
            auto pixelCandidateOne = getAdjacent(input_image, y, x, pixelToCheckOne);
            auto pixelCandidateTwo = getAdjacent(input_image, y, x, pixelToCheckTwo);

            auto pCan1Coordinates = getIndicesForGradientVector(pixelToCheckOne);
            auto pCan2Coordinates = getIndicesForGradientVector(pixelToCheckTwo);
            try {
                referenceGradients.at(y-1+pCan1Coordinates.first)
                    .at(x-1-pCan1Coordinates.second);
                referenceGradients.at(y-1+pCan2Coordinates.first)
                    .at(x-1-pCan2Coordinates.second);
            } catch (std::exception e) {
                continue;
            }
            if (mag > 20 && firstIsMax(referenceGradients[y-1][x-1].first,
             referenceGradients[y-1+pCan1Coordinates.first][x-1-pCan1Coordinates.second].first,
              referenceGradients[y-1+pCan2Coordinates.first][x-1-pCan2Coordinates.second].first))
                input_image[y][x] = png::rgba_pixel(255, 255, 255, 255);
            else
                input_image[y][x] = png::rgba_pixel( 0, 0, 0, 255);
        }
    }
    //input_image.write(filePath);
}

void makeBnW(png::image<png::rgba_pixel>& input_image) {
    for (size_t y = 0; y < input_image.get_height(); ++y) {
        for (size_t x = 0; x < input_image.get_width(); ++x) {
            double color = input_image[y][x].red + input_image[y][x].green + input_image[y][x].blue;
            color /= 3;
            input_image[y][x] = png::rgba_pixel(color, color, color, input_image[y][x].alpha);
        }
    }
}


double getMag(const pixelGradient& i_gradient) {
    return sqrt(pow(i_gradient.red, 2) +
                pow(i_gradient.green, 2) +
                pow(i_gradient.blue, 2)) / 3;
}
int hysterysisThreshold(png::image<png::rgba_pixel>& input_image,
    const std::vector<std::vector<std::pair<pixelGradient, pixelGradient>>>& referenceGradients,
    double lowerThreshold) {
    int ret = 0;
    for (size_t y = 1; y < input_image.get_height() - 1; ++y) {
        for (size_t x = 1; x < input_image.get_width() - 1; ++x) {
            if (input_image[y][x].red < 5)
                continue;
            int p1index = (checkOrientation(referenceGradients[y - 1][x - 1].second) + 2) % 8;
            int p2index = (p1index + 4) % 8;
            auto pixelCandidateOne = getAdjacent(input_image, y, x, p1index);
            auto pixelCandidateTwo = getAdjacent(input_image, y, x, p2index);

            int firstPixelY = getIndicesForGradientVector(p1index).first;
            int firstPixelX = getIndicesForGradientVector(p1index).second;
            int secondPixelY = getIndicesForGradientVector(p2index).first;
            int secondPixelX = getIndicesForGradientVector(p2index).second;
            try {
                referenceGradients.at(y  - 1 + firstPixelY).at(x - 1 + firstPixelX);
                referenceGradients.at(y - 1 + secondPixelY).at(x - 1 + secondPixelX);
            } catch (std::exception e) {
                continue;
            }
            auto p1Ori = checkOrientation(
                referenceGradients[y  - 1 + firstPixelY][x - 1 + firstPixelX].second);

            auto p2Ori = checkOrientation(
                referenceGradients[y - 1 + secondPixelY][y - 1 + secondPixelX].second
            );
            if (checkOrientation(referenceGradients[y - 1][x - 1].second)
                == p1Ori && getMag(referenceGradients
                [y - 1 + firstPixelY][x - 1 + firstPixelX].first) > lowerThreshold) {
                int p1Adj1 = p1Ori, p1Adj2 = (p1Ori +4) % 8;

                auto additionX1 = getIndicesForGradientVector(p1Adj1).second;
                auto additionY1 = getIndicesForGradientVector(p1Adj1).first;
                auto additionX2 = getIndicesForGradientVector(p1Adj2).second;
                auto additionY2 = getIndicesForGradientVector(p1Adj2).first;
                try {
                    referenceGradients.at(y-1+firstPixelY+additionY1).at(x-1+firstPixelX+additionX1);
                    referenceGradients.at(y-1+firstPixelY+additionY2).at(x-1+firstPixelX+additionX2);
                }
                catch (std::exception e) {
                    continue;
                }
                if (firstIsMax(referenceGradients[y - 1 + firstPixelY][x - 1 + firstPixelX].first,
                                referenceGradients[y - 1 + firstPixelY + additionY1]
                                [x - 1 + firstPixelX + additionX1].first,
                                referenceGradients[y - 1 + firstPixelY + additionY2]
                                [x - 1 + firstPixelY + additionX2].first)) {
                    if (input_image[y + firstPixelY][x + firstPixelX].red > 5)
                        continue;
                    ++ret;
                    input_image[y + firstPixelY][x + firstPixelX] = png::rgba_pixel(255,255,255,255);
                }
            }
            if (checkOrientation(referenceGradients[y - 1][x - 1].second)
                == p2Ori && getMag(referenceGradients
                [y - 1 + secondPixelY][x - 1 + secondPixelX].first) > lowerThreshold) {
                int p1Adj1 = p2Ori, p1Adj2 = (p2Ori + 4) % 8;

                auto additionX1 = getIndicesForGradientVector(p1Adj1).second;
                auto additionY1 = getIndicesForGradientVector(p1Adj1).first;
                auto additionX2 = getIndicesForGradientVector(p1Adj2).second;
                auto additionY2 = getIndicesForGradientVector(p1Adj2).first;

                try {
                    referenceGradients.at(y - 1 + secondPixelY + additionY1)
                             .at(x - 1 + secondPixelX + additionX1);
                    referenceGradients.at(y - 1 + secondPixelY + additionY2)
                             .at(x - 1 + secondPixelX + additionX2);
                } catch (std::exception e) {
                    continue;
                }
                if (firstIsMax(referenceGradients[y - 1 + secondPixelY][x - 1 + secondPixelX].first,
                                referenceGradients[y - 1 + secondPixelY + additionY1]
                                [x - 1 + secondPixelX + additionX1].first,
                                referenceGradients[y - 1 + secondPixelY + additionY2]
                                [x - 1 + secondPixelY + additionX2].first)) {
                    if (input_image[y + secondPixelY][x + secondPixelX].red > 5)
                        continue;
                    ++ret;
                    input_image[y + secondPixelY][x + secondPixelX] = png::rgba_pixel(255,255,255,255);
                }
            }
        }
    }
    return ret;
}
#include <algorithm>
int main() {
    png::image<png::rgba_pixel> dum("/home/ineria/Downloads/chaosknight.png");
    // makeBnW(dum);
    // gaussianFilter(dum, "/home/ineria/Desktop/new3.png");
    // auto gradients = sobelGradient(dum);
    // auto extractedGradients = gradientExtract(gradients);
    // nonMaximumSupress(dum, extractedGradients, "/home/ineria/Desktop/new4.png");
    // while (true) {
    //     int i = hysterysisThreshold(dum, extractedGradients, 20);
    //     std::clog << i << "\t" ;
    //     if (i == 0)
    //         break;
    // }
    // std::vector<int> io(5, 0);
    // dum.write("/home/ineria/Downloads/rougeknight-2.png");
    // LineManager wtf(dum);
    // wtf.output_bash_file("/home/ineria/Desktop/lol-2.sh", 202, 44);
}