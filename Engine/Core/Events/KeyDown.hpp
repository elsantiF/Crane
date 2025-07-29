#pragma once

#include "Base/Types.hpp"
#include "IEvent.hpp"

namespace Crane::Events {
  class KeyDownEvent : public IEvent {
  public:
    KeyDownEvent(u32 keyCode) : m_KeyCode(keyCode) {}

    u32 GetKeyCode() const {
      return m_KeyCode;
    }

  private:
    u32 m_KeyCode;
  };
}