#include "../../include/pages/addGoalsPage.hpp"
#include <memory>

void AddGoalsPage::Init(float windowWidth, float windowHeight)
{
  // Load logo
  Texture2D logo = LoadTexture("assets/images/vault.png");
  logo.width >>= 1;
  logo.height >>= 1;

  // Animation error keyframes
  std::vector<KeyFrame> errorAnimationKeyframes = { 
    { { 0.5f*windowWidth-(0.25f*windowWidth), -0.12f*windowHeight, 0.5f*windowWidth, 0.1f*windowHeight }, WHITE, 0.0f, 0.0f },
    { { 0.5f*windowWidth-(0.25f*windowWidth), 0.11f*windowHeight, 0.5f*windowWidth, 0.1f*windowHeight }, WHITE, 0.0f, 0.25f },
  };

  // Animation error
  Animation errorAnimation = Animation(errorAnimationKeyframes);

  // Text styles
  GUI::TextStyle headerStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 110, GUI::TEXT_ALIGNMENT_CENTER, { 0 } };

  GUI::TextStyle goalPrioityTextStyle = headerStyle;
  goalPrioityTextStyle.fontSize = 25.0f;
  goalPrioityTextStyle.textAlignment = GUI::TEXT_ALIGNMENT_LEFT;

  // Input style(s)
  GUI::InputStyle itemNameStyle = { TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, GREEN, WHITE, BLUE, font, 30, 0.25f, 0.1f, 0.2f, false, false };

  // Dropdown style(s)
  GUI::DropdownStyle goalPrioityStyle = { DARKGREEN, WHITE, GREEN, WHITE, font, 40, GUI::TEXT_ALIGNMENT_CENTER };

  // Dropdown style(s)
  Color alphaGreen = SEA_GREEN;
  alphaGreen.a = 100;
  GUI::ButtonStyle submitButtonStyle = { TRANSPARENT, WHITE, SEA_GREEN, alphaGreen, WHITE, GREEN, font, 25, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 0.1f, 0.2f, false, 1.05f };

  GUI::ButtonStyle backButtonStyle = { TRANSPARENT, WHITE, TRANSPARENT, TRANSPARENT, GREEN, TRANSPARENT, font, 75, GUI::TEXT_ALIGNMENT_CENTER, 0, 0, 0, false, 1 };

  // Text component(s)
  GUI::Text header = GUI::Text(Rectangle{ 0, 0.15f*windowHeight, windowWidth, 0.1f*windowHeight }, headerStyle, "Create a Goal");

  // Button component(s)
  GUI::Button submitButton = GUI::Button(Rectangle{ 0.4f*windowWidth, 0.75f*windowHeight, 0.3f*windowWidth, 0.05f*windowHeight }, submitButtonStyle, "Add Goal");

  GUI::Button backButton = GUI::Button(Rectangle{ 0, 0, 0.1f*windowWidth, 0.1f*windowHeight }, backButtonStyle, "<- Back");

  GUI::ButtonStyle errorStyle = { RED, WHITE, PINK, RED, WHITE, PINK, font, 30, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 2, 1.5f, false, 1 };
  GUI::Button errorButton = GUI::Button(Rectangle{ 0 }, errorStyle, "");

  // Input component(s)
  GUI::Input itemNameInput = GUI::Input(Rectangle{ 0.15f*windowWidth, 0.25f*windowHeight, 0.38f*windowWidth, 0.05f*windowHeight }, itemNameStyle, "Goal name...");
  GUI::Input itemAmountInput = GUI::Input(Rectangle{ 0.58f*windowWidth, 0.25f*windowHeight, 0.38f*windowWidth, 0.05f*windowHeight }, itemNameStyle, "Goal amount...");

  GUI::Input itemInterestPeriodInput = GUI::Input(Rectangle{ 0.15f*windowWidth, 0.35f*windowHeight, 0.38f*windowWidth, 0.05f*windowHeight }, itemNameStyle, "Interest period in weeks (Optional)...");
  GUI::Input itemInterestRateInput = GUI::Input(Rectangle{ 0.58f*windowWidth, 0.35f*windowHeight, 0.38f*windowWidth, 0.05f*windowHeight }, itemNameStyle, "Interest rate (Optional)...");

  GUI::Input itemIntialSavingsInput = GUI::Input(Rectangle{ 0.15f*windowWidth, 0.45f*windowHeight, 0.38f*windowWidth, 0.05f*windowHeight }, itemNameStyle, "Initial savings / Down payment (Optional)...");

  // Input component(s)
  GUI::Text prioityText = GUI::Text(Rectangle{ 0.58f*windowWidth, 0.45f*windowHeight, 0.19f*windowWidth, 0.05f*windowHeight }, goalPrioityTextStyle, "Goal prioity: ");
  const std::vector<std::string> priorityOptions = { "High", "Medium", "Low" };
  GUI::Dropdown goalPrioityDropdown = GUI::Dropdown(Rectangle{ 0.7f*windowWidth, 0.45f*windowHeight, 0.24f*windowWidth, 0.05f*windowHeight }, goalPrioityStyle, priorityOptions, windowWidth, windowHeight);

  GUI::Input yearsToCompleteInput = GUI::Input(Rectangle{ 0.15f*windowWidth, 0.55f*windowHeight, 0.38f*windowWidth, 0.05f*windowHeight }, itemNameStyle, "Years to complete (defaults to 0)...");

  GUI::Input monthsToCompleteInput = GUI::Input(Rectangle{ 0.58f*windowWidth, 0.55f*windowHeight, 0.38f*windowWidth, 0.05f*windowHeight }, itemNameStyle, "Months to complete (defaults to 1)...");

  // Set all components in the GUI 'structure'
  textElements["savingsHeader"] = header;
  textElements["prioityText"] = prioityText;

  buttonElements["submit"] = submitButton;
  buttonElements["back"] = backButton;
  buttonElements["error"] = errorButton;

  inputElements["itemName"] = itemNameInput;
  inputElements["itemAmount"] = itemAmountInput;
  inputElements["itemInterestPeriod"] = itemInterestPeriodInput;
  inputElements["itemInterestRate"] = itemInterestRateInput;
  inputElements["itemInitialSavings"] = itemIntialSavingsInput;
  inputElements["yearsToComplete"] = yearsToCompleteInput;
  inputElements["monthsToComplete"] = monthsToCompleteInput;

  dropdownElements["goalPrioity"] = goalPrioityDropdown;

  images["logo"] = logo;

  animations["error"] = errorAnimation;
  etc["errorMessage"] = "";
}

