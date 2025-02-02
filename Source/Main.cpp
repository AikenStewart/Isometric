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
    InitWindow(window_x, window_y, "Isometric");
    SetTargetFPS(60);

    Game game;
    game.world.MakeEmptyWorldData();

    while(WindowShouldClose() == false) {

        if(game.playing) {
            if(EventTriggered(game.delay))
                game.ConwayUpdate();
        }
        if (game.playing == 0) {
            game.Update();
        }

        game.world.ChangeSize();
        string pos_string = to_string(GetFPS());
        char* pos_char = (char*)pos_string.c_str();

        BeginDrawing();
        game.world.Build();
        game.world.Cursor();
        game.Play();
        game.Slider();

        DrawText(pos_char, 1920-50, 5, 30, WHITE);

        ClearBackground(Color{0, 0, 0});
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

