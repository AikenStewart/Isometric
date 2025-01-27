#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <raylib.h>
#include <raymath.h>

using namespace std;

bool EventTriggered(double interval);

pair <int, int> IsoToScreen(pair<int, int> iso);

pair<int, int> ScreenToIso(pair<int, int> screen);

class World {
public:
    vector<vector<int>> world_data;
    Image block = LoadImage("Assets/box.png");
    Texture2D block_texture = LoadTextureFromImage(block);
    Image cursor = LoadImage("Assets/cursor.png");
    Texture2D cursor_texture = LoadTextureFromImage(cursor);
    Image invalid_cursor = LoadImage("Assets/invalid_cursor.png");
    Texture2D invalid_cursor_texture = LoadTextureFromImage(invalid_cursor);

    void MakeEmptyWorldData();

    void Clear();

    void Edit();

    void Build();

    void Cursor();


    ~World();
};

class Game {
    public:
        World world;
        bool playing = false;

        void CellInfo();

        int NumberAliveNeighbours(pair<int, int> block);
        
        int WillBeAlive(pair<int, int> block);

        void MakeNextBoard();

        void Update();

        void ConwayUpdate();
};

