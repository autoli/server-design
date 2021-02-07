#include "../autoli/application.h"

int main(int argc, char** argv) {
    autoli::Application app;
    if(app.init(argc, argv)) {
        return app.run();
    }
    return 0;
}
