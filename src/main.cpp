#include "raylib.h"
#include "resource_dir.h"
#include "PCG.h" // Import our new module
#include "raygui.h"


int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(PCG::SCREEN_WIDTH, PCG::SCREEN_HEIGHT, "Construct Map Editor");

    // Create the tile map
    PCG::TileMap tileMap;

    tileMap.CreateMap(1);
    // Save text and image
    /*PCG_SaveMapData(tileArray, MAP_TEXT_FILENAME);
    PCG_SaveMapImage(tileArray, MAP_IMAGE_FILENAME);*/

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        //PCG::DrawMap(tileArray); // Function from PCG.c
        tileMap.DrawMap();
        DrawText("Construct Map Editor", 20, 20, 20, WHITE);
        //PCG::DrawGUI(tileArray);
        tileMap.DrawGUI();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}