#include "../handler.h"

class HandlerGeneric : public Handler {
    private:
        std::string dirname;
        Image ref;
        int y1, x1, y2, x2;

    public:
        HandlerGeneric(std::string dirname, std::string refpath, int y1, int x1, int y2, int x2):
            dirname(dirname), ref(refpath), y1(y1), x1(x1), y2(y2), x2(x2) {}
        dim dims() const { return ref.dims(); }
        std::string dir() const { return dirname; }
        bool test(const Image &img) const {
            if (img.differs(ref, y1, x1)) return false;
            if (img.differs(ref, y2, x2)) return false;
            return true;
        }
};
