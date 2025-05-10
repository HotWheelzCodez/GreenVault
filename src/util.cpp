#include "../include/util.hpp"

std::string Util::TrimStart(std::string& str)
{
  str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](uint8_t c) {
    return !std::isspace(c);
  }));
  return str;
}

std::string Util::TrimEnd(std::string& str)
{
  str.erase(std::find_if(str.rbegin(), str.rend(), [](uint8_t c) {
    return !std::isspace(c);
  }).base(), str.end());
  return str;
}

std::string Util::Trim(std::string& str)
{
  TrimStart(str);
  TrimEnd(str);
  return str;
}

std::string Util::Uppercase(std::string& str)
{
  if (isupper(str[0]))
    return str;

  str[0] = toupper(str[0]);
  return str;
}

char Util::Uppercase(char& ch)
{
  if (isupper(ch))
    return ch;

  ch = toupper(ch);
  return ch;
}

// Split the string, ensure the splits are valid, then make sure each split is the right relative size for an email
bool Util::IsValidEmailFormat(const std::string& email)
{
  size_t atPos = email.find('@');
  size_t dotPos = email.find('.', atPos);
  return (atPos != std::string::npos && dotPos != std::string::npos && atPos > 0 && dotPos > atPos+1 && dotPos+2 < email.length());
}

// Ensures that the data is valid, expect YYYY/MM/DD format
bool Util::IsValidDate(const std::string& date) 
{
  std::istringstream iss(date);
  std::string token;
  std::vector<int> parts;

  // Parse that date parts
  while (std::getline(iss, token, '/')) 
  {
    try 
    {
      parts.push_back(std::stoi(token));
    } 
    catch (...) 
    {
      return false;
    }
  }

  if (parts.size() != 3)
    return false;

  int year = parts[0];
  int month = parts[1];
  int day = parts[2];

  if (year < 1901 || year > 2038)
    return false;
  if (month < 1 || month > 12)
    return false;
  if (day < 1 || day > 31)
    return false;

  // Create that unix timestamp time, and check that against our time
  tm timeinfo = {};
  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = month - 1;
  timeinfo.tm_mday = day;

  mktime(&timeinfo);

  if (timeinfo.tm_year != year - 1900 || timeinfo.tm_mon != month - 1 || timeinfo.tm_mday != day)
    return false;

  return true;
}

// Convert a YYYY/MM/DD format into a unixt timestamp
time_t Util::GetUnixTimestamp(const std::string& date) 
{
  std::istringstream iss(date);
  std::string token;
  int year;
  int month;
  int day;

  std::getline(iss, token, '/');
  year = std::stoi(token);

  std::getline(iss, token, '/');
  month = std::stoi(token);

  std::getline(iss, token, '/');
  day = std::stoi(token);

  tm timeinfo = {};
  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = month - 1;
  timeinfo.tm_mday = day;
  timeinfo.tm_hour = 0;
  timeinfo.tm_min = 0;
  timeinfo.tm_sec = 0;
  timeinfo.tm_isdst = -1;

  return mktime(&timeinfo);
}

// Filters out ',', and '$' charachters
std::string Util::GetMoney(std::string input)
{
  std::string money;
  for (const char& c : input)
  {
    if (c == ',' || c == '$')
      continue;
    money += c;
  }
  return money;
}

// Adds commas to the money to make it easier to read
std::string Util::FormatMoney(std::string input)
{
  size_t count = 0;
  std::string money = "00.";
  for (int i = input.find('.')-1; i >= 0; i--)
  {
    if (count != 0 && input[i] != '-' && count % 3 == 0)
      money += ',';

    money += input[i];
    count++;
  }
  reverse(money.begin(), money.end());
  return money;
}
