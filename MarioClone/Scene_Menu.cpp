#include "Scene_Menu.h"
#include "GameEngine.h"
#include "Scene_Play.h"
#include <iostream>


void Scene_Menu::init()
{
	registerAction(sf::Keyboard::Space, "PLAY");
	// Load the font during initialization

	Menu_font = m_game->assets().getFont("MagicRetro");
	m_MenuText.setFont(Menu_font);
	m_MenuText.setCharacterSize(34);
	m_MenuText.setString("Please work");
}

Scene_Menu::Scene_Menu(GameEngine* gameEngine):
	Scene(gameEngine)
{
	init();
}



void Scene_Menu::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "PLAY") {
			std::cout << "Playing...";
			onEnd();
		}
		//else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
		//else if (action.name() == "TOGGLE_GRID") { drawGrid = !drawGrid; }
		//else if (action.name() == "QUIT") { m_drawTexture = !m_drawTexture }
		//else if (action.name() == "TOGGLE_TEXTURE") { m_drawTexture = !m_drawTexture }
	}
	else if (action.type() == "END")
	{

	}
}

void Scene_Menu::update()
{
	m_entitiesManger.update();
	//sMovement();
	//sLifeSpan();
	//sCollision();
	//sAnimation();
	sRender();
}

void Scene_Menu::onEnd()
{
	m_game->ChangeScene("PLAY", std::make_shared<Scene_Play>(m_game, "Level_Configuration/Level1.txt"), false);
}

void Scene_Menu::sRender()
{

	m_game->window().clear(sf::Color(0, 0, 0));
	//m_MenuText.setString("Please work");
	//sf::Font font_test;
	///*if (!font_test.loadFromFile("MagicRetro.ttf"))
	//{
	//	std::cout << "FUCCCCCCCCCCCK\n";
	//}*/
	//font_test = m_game->assets().getFont("MagicRetro");
	//m_MenuText.setFont(font_test);
	m_game->window().draw(m_MenuText);
	m_game->window().display();
}