#include "../../include/pages/signupPage.hpp"

void SignupPage::Init(float windowWidth, float windowHeight) 
{
  // Error animation keyframes
  std::vector<KeyFrame> signupErrorAnimationKeyframes = { 
    { { 0.5f*windowWidth-(0.25f*windowWidth), -0.12f*windowHeight, 0.5f*windowWidth, 0.1f*windowHeight }, WHITE, 0.0f, 0.0f },
    { { 0.5f*windowWidth-(0.25f*windowWidth), 0.11f*windowHeight, 0.5f*windowWidth, 0.1f*windowHeight }, WHITE, 0.0f, 0.25f },
  };

  // Error animation
  Animation signupErrorAnimation = Animation(signupErrorAnimationKeyframes);

  // Text style(s)
  GUI::TextStyle signupDisplaySubStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 300, GUI::TEXT_ALIGNMENT_CENTER, { 0.0f, 0.0f } };
  GUI::TextStyle signupDisplayStyle = { TRANSPARENT, SEA_GREEN, TRANSPARENT, SEA_GREEN, font, 300, GUI::TEXT_ALIGNMENT_CENTER, { 0.0f, 0.0f } };
  GUI::TextStyle signupHeaderStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 80, GUI::TEXT_ALIGNMENT_CENTER, { 0.0f, 0.0f } };

  // Input style(s)
  GUI::InputStyle signupInputStyle = { TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, GREEN, WHITE, BLUE, font, 20, 0.25f, 0.1f, 0.2f, false, false };
  GUI::InputStyle signupInputStylePassword = { TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, GREEN, WHITE, BLUE, font, 20, 0.25f, 0.1f, 0.2f, false, true };

  // Button style(s)
  Color alphaGreen = SEA_GREEN;
  alphaGreen.a = 100;
  GUI::ButtonStyle signupSubmitStyle = { TRANSPARENT, WHITE, SEA_GREEN, alphaGreen, WHITE, GREEN, font, 25, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 0.1f, 0.2f, false, 1.05f };
  GUI::ButtonStyle signupErrorStyle = { RED, WHITE, PINK, RED, WHITE, PINK, font, 30, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 2, 1.5f, false, 1 };
  GUI::ButtonStyle signupViewPasswordStyle = { TRANSPARENT, SEA_GREEN, TRANSPARENT, TRANSPARENT, SEA_GREEN, TRANSPARENT, font, 20, GUI::TEXT_ALIGNMENT_LEFT, 0, 0, 0, false, 1 };
  GUI::ButtonStyle signupReturnStyle = { TRANSPARENT, SEA_GREEN, TRANSPARENT, TRANSPARENT, SEA_GREEN, TRANSPARENT, font, 20, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 0.1f, 0.2f, false, 1 };

  // Text component(s)
  GUI::Text signupDisplaySub = GUI::Text(Rectangle{ -0.22f*windowWidth, 0.2f*windowHeight, (float)windowWidth, (float)windowHeight }, signupDisplaySubStyle, "Green");
  GUI::Text signupDisplay = GUI::Text(Rectangle{ 0.18f*windowWidth, 0.2f*windowHeight, (float)windowWidth, (float)windowHeight }, signupDisplayStyle, "Vault");
  GUI::Text signupHeader = GUI::Text(Rectangle{ 0, 0.5f*windowHeight, (float)windowWidth, (float)windowHeight }, signupHeaderStyle, "Signup");

  // Input component(s)
  GUI::Input signupFirstName = GUI::Input(Rectangle{ 0.49f*windowWidth-(0.25f*windowWidth), 0.6f*windowHeight, 0.25f*windowWidth, 0.03f*windowHeight }, signupInputStyle, "First Name...");
  GUI::Input signupLastName = GUI::Input(Rectangle{ 0.76f*windowWidth-(0.25f*windowWidth), 0.6f*windowHeight, 0.25f*windowWidth, 0.03f*windowHeight }, signupInputStyle, "Last Name...");
  GUI::Input signupUsername = GUI::Input(Rectangle{ 0.49f*windowWidth-(0.25f*windowWidth), 0.65f*windowHeight, 0.25f*windowWidth, 0.03f*windowHeight }, signupInputStyle, "Username...");
  GUI::Input signupEmail = GUI::Input(Rectangle{ 0.76f*windowWidth-(0.25f*windowWidth), 0.65f*windowHeight, 0.25f*windowWidth, 0.03f*windowHeight }, signupInputStyle, "Email (only used for account recover)...");
  GUI::Input signupPassword = GUI::Input(Rectangle{ 0.49f*windowWidth-(0.25f*windowWidth), 0.7f*windowHeight, 0.25f*windowWidth, 0.03f*windowHeight }, signupInputStylePassword, "Password...");
  GUI::Input signupPasswordAgain = GUI::Input(Rectangle{ 0.76f*windowWidth-(0.25f*windowWidth), 0.7f*windowHeight, 0.25f*windowWidth, 0.03f*windowHeight }, signupInputStylePassword, "Password again...");

  // Button component(s)
  GUI::Button signupSubmit = GUI::Button(Rectangle{ 0.5f*windowWidth-(0.1f*windowWidth), 0.8f*windowHeight, 0.2f*windowWidth, 0.05f*windowHeight }, signupSubmitStyle, "Create Account");
  GUI::Button signupError = GUI::Button(Rectangle{ 0 }, signupErrorStyle, "");
  GUI::Button signupViewPassword = GUI::Button(Rectangle{ 0.49f*windowWidth-(0.25f*windowWidth), 0.73f*windowHeight, 0.2f*windowWidth, 0.03f*windowHeight }, signupViewPasswordStyle, "Show password");
  GUI::Button signupViewPasswordAgain = GUI::Button(Rectangle{ 0.76f*windowWidth-(0.25f*windowWidth), 0.73f*windowHeight, 0.2f*windowWidth, 0.03f*windowHeight }, signupViewPasswordStyle, "Show password");
  GUI::Button signupReturn = GUI::Button(Rectangle{ 0.5f*windowWidth-(0.1f*windowWidth), 0.85f*windowHeight, 0.2f*windowWidth, 0.022f*windowHeight }, signupReturnStyle, "Return to login");

  // Load logo
  Texture2D signupIcon = LoadTexture("assets/images/vault.png");

  // Load all compoenents into GUI 'structure'
  textElements["displaySub"] = signupDisplaySub;
  textElements["display"] = signupDisplay;
  textElements["header"] = signupHeader;

  inputElements["firstName"] = signupFirstName;
  inputElements["lastName"] = signupLastName;
  inputElements["username"] = signupUsername;
  inputElements["email"] = signupEmail;
  inputElements["password"] = signupPassword;
  inputElements["passwordAgain"] = signupPasswordAgain;

  buttonElements["submit"] = signupSubmit;
  buttonElements["return"] = signupReturn;
  buttonElements["error"] = signupError;
  buttonElements["viewPassword"] = signupViewPassword;
  buttonElements["viewPasswordAgain"] = signupViewPasswordAgain;

  animations["error"] = signupErrorAnimation;

  images["logo"] = signupIcon;

  etc["errorMessage"] = "";
}

