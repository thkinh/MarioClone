#pragma once
#include "Scene.h"
#include "Assets.h"
#include <memory>
#include <SFML/Graphics.hpp>

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{
protected:
	sf::RenderWindow m_window;
	Assets m_assets;
	std::string m_currentScene;
	SceneMap m_scenesMap;
  size_t m_fps = 30;
	size_t m_simulationSpeed = 1;
	bool m_running = true;

	void init(const std::string& path);
	void update();

	void sUserInput();
	std::shared_ptr<Scene> currentScene();
public:
	GameEngine(const std::string& path);
	
	void ChangeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

	void quit();
	void run();

	sf::RenderWindow& window();
	Assets assets() const;
	bool isRunning();
  size_t getFPS() const;

};
 
