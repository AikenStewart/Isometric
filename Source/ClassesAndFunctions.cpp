#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <raylib.h>
#include <raymath.h>

#include "ClassesAndFunctions.hpp"

using namespace std;

int blocksize = 32;
float face_x = blocksize / 2;
float face_y = blocksize / 4;

int x_offset = 960;
int y_offset = 540;

double last_update_time = 0;
bool EventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - last_update_time >= interval) {
        last_update_time = currentTime;
        return true;
    }
    return false;
}

Vector2 IsoToScreen(Vector2 iso) {
    Vector2 screen = {(iso.x * face_x) - (iso.y * face_x) + x_offset, (iso.x * face_y) + (iso.y * face_y) - face_y + y_offset};
    screen.x = static_cast<int>(screen.x);
    screen.y = static_cast<int>(screen.y);
    return screen;
}

Vector2 ScreenToIso(Vector2 screen) {
    screen.x -= x_offset;
    screen.y -= y_offset - face_x;

    Vector2 iso = { ((screen.x / face_x) + (2 * (screen.y / face_x))) / 2, ((2 * (screen.y / face_x)) - (screen.x / face_x)) / 2 };
    if(iso.y < 0)
        iso.y = static_cast<int>(iso.y);
    if(iso.y > 0)
        iso.y = static_cast<int>(iso.y) + 1;
    if(iso.x < 0)
        iso.x = static_cast<int>(iso.x) - 1;
    if(iso.x > 0)
        iso.x = static_cast<int>(iso.x);

    return iso;
}

void World::Clear() {
    ofstream world_file;
    world_file.open("world.csv", ofstream::out | ofstream::trunc);
    world_file.close();
}

bool World::Search(Vector2 search_item) {

    //iterates through each block in world data
    for (Vector2 block : world_data) {
        if (Vector2Equals(search_item, block))
            //returns true if search item is in world data
            return true;
    }

    return false;
}

void World::Delete(Vector2 item) {
    //iterates through each block in world data
    vector<Vector2> temp_data;
    for (Vector2 block : world_data) {
        //checks if it is the item to be deleted
        bool is_item = false;
        if (Vector2Equals(block, item))
            is_item = true;
        //adds it back to vector assuming it isnt the item
        if (is_item == false)
            temp_data.push_back(block);
    }

    //updates world_data
    world_data = temp_data;
}

void World::Clean() {
    //iterates through each block in world data
    vector<Vector2> temp_data;
    for(Vector2 block : world_data) {
        //checks if it is already in temp_data
        bool in_vector = false;
        for (Vector2 checker_block : temp_data) {
            if (Vector2Equals(checker_block, block))
                in_vector = true;
        }

        //adds it to push vector if it isnt in it already and isnt an empty line
        if (in_vector == false)
            temp_data.push_back(block);
    }

    //updates world_data
    world_data = temp_data;
}

void World::Sort() {
    //Finds the furthest and closest values of y in world data
    int furthest_y = 204;
    int closest_y = -204;
    for (Vector2 block : world_data) {
        if (block.y < furthest_y)
            furthest_y = block.y;
        if (block.y > closest_y)
            closest_y = block.y;
    }

    //iterates through furthest and closest y
    vector<Vector2> temp_data;
    for(int y = furthest_y; y < closest_y + 1; y++) {
        //finds furthest and closest x for current y
        int closest_x = -204;
        int furthest_x = 204;
        for (Vector2 block : world_data) {
            if (block.y == y && block.x < furthest_x)
                furthest_x = block.x;
            if (block.y == y && block.x > closest_x)
                closest_x = block.x;
        }

        //iterates through closest to furthest x
        for(int x = furthest_x; x < closest_x + 1; x++) {
            Vector2 current_block = {x, y};
            //if the current block exists, it is pushed to temp_data in the right order
            if (Search(current_block))
                temp_data.push_back(current_block);
        }
    }

    //updates world_data
    world_data = temp_data;
}

void World::Edit() {
    //deletes or adds clicked block to world.csv
    bool changed = false;
    Vector2 mouse_pos = ScreenToIso(GetMousePosition());

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        world_data.push_back(mouse_pos);
        changed = true;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Delete(mouse_pos);
        changed = true;
    }

    //cleans and sorts world_data if it changed
    if(changed) {
        Clean();
        Sort();
    }
}

