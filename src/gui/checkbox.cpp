#include "../../include/core/gui.hpp"

GUI::Checkbox::Checkbox(Rectangle bounds, GUI::CheckboxStyle style) noexcept
  : m_Bounds(bounds), m_Style(style), m_Checked(false) { }

void GUI::Checkbox::UpdateAndRender(GUI::MouseState& mouseState) noexcept
{
  Color bgColor = m_Style.backgroundColor;
  Color propColor = m_Style.checkColor;

  if (CheckCollisionPointRec(mouseState.position, m_Bounds))
  {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      m_Checked = !m_Checked;
  }
    
  if (!m_Style.roundness) 
  {
    DrawRectangleRec(m_Bounds, bgColor);

    if (m_Checked)
      DrawCheckmark(); 
    return;
  }

  DrawRectangleRoundedLines(m_Bounds, m_Style.roundness, SEGMENTS, 1.0f, m_Style.outlineColor);

  if (m_Checked)
    DrawCheckmark(); 
}

bool GUI::Checkbox::GetChecked(void) noexcept
{
  return m_Checked;
}

void GUI::Checkbox::SetChecked(bool checkState) noexcept
{
  m_Checked = checkState;
}
