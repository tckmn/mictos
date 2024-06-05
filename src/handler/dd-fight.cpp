#include "../handler.h"

#include <iostream>

#define y1 615
#define x1 241
#define y2 657
#define x2 243

class HandlerDDFight : public Handler {
    private:
        Image normal, glow, silenced, blind;

    public:
        HandlerDDFight(): normal("ref/dd-fight.png"), glow("ref/dd-fight-glow.png"), silenced("ref/dd-fight-silenced.png"), blind("ref/dd-fight-blind.png") {}
        dim dims() const { return normal.dims(); }
        std::string dir() const { return "dd"; }
        bool test(const Image &img, bool debug) const {
            // TODO better debug messages
            if (img.differs(normal, y1, x1, 0) && img.differs(blind, y1, x1, 0)) {
                if (debug) std::cout << "1st check for dd-fight fails" << std::endl;
                return false;
            }
            if (img.differs(normal, y2, x2, 0) && img.differs(glow, y2, x2, 0) && img.differs(silenced, y2, x2, 0)) {
                if (debug) std::cout << "2nd check for dd-fight fails" << std::endl;
                return false;
            }
            return true;
        }
};
