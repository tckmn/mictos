#include "handler.h"

int test_sts(png_bytepp rows) {
    return 1;
}

const struct handler handler_sts = (struct handler) {1920, 1080, test_sts, {"sts"}, 0};
