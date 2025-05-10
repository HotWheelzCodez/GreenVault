#include "../../include/pages/addAccountPage.hpp"

void AddAccountPage::Init(float windowWidth, float windowHeight) 
{
  // Load loggo
  Texture2D logo = LoadTexture("assets/images/vault.png");
  logo.width >>= 1;
  logo.height >>= 1;

  // Animation keyframes for the error display
  std::vector<KeyFrame> errorAnimationKeyframes = { 
    { { 0.5f*windowWidth-(0.25f*windowWidth), -0.12f*windowHeight, 0.5f*windowWidth, 0.1f*windowHeight }, WHITE, 0.0f, 0.0f },
    { { 0.5f*windowWidth-(0.25f*windowWidth), 0.11f*windowHeight, 0.5f*windowWidth, 0.1f*windowHeight }, WHITE, 0.0f, 0.25f },
  };

  // Error animation
  Animation errorAnimation = Animation(errorAnimationKeyframes);

  // Text styles
  GUI::TextStyle headerStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 150, GUI::TEXT_ALIGNMENT_CENTER, { 0 } };
  GUI::TextStyle accountTypeStyle = headerStyle;
  accountTypeStyle.textAlignment = GUI::TEXT_ALIGNMENT_LEFT;
  accountTypeStyle.fontSize = 50;

  // Button styles
  GUI::ButtonStyle backButtonStyle = { TRANSPARENT, WHITE, TRANSPARENT, TRANSPARENT, GREEN, TRANSPARENT, font, 75, GUI::TEXT_ALIGNMENT_CENTER, 0, 0, 0, false, 1 };
  Color alphaGreen = SEA_GREEN;
  alphaGreen.a = 100;
  GUI::ButtonStyle createAccountButtonStyle = { TRANSPARENT, WHITE, SEA_GREEN, alphaGreen, WHITE, GREEN, font, 25, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 0.1f, 0.2f, false, 1.05f };

  GUI::ButtonStyle errorStyle = { RED, WHITE, PINK, RED, WHITE, PINK, font, 30, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 2, 1.5f, false, 1 };
  GUI::Button errorButton = GUI::Button(Rectangle{ 0 }, errorStyle, "");

  // Dropdown style(s)
  GUI::DropdownStyle accountTypeSelection = { DARKGREEN, WHITE, GREEN, WHITE, font, 40, GUI::TEXT_ALIGNMENT_CENTER };

  // Input style(s)
  GUI::InputStyle accountNameStyle = { TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, GREEN, WHITE, BLUE, font, 20, 0.25f, 0.1f, 0.2f, false, false };

  // Text components
  GUI::Text header = GUI::Text(Rectangle{ 0, 0.2f*windowHeight, windowWidth, 0.1f*windowHeight }, headerStyle, "Create an Account");
  GUI::Text accountTypeText = GUI::Text(Rectangle{ 0.5f*windowWidth, 0.5f*windowHeight, 0.5f*windowWidth, 0.1f*windowHeight }, accountTypeStyle, "Account type: ");

  // Button components
  GUI::Button backButton = GUI::Button(Rectangle{ 0, 0, 0.1f*windowWidth, 0.1f*windowHeight }, backButtonStyle, "<- Back");
  GUI::Button createAccountButton = GUI::Button(Rectangle{ 0.5f*windowWidth-(0.25f*windowWidth), 0.75f*windowHeight, 0.5f*windowWidth, 0.05f*windowHeight }, createAccountButtonStyle, "Create Account!");

  // Input component(s)
  GUI::Input accountNameInput = GUI::Input(Rectangle{ 0.25f*windowWidth, 0.5f*windowHeight, 0.2f*windowWidth, 0.04f*windowHeight }, accountNameStyle, "Account name...");

  // Dropdown component(s)
  std::vector<std::string> accountTypes = { "Checking", "Savings" };
  GUI::Dropdown accountType = GUI::Dropdown(Rectangle{ 0.65f*windowWidth, 0.5f*windowHeight, 0.2f*windowWidth, 0.04f*windowHeight }, accountTypeSelection, accountTypes, windowWidth, windowHeight);

  // Set all components in GUI 'structure'
  textElements["header"] = header;
  textElements["accountType"] = accountTypeText;

  buttonElements["back"] = backButton;
  buttonElements["createAccount"] = createAccountButton;
  buttonElements["error"] = errorButton;

  inputElements["accountName"] = accountNameInput;

  dropdownElements["accountType"] = accountType;

  images["logo"] = logo;

  animations["error"] = errorAnimation;
  etc["errorMessage"] = "";
}

int AddAccountPage::Render(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  if (init)
  {
    Init(windowWidth, windowHeight);
    init = false;
  }

  // Render GUI 'structure' and process user input

  DrawTexture(images["logo"], (0.5f*windowWidth)-(images["logo"].width/2.0f), 0.05f*windowHeight, WHITE);

  textElements["header"].UpdateAndRender(mouseState);
  textElements["accountType"].UpdateAndRender(mouseState);

  inputElements["accountName"].UpdateAndRender(mouseState);
   
  dropdownElements["accountType"].UpdateAndRender(windowWidth, windowHeight, mouseState);

  if (buttonElements["createAccount"].UpdateAndRender(mouseState) || IsKeyPressed(KEY_ENTER))
  {
    const std::string name = inputElements["accountName"].GetText();

    // Input sanatization
    if (name.empty())
    {
      etc["errorMessage"] = "No account name given!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }

    // Add account 
    Database::AddAccount(name, dropdownElements["accountType"].GetSelectedOption());
    inputElements["accountName"].SetText("");
    inputElements["accountName"].SetSelected(false);

    DrawRectangle(0, 0, windowWidth, windowHeight, THEME_COLOR);
    DeInit();
    return 3; // Main page
  }

  if (buttonElements["back"].UpdateAndRender(mouseState))
  {
    DrawRectangle(0, 0, windowWidth, windowHeight, THEME_COLOR);
    DeInit();
    return 3; // Main page
  }

  // Display / animate error animation
  animations["error"].Animate();
  Rectangle errorAnimationBounds = animations["error"].GetCurrentBounds();

  buttonElements["error"].SetText(etc["errorMessage"]);
  buttonElements["error"].SetBounds(errorAnimationBounds);

  if (buttonElements["error"].UpdateAndRender(mouseState))
    animations["error"].Reset();

  return 0;
}
