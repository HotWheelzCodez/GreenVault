#pragma once /* GUI_HPP */

#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <functional>
#include <algorithm>

#include "raylib.h"

/* GUI namespace - start 
 * 
 * This namespace will be used to manage all the needed GUI funictionalty,
 * this will only be used in the 2d render stage of the game
 * */
namespace GUI
{
  /* MACROS & VARIABLES */

  #define SEGMENTS 10 // Predefined number of segments for any rounded object
  #define SPACING 1   // Set space in between characters when renderin text

  #undef GREEN
  #define GREEN CLITERAL(Color){ 145, 237, 157, 255 }
  #define THEME_COLOR { 25, 25, 55, 255 }
  #define THEME_COLOR_LIGHT { 60, 60, 95, 255 }

  #define TRANSPARENT { 0, 0, 0, 0 } // Transparent color
  #define SEA_GREEN { 159, 212, 174, 255 }

  extern float fontScale;

  /* STRUCTURES & ENUMS */

  // Used to store the state of the mouse per frame
  struct MouseState
  {
    Vector2 position;
    bool clicked;
    MouseCursor cursor;
  };

  // Used to represent text alignment, for other GUI components
  enum TextAlignments : uint8_t
  {
    TEXT_ALIGNMENT_LEFT = 0,
    TEXT_ALIGNMENT_CENTER,
    TEXT_ALIGNMENT_RIGHT,
  };

  // Properties for styling GUI Text component
  struct TextStyle
  {
    Color   backgroundColor;
    Color   textColor;
    Color   backgroundHoverColor;
    Color   textHoverColor;
    Font    font;
    float   fontSize;
    int     textAlignment;
    Vector2 padding;
  };

  // Properties for styling GUI Button component
  struct ButtonStyle
  {
    Color baseBackgroundColor;
    Color baseTextColor;
    Color baseOutlineColor;
    Color hoverBackgroundColor;
    Color hoverTextColor;
    Color hoverOutlineColor;
    Font  font;
    float fontSize;
    int   textAlignment;
    float roundness;
    float outlineThickness;
    float outlineDistance;
    bool  outlineFill;
    float hoverScale;
  };

  // Properties for styling GUI Input component
  struct InputStyle
  {
    Color baseBackgroundColor;
    Color baseOutlineColor;
    Color baseTextColor;
    Color basePlaceholderColor;
    Color hoverBackgroundColor;
    Color hoverOutlineColor;
    Color hoverTextColor;
    Color hoverPlaceholderColor;
    Color selectedBackgroundColor;
    Color selectedOutlineColor;
    Color selectedTextColor;
    Color highlightColor;
    Font  font;
    float fontSize;
    float roundness;
    float outlineThickness;
    float outlineDistance;
    bool  outlineFill;
    bool  isPassword;
  };

  // Properties for styling GUI ScrollArea component
  struct ScrollAreaStyle
  {
    Color   backgroundColor;
    Color   scrollbarColor;
    Color   backgroundHoverColor;
    Color   scrollbarHoverColor;
    Vector2 padding;
    float   roundness;
    float   scrollSpeed;
  };

  // Properties for styling GUI Dropdown component
  struct DropdownStyle
  {
    Color backgroundColor;
    Color textColor;
    Color hoverBackgroundColor;
    Color hoverTextColor;
    Font  font;
    float fontSize;
    int   textAlignment; 
  };

  // Properties for styling GUI Checkbox component
  struct CheckboxStyle
  {
    Color backgroundColor;
    Color outlineColor;
    Color checkColor;
    float roundness;
  };

  /* CLASSES */

  // Text class, GUI Text component, used for rendering styled text element
  class Text
  {
  public:
    // Empty default constructor
    Text(void) = default; 
    // Normal constructore for initalizing GUI Text component
    Text(Rectangle bounds, GUI::TextStyle style, const std::string& text) noexcept; 

    // Updates GUI text component as needed, than renders it
    float UpdateAndRender(GUI::MouseState& mouseState) noexcept;

