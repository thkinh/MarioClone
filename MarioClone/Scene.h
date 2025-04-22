#pragma once
#include "Action.h"
#include "EntityManager.h"
#include <memory>
#include "Vec2D.h"
class GameEngine;

typedef std::map<sf::Keyboard::Scancode, std::string> ActionMap;

class Scene
{
protected:
	GameEngine* m_game = nullptr;
	EntityManager m_entitiesManger;
	ActionMap m_actionMap;
	bool m_paused = false;
	bool hasEnded = false;
	size_t m_currentFrame = 0;

	virtual void onEnd() = 0;
	//void setPaused(bool Ispaused);

public:

	Scene();
	Scene(GameEngine* gameEngine);

	virtual void update() = 0;
	virtual void sDoAction(const Action& action) = 0;
	virtual void sRender() = 0;

	virtual void doAction(const Action& action);
	//void simulate(const size_t frames);
	void registerAction(sf::Keyboard::Scancode inputKey, const std::string& actionName);

	size_t width() const;
	size_t height() const;
	size_t CurrentFrame() const;

	//bool hasEnded() const;
	const ActionMap& getActionMap() const;

};
