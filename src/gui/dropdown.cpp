#include "../../include/core/gui.hpp"

GUI::Dropdown::Dropdown(Rectangle bounds, GUI::DropdownStyle style, const std::vector<std::string>& options, float windowWidth, float windowHeight) noexcept
  : m_Bounds(std::move(bounds)), m_Style(std::move(style)), m_Options(std::move(options))
{
  GUI::ScrollAreaStyle scrollContainerStyle = { m_Style.backgroundColor, TRANSPARENT, m_Style.backgroundColor, TRANSPARENT, { 0 }, 0, 2.0f };
  m_ScrollContianer = GUI::ScrollArea({ bounds.x, bounds.y+bounds.height, bounds.width, 0.2f*windowHeight }, scrollContainerStyle);

  // Convert options to scroll area options
  for (std::string option : m_Options)
  {
    std::map<std::string, std::string> temp;
    temp["option"] = option;
    m_ScrollOptions.push_back(temp);
  }

  GUI::TextStyle displayTextStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, m_Style.font, m_Style.fontSize, GUI::TEXT_ALIGNMENT_CENTER, { 0 } };
  m_DisplayText = GUI::Text({ 0 }, displayTextStyle, ""); // Set properties when rendered

  m_SelectedOption = m_Options[0];
}

std::string GUI::Dropdown::GetSelectedOption(void) noexcept
{
  return m_SelectedOption;
}

void GUI::Dropdown::SetElements(const std::vector<std::string>& elements) noexcept
{
  m_ScrollOptions.clear();
  for (std::string option : elements)
  {
    std::map<std::string, std::string> temp;
    temp["option"] = option;
    m_ScrollOptions.push_back(temp);
  }
}

void GUI::Dropdown::UpdateAndRender(float windowWidth, float windowHeight, GUI::MouseState& mouseState) noexcept
{
  Color backgroundColor = m_Style.backgroundColor;
  Color textColor = m_Style.textColor;

  if (CheckCollisionPointRec(mouseState.position, m_Bounds))
  {
    mouseState.cursor = MOUSE_CURSOR_POINTING_HAND;

    backgroundColor = m_Style.hoverBackgroundColor;
    textColor = m_Style.hoverTextColor;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
      m_Selected = !m_Selected;
      mouseState.clicked = true;
    }
  }

  DrawRectangleRec(m_Bounds, backgroundColor);

  {
    Rectangle bounds = m_Bounds;
    bounds.y += (m_Bounds.height/2.0f)-(m_Style.fontSize/2.0f);
    m_DisplayText.SetBounds(bounds);
    m_DisplayText.SetText(m_SelectedOption);
    m_DisplayText.UpdateAndRender(mouseState);
  }

  if (m_Selected)
  {
    m_ScrollContianer.UpdateAndRender(windowWidth, windowHeight, mouseState, m_ScrollOptions, [this](float windowWidth, float windowHeight, GUI::MouseState& mouseState, Vector2 position, std::map<std::string, std::string> parameters) -> float {
      float itemHeight = 0.05f*windowHeight;

      Color backgroundColor = m_Style.backgroundColor;
      Color textColor = m_Style.textColor;

      if (CheckCollisionPointRec(mouseState.position, { position.x, position.y, m_Bounds.width, itemHeight }))
      {
        mouseState.cursor = MOUSE_CURSOR_POINTING_HAND;
        backgroundColor = m_Style.hoverBackgroundColor;
        textColor = m_Style.hoverTextColor;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
          m_SelectedOption = parameters["option"];
          m_Selected = false;
          mouseState.clicked = true;
        }
      }

      DrawRectangle(position.x, position.y, m_Bounds.width, itemHeight, backgroundColor);

      m_DisplayText.SetBounds({ position.x, position.y+(m_Bounds.height/2.0f)-(m_Style.fontSize/2.0f), m_Bounds.width, itemHeight });
      m_DisplayText.SetText(parameters["option"]);
      m_DisplayText.UpdateAndRender(mouseState);

      return itemHeight;
    });
  }
  else
  {
    m_ScrollContianer.ResetScroll();
  }
}
