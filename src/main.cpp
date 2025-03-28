#include <cstdlib>
#include <raylib.h>
#include <string>
#include <vector>

enum GameState
{
    MENU,
    GAME,
    PAUSED
};

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

    void update(GameState &currentState)
    {
        Vector2 mousePos = GetMousePosition();

        if (CheckCollisionPointRec(mousePos, startBtnRec))
        {
            startBtnColor = GRAY;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentState = GAME;
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

    int getRows() const
    {
        return rows;
    }

    int getCols() const
    {
        return cols;
    }

    int getCell(int row, int col) const
    {
        return cells[row][col];
    }

    void setCell(int row, int col, int val)
    {
        cells[row][col] = val;
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

    void randomize()
    {
        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                cells[row][col] = (rand() / (float)RAND_MAX) < 0.25f ? 1 : 0;
            }
        }
    }

    void clear()
    {
        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                cells[row][col] = 0;
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
// :sim
//
class Simulation
{
  public:
    Simulation(Grid &grid, GameState &currentState)
        : accumulator(0.0f), tickrate(0.1f), grid(grid), currentState(currentState),

          // possible neighbors
          neighbors({{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {1, 1}, {-1, -1}, {-1, 1}, {1, -1}})
    {
    }

    void update(float dt)
    {
        if (currentState != GAME)
        {
            return;
        }

        accumulator += dt;

        if (accumulator >= tickrate)
        {
            accumulator = 0.0f;

            // temp grid
            std::vector<std::vector<int>> tempGrid(grid.getRows(), std::vector<int>(grid.getCols(), 0));

            for (int row = 0; row < grid.getRows(); ++row)
            {
                for (int col = 0; col < grid.getCols(); ++col)
                {
                    int activeNeighbors = 0;

                    // determine active neighbors
                    for (const auto &[dr, dc] : neighbors)
                    {
                        int r = row + dr;
                        int c = col + dc;
                        if (r >= 0 && r < grid.getRows() && c >= 0 && c < grid.getCols())
                        {
                            activeNeighbors += grid.getCell(r, c);
                        }
                    }

                    int currentCell = grid.getCell(row, col);

                    if (currentCell == 1)
                    {
                        // any live cell with 2 or 3 live neighbors lives, otherwise die
                        tempGrid[row][col] = (activeNeighbors == 2 || activeNeighbors == 3) ? 1 : 0;
                    }
                    else
                    {
                        // any dead cell with exactly 3 neighbors becomes alive
                        tempGrid[row][col] = (activeNeighbors == 3) ? 1 : 0;
                    }
                }
            }

            // set cell status in grid
            for (int row = 0; row < grid.getRows(); ++row)
            {
                for (int col = 0; col < grid.getCols(); ++col)
                {
                    grid.setCell(row, col, tempGrid[row][col]);
                }
            }
        }
    }

  private:
    float accumulator;
    float tickrate;
    Grid &grid;
    GameState &currentState;
    std::vector<std::pair<int, int>> neighbors;
};

//
// :main
//
int main()
{
    InitWindow(windowWidth, windowHeight, title.c_str());
    SetTargetFPS(120);

    // track game state
    GameState currentState = MENU;

    Menu menu;
    Grid grid(32, 32);
    Simulation sim(grid, currentState);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        switch (currentState)
        {
        case MENU:
            menu.update(currentState);
            menu.draw();
            break;
        case GAME:
            sim.update(GetFrameTime());
            grid.update();
            grid.draw();

            if (IsKeyPressed(KEY_SPACE))
                currentState = (currentState == GAME) ? PAUSED : GAME;

            if (IsKeyPressed(KEY_R))
                grid.randomize();

            if (IsKeyPressed(KEY_C))
                grid.clear();

            break;
        case PAUSED:
            grid.update();
            grid.draw();

            if (IsKeyPressed(KEY_SPACE))
                currentState = GAME;

            break;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
};
