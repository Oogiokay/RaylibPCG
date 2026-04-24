#include "PCG.h"
#include <stdio.h>
#include <fstream>
#include <iostream>


PCG::TileMap::TileMap() {
    // Initialise our tileMap array to all grass tiles by default when we create a new TileMap object. 
    // We can change this later using the CreateMap() function, or by setting individual tiles with SetTile().

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            tileArray[y][x] = TILE_TYPE_GRASS;
        }
    }
}


// =============================================
// Destructor for our TileMap class.
// =============================================
PCG::TileMap::~TileMap()
{


}



// ============================================= 
// void PCG_CreateMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
// ============================================= 
void PCG::TileMap::CreateMap(int weight) {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            int weightedChance = GetRandomValue(0, weight);

            if (weightedChance == 1)
            {
                tileArray[y][x] = (TileType)0;
            }
            else
            {
                tileArray[y][x] = (TileType)1;
            }
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
    case PCG::TileType::TILE_TYPE_GRASS: return GRASS_COLOR;
    case TILE_TYPE_ROCK: return ROCK_COLOR;
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
    case TILE_TYPE_GRASS: return GRASS_CHAR;
    case TILE_TYPE_ROCK: return ROCK_CHAR;
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

            if (ch == PCG::GRASS_CHAR) {
                tileArray[y][x] = PCG::TileType::TILE_TYPE_GRASS;
            }
            else if (ch == PCG::ROCK_CHAR) {
                tileArray[y][x] = PCG::TileType::TILE_TYPE_ROCK;
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


// Required to call Raylib gui buttons. Add this near the top of PCG.c
#define RAYGUI_IMPLEMENTATION
#include "raygui.h" 

// ============================================= 
// void PCG_DrawGUI(TileType tileArray[MAP_ROWS][MAP_COLUMNS])
// ============================================= 
void PCG::TileMap::DrawGUI() {
    // Reset Button
    if (GuiButton(RESET_BUTTON_BOUNDS, "Reset Map")) {
        CreateMap(1);
    }

    // Save Data Button
    Rectangle saveRect = { BUTTON_X, BUTTON_Y - 70, BUTTON_WIDTH, BUTTON_HEIGHT };
    if (GuiButton(saveRect, "Save Map Data")) {
        SaveMapData(MAP_TEXT_FILENAME);
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
}