int AddGoalsPage::Render(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  if (init)
  {
    Init(windowWidth, windowHeight);
    init = false;
  }

  // Unselect all input boxes
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
  {
    for (auto it = inputElements.begin(); it != inputElements.end(); it++) 
      it->second.SetSelected(false);
  }
  
  // Render GUI 'structure'

  DrawTexture(images["logo"], (0.5f*windowWidth)-(images["logo"].width/2.0f), 0.04f*windowHeight, WHITE);

  textElements["savingsHeader"].UpdateAndRender(mouseState);
  textElements["prioityText"].UpdateAndRender(mouseState);

  inputElements["itemName"].UpdateAndRender(mouseState);
  inputElements["itemAmount"].UpdateAndRender(mouseState);
  inputElements["itemInterestPeriod"].UpdateAndRender(mouseState);
  inputElements["itemInterestRate"].UpdateAndRender(mouseState);
  inputElements["itemInitialSavings"].UpdateAndRender(mouseState);
  inputElements["yearsToComplete"].UpdateAndRender(mouseState);
  inputElements["monthsToComplete"].UpdateAndRender(mouseState);

  dropdownElements["goalPrioity"].UpdateAndRender(windowWidth, windowHeight, mouseState);

  if (buttonElements["submit"].UpdateAndRender(mouseState) || IsKeyPressed(KEY_ENTER))
  {
    // Append the new goal to a file
    std::string name = inputElements["itemName"].GetText();
    std::string amount = inputElements["itemAmount"].GetText();
    std::string interestPeriod = inputElements["itemInterestPeriod"].GetText();
    std::string interestAmount = inputElements["itemInterestRate"].GetText();
    std::string initialSavings = inputElements["itemInitialSavings"].GetText();
    std::string priority = dropdownElements["goalPrioity"].GetSelectedOption();
    std::string yearsToComplete = inputElements["yearsToComplete"].GetText();
    if (yearsToComplete.empty())
      yearsToComplete = "0";
    std::string monthsToComplete = inputElements["monthsToComplete"].GetText();
    if (monthsToComplete.empty())
      monthsToComplete = "1";

    if (interestPeriod.empty())
      interestPeriod = "1";
    if (interestAmount.empty())
      interestAmount = "0";
    if (initialSavings.empty())
      initialSavings = "0";

    // Sanatize input
    double amountValue; 
    double initialSavingsValue;
    float interestAmountValue;
    int interestPeriodValue;
    int yearsToCompleteValue;
    int monthsToCompleteValue;

    if (name.empty())
    {
      etc["errorMessage"] = "The input field 'Name' is empty!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }
    if (amount.empty())
    {
      etc["errorMessage"] = "The input field 'Amount' is empty!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }

    try
    {
      amountValue = std::stod(Util::GetMoney(amount));
    }
    catch (...)
    {
      etc["errorMessage"] = "The input field 'Amount' does not have a valid number!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;   
    }
    try
    {
      initialSavingsValue = std::stod(initialSavings);
    }
    catch (...)
    {
      etc["errorMessage"] = "The input field 'Initial savings' does not have a valid number!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;   
    }
    try
    {
      interestAmountValue = std::stod(interestAmount);
    }
    catch (...)
    {
      etc["errorMessage"] = "The input field 'Interest amount' does not have a valid number!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;   
    }
    try
    {
      interestPeriodValue = std::stoi(interestPeriod);
    }
    catch (...)
    {
      etc["errorMessage"] = "The input field 'Interest period' does not have a valid number!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;   
    }
    try
    {
      yearsToCompleteValue = std::stoi(yearsToComplete);
    }
    catch (...)
    {
      etc["errorMessage"] = "The input field 'Years to complete' does not have a valid number!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;   
    }
    try
    {
      monthsToCompleteValue = std::stoi(monthsToComplete);
    }
    catch (...)
    {
      etc["errorMessage"] = "The input field 'Months to complete' does not have a valid number!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;   
    }

    if (yearsToCompleteValue < 0)
    {
      etc["errorMessage"] = "The input field 'Years to complete' does not have a valid number!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0; 
    }
    if (monthsToCompleteValue < 0)
    {
      etc["errorMessage"] = "The input field 'Months to complete' does not have a valid number!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0; 
    }
    if (!monthsToCompleteValue && !yearsToCompleteValue)
    {
      etc["errorMessage"] = "'Years to complete' and 'Months to complete' both have zeros, not valid!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0; 
    }

    Database::AddGoal(name, amountValue, interestPeriodValue, interestAmountValue, initialSavingsValue, yearsToCompleteValue, monthsToCompleteValue, priority);

    DrawRectangle(0, 0, windowWidth, windowHeight, THEME_COLOR);
    DeInit();
    return 3;
  }

  if (buttonElements["back"].UpdateAndRender(mouseState))
  {
    DrawRectangle(0, 0, windowWidth, windowHeight, THEME_COLOR);
    DeInit();
    return 3; // Main page
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
