#pragma once
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

using namespace std;

bool EventTriggered(double interval);

pair <int, int> IsoToScreen(pair<int, int> iso);

pair<int, int> ScreenToIso(pair<int, int> screen);

class World {
public:
    int gridsize = 16 * 64;
    vector<pair<int, int>> world_data;
    array<array<int, 9>, 2> lookup_table = { array<int, 9>{0, 0, 0, 1, 0, 0, 0 ,0 ,0}, array<int, 9>{0, 0, 1, 1, 0, 0, 0 ,0 ,0} };
    Image large_block = LoadImage("Assets/box.png");
    Image block = LoadImage("Assets/small_box.png");
    Image small_block = LoadImage("Assets/smaller_box.png");
    Image smaller_block = LoadImage("Assets/smallest_box.png");
    Texture2D large_block_texture = LoadTextureFromImage(large_block);
    Texture2D block_texture = LoadTextureFromImage(block);
    Texture2D small_block_texture = LoadTextureFromImage(small_block);
    Texture2D smaller_block_texture = LoadTextureFromImage(smaller_block);
    Image cursor = LoadImage("Assets/cursor.png");
    Texture2D cursor_texture = LoadTextureFromImage(cursor);
    Image invalid_cursor = LoadImage("Assets/invalid_cursor.png");
    Texture2D invalid_cursor_texture = LoadTextureFromImage(invalid_cursor);
    Texture2D current_texture = block_texture;

    void ChangeMode();

    void MakeEmptyWorldData();

    void Place(pair<int, int> block);

    void Delete(pair<int, int> block);

    bool Check(pair<int, int> block);

    void Edit();

    void Build();

    void Cursor();


    ~World();
};

class Game {
    public:
        World world;
        bool playing = false;
        
        int WillBeAlive(pair<int, int> block);

        void MakeNextBoard();

        void Update();

        void ConwayUpdate();
};

