#include <iostream>
#include <raylib.h>
#include "button.hpp"

using namespace std;

int player_score = 49;
int cpu_score = 0;

class Ball{
public:
    float x, y;
    int speedX, speedY;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update(){
        x += speedX;
        y += speedY;

        if(y + radius >= GetScreenHeight() || y - radius <= 0){
            speedY *= -1;
        }
        // if(x + radius >= GetScreenHeight() || x - radius <= 0){
        //     speedX *= -1;
        // }
        if(x + radius >= GetScreenWidth()){ //cpu wins
            cpu_score++;
            ResetBall(-1);
        }
        if(x - radius <= 0){ //player wins
            player_score++;
            ResetBall(1);
        }
    }

    void ResetBall(int directionX){
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        speedX = 7 * directionX;
        speedY = 7 * (GetRandomValue(0,1));
    }
};

class Paddle{
protected:
    void LimitMovement(){
        if(y <= 0){
            y = 0;
        }
        if(y + height >=GetScreenHeight()){
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw(){
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update() {
        if(IsKeyDown(KEY_UP)) {
            y = y - speed;
        }
        if(IsKeyDown(KEY_DOWN)) {
            y = y + speed;
        }

        LimitMovement();
    }
};

class CpuPaddle: public Paddle{
public:
    void Update(int ballY) {
        if(y + height / 2 > ballY){
            y = y - speed;
        }
        if(y + height / 2 < ballY){
            y = y + speed;
        }

        LimitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

void RestartGame(){
    // Reset score
    player_score = 0;
    cpu_score = 0;

    // Reset ball
    ball.x = GetScreenWidth() / 2.0f;
    ball.y = GetScreenHeight() / 2.0f;
    ball.speedX = 7;
    ball.speedY = 7;

    // Reset player paddle
    player.y = GetScreenHeight() / 2.0f - player.height / 2.0f;

    // Reset CPU paddle
    cpu.y = GetScreenHeight() / 2.0f - cpu.height / 2.0f;
}

enum GameScreen {
    Menu,
    Gameplay,
    Gameover,
    Win
};


int main(){
    // Window
    const int width = 1280;
    const int height = 720;
    InitWindow(width, height, "Pong games");
    SetTargetFPS(60);

    GameScreen currentscene = Menu;

    Button startButton{"Graphics/start.png", {580, 320}, 2.0};
    Button exitButton{"Graphics/quit.png", {580, 400}, 2.0};
    bool exit = false;

    ball.radius = 20;
    ball.x = width / 2;
    ball.y = height / 2;
    ball.speedX = 7;
    ball.speedY = 7;

    player.width = 25;
    player.height = 125;
    player.x = width - player.width - 10;
    player.y = height / 2 - player.height / 2;
    player.speed = 6;

    cpu.width = 25;
    cpu.height = 125;
    cpu.x = 10;
    cpu.y = height / 2 - player.height / 2;
    cpu.speed = 6;

    while(WindowShouldClose() == false && exit == false) {
        Vector2 mouseposition = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        if(currentscene == Menu){
            if(startButton.IsPressed(mouseposition, mousePressed)){
                currentscene = Gameplay;
            }
            if(exitButton.IsPressed(mouseposition, mousePressed)){
                exit = true;
            }
        }

        if(currentscene == Win || currentscene == Gameover){
            if(IsKeyPressed(KEY_R)){
                RestartGame();
                currentscene = Gameplay;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);


        if(currentscene == Menu){
            DrawText("Pong 100", width / 2 - 150, height / 2 - 150, 80, WHITE);
            startButton.Draw();
            exitButton.Draw();
        }
        else if(currentscene == Gameplay){
            // updating
            ball.Update();
            player.Update();
            cpu.Update(ball.y);

            if(player_score >= 50){
                currentscene = Win;
            } else if(cpu_score >= 50){
                currentscene = Gameover;
            }

            // check collision
            if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
                ball.speedX *= -1;
            }

            if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
                ball.speedX *= -1;
            }

            // Drawing
            // DrawRectangle(width /2, 0, width /2, height, BLACK);
            // DrawCircle(width /2, height/2, 150, WHITE);
            DrawLine(width/2.0, 0, width/2, height, WHITE);
        
            ball.Draw();
            cpu.Draw();
            player.Draw();
            DrawText(TextFormat("%i", cpu_score), width/4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player_score), 3 * width/4 - 20, 20, 80, WHITE);

        }
        else if(currentscene == Win){
            DrawText("You win", width /2 - 160, height / 2 - 40, 60, WHITE);
            DrawText("Press R to Restart", width / 2 - 150, height / 2 + 40, 30, WHITE);
        }
        else if(currentscene == Gameover)
        {
            DrawText("Press R to Restart", width / 2 - 160, height / 2 + 40, 30, WHITE);
        }


        EndDrawing();
    }

    CloseWindow();
    return 0;
}