#include "page.hpp"

#include "../vendor/tinyfiledialogs.h"

// Add Reports Page class - derives Page class
class AddReportsPage : public Page
{
public:
  AddReportsPage() = default;

  int Render(float windowWidth, float windowHeight, GUI::MouseState& mouseState) override;
  void Init(float windowWidth, float windowHeight) override;
};
