#include "../../include/pages/mainPage.hpp"
#include "../../include/vendor/tinyfiledialogs.h"
#include <filesystem>
#include <fstream>

void MainPage::Init(float windowWidth, float windowHeight)
{
  LoadData();

  // Load images 
  Texture2D imageLogo = LoadTexture("assets/images/vault.png");
  Texture2D accountHelpImage = LoadTexture("assets/images/accounts.png");
  Texture2D addAccountHelpImage = LoadTexture("assets/images/createAccount.png");
  Texture2D transactionsHelpImage = LoadTexture("assets/images/transactions.png");
  Texture2D addTransactionsHelpImage = LoadTexture("assets/images/createTransaction.png");
  Texture2D goalsHelpImage = LoadTexture("assets/images/goals.png");
  Texture2D addGoalsHelpImage = LoadTexture("assets/images/createGoal.png");
  Texture2D reportsHelpImage = LoadTexture("assets/images/reports.png");
  Texture2D addReportsHelpImage = LoadTexture("assets/images/createReport.png");

  Texture2D chatImage = LoadTexture("assets/images/chat.png");

  // Base styles
  Color customGray = { 45, 45, 75, 255 };
  Color customLightGray = { 55, 55, 85, 255 };
  Color customSuperLightGray = { 75, 75, 105, 255 };

  // Error animation keyframes
  std::vector<KeyFrame> verifyAccountDeleteKeyframes = {
    { { windowWidth, 0.25f*windowHeight, 0.25f*windowWidth, 0.5f*windowHeight }, customSuperLightGray, 0.0f, 0.0f },
    { { 0.5f*windowWidth, 0.25f*windowHeight, 0.4f*windowWidth, 0.5f*windowHeight }, customSuperLightGray, 0.0f, 0.25f }
  };

  // Error animation
  Animation verifyAccountDeleteAnimation = Animation(verifyAccountDeleteKeyframes);

  Color alphaGreen = SEA_GREEN;
  alphaGreen.a = 100;
 
  GUI::TextStyle areYouSureStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 50, GUI::TEXT_ALIGNMENT_CENTER, { 0 } };

  GUI::Text areYouSure = GUI::Text(Rectangle{ 0 }, areYouSureStyle, ""); // Some data will be set rendered

  // Load top bar information and styles
  GUI::TextStyle topBarNameStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 50, GUI::TEXT_ALIGNMENT_LEFT, { 15, 15 } };
  GUI::TextStyle topBarTotalAmountStyle = { TRANSPARENT, GREEN, TRANSPARENT, GREEN, font, 75, GUI::TEXT_ALIGNMENT_RIGHT, { 0 } };

  Color lightRed = RED;
  lightRed.r = 200;
  GUI::ButtonStyle yesStyle = { DARKGREEN, WHITE, WHITE, GREEN, WHITE, WHITE, font, 20, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 0.5f, 0.1f, false, 1.0f  };
  GUI::ButtonStyle noStyle = { RED, WHITE, WHITE, lightRed, WHITE, WHITE, font, 20, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 0.5f, 0.1f, false, 1.0f  };

  GUI::Text topBarNameText = GUI::Text(Rectangle{ 0.1f*windowWidth, 0, 0.9f*windowWidth, 0.1f*windowHeight }, topBarNameStyle, "Hello, "+m_FormatedName);
  GUI::Text topBarTotalAmount = GUI::Text(Rectangle{ 0.1f*windowWidth, 0, 0.9f*windowWidth, 0.1f*windowHeight}, topBarTotalAmountStyle, "");

  GUI::Button yesButton = GUI::Button(Rectangle{ 0 }, yesStyle, "Yes");
  GUI::Button noButton = GUI::Button(Rectangle{ 0 }, noStyle, "No");

  // Load side bar information
  GUI::ButtonStyle leftBarButtonStyle = { TRANSPARENT, SEA_GREEN, SEA_GREEN, alphaGreen, SEA_GREEN, SEA_GREEN, font, 30, GUI::TEXT_ALIGNMENT_CENTER, 0, 0.5f, 0.1f, false, 1.0f };
 
  GUI::Button leftBarAccountButton = GUI::Button(Rectangle{ 0, 0.1f*windowHeight, 0.1f*windowWidth, 0.15f*windowHeight }, leftBarButtonStyle, "Accounts");
  GUI::Button leftBarTransactionButton = GUI::Button(Rectangle{ 0, 0.25f*windowHeight, 0.1f*windowWidth, 0.15f*windowHeight }, leftBarButtonStyle, "Transactions");
  GUI::Button leftBarSavingsButton = GUI::Button(Rectangle{ 0, 0.4f*windowHeight, 0.1f*windowWidth, 0.15f*windowHeight }, leftBarButtonStyle, "Goals");
  GUI::Button leftBarReportsButton = GUI::Button(Rectangle{ 0, 0.55f*windowHeight, 0.1f*windowWidth, 0.15f*windowHeight }, leftBarButtonStyle, "Reports");
  GUI::Button leftBarSettingsButton = GUI::Button(Rectangle{ 0, 0.7f*windowHeight, 0.1f*windowWidth, 0.15f*windowHeight }, leftBarButtonStyle, "Settings");
  GUI::Button leftBarHelpButton = GUI::Button(Rectangle{ 0, 0.85f*windowHeight, 0.1f*windowWidth, 0.15f*windowHeight }, leftBarButtonStyle, "Help");
  
  // Load dashboard style information
  GUI::TextStyle greenVaultNameStyle = { TRANSPARENT, SEA_GREEN, TRANSPARENT, SEA_GREEN, font, 100, GUI::TEXT_ALIGNMENT_LEFT, { 15, 15 } };
  GUI::TextStyle dashboardHeaderStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 90, GUI::TEXT_ALIGNMENT_LEFT, { 15, 20 } };
  GUI::TextStyle dashboardSavingStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 60, GUI::TEXT_ALIGNMENT_LEFT, { 15, 5 } };
  GUI::TextStyle dashboardNoReportStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 80, GUI::TEXT_ALIGNMENT_CENTER, { 0 } };
  GUI::TextStyle dashboardNoInfoStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 30, GUI::TEXT_ALIGNMENT_CENTER, { 0 } };
  GUI::TextStyle dashboardActionStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 30, GUI::TEXT_ALIGNMENT_LEFT, { 0 } };

  Rectangle savingsBounds = { 0.12f*windowWidth, 0.24f*windowHeight, 0.4f*windowWidth, 0.32f*windowHeight };
  Rectangle recentActionBounds = { 0.12f*windowWidth, 0.59f*windowHeight, 0.4f*windowWidth, 0.32f*windowHeight };

  GUI::Text greenVaultName = GUI::Text(Rectangle{ 0.1f*windowWidth, 0.1f*windowHeight, 0.9f*windowWidth, 0.1f*windowHeight }, greenVaultNameStyle, "Green Vault");
  GUI::Text dashboardHeader = GUI::Text(Rectangle{ 0.35f*windowWidth, 0.1f*windowHeight, 0.9f*windowWidth, 0.1f*windowHeight }, dashboardHeaderStyle, "Dashboard");
  GUI::Text dashboardSaving = GUI::Text(Rectangle{ savingsBounds.x, savingsBounds.y, savingsBounds.width, 0.1f*windowHeight }, dashboardSavingStyle, "Goals");
  GUI::Text dashboardRecentActions = GUI::Text(Rectangle{ recentActionBounds.x, recentActionBounds.y, recentActionBounds.width, 0.1f*windowHeight }, dashboardSavingStyle, "Recent Actions");
  GUI::Text dashboardNoReport = GUI::Text(Rectangle{ 0.5f*windowWidth, 0.2f*windowHeight, 0.5f*windowWidth, 0.1f*windowHeight }, dashboardNoReportStyle, "No report found!");
  GUI::Text dashboardNoSavings = GUI::Text(Rectangle{ savingsBounds.x, savingsBounds.y+(savingsBounds.height/2), savingsBounds.width, 0.1f*windowHeight }, dashboardNoInfoStyle, "No goals found!");
  GUI::Text dashboardNoRecentActions = GUI::Text(Rectangle{ recentActionBounds.x, recentActionBounds.y+(recentActionBounds.height/2), recentActionBounds.width, 0.1f*windowHeight }, dashboardNoInfoStyle, "No recent actions found!");
  GUI::Text dashboardAction = GUI::Text(Rectangle{ 0 }, dashboardActionStyle, ""); // Information gets set when rendered
  GUI::Text dashboardGoal = GUI::Text(Rectangle{ 0 }, dashboardActionStyle, ""); // Information gets set when rendered
  
  // Load accounts style information
  GUI::ScrollAreaStyle accountsScrollStyle = { TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT, { 5, 5 }, 0, 2.0f };

  GUI::TextStyle accountsHeaderStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 100, GUI::TEXT_ALIGNMENT_LEFT, { 10, 10 } };
  GUI::TextStyle accountsNameStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 50, GUI::TEXT_ALIGNMENT_LEFT, { 15, 15 } };
  GUI::TextStyle accountsTypeStyle = accountsNameStyle;
  accountsTypeStyle.fontSize = 20.0f;
  GUI::TextStyle accountsAmountStyle = accountsNameStyle;
  accountsAmountStyle.textColor = GREEN;
  accountsAmountStyle.textHoverColor = GREEN;
  accountsAmountStyle.textAlignment = GUI::TEXT_ALIGNMENT_RIGHT;
  accountsAmountStyle.padding = { -5, 5 };
  GUI::TextStyle noAccountStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 80, GUI::TEXT_ALIGNMENT_CENTER, { 0 } };

  GUI::ButtonStyle addAccountStyle = { DARKGREEN, WHITE, WHITE, GREEN, WHITE, WHITE, font, 30, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 0.5f, 0.5f, true, 1.05f };
  GUI::ButtonStyle viewAccountStyle = { BROWN, WHITE, WHITE, BEIGE, WHITE, WHITE, font, 25, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 0.5f, 0.5f, true, 1.025f  };
  GUI::ButtonStyle deleteAccountStyle = { RED, WHITE, WHITE, lightRed, WHITE, WHITE, font, 25, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 0.5f, 0.5f, true, 1.025f  };

  GUI::InputStyle accountQueryStyle = { TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, GREEN, WHITE, BLUE, font, 30, 0.25f, 0.1f, 0.2f, false, false };
 
  GUI::ScrollArea accountsScroll = GUI::ScrollArea(Rectangle{ 0.1f*windowWidth, 0.2f*windowHeight, 0.9f*windowWidth, 0.8f*windowHeight }, accountsScrollStyle);

  GUI::Text accountsHeader = GUI::Text(Rectangle{ 0.1f*windowWidth, 0.1f*windowHeight, windowWidth, 0.1f*windowHeight }, accountsHeaderStyle, "Accounts");
  GUI::Text accountsName = GUI::Text(Rectangle{ 0 }, accountsNameStyle, ""); // Information gets set when rendered
  GUI::Text accountsType = GUI::Text(Rectangle{ 0 }, accountsTypeStyle, ""); // Information gets set when rendered
  GUI::Text accountsAmount = GUI::Text(Rectangle{ 0 }, accountsAmountStyle, ""); // Information gets set when rendered
  GUI::Text accountsCreated = GUI::Text(Rectangle{ 0 }, accountsTypeStyle, ""); // Information gets set when rendered
  GUI::Text noAccount = GUI::Text(Rectangle{ 0.1f*windowWidth, 0.2f*windowHeight, 0.9f*windowWidth, 0.1f*windowHeight }, noAccountStyle, "No accounts created! Try creating one!");

  GUI::Button addAccountButton = GUI::Button(Rectangle{ 0.79f*windowWidth, 0.125f*windowHeight, 0.2f*windowWidth, 0.05f*windowHeight }, addAccountStyle, "Add Account");
  GUI::Button viewAccountButton = GUI::Button(Rectangle{ 0 }, viewAccountStyle, "View"); // Information gets set when rendered
  GUI::Button deleteAccountButton = GUI::Button(Rectangle{ 0 }, deleteAccountStyle, "Delete"); // Information gets set when rendered

  GUI::Input accountQuery = GUI::Input(Rectangle{ 0.79f*windowWidth-(0.21f*windowWidth), 0.125f*windowHeight, 0.2f*windowWidth, 0.05f*windowHeight }, accountQueryStyle, "Search...");

  GUI::Popup verifyAccountDelete = GUI::Popup(Rectangle{ 0.5f*windowWidth, 0.25f*windowHeight, 0.4f*windowWidth, 0.5f*windowHeight }, customSuperLightGray);

  // Load transactions style information
  GUI::TextStyle transactionsHeaderStyle = accountsHeaderStyle;
  GUI::TextStyle transactionsFromStyle = accountsNameStyle;
  GUI::TextStyle transactionsToStyle = accountsTypeStyle;
  GUI::TextStyle transactionsAmountStyle = accountsAmountStyle;
  GUI::TextStyle noTransactionsStyle = noAccountStyle;

  GUI::ButtonStyle addTransactionStyle = addAccountStyle;

  GUI::ScrollArea transactionsScroll = GUI::ScrollArea(Rectangle{ 0.1f*windowWidth, 0.2f*windowHeight, 0.9f*windowWidth, 0.8f*windowHeight }, accountsScrollStyle);

  GUI::Button addTransactionButton = GUI::Button(Rectangle{ 0.79f*windowWidth, 0.125f*windowHeight, 0.2f*windowWidth, 0.05f*windowHeight }, addTransactionStyle, "Add Transaction");

  GUI::Text transactionsHeader = GUI::Text(Rectangle{ 0.1f*windowWidth, 0.1f*windowHeight, windowWidth, 0.1f*windowHeight}, transactionsHeaderStyle, "Transactions");
  GUI::Text transactionsFrom = GUI::Text(Rectangle{ 0 }, transactionsFromStyle, ""); // Information gets set when rendered
  GUI::Text transactionsTo = GUI::Text(Rectangle{ 0 }, transactionsToStyle, ""); // Information gets set when rendered
  GUI::Text transactionsAmount = GUI::Text(Rectangle{ 0 }, transactionsAmountStyle, ""); // Information gets set when rendered
  GUI::Text noTransactions = GUI::Text(Rectangle{ 0.1f*windowWidth, 0.2f*windowHeight, 0.9f*windowWidth, 0.1f*windowHeight }, noAccountStyle, "No transactions created! Try creating one!");

  GUI::Input transactionQuery = GUI::Input(Rectangle{ 0.79f*windowWidth-(0.21f*windowWidth), 0.125f*windowHeight, 0.2f*windowWidth, 0.05f*windowHeight }, accountQueryStyle, "Search...");

  // Load goals style information
  GUI::Text goalsHeader = GUI::Text(Rectangle{ 0.1f*windowWidth, 0.1f*windowHeight, windowWidth, 0.1f*windowHeight}, transactionsHeaderStyle, "Goals");
  GUI::ScrollArea goalsScroll = GUI::ScrollArea(Rectangle{ 0.1f*windowWidth, 0.2f*windowHeight, 0.9f*windowWidth, 0.8f*windowHeight }, accountsScrollStyle);

  GUI::TextStyle goalPriorityStyle = accountsTypeStyle;
  goalPriorityStyle.fontSize *= 1.25f;

  GUI::Text goalName = GUI::Text(Rectangle{ 0 }, accountsNameStyle, ""); // Information gets set when rendered
  GUI::Text goalAmount = GUI::Text(Rectangle{ 0 }, accountsAmountStyle, ""); // Information gets set when rendered
  GUI::Text goalPriority = GUI::Text(Rectangle{ 0 }, goalPriorityStyle, ""); // Information gets set when rendered
  GUI::Text goalInterestPeirod = GUI::Text(Rectangle{ 0 }, accountsTypeStyle, ""); // Information gets set when rendered
  GUI::Text goalInterestAmount = GUI::Text(Rectangle{ 0 }, accountsTypeStyle, ""); // Information gets set when rendered
  GUI::Text goalPayedOff = GUI::Text(Rectangle{ 0 }, accountsTypeStyle, ""); // Information gets set when rendered
  GUI::Text goalCreated = GUI::Text(Rectangle{ 0 }, accountsTypeStyle, ""); // Information gets set when rendered
  GUI::TextStyle goalFinishedStyle = accountsTypeStyle; 
  goalFinishedStyle.textColor = GREEN;
  goalFinishedStyle.textHoverColor = GREEN;
  goalFinishedStyle.textAlignment = GUI::TEXT_ALIGNMENT_CENTER;
  goalFinishedStyle.fontSize = 50;
  GUI::Text goalFinished = GUI::Text(Rectangle{ 0, 0.5f*windowHeight, windowWidth, 0.1f*windowHeight }, goalFinishedStyle, "Goal Completed!");

  GUI::Button addGoalsButton = GUI::Button(Rectangle{ 0.79f*windowWidth, 0.125f*windowHeight, 0.2f*windowWidth, 0.05f*windowHeight }, addTransactionStyle, "Add Goal");

  // Load reports style information
  GUI::Text reportsHeader = GUI::Text(Rectangle{ 0.1f*windowWidth, 0.1f*windowHeight, windowWidth, 0.1f*windowHeight }, transactionsHeaderStyle, "Reports");
  GUI::Text reportsName = GUI::Text(Rectangle{ 0 }, transactionsFromStyle, ""); // Information gets set when rendered
  GUI::Text reportsLocation = GUI::Text(Rectangle{ 0 }, transactionsToStyle, ""); // Information gets set when rendered

  GUI::Button addReportsButton = GUI::Button(Rectangle{ 0.79f*windowWidth, 0.125f*windowHeight, 0.2f*windowWidth, 0.05f*windowHeight }, addTransactionStyle, "Add Report");
  GUI::ScrollArea reportsScroll = GUI::ScrollArea(Rectangle{ 0.1f*windowWidth, 0.2f*windowHeight, 0.9f*windowWidth, 0.8f*windowHeight }, accountsScrollStyle);

  // Load settings style information
  
  GUI::Text settingsHeader = GUI::Text(Rectangle{ 0.1f*windowWidth, 0.1f*windowHeight, windowWidth, 0.1f*windowHeight}, transactionsHeaderStyle, "Settings");

  GUI::TextStyle settingsTextStyle = transactionsHeaderStyle;
  settingsTextStyle.fontSize = 30;
  GUI::Text settingsDeleteRecentsText = GUI::Text(Rectangle{ 0.1f*windowWidth, 0.2f*windowHeight, windowWidth, 0.01f*windowHeight }, settingsTextStyle, "Delete all recent actions? THIS ACTION CAN NOT BE UNDONE!");

  GUI::ButtonStyle settingsDeleteStyle = noStyle;
  
  GUI::Button deleteRecentsButton = GUI::Button(Rectangle{ 0.1f*windowWidth+MeasureTextEx(settingsDeleteRecentsText.GetStyle().font, settingsDeleteRecentsText.GetText().c_str(), settingsDeleteRecentsText.GetStyle().fontSize*GUI::CalculateFontScale(GetMonitorWidth(GetCurrentMonitor())), SPACING).x+(0.025f*windowWidth), 0.2f*windowHeight, 0.2f*windowWidth, 0.035f*windowHeight }, settingsDeleteStyle, "Delete");

  GUI::CheckboxStyle checkboxStyle = { TRANSPARENT, SEA_GREEN, SEA_GREEN, 0.1f };
  GUI::Checkbox chatStateCheckbox({ 0.125f*windowWidth, 0.4f*windowHeight, 0.025f*windowWidth, 0.025f*windowWidth }, checkboxStyle);

  // Load help style information
  GUI::TextStyle helpInfoHeaderStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 80, GUI::TEXT_ALIGNMENT_LEFT, { 30, 30 } };
  GUI::TextStyle helpInfoInfoStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 40, GUI::TEXT_ALIGNMENT_LEFT, { 0, 0 } };
  GUI::TextStyle messageStyle = { GREEN, BLACK, GREEN, BLACK, font, 25.0f, GUI::TEXT_ALIGNMENT_CENTER, { 0 } };

  GUI::Text helpHeader = GUI::Text(Rectangle{ 0.1f*windowWidth, 0.1f*windowHeight, windowWidth, 0.1f*windowHeight}, transactionsHeaderStyle, "Help");
  GUI::Text helpInfoHeader = GUI::Text(Rectangle{ 0 }, helpInfoHeaderStyle, "");
  GUI::Text helpInfoInfo = GUI::Text(Rectangle{ 0 }, helpInfoInfoStyle, "");
  GUI::Text messageText = GUI::Text(Rectangle{ 0 }, messageStyle, "");

  GUI::InputStyle chatBoxStyle = { TRANSPARENT, BLACK, BLACK, GRAY, TRANSPARENT, BLACK, BLACK, GRAY, TRANSPARENT, BLACK, BLACK, BLACK, font, 30, 0.25f, 0.1f, 0.2f, false, false };
  GUI::Input chatBox({ 0.51f*windowWidth, (0.3f*windowHeight+(0.7f*windowHeight))-(0.1f*windowHeight), 0.35f*windowWidth, 0.05f*windowHeight }, chatBoxStyle, "Your message...");

  GUI::ButtonStyle chatButton = { SEA_GREEN, WHITE, SEA_GREEN, SEA_GREEN, WHITE, SEA_GREEN, GetFontDefault(), 0.0f, GUI::TEXT_ALIGNMENT_CENTER, 1.0f, 1.0f, 1.0f, true, 1.25f };
  GUI::ButtonStyle submitChatStyle = { SEA_GREEN, BLACK, BLACK, BLACK, WHITE, BLACK, font, 20.0f, GUI::TEXT_ALIGNMENT_CENTER, 0.1f, 1.0f, 1.0f, true, 1.05f };

  GUI::Button openChatButton({ windowWidth-(0.1f*windowHeight), windowHeight-(0.1f*windowHeight), 0.05f*windowWidth, 0.05f*windowWidth }, chatButton, "");
  GUI::Button submitChatButton({ 0.51f*windowWidth+(0.36f*windowWidth), (0.3f*windowHeight+(0.7f*windowHeight))-(0.1f*windowHeight), 0.07f*windowWidth, 0.05f*windowHeight }, submitChatStyle, "Send");
  GUI::Button closeChat({ 0.51f*windowWidth+(0.39f*windowWidth), 0.32f*windowHeight, 0.025f*windowWidth, 0.025f*windowWidth }, submitChatStyle, "X");

  GUI::ScrollArea helpInformation = GUI::ScrollArea(Rectangle{ 0.1f*windowWidth, 0.2f*windowHeight, 0.9f*windowWidth, 0.8f*windowHeight }, accountsScrollStyle);

  Rectangle chatBounds = { 0.5f*windowWidth, 0.3f*windowHeight, 0.45f*windowWidth, 0.7f*windowHeight };

  // Load all GUI components into GUI 'structure'
  bounds["goals"] = savingsBounds;
  bounds["recentActions"] = recentActionBounds;
  bounds["chat"] = chatBounds;

  buttonElements["leftBarAccount"] = leftBarAccountButton;
  buttonElements["leftBarTransaction"] = leftBarTransactionButton;
  buttonElements["leftBarSavings"] = leftBarSavingsButton;
  buttonElements["leftBarReports"] = leftBarReportsButton;
  buttonElements["leftBarSettings"] = leftBarSettingsButton;
  buttonElements["leftBarHelp"] = leftBarHelpButton;
  buttonElements["addAccount"] = addAccountButton;
  buttonElements["addTransaction"] = addTransactionButton;
  buttonElements["addGoals"] = addGoalsButton;
  buttonElements["addReports"] = addReportsButton;
  buttonElements["viewAccount"] = viewAccountButton;
  buttonElements["deleteAccount"] = deleteAccountButton;
  buttonElements["settingsDeleteRecents"] = deleteRecentsButton;
  buttonElements["openChat"] = openChatButton;
  buttonElements["submitChat"] = submitChatButton;
  buttonElements["closeChat"] = closeChat;
  buttonElements["yes"] = yesButton;
  buttonElements["no"] = noButton;

  textElements["topBarName"] = topBarNameText;
  textElements["topBarTotalAmount"] = topBarTotalAmount;
  textElements["greenVaultName"] = greenVaultName;
  textElements["dashboardHeader"] = dashboardHeader;
  textElements["dashboardSaving"] = dashboardSaving;
  textElements["dashboardRecentActions"] = dashboardRecentActions;
  textElements["dashboardNoSavings"] = dashboardNoSavings;
  textElements["dashboardNoRecentActions"] = dashboardNoRecentActions;
  textElements["dashboardNoReport"] = dashboardNoReport;
  textElements["action"] = dashboardAction;
  textElements["goal"] = dashboardGoal;
  textElements["accountsHeader"] = accountsHeader;
  textElements["transactionsHeader"] = transactionsHeader;
  textElements["goalsHeader"] = goalsHeader;
  textElements["goalName"] = goalName;
  textElements["goalAmount"] = goalAmount;
  textElements["goalPriority"] = goalPriority;
  textElements["goalInterestPeirod"] = goalInterestPeirod;
  textElements["goalInterestAmount"] = goalInterestAmount;
  textElements["goalPayedOff"] = goalPayedOff;
  textElements["goalCreated"] = goalCreated;
  textElements["goalFinished"] = goalFinished;
  textElements["accountsName"] = accountsName;
  textElements["reportsHeader"] = reportsHeader;
  textElements["reportsName"] = reportsName;
  textElements["reportsLocation"] = reportsLocation;
  textElements["settingsHeader"] = settingsHeader;
  textElements["settingsDeleteRecents"] = settingsDeleteRecentsText;
  textElements["helpHeader"] = helpHeader;
  textElements["helpInfoHeader"] = helpInfoHeader;
  textElements["helpInfoInfo"] = helpInfoInfo;
  textElements["accountsType"] = accountsType;
  textElements["accountsAmount"] = accountsAmount;
  textElements["accountsCreated"] = accountsCreated;
  textElements["transactionsFrom"] = transactionsFrom;
  textElements["transactionsTo"] = transactionsTo;
  textElements["transactionsAmount"] = transactionsAmount;
  textElements["noAccounts"] = noAccount;
  textElements["noTransactions"] = noTransactions;
  textElements["areYouSure"] = areYouSure;
  textElements["message"] = messageText;

  inputElements["accountQuery"] = accountQuery;
  inputElements["transactionQuery"] = transactionQuery;
  inputElements["chatBox"] = chatBox;

  scrollAreaElements["accounts"] = accountsScroll;
  scrollAreaElements["transactions"] = transactionsScroll;
  scrollAreaElements["helpInformation"] = helpInformation;
  scrollAreaElements["goals"] = goalsScroll;
  scrollAreaElements["reports"] = reportsScroll;

  checkboxElements["chatState"] = chatStateCheckbox;
  checkboxElements["chatState"].SetChecked(!m_UseChat);

  popupElements["verifyAccountDelete"] = verifyAccountDelete;

  animations["verifyAccountDelete"] = verifyAccountDeleteAnimation;

  colors["customGray"] = customGray;
  colors["customLightGray"] = customLightGray;

  images["logo"] = imageLogo;
  images["accounts"] = accountHelpImage;
  images["addAccounts"] = addAccountHelpImage;
  images["transactions"] = transactionsHelpImage;
  images["addTransactions"] = addTransactionsHelpImage;
  images["goals"] = goalsHelpImage;
  images["addGoals"] = addGoalsHelpImage;
  images["reports"] = reportsHelpImage;
  images["addReports"] = addReportsHelpImage;
  images["chat"] = chatImage;
}

