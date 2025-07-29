#pragma once

#include "Base/Math/Vector.hpp"
#include "IEvent.hpp"

namespace Crane::Events {
  enum class MouseButton { Left, Right, Middle };

  class MouseClickEvent : public IEvent {
  public:
    MouseClickEvent(const Math::Vec2f &position, MouseButton button) : m_Position(position), m_Button(button) {}

    Math::Vec2f GetPosition() const {
      return m_Position;
    }

    MouseButton GetButton() const {
      return m_Button;
    }

  private:
    Math::Vec2f m_Position;
    MouseButton m_Button;
  };
}