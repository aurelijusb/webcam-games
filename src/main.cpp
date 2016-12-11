#include <iostream>
#include "Output/Output3D.h"
#include "Output/BubbleShot.h"
#include "Output/WebCamVJ.h"
int main(int argc, char** argv) {
    TrackerBase *game = NULL;
    if (argc > 1) {
        if (strcmp(argv[1], "3d") == 0) {
            game = new Output3D();
        } else if (strcmp(argv[1], "bub") == 0) {
            game = new BubbleShot();
        } else if (strcmp(argv[1], "vj") == 0) {
            game = new WebCamVJ();
        }
    }
    cout << "(C) Aurelijus Banelis <aurelijus@banelis.lt>" << endl;
    if (game != NULL) {
        cout << "Alpha version. Press ESC or q to exit." << endl;
        game->run();
        delete game;
    } else {
        cout << "Usage game [argument].\nArguments:" << endl;
        cout << "  3d   Camera based 3D object rotation" << endl;
        cout << "  bub  Motion based bubble shot game" << endl;
        cout << "  vj   Motion based image fading" << endl;
    }
    return 0;
}