void MainPage::LoadData()
{
  // Get base user information, accounts, transactions, etc...
  m_User = Database::GetUser();
  m_Accounts = Database::GetAccountsMap();
  m_Transactions = Database::GetTransactionsMap();

  for (std::map<std::string, std::string>& account : m_Accounts)
  {
    account["amount"] = "0.00";
    for (std::map<std::string, std::string>& transaction : m_Transactions)
    {
      if (transaction["from"] == account["name"])
        account["amount"] = std::to_string(std::stod(account["amount"])-std::stod(transaction["amount"]));
      else if (transaction["to"] == account["name"])
        account["amount"] = std::to_string(std::stod(account["amount"])+std::stod(transaction["amount"]));
    }
  }

  m_FormatedName = Util::Uppercase(m_User.firstName)+", "+Util::Uppercase(m_User.lastName[0])+'.';

  // Calculate total account(s) balance
  m_TotalAmount = 0;
  for (std::map<std::string, std::string>& account : m_Accounts)
  {
    try
    {
      double amount = std::stod(account["amount"]);
      m_TotalAmount += amount;
    }
    catch (...) { }
  }

  // Get recent actions
  m_RecentActions = Database::GetActions();

  std::stringstream ss;
  ss << std::fixed << std::setprecision(2) << m_TotalAmount;
  m_TotalAmountDisplay = "Total Accounts Balance: $"+Util::FormatMoney(ss.str());

  // Apply all reccuring transactions if applicable
  for (const std::map<std::string, std::string>& reccuring : Database::GetRecurringTransactions())
    m_Transactions.push_back(reccuring);

  m_Goals = Database::GetGoalsMap();

  // Load reports
  m_Reports.clear(); 
  std::fstream file("data/reports.djb");
  std::string line;
  while (std::getline(file, line))
  {
    const std::string name = line.substr(line.find_last_of('/')+1, line.length()-line.find_last_of('/')-5); // Parse name from file path
    std::map<std::string, std::string> temp;
    temp["name"] = name;
    temp["path"] = line;
    m_Reports.push_back(temp);
    const std::string texturePath = "data/reports/"+name+".png";
    Texture2D report = LoadTexture(texturePath.c_str());
    report.width *= 0.25f;
    report.height *= 0.25f;
    images[name] = report;
  }
  file.close();

  m_UseChat = Database::GetChatSetting();
}

