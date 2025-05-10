#include "../../include/core/gui.hpp"

GUI::Popup::Popup(Rectangle bounds, Color windowColor) noexcept
  : m_Bounds(std::move(bounds)), m_WindowColor(std::move(windowColor)) 
{ }

void GUI::Popup::SetBounds(const Rectangle& bounds) noexcept
{
  m_Bounds = bounds;
}

int GUI::Popup::UpdateAndRender(GUI::MouseState& mouseState, float windowWidth, float windowHeight, const std::function<int(const Rectangle& parentBounds, GUI::MouseState& mouseState, float windowWidth, float windowHeight)>& render) noexcept
{
  DrawRectangleRounded(m_Bounds, 0.1f, SEGMENTS, m_WindowColor);
  return render(m_Bounds, mouseState, windowWidth, windowHeight);
}
