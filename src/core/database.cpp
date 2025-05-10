#include "../../include/core/database.hpp"
#include <sstream>

// Helper functions
// --------------------------------------------------

// Used to write all data, user, accounts, and transactions
void WriteData(const Database::User& user, const std::vector<Database::Account>& accounts, const std::vector<Database::Transaction>& transactions)
{
  std::ofstream file("data/user.djb");
  file << "User\n";
  file << "FirstName=" << user.firstName << '\n';
  file << "LastName=" << user.lastName << '\n';
  file << "Username=" << user.username << '\n';
  file << "Email=" << user.email << '\n';
  file << "Password=" << user.password << '\n';
  file << "Accounts\n";
  for (const Database::Account& account : accounts)
  {
    file << "Name=" << account.name << '\n';
    file << "Type=" << account.type << '\n';
    file << "Amount=" << std::to_string(account.amount) << '\n';
    file << "TimeCreated=" << account.timeCreated << '\n';
  }
  file << "Transactions\n";
  for (const Database::Transaction& transaction : transactions)
  {
    if (transaction.from == "")
      continue;

    file << "From=" << transaction.from << '\n';
    file << "To=" << transaction.to << '\n';
    file << "Amount=" << std::to_string(transaction.amount) << '\n';
    file << "TimeCreated=" << transaction.timeCreated << '\n';
  }
  file.close();
}

// Used to write recent action data
void WriteRecents(const std::vector<Database::Action>& actions)
{
  std::ofstream file("data/recents.djb");

  for (const Database::Action& action : actions)
  {
    std::string type;
    switch (action.type)
    {
      case Database::ActionType::ACCOUNT: type = "Account"; break;
      case Database::ActionType::TRANSACTION: type = "Transaction"; break;
      case Database::ActionType::GOAL: type = "Goal"; break;
    }
    std::string added = (action.added) ? "true" : "false";
    
    file << "Type=" << type << '\n';
    file << "Name=" << action.name << '\n';
    file << "Added=" << added << '\n';
    file << "Amount=" << action.amount << '\n';
  }

  file.close();
}

// Used to write goal data
void WriteGoals(std::vector<std::map<std::string, std::string>>& goals)
{
  std::ofstream file("data/goals.djb");
  for (std::map<std::string, std::string>& goal : goals)
    file << goal["name"] << ',' << goal["amount"] << ',' << goal["interestPeriod"] << ',' << goal["interestAmount"] << ',' << goal["initialSavings"] << ',' << goal["priority"] << ',' << goal["yearsToComplete"] << ',' << goal["monthsToComplete"] << ',' << goal["payedOff"] << ',' << goal["created"] << '\n';
}

void Database::Init()
{
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
}

void Database::Close()
{
  curl_easy_cleanup(curl);
}

Database::ServerResponse Database::GetServerResponse(const std::string& url)
{
  std::string response;

  // Set curl options
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

  // Make the request and validate it
  CURLcode res = curl_easy_perform(curl);
  bool success = false;
  if (res == CURLE_OK)
    success = true;

  // Return the response with the status
  ServerResponse serverResponse;
  serverResponse.success = success;
  serverResponse.response = response;
  return serverResponse;
}

Database::UserStatus Database::AddUser(const User& user)
{
  // Check if local file exists 
  {
    std::fstream dataFile("data/user.djb");
    if (!dataFile.bad() && dataFile.is_open())
      return UserStatus::IS_YOU;
  }

  std::string encryptedPassword;
  // Check if server is running, and if user exists in remote database
  {
    ServerResponse serverHealth = GetServerResponse("http://127.0.0.1:8080/health");
    if (serverHealth.success && serverHealth.response == "server_is_up")
    {
      ServerResponse userStatus = GetServerResponse("http://127.0.0.1:8080/user-status?username="+user.username);
      if (userStatus.success && userStatus.response == "exists")
        return UserStatus::EXISTS;

      ServerResponse addUser = GetServerResponse("http://127.0.0.1:8080/add-user?first_name="+user.firstName+"&last_name="+user.lastName+"&username="+user.username+"&email="+user.email+"&password="+user.password);
      if (addUser.success)
      {
        const std::string& response = addUser.response;

        if (response == "")
          return UserStatus::FAILURE;

        encryptedPassword = response;
      }
    }
  }

  // Write the given user data to a local file
  std::fstream dataFile("data/user.djb", std::ios::out);
  if (dataFile.bad() || !dataFile.is_open())
    return UserStatus::FAILURE;

  dataFile << "FirstName=" << user.firstName << '\n';
  dataFile << "LastName=" << user.lastName << '\n';
  dataFile << "Username=" << user.username << '\n';
  dataFile << "Email=" << user.email << '\n';
  dataFile << "Password=" << encryptedPassword << "\n";
  dataFile << "Accounts\nTransactions";

  return UserStatus::SUCCESS;
}

