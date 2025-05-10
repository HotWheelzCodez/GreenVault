#include "../../include/core/animation.hpp"

// Set default private member variables
Animation::Animation(std::vector<KeyFrame> keyFrames)
  : m_KeyFrames(keyFrames),
    m_OriginalKeyFrames(keyFrames),
    m_CurrentBounds(m_KeyFrames[0].bounds),
    m_CurrentColor(m_KeyFrames[0].color),
    m_CurrentRotation(m_KeyFrames[0].rotation),
    m_Play(false),
    m_Reverse(false),
    m_IsFinished(false),
    m_TargetKeyFrame(1)
{ }

// Getter methods
// -----------------------

size_t Animation::GetNumOfKeyFrames(void) const noexcept
{
  return m_KeyFrames.size();
}

Rectangle Animation::GetCurrentBounds(void) const noexcept
{
  return m_CurrentBounds;
}

Color Animation::GetCurrentColor(void) const noexcept
{
  return m_CurrentColor;
}

float Animation::GetCurrentRotation(void) const noexcept
{
  return m_CurrentRotation;
}

KeyFrame Animation::GetKeyFrame(size_t keyFrameIndex) const noexcept
{
  if (keyFrameIndex > m_KeyFrames.size())
    return m_KeyFrames[m_KeyFrames.size()-1];
  return m_KeyFrames[keyFrameIndex];
}

Rectangle Animation::GetKeyFrameBounds(size_t keyFrameIndex) const noexcept
{
  if (keyFrameIndex > m_KeyFrames.size())
    return m_KeyFrames[m_KeyFrames.size()-1].bounds;
  return m_KeyFrames[keyFrameIndex].bounds;
}

Color Animation::GetKeyFrameColor(size_t keyFrameIndex) const noexcept
{
  if (keyFrameIndex > m_KeyFrames.size())
    return m_KeyFrames[m_KeyFrames.size()-1].color;
  return m_KeyFrames[keyFrameIndex].color;
}

float Animation::GetKeyFrameRotation(size_t keyFrameIndex) const noexcept
{
  if (keyFrameIndex > m_KeyFrames.size())
    return m_KeyFrames[m_KeyFrames.size()-1].rotation;
  return m_KeyFrames[keyFrameIndex].rotation;
}

// Setter methods
// -----------------------

void Animation::SetKeyFrame(size_t keyFrameIndex, KeyFrame keyFrame) noexcept
{
  if (keyFrameIndex > m_KeyFrames.size())
    m_KeyFrames[m_KeyFrames.size()-1] = keyFrame;
  else
    m_KeyFrames[keyFrameIndex] = keyFrame;
}
 
void Animation::SetKeyFrameBounds(size_t keyFrameIndex, Rectangle bounds) noexcept
{
  if (keyFrameIndex > m_KeyFrames.size())
    m_KeyFrames[m_KeyFrames.size()-1].bounds = bounds;
  else
    m_KeyFrames[keyFrameIndex].bounds = bounds;
}

void Animation::SetKeyFrameColor(size_t keyFrameIndex, Color color) noexcept
{
  if (keyFrameIndex > m_KeyFrames.size())
    m_KeyFrames[m_KeyFrames.size()-1].color = color;
  else
    m_KeyFrames[keyFrameIndex].color = color;
}

void Animation::SetKeyFrameRotaion(size_t keyFrameIndex, float rotation) noexcept
{
  if (keyFrameIndex > m_KeyFrames.size())
    m_KeyFrames[m_KeyFrames.size()-1].rotation = rotation;
  else
    m_KeyFrames[keyFrameIndex].rotation = rotation;
}

void Animation::Play(void) noexcept
{
  m_Play = true;
}

void Animation::Pause(void) noexcept
{
  m_Play = false;
}

bool Animation::IsFinished(void) const noexcept
{
  return m_IsFinished;
}

bool Animation::IsPlaying(void) const noexcept
{
  return m_IsFinished;
}

void Animation::Reverse(bool reverse) noexcept
{
  if (m_Reverse == reverse)
    return;
   
  m_Reverse = reverse;
  std::reverse(m_KeyFrames.begin(), m_KeyFrames.end());
}

void Animation::Reset(void) noexcept
{
  m_CurrentBounds = m_KeyFrames[0].bounds;
  m_CurrentColor = m_KeyFrames[0].color;
  m_CurrentRotation = m_KeyFrames[0].rotation;
  m_TargetKeyFrame = 1;
  m_Play = false;
  m_IsFinished = false;
}

