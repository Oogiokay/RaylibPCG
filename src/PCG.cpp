#include "PCG.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
// Required to call Raylib gui buttons. Add this near the top of PCG.c
#define RAYGUI_IMPLEMENTATION
#include "raygui.h" 


bool PCG::bBulkSaving = false;
int PCG::BulkSaveIndex = 0;

PCG::TileMap::TileMap() {
    // Initialise our tileMap array to all grass tiles by default when we create a new TileMap object. 
    // We can change this later using the CreateMap() function, or by setting individual tiles with SetTile().

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            tileArray[y][x] = TILE_TYPE_FOREST;
        }
    }

    // initialise the mapGenerator to null
    mapGenerator = nullptr;
}


// =============================================
// Destructor for our TileMap class.
// =============================================
PCG::TileMap::~TileMap()
{
    if (mapGenerator != nullptr)
    {
        delete mapGenerator;
        mapGenerator = nullptr;
    }
}


PCG::TileSettings PCG::settings = { 1.0f, 1.0f, 1.0f, 1.0f };
float PCG::NOISE_SCALE = 1;



PCG::TileType PCG::GetWeightedTile() {

    float total = 0.0f;     //local variable initialised to track WEIGHTED TOTAL

    for (int i = 0; i < TILE_COUNT; i++)       //ADDS UP THE WEIGHT
        total += PCG::settings.weights[i];

    float r = (float)GetRandomValue(0, 10000) / 10000.0f * total;   //the get random value is an integer so for rpecision it is multiplied by 10000.

    float cumulative = 0.0f;

    for (int i = 0; i < TILE_COUNT; i++)
    {
        cumulative += PCG::settings.weights[i]; //The epic weight distribution model where if t

        if (r <= cumulative)
            return (TileType)i;
    }

    return TILE_TYPE_FOREST;

}

// ============================================= 
// void CreateMap()
// ============================================= 
void PCG::TileMap::CreateMap() {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            tileArray[y][x] = GetWeightedTile();
        }
    }
}


// ============================================= 
// void SetTile(int x, int y, TileType tileType)
// set a tile in our tileMap array, using the input x and y coordinates, and the type of tile we want to set it to (tileType)
// ============================================= 
void PCG::TileMap::SetTile(int x, int y, TileType tileType)
{
    if (x >= 0 && x < MAP_COLUMNS && y >= 0 && y < MAP_ROWS) {
        tileArray[y][x] = tileType;
    }
}

// ============================================= 
// Color PCG_GetTileColor(TileType tileType)
// Return a colour based on the type type input
// ============================================= 
Color PCG::TileMap::GetTileColor(TileType tileType) const {
    switch (tileType) {
    case PCG::TileType::TILE_TYPE_FOREST: return FOREST_COLOR;
    case TILE_TYPE_PLAINS: return PLAINS_COLOR;
    case TILE_TYPE_SAND: return SAND_COLOR;
    case TILE_TYPE_WATER: return WATER_COLOR;
    default: return UNKNOWN_COLOR;
    }
}


// ============================================= 
// void PCG_DrawMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
// ============================================= 
void PCG::TileMap::DrawMap() const {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            DrawRectangle(x * PCG::TILE_SIZE, y * PCG::TILE_SIZE, PCG::TILE_SIZE, PCG::TILE_SIZE, PCG::TileMap::GetTileColor(tileArray[y][x]));
        }
    }
}

// ============================================= 
// void PCG_PrintMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
// ============================================= 
void PCG::TileMap::PrintMap() const {
    printf("\n-------Map Layout:--------\n");
    // (Existing Print Logic here...)
    printf("--------------------------\n");
}

// ============================================= 
// char GetTileChar(TileType tileType)
// Return a char value based on the type of tile passed in
// ============================================= 
char PCG::TileMap::GetTileChar(TileType tileType) const {
    switch (tileType) {
    case TILE_TYPE_FOREST: return FOREST_CHAR;
    case TILE_TYPE_PLAINS: return PLAINS_CHAR;
    case TILE_TYPE_SAND: return SAND_CHAR;
    case TILE_TYPE_WATER: return WATER_CHAR;
    default: return '?';
    }
}

// ============================================= 
// void PCG_SaveMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* _filename)
// Store our tilemap data to a text file using the input _filename
// ============================================= 
void PCG::TileMap::SaveMapData(const char* _filename) const {
    std::fstream file;
    file.open(_filename, std::ios::out); // Open C++ file stream for writing
    //if (file == nullptr) {    // old C-style file open check
    if (!file.is_open()) { // Check if file opened successfully
        return;
    }

    // Write each single tileArray charater into our file stream
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            //fputc(PCG::GetTileChar(_tileArray[y][x]), file);  // old c-style file write
            file.put(PCG::TileMap::GetTileChar(tileArray[y][x])); // Write char to C++ file stream
        }
        //fputc('\n', file); // New line at end of row
        file.put('\n'); // New line at end of row for C++ file stream
    }
    //fclose(file); // old C-style file close
    file.close(); // Close C++ file stream
    std::cout << "Map saved to " << _filename << std::endl;
}