void MainPage::TopBar(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  DrawRectangle(0.1f*windowWidth, 0.0f, 0.9f*windowWidth, 0.1f*windowHeight, colors["customGray"]);

  textElements["topBarName"].UpdateAndRender(mouseState);

  textElements["topBarTotalAmount"].SetText(m_TotalAmountDisplay);

  if (m_TotalAmount < 0)
    textElements["topBarTotalAmount"].SetColor(RED);

  textElements["topBarTotalAmount"].UpdateAndRender(mouseState);
}

void MainPage::LeftBar(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  DrawTextureEx(images["logo"], { 0.02f*windowWidth, 0 }, 0.0f, 0.3f, WHITE);
  // Check if the logo is pressed
  if (CheckCollisionPointRec(mouseState.position, { 0.02f*windowWidth, 0, (float)images["logo"].width/4.0f, (float)images["logo"].height/4.0f }))
  {
    mouseState.cursor = MOUSE_CURSOR_POINTING_HAND;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
      mouseState.clicked = true;
      m_DisplayContent = PageContent::DASHBOARD;
    }
  }

  // Render the buttons
  if (buttonElements["leftBarAccount"].UpdateAndRender(mouseState) || IsKeyPressed(KEY_ONE))
    m_DisplayContent = PageContent::ACCOUNTS;
  if (buttonElements["leftBarTransaction"].UpdateAndRender(mouseState) || IsKeyPressed(KEY_TWO))
    m_DisplayContent = PageContent::TRANSACTIONS;
  if (buttonElements["leftBarSavings"].UpdateAndRender(mouseState) || IsKeyPressed(KEY_THREE))
    m_DisplayContent = PageContent::SAVINGS;
  if (buttonElements["leftBarReports"].UpdateAndRender(mouseState) || IsKeyPressed(KEY_FOUR))
    m_DisplayContent = PageContent::REPORTS;
  if (buttonElements["leftBarSettings"].UpdateAndRender(mouseState) || IsKeyPressed(KEY_FIVE))
    m_DisplayContent = PageContent::SETTINGS;
  if (buttonElements["leftBarHelp"].UpdateAndRender(mouseState) || IsKeyPressed(KEY_SIX))
    m_DisplayContent = PageContent::HELP;
}

