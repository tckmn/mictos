#ifndef __HANDLER_H__
#define __HANDLER_H__

#include <png.h>

struct category {
    char *dir;
};

const struct category NO_MATCH = (struct category) { 0 };

struct handler {
    int w;
    int h;
    int (*testfn)(png_bytepp);
    struct category cat;
    struct handler *next;
};

#endif