// ============================================= 
// void PCG_LoadMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* _filename)
// Load our tilemap data from a text file, using input _filename
// ============================================= 
void PCG::TileMap::LoadMapData(const char* _filename) {
    //FILE* file = fopen(_filename, "r"); // "r" = Read // old c style
    std::fstream file;  // C++ file stream object for reading
    file.open(_filename, std::ios::in); // Open C++ file stream for reading
    //if (file == NULL) {   // old C-style file open check
    if (!file.is_open()) { // Check if file opened successfully for C++ stream
        return;
    }

    // Get each character from our file stream, and load it into our tileMap array
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            //int ch = fgetc(file); // old C-style file read
            int ch = file.get(); // Get char from C++ file stream
            // Skip invisible newline characters
            while (ch == '\n' || ch == '\r') {
                //ch = fgetc(file); // old C-style file read for skipping newlines
                ch = file.get(); // Get char from C++ file stream for skipping newlines
            }

            if (ch == PCG::FOREST_CHAR) {
                tileArray[y][x] = PCG::TileType::TILE_TYPE_FOREST;
            }
            else if (ch == PCG::PLAINS_CHAR) {
                tileArray[y][x] = PCG::TileType::TILE_TYPE_PLAINS;
            }
            else if (ch == PCG::SAND_CHAR){
                tileArray[y][x] = PCG::TileType::TILE_TYPE_SAND;
            }
            else if (ch == PCG::WATER_CHAR){
                tileArray[y][x] = PCG::TileType::TILE_TYPE_WATER;
            }
        }
    }
    //fclose(file); // old C-style file close
    file.close(); // Close C++ file stream
    // printf("Map loaded from %s\n", _filename);   // old C-style print statement
    std::cout << "Map loaded from " << _filename << std::endl; // C++ style print statement
}

// ============================================= 
// void PCG_SaveMapImage(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* filename)
// Store our tileMap data as a .png image, using the input filename.
// ============================================= 
void PCG::TileMap::SaveMapImage(const char* filename) const {
    Image mapImage = GenImageColor(MAP_COLUMNS, MAP_ROWS, BLACK);

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            Color c = PCG::TileMap::GetTileColor(tileArray[y][x]);
            ImageDrawPixel(&mapImage, x, y, c);
        }
    }
    if (ExportImage(mapImage, filename)) {
        printf("Image saved: %s\n", filename);
    }
    UnloadImage(mapImage);
}




// ============================================= 
// void PCG_DrawGUI(TileType tileArray[MAP_ROWS][MAP_COLUMNS])
// ============================================= 

void PCG::SliderQuickSetup(float offset, const char *sliderName, float *sliderValue, int min, int max) {

    Rectangle MainShape = { PCG::BUTTON_X, PCG::BUTTON_Y - 240 - offset, PCG::BUTTON_WIDTH, 20};
    Rectangle ValueBoxOffset = {PCG::BUTTON_X - 65, PCG::BUTTON_Y - 240 - offset, 20, 20};
    
    if (GuiSlider(MainShape, sliderName, NULL, sliderValue, min, max)) {

    }
    int rounded = std::round(*sliderValue);
          
    GuiValueBox(ValueBoxOffset, NULL, &rounded, 0, 10, false);

}



void PCG::TileMap::DrawGUI() {
    
    if (GuiButton(RESET_BUTTON_BOUNDS, "Reset Map")) {

        GetMapGenerator()->Generate(tileArray, false);
    }

    // Save Data Button
    Rectangle saveRect = { BUTTON_X, BUTTON_Y - 70, BUTTON_WIDTH, BUTTON_HEIGHT };
    if (GuiButton(saveRect, "Save Map Data")) {
        SaveMapData(MAP_TEXT_FILENAME);
    }
    
    Rectangle saveBulkRect = { BUTTON_X, BUTTON_Y - 320, BUTTON_WIDTH, BUTTON_HEIGHT };
    if (GuiButton(saveBulkRect, "Bulk Save Map Data")) {
        for (size_t i = 0; i < 60; i++)
        {
            PCG::bBulkSaving = true;
            PCG::BulkSaveIndex = 0;
        }
    }
    // Load Data Button
    Rectangle loadRect = { BUTTON_X, BUTTON_Y - 140, BUTTON_WIDTH, BUTTON_HEIGHT };
    if (GuiButton(loadRect, "Load Map Data")) {
        LoadMapData(MAP_TEXT_FILENAME);
    }

    // Save Image Button
    Rectangle imgRect = { BUTTON_X, BUTTON_Y - 210, BUTTON_WIDTH, BUTTON_HEIGHT };
    if (GuiButton(imgRect, "Save Map PNG")) {
        SaveMapImage(MAP_IMAGE_FILENAME);
    }

    PCG::SliderQuickSetup(80, "Forest", &PCG::settings.weights[TILE_TYPE_FOREST], 1, 100);
    PCG::SliderQuickSetup(120, "Plains", &PCG::settings.weights[TILE_TYPE_PLAINS], 1, 100);
    PCG::SliderQuickSetup(160, "Sand", &PCG::settings.weights[TILE_TYPE_SAND], 1, 100);
    PCG::SliderQuickSetup(200, "Water", &PCG::settings.weights[TILE_TYPE_WATER], 1, 100);
    PCG::SliderQuickSetup(280, "Scale", &PCG::NOISE_SCALE, 1, 8);

}

