#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <string>
#include <png.h>

typedef struct { unsigned int w; unsigned int h; } dim;
bool operator==(const dim &d1, const dim &d2);

class Image {
private:
    png_structp read;
    png_infop info;

protected:
    png_bytepp rows;

public:
    Image(std::string fname);
    Image(const Image &other) = delete;
    Image &operator=(const Image &other) = delete;
    ~Image();
    dim dims() const;
    bool differs(const Image &other, int y, int x, int threshold) const;
    std::string pxstr(int y, int x) const;
};

#endif
