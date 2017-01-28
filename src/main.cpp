#include <iostream>

#include "Output/Output3D.h"
#include "Output/BubbleShot.h"
#include "Output/WebCamVJ.h"
#include "Output/DebugView.h"
#include "Helpers/cli.h"

using namespace std;

int main(int argc, char** argv) {
    Cli cli(argc, argv);
    int camera = cli.getInt("camera", 0);
    TrackerBase *game = NULL;
    if (argc > 1) {
        string gameName = argv[1];
        if (gameName == "3d") {
            game = new Output3D(camera);
        } else if (gameName == "bub") {
            game = new BubbleShot(camera);
        } else if (gameName == "vj") {
            game = new WebCamVJ(camera);
        } else if (gameName == "debug") {
            game = new DebugView(camera, 100.0, 200.0);
        }
    }
    if (game != NULL) {
        cout << "Alpha version. Press ESC or q to exit." << endl;
        game->run();
        delete game;
    } else {
        cout << "Usage " << argv[0] << " [MODE] [OPTIONS].";
        cout << endl;
        cout << endl;
        cout << "Available modes:" << endl;
        cout << "  3d    Camera based 3D object rotation" << endl;
        cout << "  bub   Motion based bubble shot game" << endl;
        cout << "  vj    Motion based image fading" << endl;
        cout << "  debug Illustrating how internals work" << endl;
        cout << endl;
        cout << "Available options:" << endl;
        cout << "  --camera=0    Which WebCam device to use" << endl;
    }
    return 0;
}
