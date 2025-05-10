#include "../../include/pages/editTransactionPage.hpp"

void EditTransactionPage::Init(float windowWidth, float windowHeight) 
{
  // Load logo
  Texture2D logo = LoadTexture("assets/images/vault.png");
  logo.width >>= 1;
  logo.height >>= 1;

  // Error animation keyframes
  std::vector<KeyFrame> errorAnimationKeyframes = { 
    { { 0.5f*windowWidth-(0.25f*windowWidth), -0.12f*windowHeight, 0.5f*windowWidth, 0.1f*windowHeight }, WHITE, 0.0f, 0.0f },
    { { 0.5f*windowWidth-(0.25f*windowWidth), 0.11f*windowHeight, 0.5f*windowWidth, 0.1f*windowHeight }, WHITE, 0.0f, 0.25f },
  };

  // Error animation
  Animation errorAnimation = Animation(errorAnimationKeyframes);

  // Text style(s)
  GUI::TextStyle headerStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 150, GUI::TEXT_ALIGNMENT_CENTER, { 0 } };
  GUI::TextStyle transactionTypeStyle = headerStyle;
  transactionTypeStyle.textAlignment = GUI::TEXT_ALIGNMENT_LEFT;
  transactionTypeStyle.fontSize = 50;

  // Button style(s)
  GUI::ButtonStyle backButtonStyle = { TRANSPARENT, WHITE, TRANSPARENT, TRANSPARENT, GREEN, TRANSPARENT, font, 75, GUI::TEXT_ALIGNMENT_CENTER, 0, 0, 0, false, 1 };
  Color alphaGreen = SEA_GREEN;
  alphaGreen.a = 100;
  GUI::ButtonStyle createTransactionButtonStyle = { TRANSPARENT, WHITE, SEA_GREEN, alphaGreen, WHITE, GREEN, font, 25, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 0.1f, 0.2f, false, 1.05f };

  // Dropdown style(s)
  GUI::DropdownStyle transactionFromSelection = { DARKGREEN, WHITE, GREEN, WHITE, font, 40, GUI::TEXT_ALIGNMENT_CENTER };

  // Input style(s)
  GUI::InputStyle transactionFromStyle = { TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, GREEN, WHITE, BLUE, font, 15, 0.25f, 0.1f, 0.2f, false, false };

  // Text component(s)
  GUI::Text header = GUI::Text(Rectangle{ 0, 0.2f*windowHeight, windowWidth, 0.1f*windowHeight }, headerStyle, "Edit a Transaction");

  // Button component(s)
  GUI::Button backButton = GUI::Button(Rectangle{ 0, 0, 0.1f*windowWidth, 0.1f*windowHeight }, backButtonStyle, "<- Back");
  GUI::Button createTransactionButton = GUI::Button(Rectangle{ 0.5f*windowWidth-(0.25f*windowWidth), 0.75f*windowHeight, 0.5f*windowWidth, 0.05f*windowHeight }, createTransactionButtonStyle, "Update Transaction!");

  GUI::ButtonStyle errorStyle = { RED, WHITE, PINK, RED, WHITE, PINK, font, 30, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 2, 1.5f, false, 1 };
  GUI::Button errorButton = GUI::Button(Rectangle{ 0 }, errorStyle, "");

  // Input component(s)
  GUI::Input transactionFromInput = GUI::Input(Rectangle{ 0.25f*windowWidth, 0.55f*windowHeight, 0.2f*windowWidth, 0.04f*windowHeight }, transactionFromStyle, "Transaction from... (needed if custom is chose)");
  GUI::Input transactionToInput = GUI::Input(Rectangle{ 0.55f*windowWidth, 0.55f*windowHeight, 0.2f*windowWidth, 0.04f*windowHeight }, transactionFromStyle, "Transaction to... (needed if custom is chose)");
  GUI::Input transactionAmountInput = GUI::Input(Rectangle{ 0.25f*windowWidth, 0.65f*windowHeight, 0.5f*windowWidth, 0.04f*windowHeight }, transactionFromStyle, "Transaction amount...");

  // Dropdown component(s)
  std::vector<std::string> accountNames = { "Custom" };
  GUI::Dropdown fromType = GUI::Dropdown(Rectangle{ 0.25f*windowWidth, 0.5f*windowHeight, 0.2f*windowWidth, 0.04f*windowHeight }, transactionFromSelection, accountNames, windowWidth, windowHeight);
  GUI::Dropdown toType = GUI::Dropdown(Rectangle{ 0.55f*windowWidth, 0.5f*windowHeight, 0.2f*windowWidth, 0.04f*windowHeight }, transactionFromSelection, accountNames, windowWidth, windowHeight);

  std::vector<std::string> periods = { "Not recurring" };
  for (uint8_t i = 1; i <= 52; i++) 
    periods.push_back(std::to_string(i)+" week(s)");
  GUI::Dropdown timePeriods = GUI::Dropdown(Rectangle{ 0.25f*windowWidth, 0.38f*windowHeight, 0.5f*windowWidth, 0.04f*windowHeight }, transactionFromSelection, periods, windowWidth, windowHeight);
 
  // Load all components into the GUI 'structure'
  textElements["header"] = header;

  buttonElements["back"] = backButton;
  buttonElements["createTransaction"] = createTransactionButton;
  buttonElements["error"] = errorButton;

  inputElements["transactionFrom"] = transactionFromInput;
  inputElements["transactionTo"] = transactionToInput;
  inputElements["transactionAmount"] = transactionAmountInput;

  Database::Transaction transaction = Database::GetTransactionToUpdate();
  inputElements["transactionFrom"].SetText(transaction.from);
  inputElements["transactionTo"].SetText(transaction.to);
  inputElements["transactionAmount"].SetText(std::to_string(transaction.amount));

  dropdownElements["transactionFrom"] = fromType;
  dropdownElements["transactionTo"] = toType;
  dropdownElements["transactionPeriod"] = timePeriods;

  images["logo"] = logo;

  animations["error"] = errorAnimation;
  etc["errorMessage"] = "";

  LoadData();
}

