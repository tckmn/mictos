#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
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
#include "handler/generic.cpp"
#include "handler/dd-fight.cpp"
std::vector<std::unique_ptr<Handler>> init_handlers() {
    auto handlers = std::vector<std::unique_ptr<Handler>>();
    handlers.push_back(std::make_unique<HandlerGeneric>("adofai", "ref/adofai.png", 1020, 869, 1022, 990));
    handlers.push_back(std::make_unique<HandlerGeneric>("balatro", "ref/balatro-lose.png", 767, 96, 987, 96));
    handlers.push_back(std::make_unique<HandlerGeneric>("balatro", "ref/balatro-normal.png", 767, 96, 987, 96));
    handlers.push_back(std::make_unique<HandlerGeneric>("balatro", "ref/balatro-overlay.png", 767, 96, 987, 96));
    handlers.push_back(std::make_unique<HandlerGeneric>("balatro", "ref/balatro-win.png", 767, 96, 987, 96));
    handlers.push_back(std::make_unique<HandlerGeneric>("dd", "ref/dd-menu.png", 686, 1236, 618, 1260, 1));
    handlers.push_back(std::make_unique<HandlerGeneric>("dd", "ref/dd-nav.png", 661, 318, 677, 799));
    handlers.push_back(std::make_unique<HandlerGeneric>("dd", "ref/dd-nav-hover.png", 661, 318, 677, 799));
    handlers.push_back(std::make_unique<HandlerDDFight>());
    handlers.push_back(std::make_unique<HandlerGeneric>("ftl", "ref/ftl.png", 17, 14, 40, 90));
    handlers.push_back(std::make_unique<HandlerGeneric>("sts", "ref/sts.png", 0, 0, 17, 1877));
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
    bool debug = argc > 1 && !strcmp(argv[1], "-d");
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

    int ret = 0;
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
            if (handler->dims() == img.dims() && handler->test(img, debug)) {
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
            auto dst = OUTPUT_DIR + ("/" + dir);
            if (rename(fullpath.c_str(), (dst + "/" + name).c_str())) {
                mkdir(dst.c_str(), 0777);
                if (rename(fullpath.c_str(), (dst + "/" + name).c_str())) {
                    perror("rename failed");
                    ret = 1;
                    break;
                }
            }
        }
    }

    cachefile.close();
    closedir(input);
    return ret;
}
