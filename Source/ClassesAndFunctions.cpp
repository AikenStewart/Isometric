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

int blocksize = 32;

double last_update_time = 0;
bool EventTriggered(double interval) {
    double currentTime = GetTime();

    if (currentTime - last_update_time >= interval) {
        last_update_time = currentTime;
        return true;
    }
    return false;
}


pair<int, int> IsoToScreen(pair<int, int> iso) {
    float face_x = blocksize / 2;
    float face_y = blocksize / 4;
    int x_offset = 960 - face_x;
    int y_offset = (512 * face_x) - 540;

    pair<int, int> screen;
    screen.first = int((iso.first * face_x) + (iso.second * -face_x)) + x_offset;
    screen.second = int((iso.first * face_y) + (iso.second * face_y)) - y_offset;
    return screen;
}


pair<int, int> ScreenToIso(pair<int, int> screen) {
    float face_x = blocksize / 2;
    float face_y = blocksize / 4;
    float determinate = (1 / ((face_x * face_y) - (-face_x * face_y)));
    int x_offset = 960 - face_x;
    int y_offset = (512 * face_x) - 540;

    screen.first -= x_offset + face_x;
    screen.second += y_offset;
    pair<int, int> iso;
    iso.first = int((screen.first * face_y * determinate) + (screen.second * face_x * determinate));
    iso.second = int((screen.first * -face_y * determinate) + (screen.second * face_x * determinate));

    return iso;
}



void World::ChangeSize() {
        if (IsKeyDown(KEY_ONE)) {
            blocksize += 4;
            texture_size = blocksize / 128.0;
        }
        if (IsKeyDown(KEY_TWO) && blocksize > 4) {
            blocksize -= 4;
            texture_size = blocksize / 128.0;
        }

}


void World::MakeEmptyWorldData() {
    vector<int> current_row;

    for (int i = 0; i < gridsize + 1; i++) {
        for (int i = 0; i < gridsize + 1; i++) {

            world_data.push_back(pair<int, int>(0 , 0));
        }
    }
}


void World::Place(pair<int, int> block) {
    int index = block.second * gridsize + block.first;
    if (Check(block) == 0 && index > -1 && index < world_data.size()) {
        int index = block.second * gridsize + block.first;
        world_data[index].second = 1;
        cout << world_data[index].first << endl;

        //iterates through each block around block
        for (int current_y = block.second - 1; current_y < block.second + 2; current_y++) {
            for (int current_x = block.first - 1; current_x < block.first + 2; current_x++) {

                //increases neighbours of each surrounding block by 1
                if (current_y > -1 && current_x > -1 && current_x < gridsize + 1 && current_y < gridsize + 1 && pair<int, int>(current_x, current_y) != block)
                    world_data[current_y * gridsize + current_x].first += 1;
            }
        }
    }
}


void World::Delete(pair<int, int> block) {
    if (Check(block)) {
        int index = block.second * gridsize + block.first;
        world_data[index].second = 0;

        //iterates through each block around block
        for (int current_y = block.second - 1; current_y < block.second + 2; current_y++) {
            for (int current_x = block.first - 1; current_x < block.first + 2; current_x++) {

                //decreases neighbours neighbour count  of each neighbour block by 1
                if (current_y > -1 && current_x > -1 && current_x < gridsize + 1 && current_y < gridsize + 1 && pair<int, int>(current_x, current_y) != block)
                    world_data[current_y * gridsize + current_x] .first -= 1;
            }
        }
    }
}


bool World::Check(pair<int, int> block) {
    int index = block.second * gridsize + block.first;
    return world_data[index].second;
}


void World::Edit() {
    //deletes or adds clicked block to world data
    bool changed = false;
    Vector2 screen_pos = GetMousePosition();
    pair<int, int> mouse_pos = ScreenToIso(pair<int, int>(screen_pos.x, screen_pos.y));

    if (mouse_pos.first < gridsize + 1 && mouse_pos.second < gridsize + 1) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Place(mouse_pos);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Delete(mouse_pos);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            for (int i = 0; i < 10; i++) {
                Place(pair<int, int>(mouse_pos.first + i, mouse_pos.second));
            }
        }
    }
}


