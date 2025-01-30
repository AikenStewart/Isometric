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

int blocksize = 8;
float face_x = blocksize / 2;
float face_y = blocksize / 4;

int centre_offset = 1920 / 2;
int downwards_offset = 0;

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
    pair<int, int> screen;
    screen.first = static_cast<int>((iso.first * face_x) - (iso.second * face_x) + centre_offset);
    screen.second = static_cast<int>((iso.first * face_y) + (iso.second * face_y) - face_y + downwards_offset);
    return screen;
}

pair<int, int> ScreenToIso(pair<int, int> screen) {
    screen.first -= centre_offset;
    screen.second -= downwards_offset - face_x;
    pair<int, int> iso;
    float first = ((screen.first / face_x) + (2 * (screen.second / face_x))) / 2;
    float second = ((2 * (screen.second / face_x)) - (screen.first / face_x)) / 2;
    if (second < 0)
        iso.second = static_cast<int>(second);
    if (second > 0)
        iso.second = static_cast<int>(second) + 1;
    if (first < 0)
        iso.first = static_cast<int>(first) - 1;
    if (first > 0)
        iso.first = static_cast<int>(first);
    return iso;
}

void World::ChangeMode() {
    if (IsKeyPressed(KEY_ONE)) {
        blocksize = 32;
        face_x = blocksize / 2;
        face_y = blocksize / 4;
        current_texture = large_block_texture;
    }
    if (IsKeyPressed(KEY_TWO)) {
        blocksize = 8;
        face_x = blocksize / 2;
        face_y = blocksize / 4;
        current_texture = block_texture;
    }
    if (IsKeyPressed(KEY_THREE)) {
        blocksize = 4;
        face_x = blocksize / 2;
        face_y = blocksize / 4;
        current_texture = small_block_texture;
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
    if (Check(block) == 0) {
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
    //deletes or adds clicked block to world.csv
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
                DrawTexture(current_texture, screen_coords.first, screen_coords.second, WHITE);
            }
        }
    }
}

void World::Cursor() {
    Vector2 screen_pos = GetMousePosition();
    pair<int, int> mouse_pos = ScreenToIso(pair<int, int>(screen_pos.x, screen_pos.y));
    if(mouse_pos.first >= gridsize + 1 || mouse_pos.second >= gridsize + 1) {
        DrawTexture(invalid_cursor_texture, IsoToScreen(mouse_pos).first, IsoToScreen(mouse_pos).second, WHITE);
    }
    else {
        DrawTexture(cursor_texture, IsoToScreen(mouse_pos).first, IsoToScreen(mouse_pos).second, WHITE);
    }
}

World::~World() {
    UnloadImage(large_block);
    UnloadTexture(large_block_texture);
    UnloadImage(block);
    UnloadTexture(block_texture);
    UnloadImage(small_block);
    UnloadTexture(small_block_texture);
    UnloadImage(smaller_block);
    UnloadTexture(smaller_block_texture);
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