void MainPage::RenderChat(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  if (!m_UseChat)
    return;

  // Render chat box and process openai chat client requests
  if (!m_ShowChat && buttonElements["openChat"].UpdateAndRender(mouseState, images["chat"]))
    m_ShowChat = true;

  if (m_ShowChat)
  {
    Rectangle bound = bounds["chat"];
    DrawRectangleRounded(bound, 0.1f, SEGMENTS, SEA_GREEN);

    inputElements["chatBox"].UpdateAndRender(mouseState);
    if (buttonElements["submitChat"].UpdateAndRender(mouseState) || IsKeyPressed(KEY_ENTER))
    {
      std::string message = inputElements["chatBox"].GetText();
      Util::Trim(message);
      if (!message.empty())
      {
        Database::GetAIMessage(message);
        inputElements["chatBox"].SetText("");
      }
    }

    // Assumes messages are in the following order, user, ai, user, ai, etc... or in reverse, ai, user, ai, user, etc...
    float yOffset = 0.11f*windowHeight; 
    std::vector<std::string> messages = Database::GetMessages();
    for (int i = messages.size()-1; i >= 0; i--)
    {
      if (((0.3f*windowHeight+(0.7f*windowHeight))-(0.1f*windowHeight))-yOffset < bound.y)
        break;

      std::string message = messages[i];
      textElements["message"].SetBounds({ 0.51f*windowWidth, ((0.3f*windowHeight+(0.7f*windowHeight))-(0.1f*windowHeight))-yOffset, 0.43f*windowWidth, 0.1f*windowHeight });
      textElements["message"].SetText(message);

      if (i+1 % 2 == 2)
        textElements["message"].SetAlignment(GUI::TEXT_ALIGNMENT_RIGHT);
      else
        textElements["message"].SetAlignment(GUI::TEXT_ALIGNMENT_LEFT);

      textElements["message"].UpdateAndRender(mouseState);

      yOffset += 0.11f*windowHeight;
    }

    if (buttonElements["closeChat"].UpdateAndRender(mouseState))
      m_ShowChat = false;
  }
}

int MainPage::RenderDashboard(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  textElements["greenVaultName"].UpdateAndRender(mouseState);
  textElements["dashboardHeader"].UpdateAndRender(mouseState);

  DrawRectangleRounded(bounds["goals"], 0.25f, SEGMENTS, colors["customLightGray"]);
  DrawRectangleRounded(bounds["recentActions"], 0.25f, SEGMENTS, colors["customLightGray"]);

  textElements["dashboardSaving"].UpdateAndRender(mouseState);

  float y = 0.0f;
  if (m_Goals.size())
  {
    uint8_t count = 0;
    for (int i = m_Goals.size()-1; i >= 0; i--)
    {
      if (count >= 8)
        break;

      std::map<std::string, std::string> goal = m_Goals[i];

      Rectangle goalBounds = bounds["goals"];
      goalBounds.y += 0.05f*windowHeight+y; // Base height offset
      goalBounds.x += 0.01f*windowWidth; // Padding
      goalBounds.y += (0.025f*windowHeight)*(count+1);
      goalBounds.width = windowWidth;

      std::string display = TextFormat("Goal for %s, item cost is $%s", goal["name"].c_str(), Util::FormatMoney(std::to_string(std::stof(goal["amount"]))).c_str());
      if (std::stod(goal["payedOff"]) >= std::stod(goal["amount"]))
        display += " - COMPLETED!";

      textElements["goal"].SetBounds(goalBounds);
      textElements["goal"].SetText(display.c_str());
      textElements["goal"].UpdateAndRender(mouseState);

      y = textElements["goal"].GetHeight(0);

      count++;
    }
  }
  else
  {
    textElements["dashboardNoSavings"].UpdateAndRender(mouseState);
  }
 
  textElements["dashboardRecentActions"].UpdateAndRender(mouseState);

  y = 0.0f;
  if (m_RecentActions.size())
  {
    uint8_t count = 0;
    for (int i = m_RecentActions.size()-1; i >= 0; i--)
    {
      if (count >= 8)
        break;

      const Database::Action& action = m_RecentActions[i];

      Rectangle actionBounds = bounds["recentActions"];
      actionBounds.y += (0.05f*windowHeight)+y; // Base height offset
      actionBounds.x += 0.01f*windowWidth; // Padding
      actionBounds.y += (0.025f*windowHeight)*(count+1);
      actionBounds.width = windowWidth;
      textElements["action"].SetBounds(actionBounds);
      textElements["action"].SetColor((action.added) ? GREEN : RED);

      std::string added = (action.added) ? "Added " : "Deleted ";
      std::string type;
      switch (action.type)
      {
        case Database::ActionType::ACCOUNT: type = "Account "; break;
        case Database::ActionType::TRANSACTION: type = "Transaction "; break;
        case Database::ActionType::GOAL: type = "Goal "; break;
      }
      std::string name = (action.type == Database::ActionType::ACCOUNT || action.type == Database::ActionType::GOAL) ? action.name : action.name.substr(0, action.name.find(' '))+" to "+action.name.substr(action.name.find(' ')+1, action.name.length());

      std::stringstream ss;
      ss << std::fixed << std::setprecision(2) << action.amount;
      std::string amount = Util::FormatMoney(ss.str());

      std::string output = added + type + name + " - Amount $" + amount;

      textElements["action"].SetText(output);
      textElements["action"].UpdateAndRender(mouseState);

      y = textElements["action"].GetHeight(0);

      count++;
    }
  }
  else
  {
    textElements["dashboardNoRecentActions"].UpdateAndRender(mouseState);
  }

  if (m_Reports.size())
  {
    std::map<std::string, std::string> report = m_Reports[m_Reports.size()-1];
    const std::string name = report["name"];
    DrawTexture(images[name], 0.6f*windowWidth, 0.3f*windowHeight, WHITE);

    textElements["dashboardNoReport"].SetText("Recent Report");
  }
  else
  {
    textElements["dashboardNoReport"].SetText("No reports found!");
  }

  textElements["dashboardNoReport"].UpdateAndRender(mouseState);

  RenderChat(windowWidth, windowHeight, mouseState);

  return 0;
}

