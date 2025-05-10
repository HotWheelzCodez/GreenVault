#include "page.hpp"

// Add Goals Page class - derives Page class
class AddGoalsPage : public Page
{
public:
  AddGoalsPage() = default;

  int Render(float windowWidth, float windowHeight, GUI::MouseState& mouseState) override;
  void Init(float windowWidth, float windowHeight) override;

private:
  void LoadData();
};
