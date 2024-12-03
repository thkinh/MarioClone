#pragma once
#include <map>
#include <string>
#include "Animation.h"
#include <memory>
//This class live inside the GameEngine class
//Initialize inside GameEngine init()
//Accessed via Scene's pointer to GameEngine // m_game->getAssets()

//Assets loaded once at beginning of program
//Assets can then be used in any scene, but they are only initialized once in memory
//For larger game, you may want to load assets per scene (Loading screens)


//Fuck this, this class is so over implemented
//Day 1: I think that the use of this class is: store all the images, texture files in the current gameplay or scene.
//With this, every entities that need to access to any of those files,
//they gonna need to ask the GameEngine to see these assets to actually use it for itself
class Assets
{
	std::map<std::string, sf::Texture > m_texture;
	std::map<std::string, Animation > m_animation;
	//std::map<std::string, sf::Texture > m_texture;
	std::map<std::string, sf::Font> m_font;
	
public:
	void addTexture(const std::string& name, const std::string & path);
	void addAnimation(const std::string& animation_name, const std::string& tex_name, size_t framecount, size_t speed);
	////void addSound(name, path);
	void addFont(const std::string& name, const std::string& path);

	sf::Texture& getTexture(const std::string & name) ;
	Animation& getAnimation(const std::string & name) ;
	////sf::Texture& getTexture() const;
	const sf::Font& getFont(const std::string& name) const;
	void LoadAssets(const std::string& name);
};