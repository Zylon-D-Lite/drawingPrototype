#include "edgemaker.hpp"

int main() {
    png::image<png::rgb_pixel> img("/home/ineria/Desktop/powerranger.png");
    EdgeMaker emm(img);
    emm.make_edges();
}