std::vector<Database::Action> Database::GetActions()
{
  std::vector<Action> actions;
  Action temp;
  std::fstream file("data/recents.djb");
  std::string line;

  // Parse file
  while (std::getline(file, line))
  {
    std::string field = line.substr(0, line.find('='));
    std::string value = line.substr(line.find('=')+1, line.length());

    if (field == "Type")
    {
      if (temp.name != "")
        actions.push_back(temp);

      if (value == "Account")
        temp.type = ActionType::ACCOUNT;
      else if (value == "Transaction")
        temp.type = ActionType::TRANSACTION;
      else if (value == "Goal")
        temp.type = ActionType::GOAL;
    }
    else if (field == "Name")
    {
      temp.name = value;
    }
    else if (field == "Added")
    {
      temp.added = (value == "true") ? true : false;
    }
    else if (field == "Amount")
    {
      temp.amount = std::stod(value);
    }
  }

  if (temp.name != "")
    actions.push_back(temp);

  return actions;
}

void Database::AddAccount(const std::string &name, const std::string &type)
{
  time_t now = time(NULL);
  tm* converted = localtime(&now);
  char buffer[50];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", converted);
  std::string timeCreated(buffer);

  Account account = { name, type, 0.0f, timeCreated };

  User user = GetUser();
  std::vector<Account> accounts = GetAccounts();
  std::vector<Transaction> transactions = GetTransactions();

  accounts.push_back(account);

  std::vector<Action> recentAction = GetActions();
  Action action = { ActionType::ACCOUNT, account.name, true, account.amount };
  recentAction.push_back(action);

  WriteData(user, accounts, transactions);
  WriteRecents(recentAction);
}

void Database::AddTransaction(const std::string& from, const std::string& to, double amount)
{
  time_t now = time(NULL);
  tm* converted = localtime(&now);
  char buffer[50];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", converted);
  std::string timeCreated(buffer);

  Transaction transaction = { from, to, amount, timeCreated };

  User user = GetUser();
  std::vector<Account> accounts = GetAccounts();
  std::vector<Transaction> transactions = GetTransactions();

  transactions.push_back(transaction);

  std::vector<Action> recentAction = GetActions();
  Action action = { ActionType::TRANSACTION, transaction.from+" "+transaction.to, true, transaction.amount };
  recentAction.push_back(action);

  // Update account(s) balance(s) to match the new transaction
  for (Account& account : accounts)
  {
    if (transaction.from == account.name)
      account.amount -= transaction.amount;
    else if (transaction.to == account.name)
      account.amount += transaction.amount;
  }

  // Update goals
  std::vector<std::map<std::string, std::string>> goals = GetGoalsMap();
  for (std::map<std::string, std::string>& goal : goals)
  {
    if (goal["name"]+" (Goal)" == transaction.to)
      goal["payedOff"] = std::to_string(std::stod(goal["payedOff"])+transaction.amount);
  }

  WriteData(user, accounts, transactions);
  WriteRecents(recentAction);
  WriteGoals(goals);
}

