#include "edgemaker.hpp"
#include <cmath>
#include <exception>
const std::vector<std::vector<int>> EdgeMaker::PREWITT_MATRIX_X
{
    {1, 0, -1},
    {1, 0, -1},
    {1, 0, -1}
};
const std::vector<std::vector<int>> EdgeMaker::PREWITT_MATRIX_Y
{
    {1, 1, 1},
    {0, 0, 0},
    {-1, -1, -1}
};
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

void EdgeMaker::make_edges(double max_thershold, double min_thershold) {
/* Currently, this function is only for test purposes.
 * Later on, this will be full of wonderful and useful
 * functions working together not unlike gears in a high-end watch,
 * the kind you would find in the showcase of a watch shop
 * at a supermarket, the kind you would stare at with mild fascination but
 * never really wanting since the scenerio of you actually buying it seems faker
 * than that sex scene of Johnny and Lisa in the movie "The Room".
 * */
    red_image.write("/home/ineria/Desktop/normal_red.png");
    green_image.write("/home/ineria/Desktop/normal_green.png");
    blue_image.write("/home/ineria/Desktop/normal_blue.png");

    gaussian_blur(red_image, 5, 1.4);
    gaussian_blur(green_image, 5, 1.4);
    gaussian_blur(blue_image, 5, 1.4);

    red_image.write("/home/ineria/Desktop/blurred_red.png");
    green_image.write("/home/ineria/Desktop/blurred_green.png");
    blue_image.write("/home/ineria/Desktop/blurred_blue.png");

    png::image<png::gray_pixel> tempRed(red_image.get_width(), red_image.get_height());
    png::image<png::gray_pixel> tempGreen(red_image.get_width(), red_image.get_height());
    png::image<png::gray_pixel> tempBlue(red_image.get_width(), red_image.get_height());

    find_edge_and_orientation(red_image, tempRed, edge_values_red);
    find_edge_and_orientation(green_image, tempGreen,edge_values_green);
    find_edge_and_orientation(blue_image, tempBlue, edge_values_blue);

    if (output_intermediate_images) {
        tempRed.write("/home/ineria/Desktop/gradient_red.png");
        tempGreen.write("/home/ineria/Desktop/gradient_green.png");
        tempBlue.write("/home/ineria/Desktop/gradient_blue.png");
    }

    nonmaxinum_suppression(red_image, max_thershold, edge_values_red);
    nonmaxinum_suppression(green_image, max_thershold, edge_values_green);
    nonmaxinum_suppression(blue_image, max_thershold, edge_values_blue);

    red_image.write("/home/ineria/Desktop/supressed_red.png");
    green_image.write("/home/ineria/Desktop/supressed_green.png");
    blue_image.write("/home/ineria/Desktop/supressed_blue.png");

}
void EdgeMaker::find_edge_and_orientation(png::image<png::gray_pixel> &i_img,
    png::image<png::gray_pixel>& o_img, EdgeData& i_edge_values) {
        auto gradient_x = calculate_gradient(i_img, PREWITT_MATRIX_X);
        auto gradient_y = calculate_gradient(i_img, PREWITT_MATRIX_Y);
        for (int y = 0; y < gradient_x.size(); ++y) {
            std::vector<std::pair<Gradient, Orientation>> row;
            for (int x = 0; x < gradient_x[0].size(); ++x) {
                /*D.O.*/double g = sqrt(pow(gradient_x[y][x], 2)
                                   + pow(gradient_y[y][x], 2));
                double theta = atan2(gradient_y[y][x], gradient_x[y][x]);
                Orientation ori = lround(theta * M_PI_4) % 8;
                row.push_back(std::make_pair(g, theta));
                o_img[y][x] = 255 -(g / 6 + 127); //linear transformation of gradient
            }
            i_edge_values.push_back(row);
        }
}

void EdgeMaker::nonmaxinum_suppression(png::image<png::gray_pixel> &i_img,
    double i_threshold, EdgeData& i_edge_values) {
    for (int y = 0; y < i_img.get_height(); ++y) {
        for (int x = 0; x < i_img.get_width(); ++x) {
            if (i_edge_values[y][x].first < i_threshold) {
                i_img[y][x] = 0;
                continue;
            }
            Gradient curr_gradient = i_edge_values[y][x].first;
            Orientation curr_ori = i_edge_values[y][x].second;
            int ori_first = (curr_ori + 2) % 8;
            int ori_second = (curr_ori + 6) % 8;

            auto coordi_1st = give_orientation_offset(ori_first);
            auto coordi_2nd = give_orientation_offset(ori_second);

            Gradient first_neighbour_gradient = 255 * 3;
            try {
                    first_neighbour_gradient =
                    i_edge_values.at(y + coordi_1st.second).at(x + coordi_1st.first).first;
            } catch (std::exception e) {}
            Gradient second_neighbour_gradient = 255 * 3;
            try {
                    second_neighbour_gradient =
                    i_edge_values.at(y + coordi_2nd.second).at(x + coordi_2nd.first).first;
            } catch (std::exception e) {}

            if (curr_gradient < first_neighbour_gradient ||
                curr_gradient < second_neighbour_gradient) {
                i_img[y][x] = 0;
                continue;
            }
            i_img[y][x] = 255;
        }
    }
}

std::pair<int, int> EdgeMaker::give_orientation_offset(Orientation i_ori) {
    //NOTE: the return form is in (y, x)
    switch (i_ori) {
    case 0:
        return std::make_pair(0, 1);
    case 1:
        return std::make_pair(1, 1);
    case 2:
        return std::make_pair(1, 0);
    case 3:
        return std::make_pair(1, -1);
    case 4:
        return std::make_pair(0, -1);
    case 5:
        return std::make_pair(-1, -1);
    case 6:
        return std::make_pair(-1, 0);
    case 7:
        return std::make_pair(-1, 1);
    default:
        std::clog << "Orientation Error\n";
        return std::make_pair(1, 1);
    }
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

std::vector<std::vector<double>> EdgeMaker::calculate_gradient(
    png::image<png::gray_pixel>& i_img,
    const std::vector<std::vector<int>> &edge_mat) {
    std::vector<std::vector<Gradient>> ret;
    //row and column constants which are useful later
    int rowConstant = edge_mat[0].size() / 2, columnConstant = edge_mat.size() / 2;
    for (int y = 0; y < i_img.get_height(); ++y) {
        std::vector<Gradient> current_row;
        for (int x = 0; x < i_img.get_width(); ++x) {
        double sum = 0.0;
            for (int j = -columnConstant; j <= columnConstant; ++j) {
                for (int i = -rowConstant; i <= rowConstant; ++i) {
                    auto neighbour_pixel = png::gray_pixel(128);
                    //check for out of bound error
                    if ((y+j) >= 0 && (y+j) < i_img.get_height() &&
                        (x+i) >= 0 && (x+i) < i_img.get_width()) {
                        neighbour_pixel = i_img[y+j][x+i];
                    }
                    sum += edge_mat[j + columnConstant][i + rowConstant]
                           * neighbour_pixel;
                }
            }
            current_row.push_back(sum);
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