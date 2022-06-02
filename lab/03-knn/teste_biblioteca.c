#include "raylib.h"

int main(void)
{
    InitWindow(800, 450, "raylib [core] example - basic window");

    for (int i = 100; i < 600; i++){

        // while (!WindowShouldClose())
        // {
            BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawRectangleLines(100, 200, 600, 60, BLACK);
                DrawRectangle(200, 200, (int)i, 12, MAROON);
                
            EndDrawing();
        // }

    }

    // CloseWindow();

    return 0;
}