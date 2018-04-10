#include <png.h>
#include <png++/png.hpp>
#include <vector>

/** EdgeMaker class is responsible for indentifying edges in
 *  an image.
 */
class EdgeMaker {
public:
// Matrices used for edge detection
    static const std::vector<std::vector<int>> SOBEL_MATRIX_X;
    static const std::vector<std::vector<int>> SOBEL_MATRIX_Y;
    static const std::vector<std::vector<int>> PREWITT_MATRIX_X;
    static const std::vector<std::vector<int>> PREWITT_MATRIX_Y;

//! Constructor
    EdgeMaker(const png::image<png::rgb_pixel> & i_image,
              const std::vector<std::vector<int>> i_edge_matrix_x
                 = PREWITT_MATRIX_X,
              const std::vector<std::vector<int>> i_edge_matrix_y
                 = PREWITT_MATRIX_Y,
              bool i_output_intermediate_images = false,
              bool i_output_final_image = true);

//! One liner for making an edge for an image.
    void make_edges();

/* Blurring the image is needed to reduce noise.
 * But it also softens the edges so careful calibration
 * is needed.
 */

//! Typical implementation for blurring
    void gaussian_blur(png::image<png::gray_pixel>& i_img,
    unsigned kernal_size, double sigma);
//! (Supposedly) better version for edge detection
    void changsha_blur(unsigned kernal_size);

private:
//! Splits the given RGB image to 3 seperate images, one for each color band
    void split(const png::image<png::rgb_pixel>& i_image);

    std::vector<std::vector<double>> generate_gaussian_matrix
             (unsigned kernal_size, double sigma);

    png::image<png::gray_pixel> red_image;
    png::image<png::gray_pixel> green_image;
    png::image<png::gray_pixel> blue_image;

    const std::vector<std::vector<int>> &
        default_edge_matrix_x;
    const std::vector<std::vector<int>> &
        default_edge_matrix_y;

    bool output_intermediate_images;
    bool output_final_image;
};