int MainPage::RenderAccounts(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  textElements["accountsHeader"].UpdateAndRender(mouseState);
  if (buttonElements["addAccount"].UpdateAndRender(mouseState))
  {
    m_LoadData = true;
    DrawRectangle(0, 0, windowWidth, windowHeight, THEME_COLOR);
    DeInit();
    return 4; // Add accounts page
  }

  if (!m_Accounts.size())
  {
    textElements["noAccounts"].UpdateAndRender(mouseState);
    RenderChat(windowWidth, windowHeight, mouseState);
    return 0;
  }

  inputElements["accountQuery"].UpdateAndRender(mouseState);
  m_AccountQuery = inputElements["accountQuery"].GetText();

  std::vector<std::map<std::string, std::string>> accounts = m_Accounts;
  if (m_AccountQuery != "")
  {
    // Find match amount for each item
    std::vector<size_t> indexStack;
    for (const std::map<std::string, std::string>& account : accounts)
    {
      // Calculate the equilvent amount
      uint16_t matchAmount = 0;
      for (auto pair = account.begin(); pair != account.end(); pair++)
      {
        const std::string value = pair->second;
        const size_t min = (m_AccountQuery.length() < value.length()) ? m_AccountQuery.length() : value.length();

        for (size_t i = 0; i < min; i++)
        {
          if (tolower(m_AccountQuery[i]) == tolower(value[i]))
            matchAmount++;
          break;
        }
      }
      indexStack.push_back(matchAmount);
    }

    // Now sort
    for (size_t i = 0; i < indexStack.size()-1; i++)
    {
      for (size_t j = i+1; j < indexStack.size(); j++)
      {
        if (indexStack[i] < indexStack[j])
        {
          // Swap the indexStack
          indexStack[i] ^= indexStack[j];
          indexStack[j] ^= indexStack[i];
          indexStack[i] ^= indexStack[j];

          // Swap the accounts
          std::swap(accounts[i], accounts[j]);
        }
      }
    }
  }

  // Render all accounts
  scrollAreaElements["accounts"].UpdateAndRender(windowWidth, windowHeight, mouseState, accounts, [this](float windowWidth, float windowHeight, GUI::MouseState& mouseState, Vector2 position, std::map<std::string, std::string>& parameters) -> float {
    float height = 0.2f*windowHeight;
    DrawRectangleRounded({ position.x, position.y, 0.9f*windowWidth, height }, 0.2f, SEGMENTS, colors["customLightGray"]);

    textElements["accountsName"].SetBounds({ position.x, position.y, 0.9f*windowWidth, height });
    textElements["accountsName"].SetText(parameters["name"]);
    textElements["accountsName"].UpdateAndRender(mouseState);

    textElements["accountsType"].SetBounds({ position.x, position.y+(0.05f*windowHeight), 0.9f*windowWidth, height });
    textElements["accountsType"].SetText(std::string("Type: "+parameters["type"]));
    textElements["accountsType"].UpdateAndRender(mouseState);

    textElements["accountsCreated"].SetBounds({ position.x, position.y+(0.075f*windowHeight), 0.9f*windowWidth, height });
    textElements["accountsCreated"].SetText(std::string("Created: "+parameters["timecreated"]));
    textElements["accountsCreated"].UpdateAndRender(mouseState);

    try
    {
      float amount = std::stof(parameters["amount"]);
      if (amount < 0)
        textElements["accountsAmount"].SetColor(RED);
      else
        textElements["accountsAmount"].SetColor(GREEN);
    }
    catch (...) { }

    textElements["accountsAmount"].SetBounds({ position.x, position.y, 0.9f*windowWidth, height });
    textElements["accountsAmount"].SetText(std::string("Balance: $"+Util::FormatMoney(parameters["amount"])));
    textElements["accountsAmount"].UpdateAndRender(mouseState);

    buttonElements["viewAccount"].SetBounds({ position.x+15, position.y+height-(0.02f*windowHeight)-15, 0.1f*windowWidth, 0.02f*windowHeight });
    
    if (buttonElements["viewAccount"].UpdateAndRender(mouseState))
    {
      inputElements["transactionQuery"].SetText(parameters["name"]);
      m_DisplayContent = PageContent::TRANSACTIONS;
    }

    buttonElements["deleteAccount"].SetBounds({ position.x+30+(0.1f*windowWidth), position.y+height-(0.02f*windowHeight)-15, 0.1f*windowWidth, 0.02f*windowHeight });
    if (buttonElements["deleteAccount"].UpdateAndRender(mouseState))
    {
      const Database::Account& account = { parameters["name"], parameters["type"], std::stod(parameters["amount"]), parameters["timecreated"] };
      m_VerifyAccountDelete = true;
      m_AccountToRemove = account;
      animations["verifyAccountDelete"].Play();
    }
    
    return height;
  });

  // Render popup
  if (m_VerifyAccountDelete)
  {
    Rectangle bounds = animations["verifyAccountDelete"].GetCurrentBounds();
    popupElements["verifyAccountDelete"].SetBounds(bounds);
    animations["verifyAccountDelete"].Animate();

    int res = popupElements["verifyAccountDelete"].UpdateAndRender(mouseState, windowWidth, windowHeight, [this](const Rectangle& parentBounds, GUI::MouseState& mouseState, float windowWidth, float windowHeight) -> int {
      textElements["areYouSure"].SetBounds(parentBounds);
      textElements["areYouSure"].SetText("Are you sure you want to delete\n\""+std::string(m_AccountToRemove.name)+"\" account?");
      textElements["areYouSure"].UpdateAndRender(mouseState);

      buttonElements["yes"].SetBounds({ parentBounds.x+(0.01f*windowWidth), parentBounds.y+parentBounds.height-(0.1f*windowHeight), 0.1f*windowWidth, 0.05f*windowHeight });
      buttonElements["no"].SetBounds({ parentBounds.x+parentBounds.width-0.11f*windowWidth, parentBounds.y+parentBounds.height-0.1f*windowHeight, 0.1f*windowWidth, 0.05f*windowHeight });

      if (buttonElements["yes"].UpdateAndRender(mouseState))
        return 1;
      if (buttonElements["no"].UpdateAndRender(mouseState))
        return -1;

      return 0;
    });

    if (res == 1)
    {
      Database::RemoveAccount(m_AccountToRemove);
      m_LoadData = true;
      m_VerifyAccountDelete = false;
      animations["verifyAccountDelete"].Reset();
    }
    else if (res == -1)
    {
      m_VerifyAccountDelete = false;
      animations["verifyAccountDelete"].Reset();
    }
  }
  
  RenderChat(windowWidth, windowHeight, mouseState);

  return 0;
}

int MainPage::RenderTransactions(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  textElements["transactionsHeader"].UpdateAndRender(mouseState);
  
  if (buttonElements["addTransaction"].UpdateAndRender(mouseState))
  {
    m_LoadData = true;

    DrawRectangle(0, 0, windowWidth, windowHeight, THEME_COLOR);
    DeInit();
    return 5; // Add transactions page
  }

  if (!m_Transactions.size())
  {
    textElements["noTransactions"].UpdateAndRender(mouseState);
  	RenderChat(windowWidth, windowHeight, mouseState);
    return 0;
  }

  inputElements["transactionQuery"].UpdateAndRender(mouseState);
  m_TransactionQuery = inputElements["transactionQuery"].GetText();

  std::vector<std::map<std::string, std::string>> transactions = m_Transactions;
  if (m_TransactionQuery != "")
  {
    // Find match amount for each item
    std::vector<size_t> indexStack;
    for (const std::map<std::string, std::string>& transaction : transactions)
    {
      // Calculate the equilvent amount
      uint16_t matchAmount = 0;
      for (auto pair = transaction.begin(); pair != transaction.end(); pair++)
      {
        const std::string value = pair->second;
        const size_t min = (m_TransactionQuery.length() < value.length()) ? m_TransactionQuery.length() : value.length();

        for (size_t i = 0; i < min; i++)
        {
          if (tolower(m_TransactionQuery[i]) == tolower(value[i]))
            matchAmount++;
          break;
        }
      }
      indexStack.push_back(matchAmount);
    }

    // Now sort
    for (size_t i = 0; i < indexStack.size()-1; i++)
    {
      for (size_t j = i+1; j < indexStack.size(); j++)
      {
        if (indexStack[i] < indexStack[j])
        {
          // Swap the indexStack
          indexStack[i] ^= indexStack[j];
          indexStack[j] ^= indexStack[i];
          indexStack[i] ^= indexStack[j];

          // Swap the accounts
          std::swap(transactions[i], transactions[j]);
        }
      }
    }
  }

  // Render all transactions
  int toReturn = -1;
  scrollAreaElements["transactions"].UpdateAndRender(windowWidth, windowHeight, mouseState, transactions, [this,&toReturn](float windowWidth, float windowHeight, GUI::MouseState& mouseState, Vector2 position, std::map<std::string, std::string>& parameters) -> float {
    float height = 0.2f*windowHeight;
    DrawRectangleRounded({ position.x, position.y, 0.9f*windowWidth, height }, 0.2f, SEGMENTS, colors["customLightGray"]);

    textElements["transactionsFrom"].SetBounds({ position.x, position.y, 0.9f*windowWidth, height });
    textElements["transactionsFrom"].SetText(std::string("From: "+parameters["from"]));
    textElements["transactionsFrom"].UpdateAndRender(mouseState);

    textElements["transactionsTo"].SetBounds({ position.x, position.y+(0.05f*windowHeight), 0.9f*windowWidth, height });
    textElements["transactionsTo"].SetText(std::string("To: "+parameters["to"]));
    textElements["transactionsTo"].UpdateAndRender(mouseState);

    textElements["transactionsTo"].SetBounds({ position.x, position.y+(0.075f*windowHeight), 0.9f*windowWidth, height });
    textElements["transactionsTo"].SetText((std::string("Created: "+parameters["timecreated"])));
    textElements["transactionsTo"].UpdateAndRender(mouseState);

    try
    {
      float amount = std::stof(parameters["amount"]);
      if (amount < 0)
        textElements["transactionsAmount"].SetColor(RED);
      else
        textElements["transactionsAmount"].SetColor(GREEN);
    }
    catch (...) { }

    textElements["transactionsAmount"].SetBounds({ position.x, position.y, 0.9f*windowWidth, height });
    textElements["transactionsAmount"].SetText(std::string("Amount: $"+Util::FormatMoney(parameters["amount"])));
    textElements["transactionsAmount"].UpdateAndRender(mouseState);

    const std::string textBefore = buttonElements["viewAccount"].GetText();
    const Rectangle boundsBefore = buttonElements["viewAccount"].GetBounds();

    buttonElements["viewAccount"].SetBounds({ position.x+0.01f*windowWidth, position.y+height-0.03f*windowHeight, 0.1f*windowWidth, 0.02f*windowHeight });
    buttonElements["viewAccount"].SetText("Edit");

    if (buttonElements["viewAccount"].UpdateAndRender(mouseState))
    {
      Database::SetTransactionToUpdate({ parameters["from"], parameters["to"], std::stod(parameters["amount"]), parameters["timecreated"] });
      m_LoadData = true;
      toReturn = 8; // Edit transaction
    }

    buttonElements["viewAccount"].SetBounds(boundsBefore);
    buttonElements["viewAccount"].SetText(textBefore);

    return height;
  });

  // If edit was pressed go to 'Edit Transaction Page' 
  if (toReturn != -1)
    return toReturn;

  RenderChat(windowWidth, windowHeight, mouseState);

  return 0;
}

