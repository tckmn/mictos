#include "../handler.h"

class HandlerBalatro : public Handler {
    private:
        Image normal, overlay, win;

    public:
        HandlerBalatro(): normal("ref/balatro-normal.png"), overlay("ref/balatro-overlay.png"), win("ref/balatro-win.png") {}
        dim dims() const { return { 1920, 1080 }; }
        std::string dir() const { return "balatro"; }
        bool test(const Image &img) const {
            if (img.differs(normal, 767, 96) && img.differs(overlay, 767, 96) && img.differs(win, 767, 96)) return false; // run info
            if (img.differs(normal, 987, 96) && img.differs(overlay, 987, 96) && img.differs(win, 987, 96)) return false; // options
            return true;
        }
};
