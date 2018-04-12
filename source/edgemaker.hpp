#include <png.h>
#include <png++/png.hpp>
#include <vector>
#include <string>
/** EdgeMaker class is responsible for indentifying edges in
 *  an image.
 */
class EdgeMaker {
    typedef double XGradient;
    typedef double YGradient;
    typedef double Gradient;
    typedef unsigned Orientation;
    typedef std::vector<std::vector<std::pair<Gradient, Orientation>>> EdgeData;
public:
    enum settings {
        SOBEL, PREWITT,
        GAUSSIAN, CHANGSHA,
    };
// Matrices used for edge detection
    static const std::vector<std::vector<int>> SOBEL_MATRIX_X;
    static const std::vector<std::vector<int>> SOBEL_MATRIX_Y;
    static const std::vector<std::vector<int>> PREWITT_MATRIX_X;
    static const std::vector<std::vector<int>> PREWITT_MATRIX_Y;

//! Constructor
    EdgeMaker(const png::image<png::rgb_pixel> & i_image,
              const std::string& i_output_filepath);

//! One liner for making an edge for an image.
    void make_edges(double max_thershold, double min_thershold);

    void blur(png::image<png::gray_pixel> &i_img);
/* There are serveral techniques for detecting edges
 * but most of them involves a convolution process.
 */
    void find_edge_and_orientation(png::image<png::gray_pixel> &i_img,
    png::image<png::gray_pixel>& o_img, EdgeData& i_edge_values);

//! Turns off all pixels whose gradient is less than give value
    void nonmaxinum_suppression(png::image<png::gray_pixel> &i_img,
         double i_threshold, EdgeData& i_edge_values);

// Setters and Getters(getters coming soon!)
    void set_edge_matrix(unsigned i_code);
    void set_blur_technique(unsigned i_code);
    void set_blur_variables(unsigned i_kernal_size, double i_sigma);
    void set_output_final_image(bool i_choice);
    void set_output_intermediate_image(bool i_choice);

    png::image<png::gray_pixel>& get_final_image() {
        return final_image;
    }
private:
/* Blurring the image is needed to reduce noise.
 * But it also softens the edges so careful calibration
 * is needed.
 */
//! Typical implementation for blurring
    void gaussian_blur(png::image<png::gray_pixel>& i_img,
    unsigned kernal_size, double sigma);
//! (Supposedly) better version for edge detection
    void changsha_blur(unsigned kernal_size);

//! gradient calculation
    std::vector<std::vector<double>> calculate_gradient(
        png::image<png::gray_pixel> &i_img,
        const std::vector<std::vector<int>> &edge_mat);
//! Splits the given RGB image to 3 seperate images, one for each color band
    void split(const png::image<png::rgb_pixel>& i_image);
//! Merges the three edgified images to one
    void merge();
    std::vector<std::vector<double>> generate_gaussian_matrix
             (unsigned kernal_size, double sigma);


// Data Members
    std::string output_filepath;

    png::image<png::gray_pixel> red_image;
    png::image<png::gray_pixel> green_image;
    png::image<png::gray_pixel> blue_image;
    png::image<png::gray_pixel>final_image;

    std::pair<int, int> give_orientation_offset(Orientation i_ori);
    std::vector<std::vector<int>> default_edge_matrix_x;
    std::vector<std::vector<int>> default_edge_matrix_y;

    unsigned blur_technique;
    unsigned kernal_size;
    double sigma;
    bool output_intermediate_images;
    bool output_final_image;

    EdgeData edge_values_red;
    EdgeData edge_values_green;
    EdgeData edge_values_blue;
    EdgeData edge_values_final;
};