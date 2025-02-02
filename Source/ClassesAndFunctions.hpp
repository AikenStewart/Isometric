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

    Image block = LoadImage("Assets/block.png");
    Texture2D block_texture = LoadTextureFromImage(block);

    Image cursor = LoadImage("Assets/cursor.png");
    Texture2D cursor_texture = LoadTextureFromImage(cursor);

    Image invalid_cursor = LoadImage("Assets/invalid_cursor.png");
    Texture2D invalid_cursor_texture = LoadTextureFromImage(invalid_cursor);
    double texture_size = 0.25;

    void ChangeSize();

    void MakeEmptyWorldData();

    void Place(pair<int, int> block);

    void Delete(pair<int, int> block);

    bool Check(pair<int, int> block);

    void Edit();

    void Cursor();

    void Build();

    ~World();
};

class Game {
    public:
        World world;
        bool playing = false;

        Image play = LoadImage("Assets/play.png");
        Texture2D play_texture = LoadTextureFromImage(play);

        Image pause = LoadImage("Assets/pause.png");
        Texture2D pause_texture = LoadTextureFromImage(pause);

        Texture2D current_button = play_texture;

        void Play();
        
        int WillBeAlive(pair<int, int> block);

        void MakeNextBoard();

        void Update();

        void ConwayUpdate();

        ~Game();
};

