#include <iostream>
#include <sstream>
#include <iomanip>
#include "image.h"

#define CHANNELS 4
#define abs(x) ((x)<0?-(x):(x))

bool operator==(const dim &d1, const dim &d2) { return d1.w == d2.w && d1.h == d2.h; }

Image::Image(std::string fname) {
    FILE *fp = fopen(fname.c_str(), "rb");
    if (!fp) {
        std::cerr << "failed to open image " << fname << std::endl;
        exit(1);
    }

    read = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info = png_create_info_struct(read);
    png_init_io(read, fp);
    png_read_png(read, info, PNG_TRANSFORM_IDENTITY, NULL);
    rows = png_get_rows(read, info);
    fclose(fp);

    if (png_get_channels(read, info) != CHANNELS) {
        std::cerr << "wrong number of channels?? " << fname << std::endl;
        exit(1);
    }
}

Image::~Image() {
    png_destroy_read_struct(&read, &info, NULL);
}

dim Image::dims() const {
    return { png_get_image_width(read, info), png_get_image_height(read, info) };
}

bool Image::differs(const Image &other, int y, int x, int threshold) const {
    return abs(rows[y][CHANNELS*x+0] - other.rows[y][CHANNELS*x+0]) > threshold
        || abs(rows[y][CHANNELS*x+1] - other.rows[y][CHANNELS*x+1]) > threshold
        || abs(rows[y][CHANNELS*x+2] - other.rows[y][CHANNELS*x+2]) > threshold;
}

std::string Image::pxstr(int y, int x) const {
    std::ostringstream s;
    s << std::hex << std::setw(2) << std::setfill('0') << (int)rows[y][CHANNELS*x+0] << (int)rows[y][CHANNELS*x+1] << (int)rows[y][CHANNELS*x+2];
    return s.str();
}