void Database::AddGoal(const std::string& name, double amount, int interestPeriod, float interestAmount, double initialSavings, int yearsToComplete, int monthsToComplete, const std::string& priority)
{
  time_t now = time(NULL);
  tm* converted = localtime(&now);
  char buffer[50];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", converted);
  std::string timeCreated(buffer);

  std::fstream file("data/goals.djb", std::ios::app);
  file << name << ',' << amount << ',' << interestPeriod << ',' << interestAmount << ',' << initialSavings << ',' << priority << ',' << yearsToComplete << ',' << monthsToComplete << ',' << initialSavings << ',' << timeCreated << '\n';
  file.close();

  std::vector<Action> recentAction = GetActions();
  Action action = { ActionType::GOAL, name, true, amount };
  recentAction.push_back(action);

  WriteRecents(recentAction);
}

void Database::AddRecurring(const std::string& from, const std::string& to, uint8_t period, double amount, uint8_t startingAmount)
{
  time_t now = time(NULL);
  std::string displayTime = std::to_string(now);

  std::fstream file("data/recurring.djb", std::ios::app);
  file << from << ',' << to << ',' << std::to_string(amount) << ',' << displayTime << ',' << std::to_string(period) << ',' << std::to_string(startingAmount) << '\n';
  file.close();
}

void Database::RemoveAccount(const Account& account)
{
  std::vector<Account> accounts = GetAccounts();

  // Accumlate accounts to remove
  auto removeAccounts = std::remove_if(accounts.begin(), accounts.end(), [&](Account& storedAccount) -> bool {
    return storedAccount == account;
  });

  // Remove them
  if (removeAccounts != accounts.end())
    accounts.erase(removeAccounts, accounts.end());

  const User& user = GetUser();
  std::vector<Transaction> transactions = GetTransactions();

  // Remove all transactions that come from the ONLY this account
  std::vector<Transaction> transactionsToRemove;
  for (const Transaction& transaction : transactions)
  {
    if (transaction.from == account.name)
    {
      // Check if the 'to' field points to a valid account
      bool valid = false;
      for (const Account& acc : accounts)
      {
        if (transaction.to == acc.name)
        {
          valid = true;
          break;
        }
      }
      if (valid)
        continue;

      transactionsToRemove.push_back(transaction);
    }
    else if (transaction.to == account.name)
    {
      // Check if the 'from' field points to a valid account
      bool valid = false;
      for (const Account& acc : accounts)
      {
        if (transaction.from == acc.name)
        {
          valid = true;
          break;
        }
      }
      if (valid)
        continue;

      transactionsToRemove.push_back(transaction);
    }
  }

  // Accumlate transactions to remove
  auto removeTransactions = std::remove_if(transactions.begin(), transactions.end(), [&](const Transaction& transaction) {
    return std::find(transactionsToRemove.begin(), transactionsToRemove.end(), transaction) != transactionsToRemove.end();
  });

  // Remove them
  transactions.erase(removeTransactions, transactions.end());

  std::vector<Action> recentActions = GetActions();
  Action action = { ActionType::ACCOUNT, account.name, false, account.amount };
  recentActions.push_back(action);

  WriteData(user, accounts, transactions);
  WriteRecents(recentActions);
}

void Database::RemoveGoal(std::map<std::string, std::string>& goal)
{
  // Get and remove goals
  std::vector<std::map<std::string, std::string>> goals = GetGoalsMap();
  
  auto removeGoals = std::remove_if(goals.begin(), goals.end(), [&](std::map<std::string, std::string>& storedGoal) -> bool {
    return storedGoal["created"] == goal["created"];
  });

  if (removeGoals != goals.end())
    goals.erase(removeGoals, goals.end());

  // Write to file the modyfied
  std::ofstream file("data/goals.djb");
  WriteGoals(goals);
  file.close();

  std::vector<Action> actions = GetActions();
  Action action = { ActionType::GOAL, goal["name"], false, std::stod(goal["amount"]) };
  actions.push_back(action);

  WriteRecents(actions);
}

