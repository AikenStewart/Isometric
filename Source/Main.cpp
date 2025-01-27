#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <raylib.h>
#include <raymath.h>

#include "ClassesAndFunctions.hpp"

using namespace std;

int window_x = 1920;
int window_y = 1080;

int main() {
    cout << "Starting the game" << endl;
    InitWindow(window_x, window_y, "Isometric");
    SetTargetFPS(60);

    Game game;
    game.world.Read();

    while(WindowShouldClose() == false) {
        BeginDrawing();
        game.world.Build();

        if(IsKeyPressed(KEY_SPACE)) {
            if (game.playing)
                game.playing = false;
            else
                game.playing = true;
        }

        if(game.playing) {
            if (EventTriggered(0.2))
                game.ConwayUpdate();
        }
        else
            game.Update();

        ClearBackground(BLACK);
        EndDrawing();
    }

    game.world.Save();
    CloseWindow();
    return 0;
}

