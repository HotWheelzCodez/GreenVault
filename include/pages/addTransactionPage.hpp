#pragma once /* ADD_TRANSACTION_PAGE_HPP */

#include "page.hpp"

#include <sstream>

// Add Transaction Page class - derives Page class
class AddTransactionPage : public Page
{
public:
  AddTransactionPage() = default;
  AddTransactionPage(float windowWidth, float windowHeight);
  ~AddTransactionPage() = default;

  // Returns 0 if the page did not change, else returns the page number
  int Render(float windowWidth, float windowHeight, GUI::MouseState& mouseState) override;
  void Init(float windowWidth, float windowHeight) override;

private:
  void LoadData();

private:
  bool m_ReloadData = true;
};