Database::UserStatus Database::AuthUser(const std::string& username, const std::string& password)
{
  // Check if local file exists
  std::fstream dataFile("data/user.djb");
  if (!dataFile.bad() && dataFile.is_open())
  {
    std::string line;
    while (std::getline(dataFile, line))
    {
      size_t deliminator = line.find('=');
      if (deliminator == std::string::npos)
        continue;

      std::string field = line.substr(0, deliminator);
      if (field == "Password")
      {
        // Get and decrypt the password
        std::string value = line.substr(deliminator+1, line.length());
        ServerResponse serverResponse = GetServerResponse("http://127.0.0.1:8080/decrypt?encrypted_text="+value);
        if (serverResponse.success)
        {
          const std::string& decryptedPassword = serverResponse.response;
          if (decryptedPassword == password)
            return UserStatus::VALID;

          return UserStatus::WRONG_PASSWORD;
        }
      }
      else if (field == "Username")
      {
        std::string value = line.substr(deliminator+1, line.length());
        if (username != value)
          return UserStatus::NOT_EXIST;
      }
    }
  }

  ServerResponse serverResponse = GetServerResponse("http://127.0.0.1:8080/auth-user?username="+username+"&password="+password);
  if (serverResponse.success)
  {
    const std::string& response = serverResponse.response;
    if (response == "user_valid")
      return UserStatus::VALID;
    if (response == "password_incorrect")
      return UserStatus::WRONG_PASSWORD;
    if (response == "user_does_not_exist")
      return UserStatus::NOT_EXIST;
  }

  return UserStatus::FAILURE;
}

Database::User Database::GetUser(void)
{
  User user;

  // Parse the file
  std::fstream file("data/user.djb");
  std::string line;
  while (std::getline(file, line))
  {
    if (line == "User")
      continue;

    std::string field = line.substr(0, line.find('='));
    std::string value = line.substr(line.find('=')+1, line.length());

    if (field == "FirstName")
      user.firstName = value;
    else if (field == "LastName")
      user.lastName = value;
    else if (field == "Username")
      user.username = value;
    else if (field == "Email")
      user.email = value;
    else if (field == "Password")
      user.password = value;
  }

  return user;
}

std::vector<Database::Account> Database::GetAccounts(void)
{
  std::vector<Database::Account> accounts; 
  Account temp;

  // Parse the file
  std::fstream file("data/user.djb");
  std::string line;
  bool parse = false;
  while (std::getline(file, line))
  {
    if (line == "Transactions")
    {
      if (temp.name.empty())
        return accounts;

      accounts.push_back(temp);
      break;
    }

    if (line == "Accounts") 
    {
      parse = true;
      continue;
    }

    if (parse)
    {
      std::string field = line.substr(0, line.find('='));
      std::string value = line.substr(line.find('=')+1, line.length());

      if (field == "Name")
      {
        if (temp.name != "")
        {
          accounts.push_back(temp);
        }

        temp.name = value;
        continue;
      }
      if (field == "Type")
      {
        temp.type = value;
        continue;
      }
      if (field == "Amount")
      {
        try
        {
          double amount = std::stof(value);
          temp.amount = amount;
        }
        catch (...) { }
        continue;
      }
      if (field == "TimeCreated")
      {
        temp.timeCreated = value;
      }
    }
  }

  return accounts;
}

std::vector<Database::Transaction> Database::GetTransactions(void)
{
  std::vector<Database::Transaction> transactions; 
  Transaction temp;

  // Parse the file
  std::fstream file("data/user.djb");
  std::string line;
  bool parse = false;
  while (std::getline(file, line))
  {
    if (line == "Transactions") 
    {
      parse = true;
      continue;
    }

    if (parse)
    {
      std::string field = line.substr(0, line.find('='));
      std::string value = line.substr(line.find('=')+1, line.length());

      if (field == "From")
      {
        if (temp.from != "")
          transactions.push_back(temp);

        temp.from = value;
        continue;
      }
      if (field == "To")
      {
        temp.to = value;
        continue;
      }
      if (field == "Amount")
      {
        try
        {
          double amount = std::stof(value);
          temp.amount = amount;
        }
        catch (...) { }
        continue;
      }
      if (field == "TimeCreated")
      {
        temp.timeCreated = value;
      }
    }
  }

  transactions.push_back(temp);
  return transactions;
}

