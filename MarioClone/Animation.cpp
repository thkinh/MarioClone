#include "Animation.h"
#include "Assets.h"
#include "GameEngine.h"
#include <cmath>
#include <iostream>

Animation::Animation():
  m_default(),
  m_sprite(m_default)
{
}

Animation::Animation(const std::string& name, const sf::Texture& t) :
	Animation(name, t, 1, 0)
{
	
}

Animation::Animation(const std::string& name, const sf::Texture& t, size_t framecount, size_t speed):
	m_name(name),
	m_sprite(t),
	m_Framecount(framecount),
	m_speed(speed)
{
  std::cout <<m_name << "'s sprite size: " << (float)t.getSize().x << ","<< (float)t.getSize().y<<" / ";
  std::cout <<m_name << "'s framecount: " << framecount << "\n";
	m_size = Vec2D((float)t.getSize().x / framecount, (float)t.getSize().y);
	m_sprite.setOrigin({m_size.x / 2.f, (m_size.y / 2.f)});
  m_sprite.setTextureRect(sf::IntRect(
    {(int)(std::floor(m_currentFrame) * m_size.x), 0},
    {(int)(m_size.x), (int)m_size.y}
));

}

void Animation::update()
{
	//std::cout << "CF/FC : " << m_currentFrame << "/" << m_Framecount << "\n";
	if (m_currentFrame >= m_Framecount)
	{
		m_currentFrame = 0;
		m_speed = 0;
	}
	if (m_currentFrame - m_speed == 2)
	{
    m_speed = m_currentFrame;
    m_sprite.setTextureRect(sf::IntRect(
          {(int)(m_currentFrame * m_size.x), 0},
          {(int)(m_size.x), (int)m_size.y}
          ));
  }
  m_currentFrame++;
}

void Animation::setSize(float sizex, float sizey) 
{
  m_size = { sizex, sizey };
}
bool Animation::hasEnded() const
{

  return false;
}

const std::string& Animation::getName() const
{
  return m_name;
}

const Vec2D& Animation::getSize() const
{
  return m_size;
}

sf::Sprite& Animation::getSprite() 
{
  return m_sprite;
}
