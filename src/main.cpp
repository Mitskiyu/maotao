#include <raylib.h>
#include <string>

constexpr int windowWidth = 1024;
constexpr int windowHeight = 768;
const std::string title = "Maotao";

//
// :main
//
int main()
{
    InitWindow(windowWidth, windowHeight, title.c_str());

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
};
