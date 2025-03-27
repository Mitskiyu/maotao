#include <raylib.h>
#include <string>
#include <vector>

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
// :grid
//
class Grid
{
  public:
    Grid(int rows, int cols)
        : rows(rows), cols(cols), cellSize((int)(float(windowHeight) * 0.9f / rows)),
          // calc offset to center grid
          offset({static_cast<float>((windowWidth - cols * cellSize)) / 2,
                  static_cast<float>((windowHeight - rows * cellSize)) / 2})
    {
        cells.resize(rows, std::vector<int>(cols, 0));
    }

    void draw() const
    {
        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                // calc cell to draw
                float x = offset.x + col * cellSize;
                float y = offset.y + row * cellSize;

                if (cells[row][col] == 1)
                {
                    DrawRectangle(x, y, cellSize + 1, cellSize + 1, GREEN);
                }

                DrawRectangleLinesEx({x, y, (float)cellSize, (float)cellSize}, 1, GRAY);
            }
        }
    }

    void update()
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePos = GetMousePosition();
            int col = (mousePos.x - offset.x) / cellSize;
            int row = (mousePos.y - offset.y) / cellSize;

            if (row >= 0 && row < rows && col >= 0 && col < cols)
            {
                // toggle state
                cells[row][col] = 1 - cells[row][col];
            }
        }
    }

  private:
    const int rows;
    const int cols;
    const int cellSize;
    Vector2 offset;
    std::vector<std::vector<int>> cells;
};

//
// :main
//
int main()
{
    InitWindow(windowWidth, windowHeight, title.c_str());

    Menu menu;
    Grid grid(16, 16);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        menu.update();
        menu.draw();

        grid.update();
        grid.draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
};
