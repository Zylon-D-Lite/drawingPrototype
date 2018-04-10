#include "edgemaker.hpp"
#include <cmath>
#include <exception>
const std::vector<std::vector<int>> EdgeMaker::PREWITT_MATRIX_X
{{
    {1, 0, -1},
    {1, 0, -1},
    {1, 0, -1}
}};
const std::vector<std::vector<int>> EdgeMaker::PREWITT_MATRIX_Y
{{
    {1, 1, 1},
    {0, 0, 0},
    {-1, -1, -1}
}};
const std::vector<std::vector<int>> EdgeMaker::SOBEL_MATRIX_X
{{
    {1, 0, -1},
    {1, 0, -2},
    {1, 0, -1}
}};
const std::vector<std::vector<int>> EdgeMaker::SOBEL_MATRIX_Y
{{
    {1, 2, 1},
    {0, 0, 0},
    {-1, -2, -1}
}};

EdgeMaker::EdgeMaker
        (const png::image<png::rgb_pixel> & i_image,
              const std::vector<std::vector<int>> i_edge_matrix_x,
              const std::vector<std::vector<int>> i_edge_matrix_y,
              bool i_output_intermediate_images,
              bool i_output_final_image) :

         red_image(i_image.get_width(), i_image.get_height()),
         green_image(i_image.get_width(), i_image.get_height()),
         blue_image(i_image.get_width(), i_image.get_height()),
         default_edge_matrix_x(i_edge_matrix_x),
         default_edge_matrix_y(i_edge_matrix_y),
         output_intermediate_images(i_output_intermediate_images),
         output_final_image(i_output_final_image)
{
    split(i_image);
}


void EdgeMaker::gaussian_blur(png::image<png::gray_pixel>& i_img,
    unsigned kernal_size, double sigma) {
    auto filter_matrix = generate_gaussian_matrix(kernal_size, sigma);
    auto dummy_pixel = png::gray_pixel(255);
    int k = kernal_size / 2;
    //iterate through the pixels
    for (int y = 0; y < i_img.get_height(); ++y) {
        for (int x = 0; x < i_img.get_width(); ++x) {
            double sum = 0.0;
            //for each pixel, apply gauss filter
            for (int j = -k; j < (int)kernal_size - k; ++j) {
                for (int i = -k; i < (int)kernal_size - k; ++i) {
                    png::gray_pixel neighbour_pixel = dummy_pixel;
                    if (y + j >= 0 && y + j < i_img.get_height() &&
                        x + i >= 0 && x + i < i_img.get_width()) {
                    //catch out of bound accesses
                        neighbour_pixel = i_img[y+j][x+i];
                    }
                    sum += (double)neighbour_pixel * filter_matrix[j + k][i + k];
                }
            }
            i_img[y][x] = lround(sum);
        }
    }
}

void EdgeMaker::make_edges() {
    red_image.write("/home/ineria/Desktop/normal_red.png");
    green_image.write("/home/ineria/Desktop/normal_green.png");
    blue_image.write("/home/ineria/Desktop/normal_blue.png");
    gaussian_blur(red_image, 5, 1.4);
    gaussian_blur(green_image, 5, 1.4);
    gaussian_blur(blue_image, 5, 1.4);
    red_image.write("/home/ineria/Desktop/blurred_red.png");
    green_image.write("/home/ineria/Desktop/blurred_green.png");
    blue_image.write("/home/ineria/Desktop/blurred_blue.png");
}

std::vector<std::vector<double>> EdgeMaker::generate_gaussian_matrix
             (unsigned kernal_size, double sigma)
{
    int k = kernal_size / 2; //based on kernal_size = (2k + 1)
    double coefficient =  1 / (2 * M_PI * pow(sigma, 2));
    double exponent_denominator = 2 * pow(sigma, 2);
    std::vector<std::vector<double>> ret;
    for (int j = 0; j < kernal_size; ++j) {
        std::vector<double> current_row;
        for (int i = 0; i < kernal_size; ++i) {
            double exponent_numerator =
                 pow((i - k), 2)  + pow((j - k), 2);
            double exponent = exponent_numerator / exponent_denominator;
            double H_i_j = coefficient * pow(M_E, -exponent);
            current_row.push_back(H_i_j);
        }
        ret.push_back(current_row);
    }
    return ret;
}

void EdgeMaker::split(const png::image<png::rgb_pixel>& i_image) {
   for (size_t y = 0; y < i_image.get_height(); ++y) {
        for (size_t x = 0; x < i_image.get_width(); ++x) {
            red_image[y][x] = i_image[y][x].red;
            green_image[y][x] = i_image[y][x].green;
            blue_image[y][x] = i_image[y][x].blue;
        }
    }
}