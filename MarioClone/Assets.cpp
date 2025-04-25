#include "Assets.h"
#include <iostream>
#include <fstream>

void Assets::LoadAssets(const std::string& name)
{
    std::ifstream file;
    file.open(name);
    std::string config;
    while (file>>config)
    {
      if (config == "#")
      {
        std::string tmp="";
        std::getline(file, tmp);
        std::cout << "Skipping: " << tmp << "\n";
      }

      if (config == "Texture")
      {
        std::string texname;
        std::string path;
        file >> texname >> path;
        addTexture(texname, path);
        continue;
      }
      if (config == "Font")
      {
        std::string fontname;
        std::string path;
        file >> fontname >> path;
        addFont(fontname, path);
        continue;
      }
      if (config == "Animation")
      {
        std::string ani_name;
        std::string texname;
        size_t framecount = 0;
        size_t speed = 0;
        file >> ani_name >> texname >> framecount >> speed;
        addAnimation(ani_name, texname, framecount, speed);
        continue;
      }
    }

    file.close();
}

void Assets::addFont(const std::string& name, const std::string& path)
{
  sf::Font font;
  if (font.openFromFile(path))
  {
    std::cout << "Loaded font from " << path << std::endl;
    m_font[name] = font;
  }
  else
  {
    std::cout << "Error loading font\n";
  }
}

void Assets::addTexture(const std::string& name, const std::string& path)
{
  (void) m_texture[name].loadFromFile(path);
  std::cout << "Loaded this texture " << name << " from " << path << "\n";
}

void Assets::addAnimation(const std::string& animation_name, const std::string& tex_name, size_t framecount, size_t speed)
{
  Animation a(animation_name, m_texture[tex_name], framecount, speed);
  m_animation[animation_name] = a;
}


const sf::Font& Assets::getFont(const std::string& name) const
{
  auto it = m_font.find(name);
  if (it != m_font.end())
  {
    return it->second;
  }
  else
  {
    // Handle the case where the font is not found
    // You might want to return a default font or throw an exception
    // A default font if the requested font is not found
    static sf::Font dummyFont;
    return dummyFont;
  }
}

sf::Texture& Assets::getTexture(const std::string& name) 
{
  return m_texture[name];
}

Animation& Assets::getAnimation(const std::string& name) 
{
  return m_animation[name];
}
