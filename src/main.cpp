#include <flecs.h>

#include <cstdint>
#include <print>
#include <raylib-cpp/raylib-cpp.hpp>
#include <swarm/components/movement.hpp>
#include <swarm/components/renderable.hpp>
#include <swarm/components/stats.hpp>
#include <swarm/manager/game.hpp>
#include <swarm/systems/movement.hpp>
#include <swarm/systems/render.hpp>

constexpr uint16_t screen_width = 800;
constexpr uint16_t screen_height = 450;

int main(int argc, char** argv) {
  swarm::managers::Game game;

  raylib::Window window(screen_width, screen_height, "C++ - Swarm");
  SetTargetFPS(144);

  game.init_systems();
  game.add_player();

  while (!window.ShouldClose()) {
    game.process();

    BeginDrawing();

    window.ClearBackground(raylib::Color::FromHSV(0, 0, 0));

    EndDrawing();
  }

  return 0;
}
