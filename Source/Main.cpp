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
    game.world.MakeEmptyWorldData();

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
            game.ConwayUpdate();
        }
        else
            game.Update();

        cout << game.world.world_data[0].size() << endl;
        game.CellInfo();
        ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

