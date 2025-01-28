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
int frame_count = 0;

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

        string pos_string = to_string(GetFPS());
        char* pos_char = (char*)pos_string.c_str();
        DrawText(pos_char, 5, 25, 30, WHITE);

        game.world.ChangeMode();
        ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

