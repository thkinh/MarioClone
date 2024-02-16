#pragma once
#include "Scene.h"
#include <map>
#include <memory>
#include "EntityManager.h"

class Scene_Play : public Scene
{
	struct PlayerConfig
	{
		float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
		std::string WEAPON;
	};

protected:
	std::shared_ptr<Entity> m_player;
	std::string m_levelpath;
	PlayerConfig m_playerConfig;
	
	bool m_drawTexture = false;
	bool m_drawCollision = true;
	bool drawGrid = false;

	const Vec2D m_gridsize = { 64,64 };
	sf::Text m_gridText;
	sf::Font Play_Font;
public:
	Scene_Play(GameEngine* g, const std::string& levelPath);

	void init(const std::string& levelPath);
	Vec2D gridToMidPixel(float gridx, float gridy, std::shared_ptr<Entity> entity);
	void LoadLevel(const std::string& filename);
	void SpawnPlayer();
	void SpawnBullet();
	void sMovement();
	//void sLifeSpan();
	void sCollision();
	//void sAnimation();
	void SpawnTile(float gridx, float gridy);


	void sRender() override;
	void update() override;
	void sDoAction(const Action& action) override;
	void onEnd();
};