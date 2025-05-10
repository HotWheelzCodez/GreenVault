#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>

#include "../vendor/json.hpp"
#include "../util.hpp"

// Used to avoid conflicts between win32 api and raylib
#if defined(_WIN32)
  #define WIN32_LEAN_AND_MEAN
  #define NOGDI
  #define NOUSER
  #define NOMINMAX
  #define NOSERVICE
  #define NOHELP
  #define NOMB
  #define NOKANJI
  #define NOCRYPT
  #define NOIME
  #define NOMCX
  #define NORPC
  #define NOTAPE
  #define NOWH
  #define NOKERNEL
  #define NONLS
#endif

#include <curl/curl.h>

#if defined(_WIN32)
  #undef near
  #undef far
#endif

/* Namespace Database
* 
* Used to interact with the local 'database', aka just storing local files
* as well as communicating with the HTTP Java server running in the background
* and managin any other network requests via curl, such as openai requests
*/
namespace Database
{
  // Different user status for controlling user authentication
  enum class UserStatus : uint8_t
  {
    NOT_EXIST,
    VALID,
    IS_YOU,
    EXISTS,
    WRONG_PASSWORD,
    SUCCESS, // Used for adding user
    FAILURE, // Used for adding user
  };

  // Different base actions the user can perform
  enum class ActionType : uint8_t
  {
    TRANSACTION,
    ACCOUNT,
    GOAL
  };

  // User information
  struct User
  {
    std::string firstName;
    std::string lastName;
    std::string username;
    std::string email;
    std::string password;
  };

  // Account information
  struct Account
  {
    std::string name;
    std::string type;
    double amount;
    std::string timeCreated;

    // Compares dates to check if the are the exact same
    bool operator==(const Account& other)
    {
      return (this->timeCreated == other.timeCreated);
    }
  };

  // Transaction information
  struct Transaction
  {
    std::string from;
    std::string to;
    double amount;
    std::string timeCreated;

    // Compares dates to check if the are the exact same
    bool operator==(const Transaction& other)
    {
      return (this->timeCreated == other.timeCreated);
    }
  };

  // General application state data
  struct Data
  {
    User user;
    std::vector<Account> accounts;
    std::vector<Transaction> transactions;
  };

  // Represents a user's action
  struct Action
  {
    ActionType type;
    std::string name;
    bool added;
    double amount;
  };

  // Used for validating and getting curl responses
  struct ServerResponse
  {
    std::string response;
    bool success;
  };

  // Used when editing a transaction to know which one to change
  static Transaction transactionToUpdate;

  // Used for messages between the user and the openai chat client
  static std::vector<std::string> messages;

  // Will initalize curl
  void Init(void);

  // Will deinitalize curl
  void Close(void);

  // Get a curl response from a url
  ServerResponse GetServerResponse(const std::string& url);

  // Get user data from file locally
  User GetUser(void);

  // Get account data from file locally
  std::vector<Account> GetAccounts(void);

  // Get transaction data from file locally
  std::vector<Transaction> GetTransactions(void);

  // Get account data from file locally - in the form of a map for easy dynamic display use
  std::vector<std::map<std::string, std::string>> GetAccountsMap(void);

  // Get transation data from file locally - in the form of a map for easy dynamic display use
  std::vector<std::map<std::string, std::string>> GetTransactionsMap(void);

  // Get goals data from file locally - in the form of a map for easy dynamic display use
  std::vector<std::map<std::string, std::string>> GetGoalsMap(void);

  // Get recurring transaction data from file locally - in the form of a map for easy dynamic display use
  std::vector<std::map<std::string, std::string>> GetRecurringTransactions(void);

  // Get actions data from file locally
  std::vector<Action> GetActions();

  // Add a user locally and remotly if able to
  UserStatus AddUser(const User& user); 

  // Check locally, then remotly
  UserStatus AuthUser(const std::string& username, const std::string& password);

  // Add's account locally and remotaly if it can
  void AddAccount(const std::string& name, const std::string& type);

  // Add's transaction locally and remotaly if it can
  void AddTransaction(const std::string& from, const std::string& to, double amount);

  // Add's goal locally and remotaly if it can
  void AddGoal(const std::string& name, double amount, int interestPeriod, float interestAmount, double initialSavings, int yearsToComplete, int monthsToComplete, const std::string& priority);

  // Add's goal only locally
  void AddRecurring(const std::string& from, const std::string& to, uint8_t period, double amount, uint8_t startingAmount);

  // Removes account locally and remotly
  void RemoveAccount(const Account& account);

  // Removes goal locally and remotly
  void RemoveGoal(std::map<std::string, std::string>& goal);

  // Used when editing transaction
  void SetTransactionToUpdate(const Transaction transaction);

  // Used to get the chat setting
  bool GetChatSetting(void);

  // Used to set the chat setting
  void SetChatSetting(bool chatState);

  // Used to get openai chat client response
  std::string GetAIMessage(const std::string& userMessage);
  
  // Used to get the messages between user and openai chat client
  std::vector<std::string> GetMessages(void);

  // Used when editing transaction
  Transaction GetTransactionToUpdate(void);

  // Used when editing transaction
  void UpdateTransaction(const std::string& from, const std::string& to, double amount);

  // Synchronize locally stored data with the remote database
  bool SyncOnline(void);

  // Used for processing curl requests
  size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);

  // Single curl instance
  static CURL* curl;
};