void World::Build() {
    for (int row = 0; row < gridsize + 1; row++) {
        for (int block = 0; block < gridsize + 1; block++) {
            if(Check(pair<int,int>(block, row))) {
                pair<int, int> screen_coords = IsoToScreen(pair<int, int>(block, row));
                DrawTextureEx(block_texture, Vector2 {float(screen_coords.first),float(screen_coords.second)}, 0, texture_size, WHITE);
            }
        }
    }
}


void World::Cursor() {
    Vector2 screen_pos = GetMousePosition();
    pair<int, int> screen_pair = { screen_pos.x, screen_pos.y };
    pair<int, int> mouse_pos = ScreenToIso(screen_pair);

    Vector2 draw_coords = {float(IsoToScreen(mouse_pos).first), float(IsoToScreen(mouse_pos).second)};
    if(mouse_pos.first >= gridsize + 1 || mouse_pos.second >= gridsize + 1) {
        DrawTextureEx(invalid_cursor_texture, draw_coords, 0, texture_size, WHITE);
    }
    else {
        DrawTextureEx(cursor_texture, draw_coords, 0, texture_size, WHITE);
    }

    string pos_string = to_string(mouse_pos.first) + ", " + to_string(mouse_pos.second);
    char* pos_char = (char*)pos_string.c_str();
    DrawText(pos_char, 5, 5, 30, WHITE);

    pos_string = to_string(screen_pair.first) + ", " + to_string(screen_pair.second);
    pos_char = (char*)pos_string.c_str();
    DrawText(pos_char, 5, 30, 30, WHITE);
}


World::~World() {
    UnloadImage(block);
    UnloadTexture(block_texture);
    UnloadImage(cursor);
    UnloadTexture(cursor_texture);
    UnloadImage(invalid_cursor);
    UnloadTexture(invalid_cursor_texture);
}




int Game::WillBeAlive(pair<int, int> block) {
    int index = block.second * world.gridsize + block.first;
    return world.lookup_table[world.Check(block)][world.world_data[index].first];
}


void Game::MakeNextBoard() {
    vector<pair<int, int>> new_board = world.world_data;

    for(int row = 0; row < world.gridsize + 1; row++) {
        for(int block = 0; block < world.gridsize + 1; block++) {
            int index = row * world.gridsize + block;
            int alive = world.world_data[index].second;

            if (alive != 0 || world.world_data[index].first > 1) {
                pair<int, int> current_block = { block, row };

                if (WillBeAlive(current_block)) {
                    if (!alive) {
                        new_board[index].second = 1;

                        //iterates through each block around block
                        for (int current_y = row - 1; current_y < row + 2; current_y++) {
                            for (int current_x = block - 1; current_x < block + 2; current_x++) {

                                //increases neighbours of each surrounding block by 1
                                if (current_y > -1 && current_x > -1 && current_x < world.gridsize + 1 && current_y < world.gridsize + 1 && pair<int, int>(current_x, current_y) != current_block)
                                    new_board[current_y * world.gridsize + current_x].first += 1;
                            }
                        }
                    }
                }
                else {
                    if (alive) {
                        new_board[index].second = 0;

                        //iterates through each block around block
                        for (int current_y = row - 1; current_y < row + 2; current_y++) {
                            for (int current_x = block - 1; current_x < block + 2; current_x++) {

                                //decreases neighbours of each surrounding block by 1
                                if (current_y > -1 && current_x > -1 && current_x < world.gridsize + 1 && current_y < world.gridsize + 1 && pair<int, int>(current_x, current_y) != current_block)
                                    new_board[current_y * world.gridsize + current_x].first -= 1;
                            }
                        }
                    }
                }
            }
        }
    }

    world.world_data = new_board;
}


void Game::Update() {
       world.Edit();
       world.Cursor();
}


void Game::ConwayUpdate() {
    MakeNextBoard();
}

