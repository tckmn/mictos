#ifndef __HANDLER_H__
#define __HANDLER_H__

#include <string>
#include <png.h>

typedef struct { unsigned int w; unsigned int h; } dim;
bool operator==(const dim &d1, const dim &d2) { return d1.w == d2.w && d1.h == d2.h; }

class Handler {
    public:
        virtual dim dims() const = 0;
        virtual std::string dir() const = 0;
        virtual bool test(png_bytepp) const = 0;
        virtual ~Handler() {};
};

#endif