const float Animation::Normalize(const float color) const noexcept
{
  return (color/255.0f);
}

const float Animation::GetNormal(const float normalizedColor) const noexcept
{
  const float color = normalizedColor*255.0f;
  return (color > 255) ? 255 : color;
}

void Animation::Animate(void) noexcept
{
  if (m_IsFinished)
  {
    int lastFrame = m_KeyFrames.size()-1;
    return;
  }

  if (!m_Play)
    return;

  // We are valid to animate
  // --------------------------------

  // Store so we don't have to access several times
  Rectangle targetBounds = m_KeyFrames[m_TargetKeyFrame].bounds;
  Color targetColor = m_KeyFrames[m_TargetKeyFrame].color;
  float targetRotation = m_KeyFrames[m_TargetKeyFrame].rotation;
  float targetTime = abs(m_KeyFrames[m_TargetKeyFrame].relativeTimeInSecs);

  Rectangle prevBounds = m_KeyFrames[m_TargetKeyFrame-1].bounds;
  Color prevColor = m_KeyFrames[m_TargetKeyFrame-1].color;
  float prevRotation = m_KeyFrames[m_TargetKeyFrame-1].rotation;

  float fps = GetFPS();

  // Weird edge case
  if (fps == 0.0f)
    return;

  // Check if we need to move to the next KeyFrame or if we are done
  if (m_CurrentBounds.x == targetBounds.x && m_CurrentBounds.y == targetBounds.y &&
      m_CurrentBounds.width == targetBounds.width && m_CurrentBounds.height == targetBounds.height && 
      m_CurrentRotation == targetRotation && m_CurrentColor.r == targetColor.r &&
      m_CurrentColor.g == targetColor.g && m_CurrentColor.b == targetColor.b &&
      m_CurrentColor.a == targetColor.a)
  {
    if (m_TargetKeyFrame+1 < m_KeyFrames.size())
      m_TargetKeyFrame++;
    else
      m_IsFinished = true;
  }

  // Animating bounds
  // -------------------------------------------
  if (prevBounds.x < targetBounds.x)
  {
    float incrementAmount = (targetBounds.x-prevBounds.x)/(fps*targetTime);
    if (m_CurrentBounds.x < targetBounds.x)
      m_CurrentBounds.x += incrementAmount;
    else
      m_CurrentBounds.x = targetBounds.x;
  }
  else if (prevBounds.x > targetBounds.x)
  {
    float decrementAmount = (prevBounds.x-targetBounds.x)/(fps*targetTime);
    if (m_CurrentBounds.x > targetBounds.x)
      m_CurrentBounds.x -= decrementAmount;
    else
      m_CurrentBounds.x = targetBounds.x;
  }

  if (prevBounds.y < targetBounds.y)
  {
    if (m_CurrentBounds.y < targetBounds.y)
      m_CurrentBounds.y += (targetBounds.y-prevBounds.y)/(fps*targetTime);
    else
      m_CurrentBounds.y = targetBounds.y;
  }
  else if (prevBounds.y > targetBounds.y)
  {
    if (m_CurrentBounds.y > targetBounds.y)
      m_CurrentBounds.y -= (prevBounds.y-targetBounds.y)/(fps*targetTime);
    else
      m_CurrentBounds.y = targetBounds.y;
  }

  if (prevBounds.width < targetBounds.width)
  {
    if (m_CurrentBounds.width < targetBounds.width)
      m_CurrentBounds.width += (targetBounds.width-prevBounds.width)/(fps*targetTime);
    else
      m_CurrentBounds.width = targetBounds.width;
  }
  else if (prevBounds.width > targetBounds.width)
  {
    if (m_CurrentBounds.width > targetBounds.width)
      m_CurrentBounds.width -= (prevBounds.width-targetBounds.width)/(fps*targetTime);
    else
      m_CurrentBounds.width = targetBounds.width;
  }

  if (prevBounds.height < targetBounds.height)
  {
    if (m_CurrentBounds.height < targetBounds.height)
      m_CurrentBounds.height += (targetBounds.height-prevBounds.height)/(fps*targetTime);
    else
      m_CurrentBounds.height = targetBounds.height;
  }
  else if (prevBounds.height > targetBounds.height)
  {
    if (m_CurrentBounds.height > targetBounds.height)
      m_CurrentBounds.height -= (prevBounds.height-targetBounds.height)/(fps*targetTime);
    else
      m_CurrentBounds.height = targetBounds.height;
  }

  // Animating rotation
  // ---------------------------------
  if (prevRotation < targetRotation)
  {
    if (m_CurrentRotation < targetRotation)
      m_CurrentRotation += (targetRotation-prevRotation)/(fps*targetTime);
    else
      m_CurrentRotation = targetRotation;
  }
  else if (prevRotation > targetRotation)
  {
    if (m_CurrentRotation > targetRotation)
      m_CurrentRotation -= (prevRotation-targetRotation)/(fps*targetTime);
    else
      m_CurrentRotation = targetRotation;
  }

  // Animating color
  // -------------------------------------------------
  if (prevColor.r < targetColor.r)
  { 
    if (m_CurrentColor.r < targetColor.r)
    {
      float incrementAmount = ((float)targetColor.r-(float)prevColor.r)/(fps*targetTime);
      if (m_CurrentColor.r+incrementAmount > 255)
        m_CurrentColor.r = 255;
      else
        m_CurrentColor.r += std::round(incrementAmount);
    }
    else
    {
      m_CurrentColor.r = targetColor.r;
    }
  }
  else if (prevColor.r > targetColor.r)
  {
    if (m_CurrentColor.r > targetColor.r)
    {
      float decrementAmount = ((float)prevColor.r-(float)targetColor.r)/(fps*targetTime); 
      if (m_CurrentColor.r < decrementAmount)
        m_CurrentColor.r = 0;
      else
        m_CurrentColor.r -= std::round(decrementAmount);
    }
    else
    {
      m_CurrentColor.r = targetColor.r;
    }
  }
  if (prevColor.g < targetColor.g)
  {
    if (m_CurrentColor.g < targetColor.g)
    {
      float incrementAmount = ((float)targetColor.g-(float)prevColor.g)/(fps*targetTime); 
      if (m_CurrentColor.g+incrementAmount > 255)
        m_CurrentColor.g = 255;
      else
        m_CurrentColor.g = std::ceil(m_CurrentColor.g+incrementAmount);
    }
    else
    {
      m_CurrentColor.g = targetColor.g;
    }
  }
  else if (prevColor.g > targetColor.g)
  {
    if (m_CurrentColor.g > targetColor.g)
    {
      float decrementAmount = ((float)prevColor.g-(float)targetColor.g)/(fps*targetTime); 
      if (m_CurrentColor.g-decrementAmount < 0)
        m_CurrentColor.g = 0;
      else
        m_CurrentColor.g -= std::ceil(decrementAmount);
    }
    else
    {
      m_CurrentColor.g = targetColor.g;
    }
  }
  if (prevColor.b < targetColor.b)
  {
    if (m_CurrentColor.b < targetColor.b)
    {
      float incrementAmount = ((float)targetColor.b-(float)prevColor.b)/(fps*targetTime); 
      if (m_CurrentColor.b+incrementAmount > 255)
        m_CurrentColor.b = 255;
      else
        m_CurrentColor.b += std::ceil(incrementAmount);
    }
    else
    {
      m_CurrentColor.b = targetColor.b;
    }
  }
  else if (prevColor.b > targetColor.b)
  {
    if (m_CurrentColor.b > targetColor.b)
    {
      float decrementAmount = ((float)prevColor.b-(float)targetColor.b)/(fps*targetTime); 
      if (m_CurrentColor.b-decrementAmount < 0)
        m_CurrentColor.b = 0;
      else
        m_CurrentColor.b -= std::ceil(decrementAmount);
    }
    else
    {
      m_CurrentColor.b = targetColor.b;
    }
  }
  if (prevColor.a < targetColor.a)
  {
    if (m_CurrentColor.a < targetColor.a)
    {
      float incrementAmount = ((float)targetColor.a-(float)prevColor.a)/(fps*targetTime); 
      if (m_CurrentColor.a+incrementAmount > 255)
        m_CurrentColor.a = 255;
      else
        m_CurrentColor.a += std::ceil(incrementAmount);
    }
    else
    {
      m_CurrentColor.a = targetColor.a;
    }
  }
  else if (prevColor.a > targetColor.a)
  {
    if (m_CurrentColor.a > targetColor.a)
    {
      float decrementAmount = ((float)prevColor.a-(float)targetColor.a)/(fps*targetTime); 
      if (m_CurrentColor.a-decrementAmount < 0)
        m_CurrentColor.a = 0;
      else
        m_CurrentColor.a -= std::ceil(decrementAmount);
    }
    else
    {
      m_CurrentColor.a = targetColor.a;
    }
  }
}
