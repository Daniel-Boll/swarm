#include <raylib-cpp/raylib-cpp.hpp>
#include <swarm/manager/game.hpp>

constexpr uint16_t screen_width = 800;
constexpr uint16_t screen_height = 450;

int main(int argc, char* argv[]) {
  swarm::managers::Game* game = swarm::managers::Game::get();

  raylib::Window window(screen_width, screen_height, "C++ - Swarm");
  SetTargetFPS(144);
  SetConfigFlags(FLAG_MSAA_4X_HINT);

  game->init_systems();
  game->add_player();

  while (!window.ShouldClose()) {
    game->process();

    BeginDrawing();

    window.ClearBackground(raylib::Color::FromHSV(0, 0, 0));

    EndDrawing();
  }

  return 0;
}
