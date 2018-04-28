#include "edgemaker.hpp"
#include "linemanager.hpp"
int main() {
    png::image<png::rgb_pixel> img("/home/ineria/Desktop/canvas.png");
    EdgeMaker emm(img, "/home/ineria/Desktop/zat.png");
    emm.make_edges(5, 0);
    LineManager lm(emm.get_final_image());
    lm.output_bash_file("/home/ineria/Desktop/canvas.sh", 458, 62);
}