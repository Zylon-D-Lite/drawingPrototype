#include "edgemaker.hpp"

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
        (const std::vector<std::vector<int>> i_blur_matrix_x
            = PREWITT_MATRIX_X,
        const std::vector<std::vector<int>> i_blur_matrix_y
            = PREWITT_MATRIX_Y) :
        default_blur_matrix_x(i_blur_matrix_x),
        default_blur_matrix_y(i_blur_matrix_y)
{
    
}
