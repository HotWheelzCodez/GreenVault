#pragma once /* UTIL_HPP */

#include <string>
#include <algorithm>
#include <stdint.h>
#include <vector>
#include <sstream>

/* Namespace Util
*
* Used for formating, processing, and manipulating strings, etc...
* and validing certain inputs
*/
namespace Util
{
  std::string TrimStart(std::string& str);
  std::string TrimEnd(std::string& str);
  std::string Trim(std::string& str);

  bool IsValidEmailFormat(const std::string& email);
  bool IsValidDate(const std::string& date);

  time_t GetUnixTimestamp(const std::string& date);

  std::string Uppercase(std::string& str);
  char Uppercase(char& ch);

  std::string GetMoney(std::string input);
  std::string FormatMoney(std::string input);
}