std::vector<std::map<std::string, std::string>> Database::GetAccountsMap(void)
{
  std::vector<std::map<std::string, std::string>> accounts;
  std::map<std::string, std::string> temp;

  // Parse the file
  std::fstream file("data/user.djb");
  std::string line;
  bool parse = false;
  while (std::getline(file, line))
  {
    if (line == "Transactions")
    {
      if (temp["name"].empty())
        return accounts;

      accounts.push_back(temp);
      break;
    }

    if (line == "Accounts") 
    {
      parse = true;
      continue;
    }

    if (parse)
    {
      // No account information
      if (line == "Transactions")
        return accounts;

      std::string field = line.substr(0, line.find('='));
      std::string value = line.substr(line.find('=')+1, line.length());

      if (field == "Name")
      {
        if (!temp["name"].empty())
        {
          accounts.push_back(temp);
          temp.clear();
        }
      }

      for (char& ch : field)
        ch = tolower(ch);

      temp[field] = value;
    }
  }

  return accounts;
}

std::vector<std::map<std::string, std::string>> Database::GetTransactionsMap(void)
{
  std::vector<std::map<std::string, std::string>> transactions;
  std::map<std::string, std::string> temp;

  // Parse the file
  std::fstream file("data/user.djb");
  std::string line;
  bool parse = false;
  while (std::getline(file, line))
  {
    if (line == "Transactions") 
    {
      parse = true;
      continue;
    }

    if (parse)
    {
      std::string field = line.substr(0, line.find('='));
      std::string value = line.substr(line.find('=')+1, line.length());

      if (field == "From")
      {
        const auto& it = temp.begin();
        if (it->second != "")
        {
          transactions.push_back(temp);
          temp.clear();
        }
      }

      for (char& ch : field)
        ch = tolower(ch);

      temp[field] = value;
    }
  }

  if (!Util::Trim(temp["from"]).empty())
    transactions.push_back(temp);
  return transactions;
}

std::vector<std::map<std::string, std::string>> Database::GetGoalsMap(void)
{
  std::vector<std::map<std::string, std::string>> goals;

  std::fstream file("data/goals.djb");
  if (file.bad() || !file.is_open())
    return goals;

  // Parse the file
  std::string line;
  while (std::getline(file, line))
  {
    std::istringstream iss(line);
    std::string part;

    std::map<std::string, std::string> goal;
    uint8_t count = 0;
    while (std::getline(iss, part, ','))
    {
      switch (count)
      {
        case 0: goal["name"] = part; break;
        case 1: goal["amount"] = part; break;
        case 2: goal["interestPeriod"] = part; break;
        case 3: goal["interestAmount"] = part; break;
        case 4: goal["initialSavings"] = part; break;
        case 5: goal["priority"] = part; break;
        case 6: goal["yearsToComplete"] = part; break;
        case 7: goal["monthsToComplete"] = part; break;
        case 8: goal["payedOff"] = part; break;
        case 9: goal["created"] = part; break;
      }
      count++;
    }

    goal["expanded"] = "0"; // Flag for GUI code to know wheither to expand the panel for more information
    goals.push_back(goal);
  }

  return goals;
}

