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
        //Image image = LoadImage("assets/avocado.png");
        //ImageResize(&image, CELL_SIZE, CELL_SIZE);
        //texture = LoadTextureFromImage(image);
        //UnloadImage(image);
    }

    ~Food() {
        //UnloadTexture(texture);
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
        DrawRectangle(position.x * CELL_SIZE, position.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, dark_green);
        //DrawTexture(texture, position.x * CELL_SIZE, position.y * CELL_SIZE, WHITE);
    }
};

class Snake {
public:
    std::vector<Vector2> body;
    std::vector<double> speed = std::vector<double>(2);
    int score = 0;

    Snake() {
        body = std::vector<Vector2>(3);
        body[0] = {4,4};
        body[1] = {5,4};
        body[2] = {6,4};
        speed = {0,0};
    }

    void Reset() {
        body.resize(3);
        body[0] = {4,4};
        body[1] = {5,4};
        body[2] = {6,4};
        speed = {0,0};
        score = 0;
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

        if (body[0].x >= CELL_COUNT) {
            body[0].x = 0;
        } else if (body[0].x < 0) {
            body[0].x = CELL_COUNT;
        }
        
        if (body[0].y >= CELL_COUNT) {
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

    void Grow() {
        Vector2 head = body[0];
        head.x += speed[0];
        head.y += speed[1];
        body.insert(body.begin(), head);
        score++;
    }

    bool DetectCollision() {
        Vector2 head_position = body[0];
        auto matches = [head_position](Vector2 elem) {
            return (elem.x == head_position.x) && (elem.y == head_position.y);
        };
        if (std::any_of(body.begin()+3, body.end(), matches)) {
            return true;
        }
        return false;
    }



};

int main () {

    InitWindow(CELL_SIZE*CELL_COUNT, CELL_SIZE*CELL_COUNT, "Snake Game by Agustin Ruiz");
    SetTargetFPS(90);
    bool start_menu = true;
    bool lose_menu = false;

    Snake snake = Snake();
    Food food = Food();
    food.CalculateNewPosition(snake.body);
    while(!WindowShouldClose()) {
        WaitTime(0.1);
        BeginDrawing();
        if (start_menu) {
            ClearBackground(green_color);
            DrawText("Snake Game", 190, 200, 50, BLACK);
            DrawText("Press ENTER to start game", 190, 320, 15, BLACK);
            if (IsKeyDown(KEY_ENTER)) {
                start_menu = false;
                snake.Reset();
                food.CalculateNewPosition(snake.body);
            }
        } else if (lose_menu) {
            DrawText("Game Over", 190, 200, 50, BLACK);
            DrawText(TextFormat("Score: %i", snake.score), 190, 270, 20, BLACK);
            DrawText("Press ENTER to reset", 190, 320, 15, BLACK);
            if (IsKeyDown(KEY_ENTER)) {
                lose_menu = false;
                snake.Reset();
                food.CalculateNewPosition(snake.body);
            }
        } else {
            ClearBackground(green_color);
            food.DrawFood();
            snake.DrawSnake();
            if (snake.speed == std::vector{0.0, 0.0}) {
                DrawText("Move with arrow keys", 190, 200, 30, BLACK);
            }
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
                    snake.Grow();
                }

                if (snake.DetectCollision()) {
                    lose_menu = true;
                }
            }
        }
        
        EndDrawing();
    }
    CloseWindow();

    return 0;
}