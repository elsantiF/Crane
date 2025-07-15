#include "Core/Application.hpp"
#include <imgui.h>
#include <iostream>

class Demo : public Crane::Core::Application {
protected:
  void OnInitialize() override { std::cout << "Demo Application Initialized" << std::endl; }
  void OnPreFixedUpdate() override {}
  void OnPostFixedUpdate() override {}
  void OnPreUpdate() override {}
  void OnPostUpdate() override {}
  void OnPreRender() override {}
  void OnPostRender() override {}
  void OnImGui() override {}
};

int main() {
  Demo demo;
  if (!demo.Initialize()) {
    std::cerr << "Failed to initialize Crane." << std::endl;
    return -1;
  }

  demo.Run();
  return 0;
}