int MainPage::RenderSavings(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  textElements["goalsHeader"].UpdateAndRender(mouseState);

  if (buttonElements["addGoals"].UpdateAndRender(mouseState))
  {
    m_LoadData = true;
    DrawRectangle(0, 0, windowWidth, windowHeight, THEME_COLOR);
    DeInit();
    return 6; // Add goals page
  }

  std::vector<std::map<std::string, std::string>> goals = m_Goals;

  if (!m_Goals.size())
  {
    const std::string prev = textElements["noAccounts"].GetText();
    textElements["noAccounts"].SetText("No goals created! Create one!");
    textElements["noAccounts"].UpdateAndRender(mouseState);
    textElements["noAccounts"].SetText(prev); // Restore previous state
  	RenderChat(windowWidth, windowHeight, mouseState);
		return 0;
  }
 
  // Render all goals
  scrollAreaElements["goals"].UpdateAndRender(windowWidth, windowHeight, mouseState, m_Goals, [this](float windowWidth, float windowHeight, GUI::MouseState& mouseState, Vector2 position, std::map<std::string, std::string>& parameters) -> float {
    float height = 0.2f*windowHeight;
    float roudness = 0.2f;
    if (parameters["expanded"] == "1")
    {
      height = 0.4f*windowHeight;
      roudness = 0.1f;
    }

    DrawRectangleRounded({ position.x, position.y, 0.9f*windowWidth, height }, roudness, SEGMENTS, colors["customLightGray"]);

    textElements["goalName"].SetBounds({ position.x, position.y, 0.9f*windowWidth, 0.1f*windowHeight });
    textElements["goalName"].SetText(parameters["name"]);
    textElements["goalName"].UpdateAndRender(mouseState);

    textElements["goalAmount"].SetBounds({ position.x, position.y, 0.9f*windowWidth, 0.1f*windowHeight });
    textElements["goalAmount"].SetText(TextFormat("Price: $%s", Util::FormatMoney(std::to_string(std::stod(parameters["amount"]))).c_str()));
    textElements["goalAmount"].UpdateAndRender(mouseState);

    const std::string priority = parameters["priority"];
    if (priority == "High")
      textElements["goalPriority"].SetColor(RED);
    else if (priority == "Medium")
      textElements["goalPriority"].SetColor(YELLOW);
    else
      textElements["goalPriority"].SetColor(GREEN);

    textElements["goalPriority"].SetBounds({ position.x, position.y+0.05f*windowHeight, 0.9f*windowWidth, 0.1f*windowHeight });
    textElements["goalPriority"].SetText(std::string("Priority: "+priority));
    textElements["goalPriority"].UpdateAndRender(mouseState);

    textElements["goalCreated"].SetBounds({ position.x, position.y+0.08f*windowHeight, 0.9f*windowWidth, 0.1f*windowHeight });
    textElements["goalCreated"].SetText(std::string("Created: "+parameters["created"]));
    textElements["goalCreated"].UpdateAndRender(mouseState);

    textElements["goalCreated"].SetBounds({ position.x, position.y+0.11f*windowHeight, 0.9f*windowWidth, 0.1f*windowHeight });
    textElements["goalCreated"].SetText(std::string("Duration: "+parameters["yearsToComplete"]+" years, and "+parameters["monthsToComplete"]+" months"));
    textElements["goalCreated"].UpdateAndRender(mouseState);

    // Reuse account button to avoid creating too many UI elements
    const std::string viewBefore = buttonElements["viewAccount"].GetText();
    buttonElements["viewAccount"].SetBounds({ position.x+0.01f*windowWidth, position.y+height-0.03f*windowHeight, 0.1f*windowWidth, 0.02f*windowHeight });
    if (parameters["expanded"] == "1") 
      buttonElements["viewAccount"].SetText("View Less");
    else
      buttonElements["viewAccount"].SetText("View More");

    if (buttonElements["viewAccount"].UpdateAndRender(mouseState))
      parameters["expanded"] = parameters["expanded"] == "0" ? "1" : "0";

    buttonElements["viewAccount"].SetText(viewBefore);

    const std::string deleteBefore = buttonElements["deleteAccount"].GetText();
    buttonElements["deleteAccount"].SetBounds({ position.x+0.02f*windowWidth+0.1f*windowWidth, position.y+height-0.03f*windowHeight, 0.1f*windowWidth, 0.02f*windowHeight });
    buttonElements["deleteAccount"].SetText("Delete Goal");

    if (buttonElements["deleteAccount"].UpdateAndRender(mouseState)) 
    {
      m_VerifyAccountDelete = true;
      m_GoalToRemove = parameters;
      animations["verifyAccountDelete"].Play();
    }

    buttonElements["deleteAccount"].SetText(deleteBefore);

    static constexpr float weeksPerMonth = 4.34524f;
    const double months = std::stod(parameters["monthsToComplete"]);
    const double remainingAmount = std::stod(parameters["amount"])-std::stod(parameters["payedOff"]);
    const double totalWeeks = (52.0f*std::stod(parameters["yearsToComplete"]))+(weeksPerMonth*months);

    const double interestRate = 1+(std::stod(parameters["interestAmount"])/100.0f);
    const double interestInterval = std::stod(parameters["interestPeriod"]);

    const double totalInterestPeriods = totalWeeks/interestInterval;
    const double futureBalance = remainingAmount*std::pow(interestRate, totalInterestPeriods);

    textElements["accountsAmount"].SetBounds({ position.x, position.y+0.05f*windowHeight, 0.9f*windowWidth, 0.1f*windowHeight });
    textElements["accountsAmount"].SetText(TextFormat("Amount left (after interest): $%s", Util::FormatMoney(std::to_string(futureBalance)).c_str()));
    textElements["accountsAmount"].UpdateAndRender(mouseState);

    // Specific options only in expanded mode
    if (parameters["expanded"] == "1")
    {
      textElements["goalInterestPeirod"].SetBounds({ position.x, position.y+height*0.4f, 0.9f*windowWidth, 0.1f*windowHeight });
      textElements["goalInterestPeirod"].SetText(TextFormat("Interest period %d week(s)", std::stoi(parameters["interestPeriod"])));
      textElements["goalInterestPeirod"].UpdateAndRender(mouseState);
      
      textElements["goalInterestAmount"].SetBounds({ position.x, position.y+height*0.45f, 0.9f*windowWidth, 0.1f*windowHeight });
      textElements["goalInterestAmount"].SetText(TextFormat("Interest amount %.2f%%", std::stod(parameters["interestAmount"])));
      textElements["goalInterestAmount"].UpdateAndRender(mouseState);

      textElements["goalPayedOff"].SetBounds({ position.x, position.y+height*0.5f, 0.9f*windowWidth, 0.1f*windowHeight });
      textElements["goalPayedOff"].SetText(TextFormat("Amount payed off $%.2f", std::stod(parameters["payedOff"])));
      textElements["goalPayedOff"].UpdateAndRender(mouseState);

      // Calculate when your next payment should be, and how much it should be
      const uint16_t twoWeekPayments = totalWeeks/2;
      const uint16_t fourWeekPayments = totalWeeks/4;
      const uint16_t sixWeekPayments = totalWeeks/6;
    
      const double twoWeekPayment = futureBalance/twoWeekPayments;
      const double fourWeekPayment = futureBalance/fourWeekPayments;
      const double sixWeekPayment = futureBalance/sixWeekPayments;

      textElements["goalCreated"].SetColor(GREEN);
      textElements["goalCreated"].SetBounds({ position.x, position.y+height*0.6f, 0.9f*windowWidth, 0.1f*windowHeight });
      textElements["goalCreated"].SetText(TextFormat("Pay $%s every 2 weeks for %d payments OR", Util::FormatMoney(std::to_string(twoWeekPayment)).c_str(), twoWeekPayments));
      textElements["goalCreated"].UpdateAndRender(mouseState);

      const std::string fourWeekDisplay = (months > 1) ? TextFormat("Pay $%s every 4 weeks for %d payments OR)", Util::FormatMoney(std::to_string(fourWeekPayment)).c_str(), fourWeekPayments) : TextFormat("Pay $%s every 4 weeks for %d payments", Util::FormatMoney(std::to_string(fourWeekPayment)).c_str(), fourWeekPayments);
      textElements["goalCreated"].SetBounds({ position.x, position.y+height*0.64f, 0.9f*windowWidth, 0.1f*windowHeight });
      textElements["goalCreated"].SetText(fourWeekDisplay);
      textElements["goalCreated"].UpdateAndRender(mouseState);

      if (std::stod(parameters["monthsToComplete"]) > 1)
      {
        textElements["goalCreated"].SetBounds({ position.x, position.y+height*0.68f, 0.9f*windowWidth, 0.1f*windowHeight });
        textElements["goalCreated"].SetText(TextFormat("Pay $%s every 6 weeks for %d payments", Util::FormatMoney(std::to_string(sixWeekPayment)).c_str(), sixWeekPayments));
        textElements["goalCreated"].UpdateAndRender(mouseState);
      }

      textElements["goalCreated"].SetColor(WHITE); // Reset the color
    }

    if (std::stod(parameters["payedOff"]) >= std::stod(parameters["amount"]))
    {
      textElements["goalFinished"].SetBounds({ position.x, (position.y+height/2.0f)-0.04f*windowHeight, 0.9f*windowWidth, 0.1f*windowHeight });
      textElements["goalFinished"].UpdateAndRender(mouseState);
    }

    return height;
  });

  // Render popup
  if (m_VerifyAccountDelete)
  {
    Rectangle bounds = animations["verifyAccountDelete"].GetCurrentBounds();
    popupElements["verifyAccountDelete"].SetBounds(bounds);
    animations["verifyAccountDelete"].Animate();

    int res = popupElements["verifyAccountDelete"].UpdateAndRender(mouseState, windowWidth, windowHeight, [this](const Rectangle& parentBounds, GUI::MouseState& mouseState, float windowWidth, float windowHeight) -> int {
      textElements["areYouSure"].SetBounds(parentBounds);
      textElements["areYouSure"].SetText("Are you sure you want to delete\n\""+std::string(m_GoalToRemove["name"])+"\" goal?");
      textElements["areYouSure"].UpdateAndRender(mouseState);

      buttonElements["yes"].SetBounds({ parentBounds.x+(0.01f*windowWidth), parentBounds.y+parentBounds.height-(0.1f*windowHeight), 0.1f*windowWidth, 0.05f*windowHeight });
      buttonElements["no"].SetBounds({ parentBounds.x+parentBounds.width-0.11f*windowWidth, parentBounds.y+parentBounds.height-0.1f*windowHeight, 0.1f*windowWidth, 0.05f*windowHeight });

      if (buttonElements["yes"].UpdateAndRender(mouseState))
        return 1;
      if (buttonElements["no"].UpdateAndRender(mouseState))
        return -1;

      return 0;
    });

    if (res == 1)
    {
      Database::RemoveGoal(m_GoalToRemove);
      m_LoadData = true;
      m_VerifyAccountDelete = false;
      animations["verifyAccountDelete"].Reset();
    }
    else if (res == -1)
    {
      m_VerifyAccountDelete = false;
      animations["verifyAccountDelete"].Reset();
    }
  }

  RenderChat(windowWidth, windowHeight, mouseState);

  return 0;
}

