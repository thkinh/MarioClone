#include "Scene_Play.h"
#include "GameEngine.h"
#include <fstream>
#include <iostream>
#include "Physics.h"

Scene_Play::Scene_Play(GameEngine* g, const std::string& levelPath):
	Scene(g), m_levelpath(levelPath)
{
	init(m_levelpath);
}
void Scene_Play::init(const std::string& levelPath)
{
	Play_Font = m_game->assets().getFont("MagicRetro");
	m_gridText.setFont(Play_Font);
	registerAction(sf::Keyboard::P, "PAUSE");
	registerAction(sf::Keyboard::Escape, "QUIT");
	registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
	registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
	registerAction(sf::Keyboard::G, "TOGGLE_GRID");
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::Space, "UP");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::LShift, "RUN");


	
	//registerAction(sf::Keyboard::P, "Pause");
	//registerAction(sf::Keyboard::P, "Pause");

	m_gridText.setCharacterSize(14);
	//m_gridText.setFont(m_game->assets().getFont("THE FONT YOU WANT"));

	LoadLevel(levelPath);
	SpawnPlayer();
	//std::cout << "Current player position: (" << m_player->getComponent<CTransform>().pos.x << ", " 
	//	<< m_player->getComponent<CTransform>().pos.y << ")\n";

}

Vec2D Scene_Play::gridToMidPixel(float gridx, float gridy, std::shared_ptr<Entity> entity)
{
	auto& ePos = entity->getComponent<CTransform>().pos;
	auto& eBB = entity->getComponent<CBoundingBox>();
	ePos.y = (height()-(gridy * m_gridsize.y)) - eBB.size.y;
	ePos.x = gridx * m_gridsize.x;
	//ePos.x = gridx * m_gridsize.x ;
	//ePos.y = (gridy * m_gridsize.y) ;
	return Vec2D(ePos.x + eBB.halfsize.x, ePos.y + eBB.halfsize.y);
}


void Scene_Play::LoadLevel(const std::string& filename)
{
	//We reset our EntitiesManager everytime we load our level.
	m_entitiesManger = EntityManager();
	std::fstream file;
	file.open(filename);
	std::string var;
	while (file>>var)
	{
		if (var == "Tile")
		{
			int x, y;
			file >> x >> y;
			SpawnTile(x, y);
			continue;
		}
		if (var == "Player")
		{
			file >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CX >> m_playerConfig.CY
				>> m_playerConfig.SPEED >> m_playerConfig.JUMP >> m_playerConfig.MAXSPEED >> m_playerConfig.GRAVITY;
		
		}
		std::cout << "Loaded player\n";
	}
	file.close();
}

void Scene_Play::SpawnPlayer()
{
	m_player = m_entitiesManger.addEntity("player");
	m_player->addComponent<CBoundingBox>(Vec2D(m_playerConfig.CX, m_playerConfig.CY));
	m_player->addComponent<CTransform>(gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player));
	m_player->addComponent<CInput>();
	m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
	m_player->addComponent<CState>();
	m_player->addComponent<CAnimation>();
	m_player->getComponent<CTransform>().scale = {2.5f,2.5f};
	m_player->getComponent<CAnimation>().anmt = m_game->assets().getAnimation("Stand");

}

void Scene_Play::SpawnBullet()
{
	auto bullet = m_entitiesManger.addEntity("bullet");
	//bullet->addComponent<CTransform>(m_player->getComponent<CTransform>().pos);
	//bullet->changeComponent<CTransform>().velocity.x = 8;
}

void Scene_Play::SpawnTile(float gridx, float gridy)
{
	auto tile = m_entitiesManger.addEntity("tile");
	tile->addComponent<CBoundingBox>(Vec2D(m_gridsize.x, m_gridsize.y));
	tile->addComponent<CTransform>(gridToMidPixel(gridx,gridy,tile));
	//std::cout << "Spawned a tile at << (" << tile->getComponent<CTransform>().pos.x << "," << tile->getComponent<CTransform>().pos.y << ")\n";
	tile->getComponent<CTransform>().scale = Vec2D(1.34f, 1.34f);
	tile->addComponent<CAnimation>();
	tile->getComponent<CAnimation>().anmt = m_game->assets().getAnimation("TileDust");
}


