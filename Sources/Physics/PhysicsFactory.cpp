#include "PhysicsFactory.hpp"
#include "PhysicsWorld.hpp"

namespace Crane::Physics {
  Pair<Components::Rigidbody, Components::BoxCollider> PhysicsFactory::CreateBoxBody(PhysicsWorld &world, BodyConfig config, f32 ppm) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = static_cast<b2BodyType>(config.type);
    bodyDef.position = {config.x / ppm, config.y / ppm};
    b2BodyId bodyId = world.CreateBody(bodyDef);

    f32 width = config.width / ppm;
    f32 height = config.height / ppm;
    b2Polygon boxShape = b2MakeBox(width / 2, height / 2);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);

    return std::make_pair(Components::Rigidbody(bodyId), Components::BoxCollider(Math::Vec2f{width, height}, shapeId));
  }
}