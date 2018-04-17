#include "edgemaker.hpp"
#include "linemanager.hpp"
int main() {
    png::image<png::rgb_pixel> img("/home/ineria/Desktop/fb_re.png");
    EdgeMaker emm(img, "/home/ineria/Desktop/zat.png");
    emm.make_edges(20, 0);
    LineManager lm(emm.get_final_image());
    lm.output_bash_file("/home/ineria/Desktop/wow.sh", 458, 62);
}