void Scene_Play::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
			 if (action.name() == "TOGGLE_TEXTURE") { m_drawTexture = !m_drawTexture; }
		else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
		else if (action.name() == "TOGGLE_GRID") { drawGrid = !drawGrid; }
		else if (action.name() == "QUIT") { onEnd(); }
		//Movements
		else if (action.name() == "UP"){ m_player->getComponent<CInput>().up = true;}
		else if (action.name() == "RIGHT") { m_player->getComponent<CInput>().right = true;  }
		else if (action.name() == "LEFT") { m_player->getComponent<CInput>().left = true; }
		else if (action.name() == "RUN") { m_player->getComponent<CInput>().running = true; }
	}
	else if (action.type() == "END")
	{
		if (action.name() == "UP") {m_player->getComponent<CInput>().up = false;}
		else if (action.name() == "RIGHT") { m_player->getComponent<CInput>().right = false; }
		else if (action.name() == "LEFT") { m_player->getComponent<CInput>().left = false; }
		else if (action.name() == "RUN") { m_player->getComponent<CInput>().running = false; }
		else if (action.name() == "DOWN") { m_player->getComponent<CInput>().down = false; }
	}
}

void Scene_Play::sMovement()
{
	Vec2D player_velocity(0, m_player->getComponent<CTransform>().velocity.y);
	int& current_state = m_player->getComponent<CState>().state;
	float player_jump_speed = 12;
	if (current_state == 3)
	{
		player_jump_speed -= 6;
	}
	if (m_player->getComponent<CInput>().up == false )
	{
		//If player just landed on the ground, only allow them to jump when they release the button
		if (current_state == 1)
		{
			current_state = 0;
		}
		//Release the button at mid air won't let them moving at the y direction anymore
		else if (current_state == 3)
		{
			current_state = 2;
		}
	}
	if (m_player->getComponent<CInput>().up && current_state!=2 && current_state != 1)
	{
		player_velocity.y -= player_jump_speed;
		current_state = 3; //jumping
		if (player_velocity.y <= -30)
		{
			current_state = 2; //reached its maxspeed, falling
		}
	}
	if (m_player->getComponent<CInput>().right)
	{
		player_velocity.x = 7;
		m_player->getComponent<CTransform>().scale.x = 2.5;
	}
	if (m_player->getComponent<CInput>().left)
	{
		player_velocity.x = -7;
		m_player->getComponent<CTransform>().scale.x = -2.5;
	}
	if (m_player->getComponent<CInput>().running == true)
	{
		current_state = 4; // player is running
		player_velocity.x *= 2;
	}
	m_player->getComponent<CTransform>().velocity = player_velocity;
	for (auto& e : m_entitiesManger.getEntity())
	{		
		if (e->getComponent<CGravity>().has)
		{
			e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity;
		}
		e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
		e->getComponent<CTransform>().pos += e->getComponent<CTransform>().velocity;
	}
}

void Scene_Play::sCollision()
{
	for (auto& t : m_entitiesManger.getEntity("tile"))
	{
		Vec2D overlap = Physics::GetOverLap(t, m_player);
		auto& PlayerPos = m_player->getComponent<CTransform>();
		if (overlap.y < 0 && overlap.x < 0)
		{
			Vec2D Pre_overlap = Physics::GetPreviousOverLap(m_player,t);
			//Overlap right here
			//Check the direction by looking at the frame before
			//remember that overlap.y or overlap.x is just a distance between object and player, dont use it to compare to each other
			if (Pre_overlap.y >= 0)
			{
				//This means the overlap caused by the "y" direction
				if (PlayerPos.pos.y >= PlayerPos.prevPos.y)
				{
					//this means the overlap occured by the player moving down_ward
					m_player->getComponent<CTransform>().pos.y += overlap.y;
					//the overlap happened, so overlap.y or overlap,x is always negative
					m_player->getComponent<CTransform>().velocity.y = 0;
					if (m_player->getComponent<CState>().state != 0)
					{
						m_player->getComponent<CState>().state = 1;
					}
				}
				else if (PlayerPos.pos.y < PlayerPos.prevPos.y)
				{
					//this means the overlap occured by the player moving up_ward
					m_player->getComponent<CTransform>().pos.y -= overlap.y;
					//the overlap happened, so overlap.y or overlap,x is always negative
					m_player->getComponent<CTransform>().velocity.y = 0;
					m_player->getComponent<CState>().state = 2; //after collide upward, player falling
				}
			}
			else if (Pre_overlap.x >= 0)
			{
				//This means the overlap caused by the "x" direction
				if (PlayerPos.pos.x > PlayerPos.prevPos.x)
				{
					//this means the overlap occured by the player moving from left to right
					m_player->getComponent<CTransform>().pos.x += overlap.x ;
					//the overlap happened, so overlap.y or overlap,x is always negative
					m_player->getComponent<CTransform>().velocity.x = 0;
				}
				else if (PlayerPos.pos.x < PlayerPos.prevPos.x)
				{			
					//this means the overlap occured by the player moving from right to left
					m_player->getComponent<CTransform>().pos.x -= overlap.x;
					//the overlap happened, so overlap.y or overlap,x is always negative
					m_player->getComponent<CTransform>().velocity.x = 0;
				}
			}
		}
	}
}


