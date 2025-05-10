#include "../../include/pages/loginPage.hpp"
#include <memory>

void LoginPage::Init(float windowWidth, float windowHeight) 
{
  // Error animation keyframes
  std::vector<KeyFrame> loginErrorAnimationKeyframes = { 
    { { 0.5f*windowWidth-(0.25f*windowWidth), -0.12f*windowHeight, 0.5f*windowWidth, 0.1f*windowHeight }, WHITE, 0.0f, 0.0f },
    { { 0.5f*windowWidth-(0.25f*windowWidth), 0.11f*windowHeight, 0.5f*windowWidth, 0.1f*windowHeight }, WHITE, 0.0f, 0.25f },
  };

  // Error animation
  Animation loginErrorAnimation = Animation(loginErrorAnimationKeyframes);

  // Text style(s)
  GUI::TextStyle loginDisplaySubStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 300, GUI::TEXT_ALIGNMENT_CENTER, { 0.0f, 0.0f } };
  GUI::TextStyle loginDisplayStyle = { TRANSPARENT, SEA_GREEN, TRANSPARENT, SEA_GREEN, font, 300, GUI::TEXT_ALIGNMENT_CENTER, { 0.0f, 0.0f } };
  GUI::InputStyle loginUsernameStyle = { TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, GREEN, WHITE, BLUE, font, 20, 0.25f, 0.1f, 0.2f, false, false };
  GUI::InputStyle loginPasswordStyle = { TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, GREEN, WHITE, BLUE, font, 20, 0.25f, 0.1f, 0.2f, false, true };
  GUI::TextStyle loginHeaderStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 80, GUI::TEXT_ALIGNMENT_CENTER, { 0.0f, 0.0f } };

  // Button style(s)
  Color alphaGreen = SEA_GREEN;
  alphaGreen.a = 100;
  GUI::ButtonStyle loginSubmitStyle = { TRANSPARENT, WHITE, SEA_GREEN, alphaGreen, WHITE, GREEN, font, 25, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 0.1f, 0.2f, false, 1.05f };
  GUI::ButtonStyle loginCreateStyle = { TRANSPARENT, SEA_GREEN, TRANSPARENT, TRANSPARENT, SEA_GREEN, TRANSPARENT, font, 20, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 0.1f, 0.2f, false, 1 };
  GUI::ButtonStyle loginViewPasswordStyle = { TRANSPARENT, SEA_GREEN, TRANSPARENT, TRANSPARENT, SEA_GREEN, TRANSPARENT, font, 20, GUI::TEXT_ALIGNMENT_LEFT, 0, 0, 0, false, 1 };
  GUI::ButtonStyle loginErrorStyle = { RED, WHITE, PINK, RED, WHITE, PINK, font, 30, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 2, 1.5f, false, 1 };

  // Text component(s)
  GUI::Text loginDisplaySub = GUI::Text(Rectangle{ -0.22f*windowWidth, 0.2f*windowHeight, (float)windowWidth, (float)windowHeight }, loginDisplaySubStyle, "Green");
  GUI::Text loginDisplay = GUI::Text(Rectangle{ 0.2f*windowWidth, 0.2f*windowHeight, (float)windowWidth, (float)windowHeight }, loginDisplayStyle, "Vault");
  GUI::Text loginHeader = GUI::Text(Rectangle{ 0, 0.5f*windowHeight, (float)windowWidth, (float)windowHeight }, loginHeaderStyle, "Login");

  // Input component(s)
  GUI::Input loginUsername = GUI::Input(Rectangle{ 0.5f*windowWidth-(0.25f*windowWidth), 0.6f*windowHeight, 0.5f*windowWidth, 0.03f*windowHeight }, loginUsernameStyle, "Username...");
  GUI::Input loginPassword = GUI::Input(Rectangle{ 0.5f*windowWidth-(0.25f*windowWidth), 0.64f*windowHeight, 0.5f*windowWidth, 0.03f*windowHeight }, loginPasswordStyle, "Password...");

  // Button component(s)
  GUI::Button loginSubmit = GUI::Button(Rectangle{ 0.5f*windowWidth-(0.1f*windowWidth), 0.75f*windowHeight, 0.2f*windowWidth, 0.05f*windowHeight }, loginSubmitStyle, "Login");
  GUI::Button loginCreate = GUI::Button(Rectangle{ 0.5f*windowWidth-(0.1f*windowWidth), 0.8f*windowHeight, 0.2f*windowWidth, 0.022f*windowHeight }, loginCreateStyle, "Don't have an account? Create one!");
  GUI::Button loginViewPassword = GUI::Button(Rectangle{ 0.5f*windowWidth-(0.25f*windowWidth), 0.67f*windowHeight, 0.5f*windowWidth, 0.03f*windowHeight }, loginViewPasswordStyle, "Show password");
  GUI::Button loginError = GUI::Button(Rectangle{ 0 }, loginErrorStyle, "");

  // Load logo
  Texture2D loginIcon = LoadTexture("assets/images/vault.png");

  // Load all components into the GUI 'structure'
  textElements["displaySubTitle"] = loginDisplaySub;
  textElements["displayTitle"] = loginDisplay;
  textElements["header"] = loginHeader;

  inputElements["username"] = loginUsername;
  inputElements["password"] = loginPassword;

  buttonElements["submit"] = loginSubmit;
  buttonElements["create"] = loginCreate;
  buttonElements["viewPassword"] = loginViewPassword;
  buttonElements["error"] = loginError;

  animations["error"] = loginErrorAnimation;

  images["logo"] = loginIcon;

  etc["errorMessage"] = "";
}

