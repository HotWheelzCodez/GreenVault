#pragma once

#include <unordered_map>

#include "../core/gui.hpp"
#include "../util.hpp"
#include "../core/database.hpp"
#include "../core/animation.hpp"

class Page
{
public:
  ~Page()
  {
    UnloadFont(font);
  }

  virtual void Init(float windowWidth, float windowHeight) = 0;
  virtual int Render(float windowWidth, float windowHeight, GUI::MouseState& mouseState) = 0;

protected:
  bool init = true;
  Font font = LoadFontEx("assets/fonts/roboto.ttf", 750, 0, 0);

  // The GUI 'structure' of each page
  std::unordered_map<std::string, GUI::Text> textElements;
  std::unordered_map<std::string, GUI::Button> buttonElements;
  std::unordered_map<std::string, GUI::Input> inputElements;
  std::unordered_map<std::string, GUI::ScrollArea> scrollAreaElements;
  std::unordered_map<std::string, GUI::Dropdown> dropdownElements;
  std::unordered_map<std::string, GUI::Popup> popupElements;
  std::unordered_map<std::string, GUI::Checkbox> checkboxElements;
  std::unordered_map<std::string, Animation> animations;
  std::unordered_map<std::string, Texture2D> images;
  std::unordered_map<std::string, Rectangle> bounds;
  std::unordered_map<std::string, Color> colors;
  std::unordered_map<std::string, std::string> etc;

protected:
  // Inline privatly shared method to clear all loaded textures destroy GUI 'structure'
  void inline DeInit()
  {
    for (const auto& image : images)
      UnloadTexture(image.second);

    std::unordered_map<std::string, GUI::Text>().swap(textElements);
    std::unordered_map<std::string, GUI::Button>().swap(buttonElements);
    std::unordered_map<std::string, GUI::Input>().swap(inputElements);
    std::unordered_map<std::string, GUI::ScrollArea>().swap(scrollAreaElements);
    std::unordered_map<std::string, GUI::Dropdown>().swap(dropdownElements);
    std::unordered_map<std::string, GUI::Popup>().swap(popupElements);
    std::unordered_map<std::string, GUI::Checkbox>().swap(checkboxElements);
    std::unordered_map<std::string, Animation>().swap(animations);
    std::unordered_map<std::string, Texture2D>().swap(images);
    std::unordered_map<std::string, Rectangle>().swap(bounds);
    std::unordered_map<std::string, Color>().swap(colors);
    std::unordered_map<std::string, std::string>().swap(etc);
    init = true;
  }
};