int MainPage::RenderReports(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  textElements["reportsHeader"].UpdateAndRender(mouseState);
  
  if (buttonElements["addReports"].UpdateAndRender(mouseState))
  {
    m_LoadData = true;
    DrawRectangle(0, 0, windowWidth, windowHeight, THEME_COLOR);
    DeInit();
    return 7; // Add reports page
  }

  if (!m_Reports.size())
  {
    const std::string before = textElements["noAccounts"].GetText();
    textElements["noAccounts"].SetText("No reports found! Create one!");
    textElements["noAccounts"].UpdateAndRender(mouseState);
    textElements["noAccounts"].SetText(before);
    RenderChat(windowWidth, windowHeight, mouseState);
    return 0;
  }

  // Render all report previews
  scrollAreaElements["reports"].UpdateAndRender(windowWidth, windowHeight, mouseState, m_Reports, [this](float windowWidth, float windowHeight, GUI::MouseState& mouseState, Vector2 position, std::map<std::string, std::string>& parameters) -> float {
    float height = 0.1f*windowHeight; 

    textElements["reportsName"].SetBounds({ position.x, position.y, windowWidth, 0.1f*windowHeight });
    textElements["reportsName"].SetText(parameters["name"]);
    textElements["reportsName"].UpdateAndRender(mouseState);

    textElements["reportsLocation"].SetBounds({ position.x, position.y+0.05f*windowHeight, windowWidth, 0.1f*windowHeight });
    textElements["reportsLocation"].SetText(parameters["path"]);
    textElements["reportsLocation"].UpdateAndRender(mouseState);

    Texture2D report = images[parameters["name"]];
    DrawTexture(report, (0.5f*windowWidth)-(0.5f*report.width), position.y+0.15f*windowHeight, WHITE);

    height += report.height+(0.2f*windowHeight);
    return height;
  });

  RenderChat(windowWidth, windowHeight, mouseState);

  return 0;
}

int MainPage::RenderSettings(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  textElements["settingsHeader"].UpdateAndRender(mouseState);
  textElements["settingsDeleteRecents"].UpdateAndRender(mouseState);

  if (buttonElements["settingsDeleteRecents"].UpdateAndRender(mouseState))
  {
    m_VerifyAccountDelete = true;
    m_DeleteRecents = true;
    animations["verifyAccountDelete"].Play();
  }

  // Delete reports text
  const std::string textBefore = textElements["settingsDeleteRecents"].GetText();
  const Rectangle boundsBefore = textElements["settingsDeleteRecents"].GetBounds();
  textElements["settingsDeleteRecents"].SetText("Delete reports? (This will not remove the PDF's, just the previews) THIS CAN NOT BE UNDONE!");
  textElements["settingsDeleteRecents"].SetBounds({ boundsBefore.x, boundsBefore.y+0.05f*windowHeight, windowWidth, 0.1f*windowHeight });
  textElements["settingsDeleteRecents"].UpdateAndRender(mouseState);

  // Delete reports button
  const Rectangle buttonBoundsBefore = buttonElements["settingsDeleteRecents"].GetBounds();
  const std::string buttonTextBefore = buttonElements["settingsDeleteRecents"].GetText();
  const Color buttonColorBefore = buttonElements["settingsDeleteRecents"].GetBackgroundColor();
  Rectangle newBounds = boundsBefore;
  float x = 0.1f*windowWidth+MeasureTextEx(textElements["settingsDeleteRecents"].GetStyle().font, textElements["settingsDeleteRecents"].GetText().c_str(), textElements["settingsDeleteRecents"].GetStyle().fontSize*GUI::CalculateFontScale(GetMonitorWidth(GetCurrentMonitor())), SPACING).x+(0.025f*windowWidth);
  newBounds.x = x;
  newBounds.y += 0.05f*windowHeight;
  newBounds.width = 0.2f*windowWidth;
  newBounds.height = 0.035f*windowHeight;
  buttonElements["settingsDeleteRecents"].SetBounds(newBounds);

  if (buttonElements["settingsDeleteRecents"].UpdateAndRender(mouseState))
  {
    m_VerifyAccountDelete = true;
    m_DeleteReports = true;
    animations["verifyAccountDelete"].Play();
  }

  // Account backup 
  newBounds.x = 0.125f*windowWidth;
  newBounds.y += 0.05f*windowHeight;
  buttonElements["settingsDeleteRecents"].SetBackgroundColor(BEIGE);
  buttonElements["settingsDeleteRecents"].SetBounds(newBounds);
  buttonElements["settingsDeleteRecents"].SetText("Backup Data");

  if (buttonElements["settingsDeleteRecents"].UpdateAndRender(mouseState))
  {
    const char* filePath = tinyfd_selectFolderDialog("Your backup location", NULL);

    // Create the backup there, just create a folder and copy the contents of all the files
    if (filePath)
    {
      std::string newPath = filePath;
      newPath += "GreenVaultBackup_"+std::to_string(time(NULL));
      std::filesystem::create_directory(newPath);
      
      // Copy all the report .png file previews
      std::string reportPath = newPath+"/reports";
      std::filesystem::create_directory(reportPath);
      std::filesystem::copy("data/reports", reportPath);

      // Copy contents
      std::string userContents;
      std::string goalsContents;
      std::string recurringContents;
      std::string reportsContents;

      // User data
      std::fstream file("data/user.djb");
      std::string line;
      while (std::getline(file, line)) { userContents += line+'\n'; }
      file.close();

      // Goals data
      file = std::fstream("data/goals.djb");
      while (std::getline(file, line)) { goalsContents += line+'\n'; }
      file.close();

      // Recurring data
      file = std::fstream("data/recurring.djb");
      while (std::getline(file, line)) { recurringContents += line+'\n'; }
      file.close();

      // Reports data
      file = std::fstream("data/reports.djb");
      while (std::getline(file, line)) { reportsContents += line+'\n'; }
      file.close();

      // Write that data to the backup
      
      // Write user data
      std::ofstream output(newPath+"/user.djb");
      output << userContents;
      output.close();

      // Write goals data
      output = std::ofstream(newPath+"/goals.djb");
      output << goalsContents;
      output.close();

      // Write recurring data
      output = std::ofstream(newPath+"/recurring.djb");
      output << recurringContents;
      output.close();

      // Write reports data
      output = std::ofstream(newPath+"/reports.djb");
      output << reportsContents;
      output.close();
    }
  }

  textElements["settingsDeleteRecents"].SetText("or");
  newBounds.x += newBounds.width+0.01f*windowWidth;
  textElements["settingsDeleteRecents"].SetBounds(newBounds);
  textElements["settingsDeleteRecents"].UpdateAndRender(mouseState);

  newBounds.x += 0.05f*windowWidth;
  buttonElements["settingsDeleteRecents"].SetBounds(newBounds);
  buttonElements["settingsDeleteRecents"].SetText("Load Backup");

  if (buttonElements["settingsDeleteRecents"].UpdateAndRender(mouseState))
  {
    m_VerifyAccountDelete = true;
    m_RestoreBackup = true;
    animations["verifyAccountDelete"].Play();
  }

  textElements["settingsDeleteRecents"].SetText("Disable chat");
  textElements["settingsDeleteRecents"].SetBounds({ 0.15f*windowWidth, 0.4f*windowHeight, windowWidth, 0.1f*windowHeight });
  textElements["settingsDeleteRecents"].UpdateAndRender(mouseState);

  // Reset properties
  textElements["settingsDeleteRecents"].SetBounds(boundsBefore);
  textElements["settingsDeleteRecents"].SetText(textBefore);
  buttonElements["settingsDeleteRecents"].SetBounds(buttonBoundsBefore);
  buttonElements["settingsDeleteRecents"].SetBackgroundColor(buttonColorBefore);
  buttonElements["settingsDeleteRecents"].SetText(buttonTextBefore);

  // Render popup
  if (m_VerifyAccountDelete)
  {
    Rectangle bounds = animations["verifyAccountDelete"].GetCurrentBounds();
    popupElements["verifyAccountDelete"].SetBounds(bounds);
    animations["verifyAccountDelete"].Animate();

    int res = popupElements["verifyAccountDelete"].UpdateAndRender(mouseState, windowWidth, windowHeight, [this](const Rectangle& parentBounds, GUI::MouseState& mouseState, float windowWidth, float windowHeight) -> int {
      textElements["areYouSure"].SetBounds(parentBounds);
      textElements["areYouSure"].SetText("Are you sure?");
      textElements["areYouSure"].UpdateAndRender(mouseState);

      buttonElements["yes"].SetBounds({ parentBounds.x+(0.01f*windowWidth), parentBounds.y+parentBounds.height-(0.1f*windowHeight), 0.1f*windowWidth, 0.05f*windowHeight });
      buttonElements["no"].SetBounds({ parentBounds.x+parentBounds.width-0.11f*windowWidth, parentBounds.y+parentBounds.height-0.1f*windowHeight, 0.1f*windowWidth, 0.05f*windowHeight });

      if (buttonElements["yes"].UpdateAndRender(mouseState))
        return 1;
      if (buttonElements["no"].UpdateAndRender(mouseState))
        return -1;

      return 0;
    });

    if (res == 1)
    {
      if (m_DeleteRecents)
      {
        std::ofstream file("data/recents.djb");
        file << ""; // Clear the file
        file.close();
      }
      else if (m_DeleteReports)
      {
        std::ofstream file("data/reports.djb");
        file << ""; // Clear the file
        file.close();

        // Remove all the .png's
        if (std::filesystem::exists("data/reports") || std::filesystem::is_directory("data/reports"))
        {
          for (const auto& entry : std::filesystem::directory_iterator("data/reports"))
            std::filesystem::remove_all(entry.path());
        }
      }
      else if (m_RestoreBackup)
      {
        const char* backupDirectory = tinyfd_selectFolderDialog("Load a backup", NULL);

        if (backupDirectory)
        {
          std::string path = backupDirectory;

          // Clear these reports
          for (const auto& report : std::filesystem::directory_iterator("data/reports"))
            std::filesystem::remove(report);

          // Copy backup 
          std::filesystem::copy(path+"/reports", "data/reports");

          // Read all files from backup
          std::string userBackup; 
          std::string goalsBackup;
          std::string recurringBackup;
          std::string recentsBackup;
          std::string reportsBackup;

          // Read user backup
          std::fstream file(path+"/user.djb");
          std::string line;
          while (std::getline(file, line)) { userBackup += line+'\n'; }
          file.close();

          // Read goal backup
          file = std::fstream(path+"/goals.djb");
          while (std::getline(file, line)) { goalsBackup += line+'\n'; }
          file.close();

          // Read recurring backup
          file = std::fstream(path+"/recurring.djb");
          while (std::getline(file, line)) { recurringBackup += line+'\n'; }
          file.close();

          // Read recents backup
          file = std::fstream(path+"/recents.djb");
          while (std::getline(file, line)) { recentsBackup += line+'\n'; }
          file.close();

          // Read reports backup
          file = std::fstream(path+"/reports.djb");
          while (std::getline(file, line)) { reportsBackup += line+'\n'; }
          file.close();

          // Write that data to the local data
          
          // User data
          std::ofstream output("data/user.djb");
          output << userBackup;
          output.close();
      
          // Goal data
          output = std::ofstream("data/goals.djb");
          output << goalsBackup;
          output.close();
          
          // Recurring data
          output = std::ofstream("data/recurring.djb");
          output << recurringBackup;
          output.close();

          // Recents data
          output = std::ofstream("data/recents.djb");
          output << recentsBackup;
          output.close();

          // Reports data
          output = std::ofstream("data/reports.djb");
          output << reportsBackup;
          output.close();

          m_LoadData = true;
        }
      }

      m_DeleteRecents = false;
      m_DeleteReports = false;
      m_RestoreBackup = false;
      m_LoadData = true;
      m_VerifyAccountDelete = false;
      animations["verifyAccountDelete"].Reset();
    }
    else if (res == -1)
    {
      m_VerifyAccountDelete = false;
      animations["verifyAccountDelete"].Reset();
    }
  }

  checkboxElements["chatState"].UpdateAndRender(mouseState);
  bool chatState = checkboxElements["chatState"].GetChecked();
  if (chatState == m_UseChat)
  {
    Database::SetChatSetting(!m_UseChat);
    m_LoadData = true;
  }

  RenderChat(windowWidth, windowHeight, mouseState);

  return 0;
}

