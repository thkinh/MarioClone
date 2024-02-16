#pragma once
#include "Entity.h"
#include <vector>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

typedef	std::vector<std::shared_ptr<Entity>> EntityVec;

class EntityManager
{
	EntityVec                        m_entities;
	EntityVec		                 m_entitiesToAdd;
	std::map<std::string, EntityVec> m_entitiesMap; 
	size_t							 m_totalEntities = 0;

	void removeDeadEntities(EntityVec& entities);
public:
	EntityManager();
	std::shared_ptr<Entity> addEntity(const std::string& tag);
	EntityVec& getEntity();
	EntityVec& getEntity(const std::string& tag);
	void update();

};
