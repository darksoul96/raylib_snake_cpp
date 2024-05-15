#include <iostream>
#include <raylib.h>
#include <vector>

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
    std::vector<Vector2> body;
    std::vector<double> speed = std::vector<double>(2);

    Snake() {
        body = std::vector<Vector2>(3);
        body[0] = {4,4};
        body[1] = {5,4};
        body[2] = {6,4};
        speed = {0,0};
    }

    void DrawSnake() {
        for (Vector2 part : body) {
            DrawRectangle(part.x * CELL_SIZE, part.y * CELL_SIZE, CELL_SIZE-1, CELL_SIZE-1, dark_green);
        }
    }

    void MoveSnake() {
        for (int i=body.size()-1; i>0; --i) {
            body[i].x = body[i-1].x;
            body[i].y = body[i-1].y;
        }
        body[0].x += speed[0];
        body[0].y += speed[1];

        if (body[0].x > CELL_COUNT) {
            body[0].x = 0;
        } else if (body[0].x < 0) {
            body[0].x = CELL_COUNT;
        } else if (body[0].y > CELL_COUNT) {
            body[0].y = 0;
        } else if (body[0].y < 0) {
            body[0].y = CELL_COUNT;
        }
    }

};

int main () {

    InitWindow(CELL_SIZE*CELL_COUNT, CELL_SIZE*CELL_COUNT, "Retro Snake");
    SetTargetFPS(60);

    Snake snake = Snake();
    Food food = Food();
    while(!WindowShouldClose()) {
        WaitTime(0.1);
        BeginDrawing();
        ClearBackground(green_color);
        food.DrawFood();
        snake.DrawSnake();
        if (IsKeyDown(KEY_UP) && snake.speed[1]>=0) {
            snake.speed = {0,-1};
        } else if (IsKeyDown(KEY_RIGHT) && snake.speed[0]<=0) {
            snake.speed = {1,0};
        } else if (IsKeyDown(KEY_DOWN) && snake.speed[1]<=0) {
            snake.speed = {0,1};
        } else if (IsKeyDown(KEY_LEFT) && snake.speed[0]>=0) {
            snake.speed = {-1,0};
        }
        if (snake.speed != std::vector<double>{0,0}) {
            snake.MoveSnake();
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}