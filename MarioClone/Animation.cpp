#include "Animation.h"
#include "Assets.h"
#include "GameEngine.h"

Animation::Animation() = default;

Animation::Animation(const std::string& name, const sf::Texture& t) :
	m_name(name),
	m_sprite(t)
{
	
}

Animation::Animation(const std::string& name, const sf::Texture& t, size_t framecount, size_t speed):
	m_name(name),
	m_sprite(t),
	m_Framecount(framecount),
	m_speed(speed)
{

}

void Animation::update()
{
	m_currentFrame++;
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
