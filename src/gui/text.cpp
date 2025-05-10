#include "../../include/core/gui.hpp"

// Base constructor to initialize private fields, and store each line seperatly, to allow for proper formating later 
GUI::Text::Text(Rectangle bounds, GUI::TextStyle style, const std::string& text) noexcept
  : m_Bounds(std::move(bounds)), m_Style(std::move(style))
{
  m_OriginalText = text;
  std::string tempText = text;
  if (text.empty())
    return;
  WrapText(tempText);

  std::string line;
  for (const char& c : tempText)
  {
    // If new line char, then append current line to the lines vector
    if (c == '\n') 
    {
      m_TextLines.push_back(line);
      line.clear(); // Clear the current line, then skip to next char
      continue;
    }
    line += c; 
  }
  m_TextLines.push_back(line); // Append the final line
}

void GUI::Text::SetText(const std::string& text) noexcept
{
  if (text == m_OriginalText)
    return;

  m_OriginalText = text;

  std::string tempText = text;
  WrapText(tempText);
  m_TextLines.clear();

  std::string line;
  for (const char& c : tempText)
  {
    // If new line char, then append current line to the lines vector
    if (c == '\n') 
    {
      m_TextLines.push_back(line);
      line.clear(); // Clear the current line, then skip to next char
      continue;
    }
    line += c; 
  }
  m_TextLines.push_back(line); // Append the final line
}

void GUI::Text::SetBounds(Rectangle bounds) noexcept
{
  m_Bounds = bounds;
}

void GUI::Text::SetColor(Color color) noexcept
{
  m_Style.textColor = color;
  m_Style.textHoverColor = color;
}

void GUI::Text::SetAlignment(GUI::TextAlignments alignment) noexcept
{
  m_Style.textAlignment = alignment;
}

std::string GUI::Text::GetText(void) noexcept
{
  std::string ret;
  for (const std::string& line : m_TextLines)
    ret += line + '\n';
  return ret;
}

GUI::TextStyle GUI::Text::GetStyle(void) noexcept
{
  return m_Style;
}

Rectangle GUI::Text::GetBounds(void) noexcept
{
  return m_Bounds;
}

float GUI::Text::GetHeight(int offset) noexcept
{
  return (m_Style.fontSize*fontScale)*(m_TextLines.size()-1);
}

// Used to convert text into an array of text based off text bounds
void GUI::Text::WrapText(std::string& text) noexcept
{
  float scaleFactor = (m_Style.fontSize/(float)m_Style.font.baseSize)*GUI::CalculateFontScale(GetMonitorWidth(GetCurrentMonitor()));
  int lineWidth = 0; // Stores the text width
  int tempWidth = 0; // Stores the temp width without style property calculations
  int byteCount = 0; // Stores the number of bytes iterated over
  std::vector<std::string> wrappedLines;
  
  size_t start = 0;
  size_t currentLineStart = 0;

  for (size_t i = 0; i < text.length(); ++i)
  {
    byteCount++;

    // Get the curretn letter
    int next;
    const char letter = GetCodepointNext(text.c_str()+i, &next);
    int index = GetGlyphIndex(m_Style.font, letter);

    i += next-1; 

    if (m_Style.font.glyphs[index].advanceX)
      tempWidth += m_Style.font.glyphs[index].advanceX;
    else
      tempWidth += m_Style.font.recs[index].width + m_Style.font.glyphs[index].offsetX;

    lineWidth = (tempWidth*scaleFactor+(float)((byteCount+10)*SPACING));

    // Add the new line to the array of lines if it has exceeded the bounds of the width
    if (lineWidth >= m_Bounds.width)
    {
      wrappedLines.push_back(text.substr(currentLineStart, i-currentLineStart));
      currentLineStart = i+1;
      tempWidth = 0;
      lineWidth = 0;
      byteCount = 0;
    }
  }

  // Add the last line
  if (currentLineStart < text.length())
    wrappedLines.push_back(text.substr(currentLineStart));

  // Reset the text to reflect the array of lines
  text.clear();
  for (const auto& line : wrappedLines)
  {
    if (!text.empty())
      text += '\n';
    text += line;
  }
}

float GUI::Text::UpdateAndRender(GUI::MouseState& mouseState) noexcept
{
  // Calculate font size from font scale, set base properties before user input
  float fontSize = m_Style.fontSize * GUI::fontScale;
  Color textColor = m_Style.textColor;
  Color backgroundColor = m_Style.backgroundColor;

  // Check if mouse hover
  if (CheckCollisionPointRec(mouseState.position, m_Bounds))
  {
    // Update text properties
    textColor = m_Style.textHoverColor;
    backgroundColor = m_Style.backgroundHoverColor;
  }

  // Draw background
  DrawRectangleRec(m_Bounds, backgroundColor);
  // Draw all the text lines
  int yPos = m_Bounds.y;
  for (const std::string& line : m_TextLines)
  {
    // Draw line based off text alignment
    switch (m_Style.textAlignment)
    {
      case TEXT_ALIGNMENT_LEFT:
        DrawTextEx(m_Style.font, line.c_str(), { m_Bounds.x+m_Style.padding.x, (float)yPos+m_Style.padding.y }, fontSize, SPACING, textColor);
        break;
      case TEXT_ALIGNMENT_CENTER:
        DrawTextEx(m_Style.font, line.c_str(), { m_Bounds.x+(m_Bounds.width/2)-(MeasureTextEx(m_Style.font, line.c_str(), fontSize, SPACING).x/2)+m_Style.padding.x, (float)yPos+m_Style.padding.y }, fontSize, SPACING, textColor);
        break;
      case TEXT_ALIGNMENT_RIGHT:
        DrawTextEx(m_Style.font, line.c_str(), { m_Bounds.x+m_Bounds.width-5-MeasureTextEx(m_Style.font, line.c_str(), fontSize, SPACING).x+m_Style.padding.x, (float)yPos+m_Style.padding.y }, fontSize, SPACING, textColor);
        break;
    }
    yPos += fontSize;
  }

  return yPos-m_Bounds.y;
}
