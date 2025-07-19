#pragma once

namespace Crane::Events {
  class IEvent {
  public:
    virtual ~IEvent() = default;
  };
}