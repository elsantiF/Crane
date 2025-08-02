#include "GravitySystem.hpp"
#include "CelestialBody.hpp"
#include "Scene/Components/Transform.hpp"
#include "Scene/World.hpp"

using namespace Crane;

constexpr double eps = 1e-10;
constexpr double G = 1000.0;

void GravitySystem::FixedUpdate(f64 deltaTime) {
  auto view = m_World.GetRegistry().view<CelestialBody, Scene::Components::Transform>();
  // Reset all accelerations to zero
  for (auto entity : view) {
    auto &body = view.get<CelestialBody>(entity);
    body.acceleration = {0.0, 0.0};
  }

  auto entities = Vector<entt::entity>(view.begin(), view.end());

  // First, calculate accelerations based on gravitational forces
  for (size_t i = 0; i < entities.size(); ++i) {
    auto [bodyA, transformA] = view.get<CelestialBody, Scene::Components::Transform>(entities[i]);
    for (size_t j = i + 1; j < entities.size(); ++j) {
      auto [bodyB, transformB] = view.get<CelestialBody, Scene::Components::Transform>(entities[j]);

      // Calculate the distance vector and its magnitude
      auto direction = transformB.transform.position - transformA.transform.position;
      auto distance = glm::length(direction);

      if (distance < eps) {
        continue; // Avoid division by zero
      }

      auto normalizedDirection = direction / distance;
      f64 force = (G * bodyA.mass * bodyB.mass) / (distance * distance);

      // Update accelerations based on the gravitational force
      bodyA.acceleration += normalizedDirection * static_cast<f32>(force / bodyA.mass);
      bodyB.acceleration -= normalizedDirection * static_cast<f32>(force / bodyB.mass);
    }
  }

  // Then, update positions based on velocities
  for (auto entity : view) {
    auto [body, transform] = view.get<CelestialBody, Scene::Components::Transform>(entity);

    body.velocity += body.acceleration * static_cast<f32>(deltaTime);
    transform.transform.position += body.velocity * static_cast<f32>(deltaTime);
  }
}