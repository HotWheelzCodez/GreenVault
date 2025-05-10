#include <algorithm>
#include <iostream>
#include <iostream>
#include <memory>

#include "../include/core/gui.hpp"
#include "../include/core/database.hpp"

#include "../include/pages/loginPage.hpp"
#include "../include/pages/signupPage.hpp"
#include "../include/pages/mainPage.hpp"
#include "../include/pages/addAccountPage.hpp"
#include "../include/pages/addTransactionPage.hpp"
#include "../include/pages/addGoalsPage.hpp"
#include "../include/pages/addReportsPage.hpp"
#include "../include/pages/editTransactionPage.hpp"

// Main entry point
// ---------------------
int main(void)
{
  int windowWidth = 0;
  int windowHeight = 0;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(windowWidth, windowHeight, "Green Vault");
  MaximizeWindow();
  SetTargetFPS(60);

  windowWidth = GetScreenWidth();
  windowHeight = GetScreenHeight();

  // Setup some GUI initialization
  GUI::CalculateFontScale(windowWidth);
  GUI::MouseState mouseState = { 0 };

  // Load / create all pages (the elements are lazy-loaded)
  std::vector<std::unique_ptr<Page>> pages;;
  pages.push_back(std::make_unique<LoginPage>());
  pages.push_back(std::make_unique<SignupPage>());
  pages.push_back(std::make_unique<MainPage>());
  pages.push_back(std::make_unique<AddAccountPage>());
  pages.push_back(std::make_unique<AddTransactionPage>());
  pages.push_back(std::make_unique<AddGoalsPage>());
  pages.push_back(std::make_unique<AddReportsPage>());
  pages.push_back(std::make_unique<EditTransactionPage>());
  uint8_t currentPage = 1;

  Database::Init();

  while (!WindowShouldClose())
  {
    // Update window state
    windowWidth = GetScreenWidth();
    windowHeight = GetScreenHeight();

    // Update mouse state
    mouseState.position = GetMousePosition();
    mouseState.clicked = false;
    mouseState.cursor = MOUSE_CURSOR_DEFAULT;

    // Render the currently selected page
    BeginDrawing();
    ClearBackground(WHITE);
    DrawRectangleGradientEx({ 0, 0, (float)windowWidth, (float)windowHeight }, THEME_COLOR_LIGHT, THEME_COLOR, THEME_COLOR_LIGHT, THEME_COLOR_LIGHT);

    uint8_t page = pages[currentPage-1]->Render(windowWidth, windowHeight, mouseState);
    if (page)
      currentPage = page;

    SetMouseCursor(mouseState.cursor);

    EndDrawing();
  }

  // Cleanup
  Database::Close();
  CloseWindow();
}