void Scene_Play::sLifeSpan()
{
	if (m_player->getComponent<CTransform>().pos.y > m_game->window().getSize().y)
	{
		m_player->destroy();
		SpawnPlayer();
	}

}

void Scene_Play::sAnimation()
{
	int& playerstate = m_player->getComponent<CState>().state;
	CAnimation& player_animation = m_player->getComponent<CAnimation>();

	if (playerstate == 1  || playerstate == 0) // just land on ground
	{
		player_animation.anmt = m_game->assets().getAnimation("Stand");
	}
	else if (playerstate == 3 || playerstate == 2)
	{
		player_animation.anmt = m_game->assets().getAnimation("Jump");
	}
	else if (playerstate == 4)
	{
		player_animation.anmt = m_game->assets().getAnimation("Run");
	}
	for (auto& e : m_entitiesManger.getEntity())
	{
		e->getComponent<CAnimation>().anmt.update();
	}

}


void Scene_Play::update()
{
	m_entitiesManger.update();
	sMovement();
	sLifeSpan();
	sCollision();
	sAnimation();
	sRender();
}

void Scene_Play::sRender () 
{
	m_game->window().clear(sf::Color(50, 50, 150));
	//Set the camera view port to be the centered on the player if it's far enough right
	Vec2D& pPos = m_player->getComponent<CTransform>().pos;
	float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
	sf::View view = m_game->window().getView();
	view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
	m_game->window().setView(view);


	//drawTexture & animation
	if (m_drawTexture)
	{
		for (auto& e : m_entitiesManger.getEntity()) 
		{
			auto& transform = e->getComponent<CTransform>();
			if (e->hasComponent<CAnimation>())
			{
				auto& animation = e->getComponent<CAnimation>().anmt;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
				animation.getSprite().setScale(transform.scale.x, transform.scale.y);
				m_game->window().draw(animation.getSprite());
			}
		}
	}

	//Draw grid
	if (drawGrid)
	{
		float leftX = m_game->window().getView().getCenter().x - width() / 2;
		float rightX = leftX + width() + m_gridsize.x;
		float nextGridX = leftX - ((int)leftX % (int)m_gridsize.x);
		sf::VertexArray lines(sf::Lines);
		for (float x = nextGridX; x < rightX; x+=m_gridsize.x)
		{
			sf::Vertex vertical_start(sf::Vector2f(x, 0));
			sf::Vertex vertical_end(sf::Vector2f(x, (float)height()));
			lines.append(vertical_start);
			lines.append(vertical_end);
		}

		for (float y = 0; y < height(); y += m_gridsize.y)
		{
			sf::Vertex vertical_start(sf::Vector2f(0, y));
			sf::Vertex vertical_end(sf::Vector2f((float)width(),y ));
			lines.append(vertical_start);
			lines.append(vertical_end);
			for (float x = nextGridX; x < rightX; x += m_gridsize.x)
			{
				std::string xCell = std::to_string((int)x / (int)m_gridsize.x);
				std::string yCell = std::to_string((int)y / (int)m_gridsize.y);
				m_gridText.setString(xCell + "," + yCell);
				m_gridText.setPosition(x + 3, height() - y - m_gridsize.y + 2);
				m_game->window().draw(m_gridText);
			}
		}
		m_game->window().draw(lines);



	
	}

	//draw Collion Bounding Boxes with a rectangle shape
	if (m_drawCollision)
	{
		for (auto& e : m_entitiesManger.getEntity())
		{
			if (e->hasComponent<CBoundingBox>())
			{
				auto& box = e->getComponent<CBoundingBox>();
				auto& transfrom = e->getComponent<CTransform>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box.size.x-1, box.size.y-1));
				rect.setOrigin(sf::Vector2f(box.halfsize.x, box.halfsize.y));
				rect.setPosition(sf::Vector2f(transfrom.pos.x, transfrom.pos.y));
				rect.setFillColor(sf::Color(0, 255, 0, 30));
				rect.setOutlineColor(sf::Color(255, 0, 0));
				rect.setOutlineThickness(1);
				m_game->window().draw(rect);
				//m_game->window().draw(m_gridText);
			}
		}
	}
	m_game->window().display();
}

void Scene_Play::onEnd()
{
	m_game->ChangeScene("MENU", nullptr, true);
}