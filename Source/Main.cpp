#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include <utility>
#include <random>
#include <raylib.h>
#include <raymath.h>

#include "ClassesAndFunctions.hpp"



using namespace std;

int window_x = 1920;
int window_y = 1060;



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
            if (EventTriggered(0))
                game.ConwayUpdate();
        }
        else
            game.Update();

        game.world.ChangeSize();

        string pos_string = to_string(GetFPS());
        char* pos_char = (char*)pos_string.c_str();
        
        DrawText(pos_char, 1920-50, 5, 30, WHITE);
        ClearBackground(Color{0, 0, 0});
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

