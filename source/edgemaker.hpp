#include <png.h>
#include <png++/png.hpp>
#include <vector>


class EdgeMaker {
public:
    static const std::vector<std::vector<int>> SOBEL_MATRIX_X;
    static const std::vector<std::vector<int>> SOBEL_MATRIX_Y;
    static const std::vector<std::vector<int>> PREWITT_MATRIX_X;
    static const std::vector<std::vector<int>> PREWITT_MATRIX_Y;
    EdgeMaker(const std::vector<std::vector<int>> i_blur_matrix_x,
              const std::vector<std::vector<int>> i_blur_matrix_y);

    void make_edges();
    void gaussian_blur(unsigned kernal_size);
    void changsha_blur(unsigned kernal_size);

private:
    const std::vector<std::vector<int>> &
        default_blur_matrix_x;
    const std::vector<std::vector<int>> &
        default_blur_matrix_y;
};