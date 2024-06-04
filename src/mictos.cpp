#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>

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
#include "sts.cpp"
std::vector<std::unique_ptr<Handler>> init_handlers() {
    auto handlers = std::vector<std::unique_ptr<Handler>>();
    handlers.push_back(std::make_unique<HandlerSTS>(HandlerSTS()));
    return handlers;
}

std::map<std::string, dim> init_cache() {
    auto cache = std::map<std::string, dim>();
    unsigned int w, h;
    std::string name;
    std::ifstream cachefile(CACHE_FILE);
    while (cachefile >> w >> h >> name) {
        cache[name] = { w, h };
    }
    cachefile.close();
    return cache;
}

int main(int argc, char* argv[]) {
    auto cache = init_cache();;
    auto handlers = init_handlers();

    // prepare to add to cache
    std::ofstream cachefile(CACHE_FILE, std::ios_base::app);

    DIR *input = opendir(INPUT_DIR);
    /* if (!input) goto fail; */
    struct dirent *d;

    while ((d = readdir(input))) {
        std::string name = d->d_name;
        if (!name.ends_with(".png")) continue;

        // check cache to see if size is illegal
        bool cached = cache.contains(name);
        // TODO check legal size, skip

        printf("reading %s\n", d->d_name);

        FILE *fp = fopen((std::string(INPUT_DIR) + "/" + name).c_str(), "rb");
        png_structp read = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        png_infop info = png_create_info_struct(read);
        png_init_io(read, fp);
        png_read_png(read, info, PNG_TRANSFORM_IDENTITY, NULL);
        dim dims = { png_get_image_width(read, info), png_get_image_height(read, info) };
        png_bytepp rows = png_get_rows(read, info);

        if (png_get_channels(read, info) != CHANNELS) {
            printf("wrong number of channels??\n");
            goto exit;
        }

        for (const auto &handler : handlers) {
            if (handler->dims() == dims && handler->test(rows)) {
                std::cout << "matched! " << handler->dir() << std::endl;
                goto matched;
            }
        }

        printf("no handler for size %dx%d, %s\n", dims.w, dims.h, cached ? "already cached" : "adding to cache");
        if (!cached) {
            cachefile << dims.w << ' ' << dims.h << ' ' << d->d_name << std::endl;
        }

matched:
        png_destroy_read_struct(&read, &info, NULL);
        fclose(fp);
    }

exit:
    cachefile.close();
    closedir(input);
    return 0;

fail:
    fprintf(stderr, "something's missing\n");
    return 1;
}
