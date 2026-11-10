#include <raylib.h>

class Player
{
  public:
  float x, y;
  int radius;
  int coins;
  double speed; // normal speed
  double diaSpeed; // diagonal speed

  void draw()
  {
    DrawCircle(x, y, radius, WHITE);
  }

  void update()
  {
    if(IsKeyDown(KEY_W) && IsKeyDown(KEY_A) && IsKeyDown(KEY_S) && IsKeyDown(KEY_D)) {}
    else if(IsKeyDown(KEY_W) && IsKeyDown(KEY_A) && IsKeyDown(KEY_D)) { y -= speed; }
    else if(IsKeyDown(KEY_W) && IsKeyDown(KEY_D) && IsKeyDown(KEY_S)) { x += speed; }
    else if(IsKeyDown(KEY_S) && IsKeyDown(KEY_D) && IsKeyDown(KEY_A)) { y += speed; }
    else if(IsKeyDown(KEY_W) && IsKeyDown(KEY_A) && IsKeyDown(KEY_S)) { x -= speed; }
    else if(IsKeyDown(KEY_W) && IsKeyDown(KEY_S)) {}
    else if(IsKeyDown(KEY_A) && IsKeyDown(KEY_D)) {}
    else if(IsKeyDown(KEY_W) && IsKeyDown(KEY_D)) { y -= diaSpeed; x += diaSpeed; }
    else if(IsKeyDown(KEY_D) && IsKeyDown(KEY_S)) { y += diaSpeed; x += diaSpeed; }
    else if(IsKeyDown(KEY_S) && IsKeyDown(KEY_A)) { y += diaSpeed; x -= diaSpeed; }
    else if(IsKeyDown(KEY_A) && IsKeyDown(KEY_W)) { y -= diaSpeed; x -= diaSpeed; }
    else if(IsKeyDown(KEY_W)) { y -= speed; }
    else if(IsKeyDown(KEY_A)) { x -= speed; }
    else if(IsKeyDown(KEY_S)) { y += speed; }
    else if(IsKeyDown(KEY_D)) { x += speed; }

    if(x < 0 + radius) { x = 0 + radius; }
    if(x > GetScreenWidth() - radius) { x = GetScreenWidth() - radius; }
    if(y < 0 + radius) { y = 0 + radius; }
    if(y > GetScreenHeight() - radius) { y = GetScreenHeight() - radius; }
  }

  void reset()
  {
    coins = 0;
    x = GetScreenWidth() / 2;
    y = GetScreenHeight() / 2;
  }
};


class Coin
{
  public:
  float x, y;
  float radius;
  bool collected;

  void draw()
  {
    DrawCircle(x, y, radius, YELLOW);
  }

  void update()
  {
    if(collected) { reset(); }
  }

  void reset()
  {
    collected = false;
    x = GetRandomValue(radius, GetScreenWidth() - radius);
    y = GetRandomValue(radius, GetScreenHeight() - radius);
  }
};

Player player;
Coin coin;

int main()
{
  constexpr int screen_width = 1280;
  constexpr int screen_height = 800;

  player.coins = 0;
  player.speed = 8;
  player.diaSpeed = player.speed * 0.7071;
  player.radius = 25;
  player.x = screen_width / 2;
  player.y = screen_height / 2;

  coin.radius = 15;
  coin.x = 0;
  coin.y = 0;
  coin.collected = true;

  InitWindow(screen_width, screen_height, "Coin Rush /");
  SetTargetFPS(60);
  bool pause = true;
  // titleScreen and timeOut are only possible if the game is paused
  bool titleScreen = true;
  bool timeOut = false;
  int high_score = 0;
  float time = 30;

  while(!WindowShouldClose())
  {
    if(pause)
    {
      if(titleScreen)
      {
        BeginDrawing();
          ClearBackground(BLACK);
          DrawText(TextFormat("C O I N   R U S H"), screen_width / 2 - 520, 120, 120, WHITE);
          DrawText(TextFormat("HIGH SCORE: %i", high_score), screen_width / 2 - 300, 320, 80, WHITE);
          DrawText(TextFormat("PRESS SPACE TO START"), screen_width / 2 - 400, 520, 60, WHITE);
        EndDrawing();

        if(IsKeyPressed(KEY_SPACE)) { time = 30; player.reset(); titleScreen = false; pause = false; }
      }
      else if(timeOut)
      {
        if(player.coins > high_score) { high_score = player.coins; }

        BeginDrawing();
          ClearBackground(BLACK);
          DrawText(TextFormat("TIME'S OUT!"), screen_width / 2 - 280, 80, 80, WHITE);
          DrawText(TextFormat("THIS GAME'S SCORE: %i", player.coins), screen_width / 2 - 480, 200, 80, WHITE);
          DrawText(TextFormat("HIGH SCORE: %i", high_score), screen_width / 2 - 300, 320, 80, WHITE);
          DrawText(TextFormat("PRESS SPACE FOR TITLESCREEN"), screen_width / 2 - 520, 520, 60, WHITE);
        EndDrawing();

        if(IsKeyPressed(KEY_SPACE)) { titleScreen = true; timeOut = false; }
      }
      else
      {
        BeginDrawing();
          ClearBackground(BLACK);
          DrawText(TextFormat("CURRENT SCORE: %i", player.coins), screen_width / 2 - 400, 80, 80, WHITE);
          DrawText(TextFormat("HIGH SCORE: %i", high_score), screen_width / 2 - 300, 200, 80, WHITE);
          DrawText(TextFormat("TIME LEFT: %.2f", time), screen_width / 2 - 350, 320, 80, WHITE);
          DrawText(TextFormat("PRESS SPACE TO RESUME"), screen_width / 2 - 400, 520, 60, WHITE);
        EndDrawing();

        if(IsKeyPressed(KEY_SPACE)) { pause = false; }
      }
    }
    else
    {
      player.update();
      coin.update();
      if(CheckCollisionCircles(Vector2{player.x, player.y}, player.radius, Vector2{coin.x, coin.y}, coin.radius))
      {
        coin.collected = true;
        player.coins++;
      }

      BeginDrawing();
        ClearBackground(BLACK);
        DrawText(TextFormat("%i", player.coins), screen_width - 140, 40, 80, WHITE);
        DrawText(TextFormat("%.2f", time), 40, 40, 80, WHITE);
        coin.draw();
        player.draw();
      EndDrawing();

      if(IsKeyPressed(KEY_SPACE)) { pause = true; }

      if(time > 0) { time -= GetFrameTime(); }
      if(time <= 0) { timeOut = true; pause = true; }
    }
  }

  CloseWindow();
  return 0;
}

// c++ coinRush.cpp $(pkg-config --libs --cflags raylib) -o coinRush