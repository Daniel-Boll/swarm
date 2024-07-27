#include <flecs.h>

#include <cstdint>
#include <raylib-cpp/raylib-cpp.hpp>
#include <swarm/components/movement.hpp>
#include <swarm/components/renderable.hpp>
#include <swarm/components/stats.hpp>
#include <swarm/systems/movement.hpp>
#include <swarm/systems/render.hpp>

constexpr uint16_t screen_width = 800;
constexpr uint16_t screen_height = 450;

int main(int argc, char** argv) {
  flecs::world ecs;
  raylib::Window window(screen_width, screen_height, "C++ - Swarm");

  SetTargetFPS(60);

  ecs.system<swarm::components::Movement, const swarm::components::Stats>().each(
      swarm::systems::movement);
  ecs.system<const swarm::components::Renderable, const swarm::components::Movement>().each(
      swarm::systems::render);

  auto player = ecs.entity()
                    .set<swarm::components::Movement>({{400, 300}, {0, 0}})
                    .set<swarm::components::Renderable>({raylib::Color::Red(), 10})
                    .set<swarm::components::Stats>({100, 100, 10, 200, 0, 1, 1, 0.05, 1, 1});

  while (!window.ShouldClose()) {
    ecs.progress();

    BeginDrawing();

    window.ClearBackground(raylib::Color::RayWhite());

    EndDrawing();
  }

  return 0;
}
