#include "raylib.h"
#include "resource_dir.h"
#include "PCG.h" // Import our new module
#include "raygui.h"



    // Create the tile map
    PCG::TileMap tileMap;
int main() {

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(PCG::SCREEN_WIDTH, PCG::SCREEN_HEIGHT, "Construct Map Editor");



    tileMap.SetMapGenerator(new PCG::NoiseMapGenerator());
    tileMap.GetMapGenerator()->Generate(tileMap.tileArray); // Generate the map using the selected generator

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

        if (IsKeyDown(KEY_R)) // Fast map previewing PHOTOSENSITIVE SEIZURE WARNING
        {
            tileMap.GetMapGenerator()->Generate(tileMap.tileArray);
        }


    }
    CloseWindow();
    return 0;
}