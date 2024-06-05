#include "../handler.h"

#include <iostream>

class HandlerGeneric : public Handler {
    private:
        std::string dirname;
        Image ref;
        int y1, x1, y2, x2, threshold;

    public:
        HandlerGeneric(std::string dirname, std::string refpath, int y1, int x1, int y2, int x2, int threshold = 0):
            dirname(dirname), ref(refpath), y1(y1), x1(x1), y2(y2), x2(x2), threshold(threshold) {}
        dim dims() const { return ref.dims(); }
        std::string dir() const { return dirname; }
        bool test(const Image &img, bool debug) const {
            if (img.differs(ref, y1, x1, threshold)) {
                if (debug) std::cout << "1st check for " << dirname << " fails: ref " << ref.pxstr(y1, x1) << " img " << img.pxstr(y1, x1) << std::endl;
                return false;
            }
            if (img.differs(ref, y2, x2, threshold)) {
                if (debug) std::cout << "2nd check for " << dirname << " fails: ref " << ref.pxstr(y2, x2) << " img " << img.pxstr(y2, x2) << std::endl;
                return false;
            }
            return true;
        }
};