    // Allows you to change GUI text component properties
    void SetText(const std::string& text) noexcept;
    void SetBounds(Rectangle bounds) noexcept;
    void SetColor(Color color) noexcept;
    void SetAlignment(GUI::TextAlignments alignment) noexcept;

    GUI::TextStyle GetStyle(void) noexcept;
    std::string GetText(void) noexcept;
    Rectangle GetBounds(void) noexcept;
    float GetHeight(int offset) noexcept;

  private:
    Rectangle m_Bounds;
    GUI::TextStyle m_Style;
    std::vector<std::string> m_TextLines;
    std::string m_OriginalText;

  private:
    void WrapText(std::string& text) noexcept;
  };

  // Button class, GUI Button component, used for rendering styled button element
  class Button
  {
  public:
    // Empty default constructor
    Button(void) = default;
    // Normal constructore for initalizing GUI Button component
    Button(Rectangle bounds, GUI::ButtonStyle style, const std::string& text) noexcept;

    // Updates GUI button component as needed, than renders it
    bool UpdateAndRender(GUI::MouseState& mouseState) noexcept;
    bool UpdateAndRender(GUI::MouseState& mouseState, const Texture2D& texture) noexcept;

    // Allows you to change GUI button component properties
    void SetText(const std::string& text) noexcept;
    void SetBounds(Rectangle bounds) noexcept;
    void SetBackgroundColor(Color color) noexcept;

    std::string GetText(void) const noexcept;
    Rectangle GetBounds(void) const noexcept;
    Color GetBackgroundColor(void) const noexcept;

  private:
    Rectangle m_Bounds;
    GUI::ButtonStyle m_Style;
    std::string m_Text;
  };

  // Input class, GUI Input component, used for rendering styled input element
  class Input
  {
  public:
    // Empty default constructor
    Input(void) = default;
    // Normal constructor for initalizing GUI Input component
    Input(Rectangle bounds, GUI::InputStyle style, const std::string& placeholderText) noexcept;

    // Updates GUI input component as needed, than renders it
    void UpdateAndRender(GUI::MouseState& mouseState) noexcept;

    std::string GetText(void) noexcept;
    bool IsHidden(void) noexcept;

    Rectangle GetBounds(void) noexcept;

    // Allows you to change GUI input component properties
    void SetPlaceholderText(const std::string& placeholderText) noexcept;
    void SetSelected(bool selected) noexcept;
    void InverseShowPassword(void) noexcept;
    void SetText(const std::string& text) noexcept;

  private:
    Rectangle m_Bounds;
    GUI::InputStyle m_Style;
    std::string m_PlaceholderText;
    std::string m_InputText;
    std::string m_HighlightText;
    Vector2 m_HighlightBounds;
    size_t m_HighlightStart;
    size_t m_CursorPosition;
    int m_XOffset;
    bool m_Selected;
    float m_TimeWaited;
    float m_KeyWaited;
    bool m_ShowPassword;

  private:
    // Will draw the input cursor
    void DrawCursor(void) noexcept;
    // Updates input cursor position based off user input
    void UpdateCursorPosition(GUI::MouseState& mouseState) noexcept;
    // Helper string functions
    void ReplaceAll(std::string& str, const char toReplace, const char replaceWith) const noexcept;
    [[nodiscard]] size_t FindLeftOf(const std::string& str, const char toFind, size_t startIndex) const noexcept;
    [[nodiscard]] size_t FindRightOf(const std::string& str, const char toFind, size_t startIndex) const noexcept;
  };

  // ScrollArea class, GUI ScrollArea component, used for rendering styled scroll area element
  class ScrollArea
  {
  public:
    // Empty default constructor
    ScrollArea(void) = default;
    // Normal constructore for initalizing GUI ScrollArea component
    ScrollArea(Rectangle bounds, GUI::ScrollAreaStyle style) noexcept;

    // Updates GUI scroll area component as needed, than renders it
    void UpdateAndRender(float windowWidth, float windowHeight, GUI::MouseState& mouseState, std::vector<std::map<std::string, std::string>>& info, const std::function<float(float windowWidth, float windowHeight, GUI::MouseState& MouseState, Vector2 position, std::map<std::string, std::string>& parameters)>& renderItem) noexcept;

