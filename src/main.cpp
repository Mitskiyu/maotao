#include <raylib.h>
#include <string>

constexpr int windowWidth = 1024;
constexpr int windowHeight = 768;
const std::string title = "Maotao";

//
// :menu
//
class Menu
{
  public:
    Menu()
        : // center
          centerX(windowWidth / 2), centerY(windowHeight / 2),

          // title
          titleFontSize(60), startFontSize(40),
          titlePos({float(centerX) - float(MeasureText(title.c_str(), titleFontSize)) / 2, 100.0f}),

          // start btn
          start("start"), startBtnWidth(300.0f), startBtnColor(DARKGRAY),
          startBtnRec({float(centerX) - startBtnWidth / 2, float(centerY) - 75.0f, startBtnWidth, 50.0f})
    {
    }

    void draw() const
    {
        // draw title
        DrawText(title.c_str(), titlePos.x, titlePos.y, titleFontSize, WHITE);

        // draw start btn
        DrawRectangleRec(startBtnRec, startBtnColor);

        // draw start text inside btn
        DrawText(start.c_str(), float(centerX) - float(MeasureText(start.c_str(), startFontSize)) / 2,
                 startBtnRec.y + (startBtnRec.height - startFontSize) / 2, startFontSize, WHITE);
    }

    void update()
    {
        Vector2 mousePos = GetMousePosition();

        if (CheckCollisionPointRec(mousePos, startBtnRec))
        {
            startBtnColor = GRAY;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // game start
            }
        }
        else
        {
            startBtnColor = DARKGRAY;
        }
    }

  private:
    const int centerX;
    const int centerY;
    const int titleFontSize;
    const int startFontSize;
    const float startBtnWidth;
    std::string start;
    Color startBtnColor;
    Vector2 titlePos;
    Rectangle startBtnRec;
};

//
// :main
//
int main()
{
    InitWindow(windowWidth, windowHeight, title.c_str());

    Menu menu;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);
        menu.update();
        menu.draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
};
