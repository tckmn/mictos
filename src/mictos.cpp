#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>

#include <dirent.h>
#include <stdio.h>
#include <png.h>
#include "image.h"
#include "handler.h"

// can't be bothered to make a proper config setup,
// nobody will care about this except me anyway
#define INPUT_DIR "/home/tckmn/img/screenshots"
#define OUTPUT_DIR "/home/tckmn/img/sorted"
#define CACHE_FILE "cache"

// flip to false when adding new handlers to retry everything
#define IGNORE_ALL_CACHED 0

// new handlers here
#include "handler/adofai.cpp"
#include "handler/balatro.cpp"
#include "handler/sts.cpp"
std::vector<std::unique_ptr<Handler>> init_handlers() {
    auto handlers = std::vector<std::unique_ptr<Handler>>();
    handlers.push_back(std::make_unique<HandlerADOFAI>());
    handlers.push_back(std::make_unique<HandlerBalatro>());
    handlers.push_back(std::make_unique<HandlerSTS>());
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
    if (!input) {
        std::cerr << "no input dir" << std::endl;
        return 1;
    }
    struct dirent *d;

    while ((d = readdir(input))) {
        std::string name = d->d_name, fullpath = std::string(INPUT_DIR) + "/" + name;
        if (!name.ends_with(".png")) continue;

        // check cache to see if size is illegal
        bool cached = cache.contains(name);
        // TODO check legal size, skip

        std::cout << "reading " << name << std::endl;
        Image img(fullpath);

        std::string dir;
        for (const auto &handler : handlers) {
            if (handler->dims() == img.dims() && handler->test(img)) {
                dir = handler->dir();
                break;
            }
        }

        if (dir.empty()) {
            std::cout << "no handler for " << img.dims().w << "x" << img.dims().h << ", " << (cached ? "already cached" : "adding to cache") << std::endl;
            if (!cached) {
                cachefile << img.dims().w << ' ' << img.dims().h << ' ' << d->d_name << std::endl;
            }
        } else {
            std::cout << "found match, moving to " << dir << std::endl;
            rename(fullpath.c_str(), (std::string(OUTPUT_DIR) + "/" + dir + "/" + name).c_str());
        }
    }

    cachefile.close();
    closedir(input);
    return 0;
}
