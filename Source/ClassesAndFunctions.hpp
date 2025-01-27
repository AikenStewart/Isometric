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

Vector2 IsoToScreen(Vector2 iso);

Vector2 ScreenToIso(Vector2 screen);

class World {
public:
    vector<Vector2> world_data;
    Image block = LoadImage("Assets/box.png");
    Texture2D block_texture = LoadTextureFromImage(block);
    Image cursor = LoadImage("Assets/cursor.png");
    Texture2D cursor_texture = LoadTextureFromImage(cursor);

    void Delete(Vector2 item);

    void Clear();

    bool Search(Vector2 search_item);

    void Clean();

    void Sort();

    void Edit();

    void Read();

    void Build();

    void Cursor();

    void Save();

    ~World();
};

class Game {
    public:
        World world;
        bool playing;

        int NumberAliveNeighbours(Vector2 block);
        
        bool WillBeAlive(Vector2 block);

        vector<Vector2> MakeNextBoard(vector<Vector2> old_board);

        void Update();

        void ConwayUpdate();
};

