#include "Scene.h"
#include "GameEngine.h"
#include <iostream>
Scene::Scene() = default;

Scene::Scene(GameEngine* gameEngine):
	m_game(gameEngine)
{
}


void Scene::doAction(const Action& action)
{
	//std::cout << "Abstract do nothing\n";
	sDoAction(action);

}

void Scene::sRender()
{
	std::cout << "Abtrack render nothing\n";

}

void Scene::registerAction(int inputKey, const std::string& actionName)
{
	m_actionMap[inputKey] = actionName;
}

void Scene::update()
{
	std::cout << "Abtract update nothing\n";
}

const ActionMap& Scene::getActionMap() const
{
	return m_actionMap;
}

size_t Scene::width() const
{
	return m_game->window().getSize().x;
}

size_t Scene::height() const
{
	return m_game->window().getSize().y;
}

size_t Scene::CurrentFrame() const
{
	return m_currentFrame;
}