std::vector<std::map<std::string, std::string>> Database::GetRecurringTransactions(void)
{
  // Local scope struct to make life a bit nicer
  struct Recurring
  {
    std::string from;
    std::string to;
    double amount;
    time_t start;
    uint8_t occurence;
    uint8_t occured;
  };

  std::vector<Recurring> recurrings;

  std::vector<std::map<std::string, std::string>> newTransactions;
  std::fstream file("data/recurring.djb");

  if (file.bad() || !file.is_open())
  {
    std::cerr << "ERROR: Failed to open 'data/recurring.djb'!" << std::endl;
    return newTransactions;
  }

  // Parse the file
  bool updated = false;
  std::string line;
  while (std::getline(file, line))
  {
    Recurring recurring;

    // Parse by comma
    std::istringstream iss(line);
    std::string part;
    uint8_t count = 0;
    while (std::getline(iss, part, ','))
    {
      switch (count)
      {
        case 0: recurring.from = part; break;
        case 1: recurring.to = part; break;
        case 2:
          try
          {
            double numAmount = std::stod(part);
            recurring.amount = numAmount;
          } 
          catch (...) { }
          break;
        case 3: 
          try
          {
            time_t time = std::stoll(part); 
            recurring.start = time;
          }
          catch (...) { }
          break;
        case 4: 
          try
          {
            uint8_t numOccurence = std::stoi(part);
            recurring.occurence = numOccurence; 
          } 
          catch (...) { }
          break;
        case 5:
          try
          {
            uint8_t numOccured = std::stoi(part);
            recurring.occured = numOccured;
          }
          catch (...) { }
          break;
      }
      count++;
    }

    // Check if time has exceeded and if you need to apply the transaction
    time_t week = 60*60*24*7;
    time_t now = time(NULL);
    time_t diff = now-recurring.start;
    if (diff > (recurring.occured*week)+(recurring.occurence*week))
    {
      std::map<std::string, std::string> transaction = { { "from", recurring.from }, { "to", recurring.to }, { "amount", std::to_string(recurring.amount), }, { "timecreated", std::to_string(recurring.start) } };
      newTransactions.push_back(transaction);
      recurring.occured++;
      updated = true;
    }

    recurrings.push_back(recurring);
  }
  file.close();

  if (!updated)
    return std::vector<std::map<std::string, std::string>>(); // If we have not updated anything, no need to write to file

  std::ofstream output("data/recurring.djb");
  if (output.bad() || !output.is_open())
  {
    std::cerr << "ERROR: Failed to open 'data/recurring.djb'!" << std::endl;
    return std::vector<std::map<std::string, std::string>>(); // If we could not update the file, return no new transactions
  }

  // Rewrite the new data
  for (const Recurring& recurring : recurrings)
    output << recurring.from << ',' << recurring.to << ',' << recurring.amount << ',' << recurring.start << ',' << recurring.occurence << ',' << recurring.occured << '\n';

  return newTransactions;
}

void Database::SetTransactionToUpdate(const Transaction transaction)
{
  transactionToUpdate = transaction;
}

Database::Transaction Database::GetTransactionToUpdate(void)
{
  Transaction copy = transactionToUpdate;
  return copy;
}

void Database::UpdateTransaction(const std::string& from, const std::string& to, double amount)
{
  std::vector<Transaction> transactions = GetTransactions();
  std::vector<Account> accounts = GetAccounts();
  User user = GetUser();
  for (Transaction& transaction : transactions)
  {
    if (transaction == transactionToUpdate)
      transaction = { from, to, amount, transaction.timeCreated };
  }
  WriteData(user, accounts, transactions);
}

