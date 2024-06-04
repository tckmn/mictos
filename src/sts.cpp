#include "handler.h"
#include "image.h"

class HandlerSTS : public Handler {
    private:
        Image ref;

    public:
        HandlerSTS(): ref("ref/sts.png") {}
        dim dims() const { return { 1920, 1080 }; }
        std::string dir() const { return "sts"; }
        bool test(const Image &rows) const {
            return true;
        }
};
