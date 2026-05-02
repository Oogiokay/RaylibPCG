// header.h 
#pragma once
#include "raylib.h"



namespace PCG {
   // Constants

    // Screen & Map Dimensions
    constexpr int SCREEN_WIDTH = 1024;
    constexpr int SCREEN_HEIGHT = 1024;
    constexpr int TILE_SIZE = 4;
    constexpr int MAP_COLUMNS = (SCREEN_WIDTH / TILE_SIZE);
    constexpr int MAP_ROWS = (SCREEN_HEIGHT / TILE_SIZE);
    constexpr int TILE_TOTAL = (SCREEN_HEIGHT * SCREEN_HEIGHT) / TILE_SIZE;


// Tile Types (Using Enum for readability)
    typedef enum {
        TILE_TYPE_FOREST = 0,
        TILE_TYPE_PLAINS = 1,
        TILE_TYPE_SAND = 2,
        TILE_TYPE_WATER = 3,
        TILE_COUNT  // Automatically counts total types
    } TileType;

    extern float NOISE_SCALE;

    struct TileSettings
    {
        float weights[TILE_COUNT];
    };

    extern TileSettings settings;

    TileType GetWeightedTile();


    // Visual & Character settings
    constexpr char FOREST_CHAR = '.';
    constexpr char PLAINS_CHAR = '#';
    constexpr char SAND_CHAR = '%';
    constexpr char WATER_CHAR = '"';
    constexpr Color FOREST_COLOR = { 40, 54, 24, 255 };
    constexpr Color PLAINS_COLOR = { 96, 108, 56, 255 };
    constexpr Color SAND_COLOR = { 254, 250, 224, 255 };
    constexpr Color WATER_COLOR = { 163, 206, 241, 255 };
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


    // NEW Polymorphic Base Class
    

    // Pure Virtual Base Class
    
    class MapGenerator {
    public:
        virtual ~MapGenerator() = default;  // virtual destructor

            // This enforces that every child class MUST write their own Generate function.
        virtual void Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) = 0;
    };


    class RandomMapGenerator : public MapGenerator {
    public:
        RandomMapGenerator();
        ~RandomMapGenerator();
        void Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) override;
    };

    class NoiseMapGenerator : public MapGenerator {
    public:
        NoiseMapGenerator();
        ~NoiseMapGenerator();
        void Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) override;
    };

    class GameOfLifeGenerator : public MapGenerator {
    public:
        GameOfLifeGenerator();
        ~GameOfLifeGenerator();
        void Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) override;
    };

    class TileMap {
    public:
        TileMap();  // constructor
        ~TileMap(); // destructor

       

        //Core Actions
        // Function Declarations
        void CreateMap();
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
        

        //Getter / Setter for map generator
        void SetMapGenerator(MapGenerator* generator);
        MapGenerator* GetMapGenerator() const;

        TileType tileArray[MAP_ROWS][MAP_COLUMNS] = { PCG::TileType::TILE_TYPE_PLAINS }; //2D array to hold tile types for the map

    private:

        MapGenerator* mapGenerator;
    };

    void SliderQuickSetup(float offset, const char* sliderName, float* sliderValue, int min, int max);

}