#include "PhysicsFactory.hpp"
#include "PhysicsWorld.hpp"

namespace Crane::Physics {
  b2BodyId PhysicsFactory::CreateBoxBody(PhysicsWorld &world, BodyConfig config, f32 ppm) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = static_cast<b2BodyType>(config.type);
    bodyDef.position = {config.x / ppm, config.y / ppm};
    b2BodyId bodyId = world.CreateBody(bodyDef);

    b2Polygon boxShape = b2MakeBox(config.width / 2 / ppm, config.height / 2 / ppm);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);

    return bodyId;
  }
}