    // Helper functions for calculation render offsets
    [[nodiscard]] Vector2 GetOffset(void) noexcept;
    [[nodiscard]] float GetYOffset(void) noexcept;
    [[nodiscard]] float GetXOffset(void) noexcept;

    // Allows you to change GUI scroll area component properties
    void SetBounds(Rectangle bounds) noexcept;

    void ResetScroll(void) noexcept;

  private:
    Rectangle m_Bounds;
    GUI::ScrollAreaStyle m_Style;
    Vector2 m_Offset;
    float m_Height;
  };

  // Dropdown class, GUI Dropdown component, used for rendering a dropdown selection
  class Dropdown
  {
  public:
    // Empty default constructor
    Dropdown(void) = default;
    // Normal constructor for initalizing GUI Dropdown component
    Dropdown(Rectangle bounds, GUI::DropdownStyle style, const std::vector<std::string>& options, float windowWidth, float windowHeight) noexcept;

    // Updates the selected option and renders the dropdown
    void UpdateAndRender(float windowWidth, float windowHeight, GUI::MouseState& mouseState) noexcept;

    // Functions to get and or dynamicly change component

    [[nodiscard]] std::string GetSelectedOption(void) noexcept;

    void SetElements(const std::vector<std::string>& elements) noexcept;

  private:
    Rectangle m_Bounds;
    GUI::DropdownStyle m_Style;
    GUI::Text m_DisplayText;
    std::vector<std::string> m_Options;
    std::vector<std::map<std::string, std::string>> m_ScrollOptions;
    std::string m_SelectedOption;
    std::string m_Display;
    GUI::ScrollArea m_ScrollContianer;
    bool m_Selected = false;
  };

  /* FUNCTIONS */

  // Calculates the font scale based off the monitor width and target resolution
  inline float CalculateFontScale(int monitorWidth) noexcept
  {
    fontScale = monitorWidth/1920.0f;
    return fontScale;
  }

  // Popup class, GUI Popup component, used for rendering a small window, that thus renders other GUI components
  class Popup
  {
  public:
    // Empty default constructor
    Popup(void) = default;
    // Normal constructor for initalizing GUI Popup component 
    Popup(Rectangle bounds, Color windowColor) noexcept;

    // Returns some 'flag' for the status of the popup window
    int UpdateAndRender(GUI::MouseState& mouseState, float windowWidt, float windowHeight, const std::function<int(const Rectangle& parentBounds, GUI::MouseState& mouseState, float windowWidth, float windowHeight)>& render) noexcept;

    // Functions to get and or dynamicly change component
    
    void SetBounds(const Rectangle& bounds) noexcept;

  private:
    Rectangle m_Bounds;
    Color m_WindowColor;
    std::string title;
  };

  // Checkbox class, GUI Checkbox component, used for rendering a GUI checkbox
  class Checkbox
  {
  public:
    // Empty default constructor
    Checkbox(void) = default;
    // Normal constructor for initalizing GUI Checkbox component
    Checkbox(Rectangle bounds, GUI::CheckboxStyle style) noexcept;

    void UpdateAndRender(GUI::MouseState& mouseState) noexcept;

    bool GetChecked(void) noexcept;

    void SetChecked(bool checkState) noexcept;

  private:
    Rectangle m_Bounds;
    GUI::CheckboxStyle m_Style;
    bool m_Checked;

  private:
    inline void DrawCheckmark(void) const noexcept
    {
      DrawLineEx({ m_Bounds.x+m_Bounds.width/4, m_Bounds.y+m_Bounds.height/2 }, { m_Bounds.x+m_Bounds.width/2, m_Bounds.y+m_Bounds.height*0.75f }, 2, m_Style.checkColor);
      DrawLineEx({ m_Bounds.x+m_Bounds.width/2, m_Bounds.y+m_Bounds.height*0.75f }, { m_Bounds.x+m_Bounds.width*0.75f, m_Bounds.y+m_Bounds.height/4 }, 2, m_Style.checkColor);
    }
  };
} /* GUI namespace - end */
/* GUI_HPP */
