#include "handler.h"

class HandlerSTS : public Handler {
    public:
        dim dims() const { return { 1920, 1080 }; }
        std::string dir() const { return "sts"; }
        bool test(png_bytepp rows) const {
            return true;
        }
};
