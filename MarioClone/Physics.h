#pragma once
#include "Vec2D.h"
#include <memory>
#include "Entity.h"

class Physics
{
public:
	static Vec2D GetOverLap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
	static Vec2D GetPreviousOverLap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);


};