#include "../handler.h"

class HandlerADOFAI : public Handler {
    private:
        Image ref;

    public:
        HandlerADOFAI(): ref("ref/adofai.png") {}
        dim dims() const { return { 1920, 1080 }; }
        std::string dir() const { return "adofai"; }
        bool test(const Image &img) const {
            if (img.differs(ref, 1020, 869)) return false; // judgment early
            if (img.differs(ref, 1022, 990)) return false; // judgment perfect
            return true;
        }
};
