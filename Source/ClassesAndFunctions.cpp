#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <raylib.h>
#include <raymath.h>

#include "ClassesAndFunctions.hpp"

using namespace std;

int blocksize = 32;
float face_x = blocksize / 2;
float face_y = blocksize / 4;

int centre_offset = 1920 / 2;
int downwards_offset = (blocksize - (face_y*0.5)) - (blocksize * 85);

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

void World::MakeEmptyWorldData() {
    vector<int> current_row;
    for (int i = 0; i < 301; i++) {
        current_row = {};
        for (int i = 0; i < 301; i++) {
            current_row.push_back(0);
        }
        world_data.push_back(current_row);
    }
}


void World::Clear() {
    ofstream world_file;
    world_file.open("world.csv", ofstream::out | ofstream::trunc);
    world_file.close();
}

void World::Edit() {
    //deletes or adds clicked block to world.csv
    bool changed = false;
    Vector2 screen_pos = GetMousePosition();
    pair<int, int> mouse_pos = ScreenToIso(pair<int, int>(screen_pos.x, screen_pos.y));
    if (mouse_pos.first < world_data[0].size() && mouse_pos.second < world_data.size()) {

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            world_data[mouse_pos.second][mouse_pos.first] = 1;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            world_data[mouse_pos.second][mouse_pos.first] = 0;
        }
    }
}


void World::Build() {
    for (int row = 0; row < world_data.size(); row++) {
        for (int block = 0; block < world_data[row].size(); block++) {
            if(world_data[row][block] == 1) {
                pair<int, int> screen_coords = IsoToScreen(pair<int, int>(block, row));
                DrawTexture(block_texture, screen_coords.first, screen_coords.second, WHITE);
            }
        }
    }
}

void World::Cursor() {
    Vector2 screen_pos = GetMousePosition();
    pair<int, int> mouse_pos = ScreenToIso(pair<int, int>(screen_pos.x, screen_pos.y));
    if(mouse_pos.first >= world_data[0].size() || mouse_pos.second >= world_data.size()) {
        DrawTexture(invalid_cursor_texture, IsoToScreen(mouse_pos).first, IsoToScreen(mouse_pos).second, WHITE);
    }
    else {
        DrawTexture(cursor_texture, IsoToScreen(mouse_pos).first, IsoToScreen(mouse_pos).second, WHITE);
    }
    string pos_string = to_string(static_cast<int>(mouse_pos.first)) + "," + to_string(static_cast<int>(mouse_pos.second));
    char* pos_char = (char*)pos_string.c_str();
    DrawText(pos_char, 5, 25, 30, WHITE);
}

World::~World() {
    UnloadImage(block);
    UnloadTexture(block_texture);
    UnloadImage(cursor);
    UnloadTexture(cursor_texture);
    UnloadImage(invalid_cursor);
    UnloadTexture(invalid_cursor_texture);
}

void Game::CellInfo() {
    Vector2 screen_pos = GetMousePosition();
    pair<int, int> mouse_pos = ScreenToIso(pair<int, int>(screen_pos.x, screen_pos.y));
    string pos_string = to_string(NumberAliveNeighbours(mouse_pos)) + " " + to_string(WillBeAlive(mouse_pos));
    char* pos_char = (char*)pos_string.c_str();
    DrawText(pos_char, 1850, 25, 30, WHITE);
}

int Game::NumberAliveNeighbours(pair<int, int> block) {
    int alive_neighbours = 0;
    //iterates through each block around block
    for (int current_y = block.second - 1; current_y < block.second + 2; current_y++) {
        for (int current_x = block.first - 1; current_x < block.first + 2; current_x++) {
            //if surrounding block is alive and within bounds, add one to alive neighbours
            if (current_y > -1 && current_x > -1 && current_x < world.world_data[0].size() && current_y < world.world_data.size() && pair<int, int>(current_x, current_y) != block)
                if(world.world_data[current_y][current_x] == 1)
                    alive_neighbours += 1;
        }
    }
    return alive_neighbours;
}

int Game::WillBeAlive(pair<int, int> block) {
    int alive_neighbours = NumberAliveNeighbours(block);

    if(alive_neighbours < 2)
        return 0;
    if(alive_neighbours == 2 && world.world_data[block.second][block.first] == 1)
        return 1;
    if(alive_neighbours == 2 && world.world_data[block.second][block.first] == 0)
        return 0;
    if(alive_neighbours == 3)
        return 1;
    if(alive_neighbours > 3)
        return 0;
}

void Game::MakeNextBoard() {
    vector<vector<int>> new_board = world.world_data;
    for(int row = 0; row < new_board.size(); row++) {
        for(int block = 0; block < new_board[row].size(); block++) {
            new_board[row][block] = WillBeAlive(pair<int, int>(block, row));
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