std::string Database::GetAIMessage(const std::string& userMessage)
{
  std::string aiMessage;
  messages.push_back(userMessage);

  // Get the api key
  ServerResponse serverResponse = GetServerResponse("http://127.0.0.1:8080/get-api-key");
  const std::string apiKey = (serverResponse.success) ? serverResponse.response : "";
  if (apiKey.empty())
    return apiKey;

  static const std::string apiUrl = "https://api.openai.com/v1/chat/completions";

  // Create / load the HTTP headers for openai chat client
  struct curl_slist* headers = NULL;
  char authHeader[256];
  sprintf(authHeader, "Authorization: Bearer %s", apiKey.c_str());
  headers = curl_slist_append(headers, authHeader);
  headers = curl_slist_append(headers, "Content-Type: application/json");
  
  // Build the conversation history prompt
  static const std::string prompt = "You are an IT help desk professional, helping someone use an application called GreenVault. Keep your answer short, simple, sweet and clean, but explain everything in enough detail, NO MORE THAN 40 WORDS. Ensure everything is in one line. Do not use markdown format, just simple response, like you are a human. The Application has a login page, which is the first page you see, from their you can input your credentials to get the main page, or go to the signup page, where you have to input your first name, last name, username, email (only for account recovery if needed), password, and password again for verification. There is a link under both submit buttons on the login page and signup page to take you to the other one. On the main page, you can view your total account balance at the top right. On the left you have six options, Accounts - where you can view all your accounts, each account section has a view button that takes you to transactions and automaticly filters it, you can also add an account with Add Account button at the top right, along with a red button on each transaction to delete it; Transactions - this allows you to view all your transactions, where they came from, to, amount, etc... (same with accounts), you can also add a transaction with the Add Transaction button at the top, and you edit each transaction using the edit button for each transaction in case you made a mistake; Goals - this lets you view and manage your goals, if you hit the expanded button on a goal it will show you different pay options to pay off that goal, again there is a add goal button at the top right; Reports - this page lets you view previews of all your reports, as well as an add report button at the top right. Then there is a settings page which allows you to remove cached data, such as recents actions and or reports (it will not delete the generated pdf files, just the preview png images), you can also create and or load backup files; Help - last page is a help page that allows you to see a description alonog with an image of every page telling you how to use it. Add Account Page - input the name of the account, and the account type, then hit the add button at the bottom; Add Transactions Page - input where it comes from, either via the dropdown where you can select an account, or chose custom, where you can then type in a custom name, i.e job, gift, etc... next to that you have to select where it is going to, following the same rules, below that you chose the amount of the transacation, and at the top you can chose how often that transaction recurrs, i.e 1-52 weeks; Add Goals Page - You have to input a goal name, and a goal amount, the rest is optional, the other optional ones are interest period in weeks, interest amount in percent, initial savings / downpayment, goal priority (defualt high, has medium, and low), the years to complete (defualts to zero), the months to complete defaults to 1; Create Report Page - You have to input a report name and use the Save File Location button to chose where to save it, optionally you can chose if you want a spending graph with the checkbox, same with the goal graph, along with chosing a time frame, you can input the start date in yyyy/mm/dd format, same with the end date.\n\n";

  // Setup the json body
  std::vector<nlohmann::json> jsonMessages;
  nlohmann::json jsonPrompt;
  jsonPrompt["role"] = "system";
  jsonPrompt["content"] = prompt;
  jsonMessages.push_back(jsonPrompt);
  for (const std::string& message : messages)
  {
    nlohmann::json jsonMessage;
    jsonMessage["role"] = "user";
    jsonMessage["content"] = message;
    jsonMessages.push_back(jsonMessage);
  }

  nlohmann::json body;
  body["model"] = "gpt-4o";
  body["max_tokens"] = 200;
  body["messages"] = jsonMessages; 
  std::string jsonBody = body.dump();

  // Set curl options
  curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody.c_str());

  // Get curl response
  std::string response;
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK)
    return "";

  // Process ouput and get message ouput
  nlohmann::json jsonResponse = nlohmann::json::parse(response);
  aiMessage = jsonResponse["choices"][0]["message"]["content"];
  messages.push_back(aiMessage);

  curl_slist_free_all(headers);
  curl_easy_reset(curl);
  return aiMessage;
}

bool Database::GetChatSetting(void)
{
  std::fstream file("data/settings.djb");
  if (file.bad() || !file.is_open())
    return true; // Default value to have it on

  std::string line;
  while (std::getline(file, line))
  {
    size_t deliminator = line.find('=');
    if (deliminator == std::string::npos)
      return true; // Default value to have it on

    const std::string key = line.substr(0, deliminator);
    const std::string value = line.substr(deliminator+1, line.length());

    if (key == "Chat")
      return std::stoi(value);
  }

  return true; // Default value to have it on
}

void Database::SetChatSetting(bool chatState)
{
  std::fstream file("data/settings.djb");
  if (file.bad() || !file.is_open())
    return;

  std::string contents = "";
  std::string line;
  while (std::getline(file, line))
  {
    size_t deliminator = line.find('=');
    if (deliminator == std::string::npos)
      return;

    const std::string key = line.substr(0, deliminator);
    const std::string value = line.substr(deliminator+1, line.length());

    if (key == "Chat")
      contents += "Chat="+(chatState ? std::string("1\n") : std::string("0\n"));
    else
      contents += line+'\n';
  }
  file.close();

  std::ofstream output("data/settings.djb");
  output << contents;
  output.close();
}

std::vector<std::string> Database::GetMessages(void)
{
  std::vector<std::string> copy = messages;
  return copy;
}

bool Database::SyncOnline(void)
{
  ServerResponse serverResponse = GetServerResponse("http://127.0.0.1:8080/sync");

  return true;
}

size_t Database::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
  size_t totalSize = size * nmemb;
  output->append((char*)contents, totalSize);
  return totalSize;
}
