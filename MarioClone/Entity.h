#pragma once
#include <string>
#include "Component.h"
#include <tuple>

class EntityManager;

typedef std::tuple<
	CTransform,
	CLifeSpan,
	CInput,
	CBoundingBox,
	CAnimation,
	CGravity,
	CState
>ComponentTuple;


class Entity
{
	friend class EntityManager;

	bool m_active = true;
	std::string m_tag = "NULL";
	size_t m_id = 0;
	ComponentTuple m_component;
	
	Entity(const size_t& id, const std::string& tag);

public:
	void destroy();
	size_t id() const;
	bool IsActive() const;
	const std::string& tag() const;

	template <typename T>
	bool hasComponent() const
	{
		return getComponent<T>().has;
	}


	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	
	template<typename T>
	T& getComponent()
	{
		return std::get<T>(m_component);
	}

	template <typename T>
	const T& getComponent() const
	{
		return std::get<T>(m_component);
	}

	template <typename T>
	void removeComponent(T& aggr)
	{
		getComponent<T>() = T();
	}

};