int MainPage::RenderHelp(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  textElements["helpHeader"].UpdateAndRender(mouseState);

  // Help options
  static std::vector<std::map<std::string, std::string>> helpOptions = {
    { { "header", "Accounts" }, { "info", "You can view the account along with the type of account. Under that you can see when the account was created and to the top right you can view the account balance. At the bottom you have to options, either to view more, which will take you to the transaction page, automatically filtering all transaction that came from or went to that account. You can easily add accounts by using the 'Add Account' button, and then filling out the information." }, { "image", "accounts" } },
    { { "header", "Add Accounts" }, { "info", "On this page you have a few options, you need to fill out the name of the account, and the type of the account. You name can be whatever you want, and for the type you have to chose from one of the given dropdown options. After that simply hit the create button and you will have your new account setup!" }, { "image", "addAccounts" } },
    { { "header", "Transactions" }, { "info", "Here you can view all of your transactions, and at the top right you have the option to add/create a new transaction. You can manipulate these transactions, they only get deleted when you remove an account associated with it. However if there are two accounts associated with it, and you delete one, the transactions will continue to live for as long as the other account lives." }, { "image", "transactions" } },
    { { "header", "Add Transactions" }, { "info", "On this page you have to specify from where the transaction is coming from and to what it is going to. You can do this either by typing in a custom name, i.e job, gift, etc... or using the dropdown. You have to also specify the amount of the transaction, and if you want you can make the transaction reccuring, i.e it will happen every 2 weeks." }, { "image", "addTransactions" } },
    { { "header", "Goals" }, { "info", "Here you can view all your current goals, there is a lot of information for you to explore here. You can view basic things like name, priority, time created, interest information, amount, completness, etc... There are two different 'modes' for each goal, expanded and not, if it is expanded you can view far more information about it." }, { "image", "goals" } },
    { { "header", "Add Goals" }, { "info", "There is a LOT to go through here, let's get through all the inputs. You have to sepcify the name of the goal, and the amount of the goal, all the rest is optional. You can specify the interest period, in weeks, i.e the interest accumaltes every 8 weeks. You can specify the interest amount, i.e 2.5% interetst. You can specify the downpayment of the item, along with the number of years and months you want to take to reach that goal!" }, { "image", "addGoals" } },
    { { "header", "Reports" }, { "info", "Here you can view all of your report previews, it only shows the first page of each report along with the name, and the saved file location so you can view the full PDF form of the report!" }, { "image", "reports" } },
    { { "header", "Add Reports" }, { "info", "On this page you have to put in the name of the report, then press the button and then select the location you want to save the report to, then hit create!" }, { "image", "addReports" } },
  };

  // Render help options
  scrollAreaElements["helpInformation"].UpdateAndRender(windowWidth, windowHeight, mouseState, helpOptions, [this](float windowWidth, float windowHeight, GUI::MouseState& mouseState, Vector2 position, std::map<std::string, std::string>& parameters) -> float {
    float height = 0.0f; 

    textElements["helpInfoHeader"].SetBounds({ position.x, position.y, 0.9f*windowWidth, 0.1f*windowHeight });
    textElements["helpInfoHeader"].SetText(parameters["header"]);
    textElements["helpInfoHeader"].UpdateAndRender(mouseState);
    height += 0.1f*windowHeight;

    textElements["helpInfoInfo"].SetBounds({ position.x+0.05f*windowWidth, position.y+0.105f*windowHeight, 0.8f*windowWidth, 0.25f*windowHeight });
    textElements["helpInfoInfo"].SetText(parameters["info"]);
    textElements["helpInfoInfo"].UpdateAndRender(mouseState);
    height += 0.25f*windowHeight;
    height += 0.105f*windowHeight;

    Texture2D image = images[parameters["image"]];
    image.width = windowWidth*0.8f;
    image.height = windowHeight*0.8f;
    DrawTexture(image, position.x+0.05f*windowWidth, position.y+height, WHITE);
    height += image.height;

    return height;
  });

  RenderChat(windowWidth, windowHeight, mouseState);

  return 0;
}

int MainPage::Render(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  if (init)
  {
    Init(windowWidth, windowHeight);
    init = false;
  }

  // Unselect all input boxes when mouse pressed
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
  {
    for (auto it = inputElements.begin(); it != inputElements.end(); it++)
      it->second.SetSelected(false);
  }

  if (m_LoadData)
  {
    m_LoadData = false;
    LoadData();
  }

  // Render the info bars first
  TopBar(windowWidth, windowHeight, mouseState);
  LeftBar(windowWidth, windowHeight, mouseState);

  // Render sub-page content
  switch (m_DisplayContent)
  {
    case PageContent::DASHBOARD:
      return RenderDashboard(windowWidth, windowHeight, mouseState);
    case PageContent::ACCOUNTS:
      return RenderAccounts(windowWidth, windowHeight, mouseState);
    case PageContent::TRANSACTIONS:
      return RenderTransactions(windowWidth, windowHeight, mouseState);
    case PageContent::SAVINGS:
      return RenderSavings(windowWidth, windowHeight, mouseState);
    case PageContent::REPORTS:
      return RenderReports(windowWidth, windowHeight, mouseState);
    case PageContent::SETTINGS:
      return RenderSettings(windowWidth, windowHeight, mouseState);
    case PageContent::HELP:
      return RenderHelp(windowWidth, windowHeight, mouseState);
  }
}
