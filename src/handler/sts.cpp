#include "../handler.h"

class HandlerSTS : public Handler {
    private:
        Image ref;

    public:
        HandlerSTS(): ref("ref/sts.png") {}
        dim dims() const { return { 1920, 1080 }; }
        std::string dir() const { return "sts"; }
        bool test(const Image &img) const {
            if (img.differs(ref, 0, 0)) return false;      // top bar
            if (img.differs(ref, 17, 1877)) return false;  // settings icon
            return true;
        }
};
