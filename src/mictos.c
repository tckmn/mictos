#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <png.h>

// can't be bothered to make a proper config setup,
// nobody will care about this except me anyway
#define INPUT_DIR "/home/tckmn/img/screenshots"
#define OUTPUT_DIR "/home/tckmn/img/sorted"
#define CACHE_FILE "cache"
#define CHANNELS 4

// flip to false when adding new handlers to retry everything
#define IGNORE_ALL_CACHED 0

// new handlers here
#include "sts.c"
struct handler *register_handler(struct handler *handlers, struct handler h) {
    struct handler *handler = malloc(sizeof *handler);
    *handler = h;
    handler->next = handlers;
    return handler;
}
struct handler *register_handlers() {
    struct handler *handlers = 0;
    handlers = register_handler(handlers, handler_sts);
    return handlers;
}

struct cache {
    char *name;
    int w;
    int h;
    struct cache *next;
};

int main(int argc, char* argv[]) {
    struct cache *cache = 0;
    struct handler *handlers = register_handlers();

    // initialize cache
    int w, h;
    char name[1000];
    FILE *cachefile = fopen(CACHE_FILE, "r");
    if (!cachefile) goto fail;
    while (fscanf(cachefile, "%d %d %999s", &w, &h, name) == 3) {
        struct cache *newcache = malloc(sizeof *newcache);
        newcache->name = malloc(strlen(name)+1);
        strcpy(newcache->name, name);
        newcache->w = w;
        newcache->h = h;
        newcache->next = cache;
        cache = newcache;
    }
    fclose(cachefile);

    // prepare to add to cache
    cachefile = fopen(CACHE_FILE, "a");
    if (!cachefile) goto fail;

    DIR *input = opendir(INPUT_DIR);
    if (!input) goto fail;
    struct dirent *d;

    while (d = readdir(input)) {
        int len = strlen(d->d_name);
        if (len < 5 || strncmp(d->d_name + (len-4), ".png", 4)) continue;

        // check cache to see if size is illegal
        int cached = 0;
        for (struct cache *c = cache; c; c = c->next) {
            if (!strcmp(d->d_name, c->name)) {
                if (!IGNORE_ALL_CACHED && 1) {
                    // TODO check legal size
                    cached = 1;
                    break;
                } else {
                    printf("skipping %s (cached as %dx%d)\n", d->d_name, c->w, c->h);
                    goto skip;
                }
            }
        }

        printf("reading %s\n", d->d_name);

        char *fname = malloc(strlen(INPUT_DIR) + len + 2);
        sprintf(fname, "%s/%s", INPUT_DIR, d->d_name);
        FILE *fp = fopen(fname, "rb");
        free(fname);

        png_structp read = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        png_infop info = png_create_info_struct(read);
        png_init_io(read, fp);
        png_read_png(read, info, PNG_TRANSFORM_IDENTITY, NULL);
        w = png_get_image_width(read, info);
        h = png_get_image_height(read, info);
        png_bytepp rows = png_get_rows(read, info);

        if (png_get_channels(read, info) != CHANNELS) {
            printf("wrong number of channels??\n");
            goto exit;
        }

        for (struct handler *handler = handlers; handler; handler = handler->next) {
            if (handler->w == w && handler->h == h && handler->testfn(rows)) {
                printf("matched! %s\n", handler->cat.dir);
                goto matched;
            }
        }

        printf("no handler for size %dx%d, %s\n", w, h, cached ? "already cached" : "adding to cache");
        if (!cached) {
            fprintf(cachefile, "%d %d %s\n", w, h, d->d_name);
            fflush(cachefile);
        }

matched:
        png_destroy_read_struct(&read, &info, NULL);
        fclose(fp);

skip:
    }

exit:
    fclose(cachefile);
    closedir(input);
    return 0;

fail:
    fprintf(stderr, "something's missing\n");
    return 1;
}
