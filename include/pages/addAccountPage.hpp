#pragma once /* ADD_ACCONT_PAGE_HPP */

#include "page.hpp"

// Add Account Page class - derives Page class
class AddAccountPage : public Page
{
public:
  AddAccountPage() = default;
  AddAccountPage(float windowWidth, float windowHeight);
  ~AddAccountPage() = default;

  // Returns 0 if the page diid not change, else return the page number
  int Render(float windowWidth, float windowHeight, GUI::MouseState& mouseState) override;

  void Init(float windowWidth, float windowHeight) override;
};
