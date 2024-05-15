#include <iostream>
#include <raylib.h>
#include <vector>
#include <algorithm>

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
    }

    ~Food() {
        UnloadTexture(texture);
    }

    Vector2 GenerateRandomPos() {
        float x = GetRandomValue(0, CELL_COUNT-1);
        float y = GetRandomValue(0, CELL_COUNT-1);
        return Vector2{x, y};
    }

    void CalculateNewPosition(std::vector<Vector2> snake_body) {
        Vector2 new_position;
        bool is_unique_pos = false;
        while (!is_unique_pos) {
            new_position = GenerateRandomPos();
            auto is_a_match = [new_position](Vector2 elem) {
                    return (elem.x == new_position.x) && (elem.y == new_position.y);
            };
            if (std::none_of(snake_body.cbegin(), snake_body.cend(), is_a_match)) {
                is_unique_pos = true;
            }
        }
        position = new_position;
    }

    
    void DrawFood() {
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

    bool DetectFoodEaten(Food food) {
        auto [food_x, food_y] = food.position;
        if (body[0].x == food_x && body[0].y == food_y)
            return true;
        else
            return false;
    }

    bool DetectCollision() {
        Vector2 head_position = body[0];
        auto do_not_match = [head_position](Vector2 elem) {
            return (elem.x != head_position.x) && (elem.y != head_position.y);
        };
        if (std::all_of(body.cbegin()+1, body.cend(), do_not_match)) {
            return true;
        }
        return false;
    }



};

int main () {

    InitWindow(CELL_SIZE*CELL_COUNT, CELL_SIZE*CELL_COUNT, "Retro Snake");
    SetTargetFPS(60);
    bool lose_menu = false;

    Snake snake = Snake();
    Food food = Food();
    food.CalculateNewPosition(snake.body);
    while(!WindowShouldClose()) {
        WaitTime(0.1);
        BeginDrawing();
        if (!lose_menu) {
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
                if (snake.DetectFoodEaten(food)) {
                    food.CalculateNewPosition(snake.body);
                }

                if (snake.DetectCollision()) {
                    lose_menu = true;
                }
            }
        } else if (lose_menu) {
            DrawText("Game Over", 190, 200, 50, LIGHTGRAY);
        }
        
        EndDrawing();
    }
    CloseWindow();

    return 0;
}