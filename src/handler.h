#ifndef __HANDLER_H__
#define __HANDLER_H__

#include <string>
#include <png.h>
#include "image.h"

class Handler {
    public:
        virtual dim dims() const = 0;
        virtual std::string dir() const = 0;
        virtual bool test(const Image&, bool debug) const = 0;
        virtual ~Handler() {};
};

#endif