int LoginPage::Render(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  if (init)
  {
    Init(windowWidth, windowHeight);
    init = false;
  }

  // Render GUI 'struture'

  textElements["displaySubTitle"].UpdateAndRender(mouseState);
  textElements["displayTitle"].UpdateAndRender(mouseState);
  textElements["header"].UpdateAndRender(mouseState);

  Vector2 position = { 0.5f*windowWidth, 0.27f*windowHeight };
  float scale = 0.5f;
  position.x -= (images["logo"].width*scale)/2;
  DrawTextureEx(images["logo"], position, 0, scale, WHITE);

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
  {
    inputElements["username"].SetSelected(false);
    inputElements["password"].SetSelected(false);
  }

  inputElements["username"].UpdateAndRender(mouseState);
  inputElements["password"].UpdateAndRender(mouseState);

  if (buttonElements["viewPassword"].UpdateAndRender(mouseState))
  {
    inputElements["password"].InverseShowPassword();
    if (inputElements["password"].IsHidden())
      buttonElements["viewPassword"].SetText("Show password");
    else
      buttonElements["viewPassword"].SetText("Hide password");
  }

  if (buttonElements["submit"].UpdateAndRender(mouseState) || IsKeyPressed(KEY_ENTER))
  {
    std::string usernameInput = inputElements["username"].GetText();
    std::string passwordInput = inputElements["password"].GetText();

    // Input santazation
    Util::Trim(usernameInput);
    Util::Trim(passwordInput);

    if (usernameInput.empty())
    {
      etc["errorMessage"] = "Username is empty!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }
    if (passwordInput.empty())
    {
      etc["errorMessage"] = "Password is empty!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }

    // Managing user status
    Database::UserStatus userStatus = Database::AuthUser(usernameInput, passwordInput);
    switch (userStatus)
    {
      case Database::UserStatus::NOT_EXIST:
        etc["errorMessage"] = "There is no user with that username!";
        animations["error"].Reset();
        animations["error"].Play();
        return 0;
      case Database::UserStatus::WRONG_PASSWORD:
        etc["errorMessage"] = "Password is incorrect!";
        animations["error"].Reset();
        animations["error"].Play();
        return 0;
      case Database::UserStatus::FAILURE:
        etc["errorMessage"] = "Failed to log you in! Try closing and re-opening the window.";
        animations["error"].Reset();
        animations["error"].Play();
        return 0;
      default:
        animations["error"].Reset();
        etc["errorMessage"] = "";

        DrawRectangle(0, 0, windowWidth, windowHeight, THEME_COLOR);
        DeInit();
        return 3; // Main page
    }
  }
  if (buttonElements["create"].UpdateAndRender(mouseState))
  {
    DeInit();
    return 2; // Signup page
  }

  // Display / play error animation
  animations["error"].Animate();
  Rectangle errorAnimationBounds = animations["error"].GetCurrentBounds();

  buttonElements["error"].SetText(etc["errorMessage"]);
  buttonElements["error"].SetBounds(errorAnimationBounds);

  if (buttonElements["error"].UpdateAndRender(mouseState))
    animations["error"].Reset();

  return 0;
}
