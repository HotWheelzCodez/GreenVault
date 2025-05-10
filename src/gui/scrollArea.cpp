#include "../../include/core/gui.hpp"

// Base constructor to initalize private fields
GUI::ScrollArea::ScrollArea(Rectangle bounds, GUI::ScrollAreaStyle style) noexcept
  : m_Bounds(std::move(bounds)), m_Style(std::move(style)), m_Offset({ 0, 0 }), m_Height(m_Bounds.y)
{ }

Vector2 GUI::ScrollArea::GetOffset(void) noexcept
{
  return m_Offset;
}

float GUI::ScrollArea::GetYOffset(void) noexcept
{
  return m_Offset.y;
}

float GUI::ScrollArea::GetXOffset(void) noexcept
{
  return m_Offset.x;
}

void GUI::ScrollArea::SetBounds(Rectangle bounds) noexcept
{
  m_Bounds = bounds;
}

void GUI::ScrollArea::ResetScroll(void) noexcept
{
  m_Offset.y = 0;
}

void GUI::ScrollArea::UpdateAndRender(float windowWidth, float windowHeight, GUI::MouseState& mouseState, std::vector<std::map<std::string, std::string>>& info, const std::function<float(float windowWidth, float windowHeight, GUI::MouseState& mouseState, Vector2 position, std::map<std::string, std::string>& parameters)>& renderItem) noexcept
{  
  // Get mouse scroll input and update offset
  Vector2 scroll = GetMouseWheelMoveV(); 
  m_Offset.x += scroll.x * m_Style.scrollSpeed;
  m_Offset.y += scroll.y * m_Style.scrollSpeed;

  // Set base properties
  Color backgroundColor = m_Style.backgroundColor;
  if (CheckCollisionPointRec(mouseState.position, m_Bounds))
    backgroundColor = m_Style.backgroundHoverColor;

  // Render background and begin scissor mode
  DrawRectangleRounded(m_Bounds, m_Style.roundness, SEGMENTS, backgroundColor);
  BeginScissorMode(m_Bounds.x + 5, m_Bounds.y + 5, m_Bounds.width - 10, m_Bounds.height - 10);

  // Calculate scrolling bounds dynamically while rendering
  Rectangle offsetBounds = m_Bounds;
  offsetBounds.y -= m_Offset.y;

  float totalHeight = 0.0f;
  for (auto& parameters : info)
  {
    // Render only if the item is within view
    if (offsetBounds.y + m_Height > m_Bounds.y && offsetBounds.y < m_Bounds.y + m_Bounds.height)
    {
      float itemHeight = renderItem(windowWidth, windowHeight, mouseState, { offsetBounds.x, offsetBounds.y }, const_cast<std::map<std::string, std::string>&>(parameters));
      if (itemHeight < 0) break;
      m_Height = itemHeight;
    }

    // Update position and track total height
    totalHeight += m_Height+m_Style.padding.y;
    offsetBounds.y += m_Height+m_Style.padding.y;
  }

  // Ensure scrolling is within bounds
  float maxScrollY = std::max(100.0f, totalHeight - m_Bounds.height);
  m_Offset.y = std::clamp(m_Offset.y, 0.0f, maxScrollY);

  EndScissorMode();
}