int SignupPage::Render(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  if (init)
  {
    Init(windowWidth, windowHeight);
    init = false;
  }

  // Render GUI 'structure'

  textElements["displaySub"].UpdateAndRender(mouseState);
  textElements["display"].UpdateAndRender(mouseState);
  textElements["header"].UpdateAndRender(mouseState);

  Vector2 position = { 0.5f*windowWidth, 0.27f*windowHeight };
  float scale = 0.5f;
  position.x -= (images["logo"].width*scale)/2;
  if (images["logo"].id) 
    DrawTextureEx(images["logo"], position, 0, scale, WHITE);

  // Unselect all input boxes when mouse pressed
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
  {
    for (auto it = inputElements.begin(); it != inputElements.end(); it++)
      it->second.SetSelected(false);
  }

  inputElements["username"].UpdateAndRender(mouseState);
  inputElements["firstName"].UpdateAndRender(mouseState);
  inputElements["lastName"].UpdateAndRender(mouseState);
  inputElements["email"].UpdateAndRender(mouseState);
  inputElements["password"].UpdateAndRender(mouseState);
  inputElements["passwordAgain"].UpdateAndRender(mouseState);

  if (buttonElements["viewPassword"].UpdateAndRender(mouseState))
  {
    inputElements["password"].InverseShowPassword();
    if (inputElements["password"].IsHidden())
      buttonElements["viewPassword"].SetText("Show password");
    else
      buttonElements["viewPassword"].SetText("Hide password");
  }

  if (buttonElements["viewPasswordAgain"].UpdateAndRender(mouseState))
  {
    inputElements["passwordAgain"].InverseShowPassword();
    if (inputElements["passwordAgain"].IsHidden())
      buttonElements["viewPasswordAgain"].SetText("Show password");
    else
      buttonElements["viewPasswordAgain"].SetText("Hide password");
  }

  if (buttonElements["submit"].UpdateAndRender(mouseState) || IsKeyPressed(KEY_ENTER))
  {
    // Get input
    std::string firstNameInput = inputElements["firstName"].GetText();
    std::string lastNameInput = inputElements["lastName"].GetText();
    std::string usernameInput = inputElements["username"].GetText();
    std::string emailInput = inputElements["email"].GetText();
    std::string passwordInput = inputElements["password"].GetText();
    std::string passwordAgainInput = inputElements["passwordAgain"].GetText();

    // Sanatize input
    Util::Trim(firstNameInput);
    Util::Trim(lastNameInput);
    Util::Trim(usernameInput);
    Util::Trim(emailInput);
    Util::Trim(passwordInput);
    Util::Trim(passwordAgainInput);

    if (firstNameInput.empty())
    {
      etc["errorMessage"] = "First name empty!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }
    if (lastNameInput.empty())
    {
      etc["errorMessage"] = "Last name empty!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }
    if (usernameInput.empty())
    {
      etc["errorMessage"] = "Username empty!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }
    if (emailInput.empty())
    {
      etc["errorMessage"] = "Email empty!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }
    if (passwordInput.empty())
    {
      etc["errorMessage"] = "Password empty!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }
    if (passwordAgainInput.empty())
    {
      etc["errorMessage"] = "Password again empty!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }
    if (passwordInput != passwordAgainInput)
    {
      etc["errorMessage"] = "Passwords do not match!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }
    if (!Util::IsValidEmailFormat(emailInput))
    {
      etc["errorMessage"] = "Email format is invalid! Must be 'a@b.ccc'!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }

    // Mangae user status
    Database::UserStatus status = Database::AddUser({ inputElements["firstName"].GetText(), inputElements["lastName"].GetText(), inputElements["username"].GetText(), inputElements["email"].GetText(), inputElements["password"].GetText() });
    switch (status)
    {
      case Database::UserStatus::IS_YOU:
        etc["errorMessage"] = "We found local data of an account! Try logging in with your information.";
        animations["error"].Reset();
        animations["error"].Play();
        return 0;
      case Database::UserStatus::EXISTS:
        etc["errorMessage"] = "A user with that username already exists!";
        animations["error"].Reset();
        animations["error"].Play();
        return 0;
      case Database::UserStatus::FAILURE:
        etc["errorMessage"] = "Failed to create your account! Close and re-open the window, and try again.";
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

  // Display / play error animation
  animations["error"].Animate();
  Rectangle errorAnimationBounds = animations["error"].GetCurrentBounds();
  
  buttonElements["error"].SetText(etc["errorMessage"]);
  buttonElements["error"].SetBounds(errorAnimationBounds);

  if (buttonElements["return"].UpdateAndRender(mouseState))
  {
    DrawRectangle(0, 0, windowWidth, windowHeight, THEME_COLOR);
    DeInit();
    return 1; // Main page
  }

  if (buttonElements["error"].UpdateAndRender(mouseState))
    animations["error"].Reset();

  return 0;
}
