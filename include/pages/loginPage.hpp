#pragma once /* LOGIN_PAGE_HPP */

#include "page.hpp"

// Login Page class - derives Page class
class LoginPage : public Page
{
public:
  LoginPage() = default;
  LoginPage(float windowWidth, float windowHeight);
  ~LoginPage() = default;

  // Return 0 if the page did not change, else return the page number
  int Render(float windowWidth, float windowHeight, GUI::MouseState& mouseState) override;
  void Init(float windowWidth, float windowHeight) override;
};
