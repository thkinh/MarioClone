#include "GameEngine.h"
#include <iostream>
#include "Scene_Menu.h"
#include "Scene_Play.h"
GameEngine::GameEngine(const std::string& path)
{
	init(path);
}

void GameEngine::init(const std::string& path)
{
	//Add the function load from file for assets later, so that we dont have to call add texture, add animation,etc
	m_assets.LoadAssets(path);
	m_window.create(sf::VideoMode(1280, 768), "Definitely not Mario");
	m_window.setFramerateLimit(60);
	ChangeScene("MENU", std::make_shared<Scene_Menu>(this));
}

sf::RenderWindow& GameEngine::window() 
{
	return m_window;
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
	return m_scenesMap[m_currentScene];
}

bool GameEngine::isRunning()
{
	return m_running && m_window.isOpen();
}

void GameEngine::run()
{
	while (m_running)
	{
		update();
	}
}

void GameEngine::update()
{
	m_window.clear();
	if (!isRunning())
	{
		return;
	}
	if (m_scenesMap.empty())
	{
		return;
	}
	currentScene()->update();
	currentScene()->sRender();
	sUserInput();
}

Assets GameEngine::assets() const
{
	return m_assets;
}

void GameEngine::ChangeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	if (scene)
	{
		m_scenesMap[sceneName] = scene;
	}
	else
	{
		if (m_scenesMap.find(sceneName) == m_scenesMap.end())
		{
			std::cerr << "Warning, this scene does not exist!\n";
			return;
		}
	}

	if (endCurrentScene)
	{
		m_scenesMap.erase(m_scenesMap.find(m_currentScene));
	}
	m_currentScene = sceneName;
}


void GameEngine::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			quit();
		}
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::X)
			{
				std::cout << "Key X pressed\n";
			}
		}
		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			if (currentScene()->getActionMap().find(event.key.code) != currentScene()->getActionMap().end())
			{
				const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";
				currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
			}
		}
	}
}


void GameEngine::quit()
{
	m_running = false;
}