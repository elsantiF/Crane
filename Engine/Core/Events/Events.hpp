#pragma once

#include "Base/Math/Vector.hpp"
#include "Base/Types.hpp"
#include "IEvent.hpp"

namespace Crane::Events {
  class KeyDown : public IEvent {
  public:
    KeyDown(u32 keyCode) : m_KeyCode(keyCode) {}

    u32 GetKeyCode() const {
      return m_KeyCode;
    }

  private:
    u32 m_KeyCode;
  };

  class KeyUp : public IEvent {
  public:
    KeyUp(u32 keyCode) : m_KeyCode(keyCode) {}

    u32 GetKeyCode() const {
      return m_KeyCode;
    }

  private:
    u32 m_KeyCode;
  };

  enum class MouseButton { Left, Right, Middle };

  class MouseClick : public IEvent {
  public:
    MouseClick(const Math::Vec2f &position, MouseButton button) : m_Position(position), m_Button(button) {}

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