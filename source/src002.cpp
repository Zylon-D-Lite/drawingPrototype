#include "edgemaker.hpp"

int main() {
    png::image<png::rgb_pixel> img("/home/ineria/Desktop/powerranger.png");
    EdgeMaker emm(img, EdgeMaker::PREWITT_MATRIX_X, EdgeMaker::PREWITT_MATRIX_Y, true);
    emm.make_edges(20, 0);
}