// =============================================
// SetMapGenerator and GetMapGenerator functions for our TileMap class, to allow us to assign a map generator to our tilemap, and retrieve it when we want to generate new maps.
// =============================================
void PCG::TileMap::SetMapGenerator(PCG::MapGenerator* generator) {
    mapGenerator = generator;
}

// =============================================
// GetMapGenerator returns a pointer to the current map generator assigned to this tilemap, so we can call its Generate function when we want to create new maps.
// =============================================
PCG::MapGenerator* PCG::TileMap::GetMapGenerator() const {
    return mapGenerator;
}


// =============================================
// MapGenerator
// =============================================
// As it is a pure virtual class, we don't need to implement anything here. The derived classes will provide the actual generation logic.


// Derived classes will implement the Generate function to create different types of maps.
// =============================================
// RandomMapGenerator
// =============================================
// Constructor
PCG::RandomMapGenerator::RandomMapGenerator() {
    // nothing to initialize for now, but you could seed a random generator here if you want reproducible maps
}

// Destructor
PCG::RandomMapGenerator::~RandomMapGenerator() {
    // nothing to clean up for now, but if you had allocated resources (like noise generators) you would release them here
}

void PCG::RandomMapGenerator::Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], bool randomiseWeights) {

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            _tileArray[y][x] = GetWeightedTile();
        }
    }
}



// =============================================
// NoiseGenerator
// =============================================
// Constructor
PCG::NoiseMapGenerator::NoiseMapGenerator() {
    // nothing to initialize for now, but you could seed a random noise here if you want reproducible maps
}

// Destructor
PCG::NoiseMapGenerator::~NoiseMapGenerator() {
    // nothing to clean up for now, but if you had allocated resources (like noise generators) you would release them here
}

void PCG::NoiseMapGenerator::Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], bool randomiseWeights) {
    // Random offsets make the map different every time
    int offsetX = GetRandomValue(0, 1000);
    int offsetY = GetRandomValue(0, 1000);
    float scale = 2.5f * NOISE_SCALE; //Noise scalar for little islands.
    float total = 0.0f;
    

    // Step 1: total tile weights
    for (int i = 0; i < TILE_COUNT; i++) {
        total += settings.weights[i];
    }

    if (randomiseWeights)
    {
        PCG::settings.weights[TILE_TYPE_FOREST] = GetRandomValue(0, 100);
        PCG::settings.weights[TILE_TYPE_PLAINS] = GetRandomValue(0, 100);
        PCG::settings.weights[TILE_TYPE_SAND] = GetRandomValue(0, 100);
        PCG::settings.weights[TILE_TYPE_WATER] = GetRandomValue(0, 100);
        PCG::NOISE_SCALE = GetRandomValue(1, 6);
    }

    // Raylib's Perlin Noise function
    Image noiseImg = GenImagePerlinNoise(MAP_COLUMNS, MAP_ROWS, offsetX, offsetY, scale);

    

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            // Read the brightness of the noise pixel
            Color col = GetImageColor(noiseImg, x, y);
            float brightness = (col.r + col.g + col.b) / (3.0f * 255.0f);

            // Step 2: map noise (0–1) into weight range
            float value = brightness * total;

            // Step 3: pick tile using cumulative weights
            float cumulative = 0.0f;

            for (int i = 0; i < TILE_COUNT; i++)
            {
                cumulative += settings.weights[i];

                if (value <= cumulative)
                {
                    _tileArray[y][x] = (TileType)i;
                    break;
                }
            }
        }
    }


    UnloadImage(noiseImg);
}


// Derived classes will implement the Generate function to create different types of maps.
// =============================================
// GameOfLifeGenerator
// =============================================
// Constructor
PCG::GameOfLifeGenerator::GameOfLifeGenerator() {
    // nothing to initialize for now, but you could seed a random generator here if you want reproducible maps
}

// Destructor
PCG::GameOfLifeGenerator::~GameOfLifeGenerator() {
    // nothing to clean up for now, but if you had allocated resources (like noise generators) you would release them here
}

void PCG::GameOfLifeGenerator::Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], bool randomiseWeights) {

  
}