void EditTransactionPage::LoadData()
{
  std::vector<std::string> accountNames = { "Custom" };
  std::vector<Database::Account> accounts = Database::GetAccounts();
  for (const Database::Account& account : accounts)
    accountNames.push_back(account.name);

  dropdownElements["transactionFrom"].SetElements(accountNames);

  // Edit the goals only for the 'to' section
  for (std::map<std::string, std::string>& goal : Database::GetGoalsMap())
    accountNames.push_back(goal["name"]+" (Goal)");

  dropdownElements["transactionTo"].SetElements(accountNames);
}

int EditTransactionPage::Render(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  if (init)
  {
    Init(windowWidth, windowHeight);
    init = false;
  }

  if (m_ReloadData)
  {
    LoadData();
    m_ReloadData = false;
  }

  DrawTexture(images["logo"], (0.5f*windowWidth)-(images["logo"].width/2.0f), 0.05f*windowHeight, WHITE);

  textElements["header"].UpdateAndRender(mouseState);

  // Unselect all input boxes if mouse is pressed
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
  {
    for (auto it = inputElements.begin(); it != inputElements.end(); it++)
      it->second.SetSelected(false);
  }

  // Render GUI 'structure'

  inputElements["transactionFrom"].UpdateAndRender(mouseState);
  inputElements["transactionTo"].UpdateAndRender(mouseState);
  inputElements["transactionAmount"].UpdateAndRender(mouseState);

  dropdownElements["transactionFrom"].UpdateAndRender(windowWidth, windowHeight, mouseState);
  dropdownElements["transactionTo"].UpdateAndRender(windowWidth, windowHeight, mouseState);
  dropdownElements["transactionPeriod"].UpdateAndRender(windowWidth, windowHeight, mouseState);
   
  if (buttonElements["createTransaction"].UpdateAndRender(mouseState))
  {
    // Edit transaction 
    std::string from = dropdownElements["transactionFrom"].GetSelectedOption();
    std::string to = dropdownElements["transactionTo"].GetSelectedOption();
    if (from == "" || from == "Custom")
      from = inputElements["transactionFrom"].GetText();
    if (to == "" || to == "Custom")
      to = inputElements["transactionTo"].GetText();

    std::string transactionPeriod = dropdownElements["transactionPeriod"].GetSelectedOption();

    // Sanatize input
    double transactionAmount; 
    if (inputElements["transactionAmount"].GetText().empty())
    {
      transactionAmount = 0.0f;
    }
    else
    {
      try
      {
        transactionAmount = std::stod(Util::GetMoney(inputElements["transactionAmount"].GetText()));
      }
      catch (...)
      {
        etc["errorMessage"] = "Transaction amount is not a valid number!";
        animations["error"].Reset();
        animations["error"].Play();
        return 0;
      }
    }
      
    if (from.empty())
    {
      etc["errorMessage"] = "Custom was selected for the 'from' field, yet nothing was provided!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }
    if (to.empty())
    {
      etc["errorMessage"] = "Custom was selected for the 'to' field, yet nothing was provided!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }

    // Edit the transaction no matter what, even if it is reccuring
    Database::UpdateTransaction(from, to, transactionAmount);

    inputElements["transactionFrom"].SetText("");
    inputElements["transactionFrom"].SetSelected(false);
    inputElements["transactionTo"].SetText("");
    inputElements["transactionTo"].SetSelected(false);
    inputElements["transactionAmount"].SetText("");
    inputElements["transactionAmount"].SetSelected(false);

    m_ReloadData = true;
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

  // Display / play error animation
  animations["error"].Animate();
  Rectangle errorAnimationBounds = animations["error"].GetCurrentBounds();

  buttonElements["error"].SetText(etc["errorMessage"]);
  buttonElements["error"].SetBounds(errorAnimationBounds);

  if (buttonElements["error"].UpdateAndRender(mouseState))
    animations["error"].Reset();

  return 0;
}
