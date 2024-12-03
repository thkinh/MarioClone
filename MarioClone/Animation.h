#pragma once
#include "Vec2D.h"
#include <vector>
#include <SFML/Graphics.hpp>
class Animation
{
	sf::Sprite m_sprite;
	size_t m_Framecount = 1;
	size_t m_currentFrame = 0;
	size_t m_speed = 0;
	Vec2D m_size = { 1,1 };
	std::string m_name = "NULL";
public:
	Animation();
	Animation(const std::string& name, const sf::Texture& t);
	Animation(const std::string& name, const sf::Texture& t, size_t framecount, size_t speed);

	void setSize(float sizex, float sizey);
	void update();
	bool hasEnded() const;
	const std::string& getName() const;
	const Vec2D& getSize() const;
	sf::Sprite& getSprite() ;	
};
