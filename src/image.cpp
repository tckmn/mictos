#include "image.h"

bool operator==(const dim &d1, const dim &d2) { return d1.w == d2.w && d1.h == d2.h; }

Image::Image(std::string fname) {
    FILE *fp = fopen(fname.c_str(), "rb");
    read = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info = png_create_info_struct(read);
    png_init_io(read, fp);
    png_read_png(read, info, PNG_TRANSFORM_IDENTITY, NULL);
    rows = png_get_rows(read, info);
    fclose(fp);

    // TODO check
    /* if (png_get_channels(read, info) != CHANNELS) { */
    /* } */
}

Image::~Image() {
    png_destroy_read_struct(&read, &info, NULL);
}

dim Image::dims() const {
    return { png_get_image_width(read, info), png_get_image_height(read, info) };
}
