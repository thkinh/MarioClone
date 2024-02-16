#include "EntityManager.h"
EntityManager::EntityManager() = default;

void EntityManager::removeDeadEntities(EntityVec& entities)
{
	entities.erase(std::remove_if(entities.begin(), entities.end(),
		[](const std::shared_ptr<Entity>& entity) { return !entity->IsActive(); }),
		entities.end());
}

void EntityManager::update()
{
	//Adding
	for (auto& e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entitiesMap[e->tag()].push_back(e);
	}
	m_entitiesToAdd.clear();

	//removing
	removeDeadEntities(m_entities);
	
	for (auto& e : m_entitiesMap)
	{
		removeDeadEntities(e.second);
	}
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities, tag));
	m_entitiesToAdd.push_back(e);
	return e;
}


EntityVec& EntityManager::getEntity()
{
	return m_entities;
}

EntityVec& EntityManager::getEntity(const std::string& tag)
{
	return m_entitiesMap[tag];
}