void World::Read() {
    //clears world data
    world_data = {};

    //opens world.csv with ifstream
    ifstream world_file;
    world_file.open("world.csv");

    //iterates through each getline() of world file
    string line;
    while (getline(world_file, line)) {
        stringstream current_line(line);

        //gets individual coords seperated by ,'s
        string string_x;
        string string_y;
        getline(current_line, string_x, ',');
        getline(current_line, string_y, ',');

        //makes sure an empty line that would throw an error when stoi() happens doesn't get through
        if (string_x == "" || string_x == " " || string_y == "" || string_y == " ") 
            break;
        else {
            //converts coords to ints and stores in vector2
            int x = stoi(string_x);
            int y = stoi(string_y);
            Vector2 block = { x, y };

            world_data.push_back(block);
        }
    }

    world_file.close();
}

void World::Build() {
    for(Vector2 block : world_data) {
        Vector2 screen_coords = IsoToScreen(block);
        DrawTexture(block_texture, screen_coords.x, screen_coords.y, WHITE);
    }
}

void World::Cursor() {
    Vector2 mouse_pos = ScreenToIso(GetMousePosition());
    DrawTexture(cursor_texture, IsoToScreen(mouse_pos).x, IsoToScreen(mouse_pos).y, WHITE);
    string pos_string = to_string(static_cast<int>(mouse_pos.x)) + "," + to_string(static_cast<int>(mouse_pos.y));
    char* pos_char = (char*)pos_string.c_str();
    DrawText(pos_char, 5, 25, 30, WHITE);
}

void World::Save() {
    //opens cleared world.csv
    ofstream world_file;
    world_file.open("world.csv", ofstream::out | ofstream::trunc);

    //adds string for each block in world_data to push vector
    vector<string> push_vector;
    string current_line;
    for(Vector2 block : world_data) {
        current_line = to_string(static_cast<int>(block.x)) + "," + to_string(static_cast<int>(block.y));
        push_vector.push_back(current_line);
    }

    //writes data into world.csv
    for (string push_line : push_vector) {
        world_file << push_line << endl;
    }

    world_file.close();
}

World::~World() {
    UnloadImage(block);
    UnloadTexture(block_texture);
    UnloadImage(cursor);
    UnloadTexture(cursor_texture);
}

int Game::NumberAliveNeighbours(Vector2 block) {
    int alive_neighbours = 0;
    //iterates through each block around block
    for (float current_y = block.y - 1; current_y < block.y + 2; current_y++) {
        for (float current_x = block.x - 1; current_x < block.x + 2; current_x++) {
            //if surrounding block is alive, add one to alive neighbours
            Vector2 current_block = {current_x, current_y};
            if (world.Search(current_block) && Vector2Equals(current_block, block) == false)
                alive_neighbours += 1;
        }
    }
    return alive_neighbours;
}

bool Game::WillBeAlive(Vector2 block) {
    int alive_neighbours = NumberAliveNeighbours(block);

    if(alive_neighbours < 2)
        return false;
    if(alive_neighbours == 2 && world.Search(block))
        return true;
    if(alive_neighbours == 2 && world.Search(block) == false)
        return false;
    if(alive_neighbours == 3)
        return true;
    if(alive_neighbours > 3)
        return false;
}

vector<Vector2> Game::MakeNextBoard(vector<Vector2> old_board) {
    //Finds the furthest and closest values of y in world data
    int furthest_y = 204;
    int closest_y = -204;
    int furthest_x = 204;
    int closest_x = -204;
    for (Vector2 block : old_board) {
        if (block.y < furthest_y)
            furthest_y = block.y;
        if (block.y > closest_y)
            closest_y = block.y;
    }
    for (Vector2 block : old_board) {
        if (block.x < furthest_x)
            furthest_x = block.x;
        if (block.x > closest_x)
            closest_x = block.x;
    }
    //iterate through each block in used squares (and extra square around them to account for outside cells touching inside ones)
    vector<Vector2> new_board;
    for (float current_y = furthest_y - 1; current_y < closest_y + 2; current_y++) {
        for (float current_x = furthest_x - 1; current_x < closest_x + 2; current_x++) {
            //find out if cell will be alive next turn and add it to new board if so
            Vector2 current_block = {current_x, current_y};
            if (WillBeAlive(current_block))
                new_board.push_back(current_block);
        }
    }

    old_board = new_board;
    return old_board;
}

void Game::Update() {
       world.Edit();
       world.Cursor();
}

void Game::ConwayUpdate() {
    world.world_data = MakeNextBoard(world.world_data);
}

