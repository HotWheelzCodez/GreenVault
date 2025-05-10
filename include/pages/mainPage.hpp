#pragma once /* MAIN_PAGE_HPP */

#include "page.hpp"

#include <sstream>
#include <iomanip>
#include <filesystem>

// Main Page class - derives Page class
class MainPage : public Page
{
public:
  MainPage() = default;
  MainPage(float windowWidth, float windowHeight);
  ~MainPage() = default;

  // Returns 0 if the page did not change, else return the page number
  int Render(float windowWidth, float windowHeight, GUI::MouseState& mouseState) override;
  void Init(float windowWidth, float windowHeight) override; 

private:
  // Private helper methods for render sub-pages and or components
  void LoadData();
  void TopBar(float windowWidth, float windowHeigh, GUI::MouseState& mouseState);
  void LeftBar(float windowWidth, float windowHeight, GUI::MouseState& mouseState);
  void RenderChat(float windowWidth, float windowHeight, GUI::MouseState& mouseState);

  int RenderDashboard(float windowWidth, float windowHeight, GUI::MouseState& mouseState);
  int RenderAccounts(float windowWidth, float windowHeight, GUI::MouseState& mouseState);
  int RenderTransactions(float windowWidth, float windowHeight, GUI::MouseState& mouseState);
  int RenderSavings(float windowWidth, float windowHeight, GUI::MouseState& mouseState);
  int RenderReports(float windowWidth, float windowHeight, GUI::MouseState& mouseState);
  int RenderSettings(float windowWidth, float windowHeight, GUI::MouseState& mouseState);
  int RenderHelp(float windowWidth, float windowHeight, GUI::MouseState& mouseState);

private:
  // Store sub-page state
  enum class PageContent : uint8_t
  {
    DASHBOARD,
    ACCOUNTS,
    TRANSACTIONS,
    SAVINGS,
    REPORTS,
    SETTINGS,
    HELP
  };

  // Store side panel buttons
  enum class Buttons : uint8_t
  {
    ACCOUNTS,
    TRANSACTIONS,
    SAVINGS,
    REPORTS,
    SETTINGS,
    HELP
  };

  // Page state, to control information presentation and loading
  Database::User m_User;
  std::vector<std::map<std::string, std::string>> m_Accounts;
  std::vector<std::map<std::string, std::string>> m_Transactions;
  std::vector<std::map<std::string, std::string>> m_Goals;
  std::vector<std::map<std::string, std::string>> m_Reports;

  Database::Account m_AccountToRemove;
  std::map<std::string, std::string> m_GoalToRemove;

  std::vector<Database::Action> m_RecentActions;

  std::string m_FormatedName;

  std::string m_AccountQuery;
  std::string m_TransactionQuery;

  bool m_LoadData = false;
  bool m_DeleteRecents = false;
  bool m_DeleteReports = false;
  bool m_RestoreBackup = false;
  bool m_ShowChat = false;
  bool m_UseChat = true;

  bool m_VerifyAccountDelete = false;

  double m_TotalAmount = 0.0f;
  std::string m_TotalAmountDisplay;

  PageContent m_DisplayContent = PageContent::DASHBOARD;
};
