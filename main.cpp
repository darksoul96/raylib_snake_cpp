#include <iostream>
#include <raylib.h>

Color green_color = {173, 204, 96, 255};
Color dark_green = {43, 51, 24, 255};

const int CELL_SIZE = 30;
const int CELL_COUNT = 25;

class Food {
public:
    Vector2 position;
    Texture2D texture;

    Food() {
        Image image = LoadImage("assets/avocado.png");
        ImageResize(&image, CELL_SIZE, CELL_SIZE);
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos();
    }

    ~Food() {
        UnloadTexture(texture);
    }

    Vector2 GenerateRandomPos() {
        float x = GetRandomValue(0, CELL_COUNT-1);
        float y = GetRandomValue(0, CELL_COUNT-1);
        return Vector2{x, y};
    }

    
    void DrawFood() {
        //DrawRectangle(position.x * CELL_SIZE, position.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, dark_green);
        DrawTexture(texture, position.x * CELL_SIZE, position.y * CELL_SIZE, WHITE);
    }
};

class Snake {
public:

};

int main () {

    InitWindow(CELL_SIZE*CELL_COUNT, CELL_SIZE*CELL_COUNT, "Retro Snake");
    SetTargetFPS(60);

    Food food = Food();
    while(!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(green_color);
        food.DrawFood();

        EndDrawing();
    }


    CloseWindow();

    return 0;
}