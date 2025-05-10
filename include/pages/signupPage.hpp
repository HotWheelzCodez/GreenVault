#pragma once /* SIGNUP_PAGE_HPP */

#include "page.hpp"

// Forward delcartion
class LoginPage;
class MainPage;

// Signup Page class - derives Page class
class SignupPage : public Page
{
public:
  SignupPage() = default;
  SignupPage(float windowWidth, float windowHeight);
  ~SignupPage() = default;

  // Returns 0 if the page did not change, else return the page number
  int Render(float windowWidth, float windowHeight, GUI::MouseState& mouseState) override;
  void Init(float windowWidth, float windowHeight) override;
};
