// header.h 
#pragma once
#include "raylib.h"

namespace PCG {

    // Screen & Map Dimensions
    constexpr int SCREEN_WIDTH = 1024;
    constexpr int SCREEN_HEIGHT = 1024;
    constexpr int TILE_SIZE = 64;
    constexpr int MAP_COLUMNS = (SCREEN_WIDTH / TILE_SIZE);
    constexpr int MAP_ROWS = (SCREEN_HEIGHT / TILE_SIZE);

// Tile Types (Using Enum for readability)
    typedef enum {
        TILE_TYPE_GRASS = 0,
        TILE_TYPE_ROCK = 1,
        TILE_COUNT  // Automatically counts total types
    } TileType;

    // Visual & Character settings
    constexpr char GRASS_CHAR = '.';
    constexpr char ROCK_CHAR = '#';
    constexpr Color GRASS_COLOR = { 69, 182, 156, 255 };
    constexpr Color ROCK_COLOR = { 114, 147, 160, 255 };
    constexpr Color UNKNOWN_COLOR = WHITE;



    // UI variable defines used to position buttons on screen
    constexpr int BUTTON_WIDTH = 200;
    constexpr int BUTTON_HEIGHT = 50;
    constexpr int BUTTON_X = (SCREEN_WIDTH - BUTTON_WIDTH - 20);
    constexpr int BUTTON_Y = (SCREEN_HEIGHT - BUTTON_HEIGHT - 20);
    constexpr Rectangle RESET_BUTTON_BOUNDS = { BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };

    // File Names
    constexpr char* MAP_TEXT_FILENAME = "pcg_map_data.txt";
    constexpr char* MAP_IMAGE_FILENAME = "pcg_map.png";

    class TileMap {
    public:
        TileMap();  // constructor
        ~TileMap(); // destructor

        //Core Actions
        // Function Declarations
        void CreateMap(int weight);
        void DrawMap() const;
        void PrintMap() const;
        void DrawGUI();
        
        // I/O Functions
        void SaveMapData(const char* filename) const;
        void SaveMapImage(const char* filename) const;
        void LoadMapData(const char* filename);
        
        void SetTile(int x, int y, PCG::TileType tileType);
        Color GetTileColor(TileType tileType) const;
        char GetTileChar(TileType tileType) const;
        
    private:
        TileType tileArray[MAP_ROWS][MAP_COLUMNS] = { PCG::TileType::TILE_TYPE_ROCK }; //2D array to hold tile types for the map
    };
}