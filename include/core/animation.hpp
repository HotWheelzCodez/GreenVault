#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "raylib.h"

// KeyFrame struct to store frame state information
struct KeyFrame
{
  Rectangle bounds;
  Color     color;
  float     rotation;
  float     relativeTimeInSecs;

  // Operator overload to check if two frames are the same (by value(s))
  bool operator==(KeyFrame const& other)
  {
    return (bounds.x == other.bounds.x && bounds.y == other.bounds.y &&
      bounds.width == other.bounds.width && bounds.height == other.bounds.height &&
      color.r == other.color.r && color.g == other.color.g && color.b == other.color.b &&
      color.a == other.color.a && rotation == other.rotation && relativeTimeInSecs == other.relativeTimeInSecs);
  }
};

/* Animation class
*
* Used to store an array of KeyFrame(s) and transition smoothly through them
* and control the animation state/flow, i.e if it is playing, reverse, etc...
*/
class Animation
{
public: 
  // Default empty constrcutor
  Animation(void) = default;
 
  // Main constructor, takes an array of type KeyFrame
  Animation(std::vector<KeyFrame> keyFrames);

  // Will update the internal state to transation frames, if playing
  void Animate(void) noexcept;

  // Resets to initial state
  void Reset(void) noexcept;

  // Reverse the order of transaition / play
  void Reverse(bool reverse) noexcept;

  // Will begin / play 
  void Play(void) noexcept;

  // Will stop / pause it
  void Pause(void) noexcept;

  // Getter methods
  [[nodiscard]] bool IsFinished(void) const noexcept;
  [[nodiscard]] bool IsPlaying(void) const noexcept;

  [[nodiscard]] Rectangle GetCurrentBounds(void) const noexcept;
  [[nodiscard]] Color GetCurrentColor(void) const noexcept;
  [[nodiscard]] float GetCurrentRotation(void) const noexcept;

  [[nodiscard]] size_t GetNumOfKeyFrames(void) const noexcept;
  [[nodiscard]] KeyFrame GetKeyFrame(size_t keyFrameIndex) const noexcept;
  [[nodiscard]] Rectangle GetKeyFrameBounds(size_t keyFrameIndex) const noexcept;
  [[nodiscard]] Color GetKeyFrameColor(size_t keyFrameIndex) const noexcept;
  [[nodiscard]] float GetKeyFrameRotation(size_t keyFrameIndex) const noexcept;

  // Setter methods
  void SetKeyFrame(size_t keyFrameIndex, KeyFrame keyFrame)  noexcept;
  void SetKeyFrameBounds(size_t keyFrameIndex, Rectangle bounds) noexcept;
  void SetKeyFrameColor(size_t keyFrameIndex, Color color) noexcept;
  void SetKeyFrameRotaion(size_t keyFrameIndex, float rotation) noexcept;

private:
  // Used to normalize rgb colors to float i.e 0.0-1.0
  [[nodiscard]] const float Normalize(const float color) const noexcept;

  // Used to 'undo' the normalization i.e 1.0 = 255, or 0.0 = 0
  [[nodiscard]] const float GetNormal(const float normalizedColor) const noexcept;

private:
  std::vector<KeyFrame> m_KeyFrames;  
  std::vector<KeyFrame> m_OriginalKeyFrames;
  Rectangle m_CurrentBounds;
  Color m_CurrentColor;
  float m_CurrentRotation;
  bool m_Play;
  bool m_Reverse;
  bool m_IsFinished;
  size_t m_TargetKeyFrame;
};
