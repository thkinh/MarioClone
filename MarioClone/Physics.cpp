#include "Physics.h"
#include <iostream>
//Detect overlap
Vec2D Physics::GetOverLap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	float dx = std::abs(a->getComponent<CTransform>().pos.x - b->getComponent<CTransform>().pos.x);
	float dy = std::abs(a->getComponent<CTransform>().pos.y - b->getComponent<CTransform>().pos.y);
	float ox = dx - (a->getComponent<CBoundingBox>().halfsize.x + b->getComponent<CBoundingBox>().halfsize.x);
	float oy = dy - (a->getComponent<CBoundingBox>().halfsize.y + b->getComponent<CBoundingBox>().halfsize.y);
	//if (ox < 0 && oy < 0)
	//{
	//	//std::cout << "There's an overlap\n";
	//	//there's an overlap
	//	return Vec2D(ox, oy);
	//}
	return Vec2D(ox, oy);
}

//Detect previous overlap

Vec2D Physics::GetPreviousOverLap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	float dx = std::abs(a->getComponent<CTransform>().prevPos.x - b->getComponent<CTransform>().prevPos.x);
	float dy = std::abs(a->getComponent<CTransform>().prevPos.y - b->getComponent<CTransform>().prevPos.y);
	float ox = dx - (a->getComponent<CBoundingBox>().halfsize.x + b->getComponent<CBoundingBox>().halfsize.x);
	float oy = dy - (a->getComponent<CBoundingBox>().halfsize.y + b->getComponent<CBoundingBox>().halfsize.y);
	//if (ox < 0 && oy < 0)
	//{
	//	std::cout << "There's an overlap\n";
	//	//there's an overlap
	//	return Vec2D(ox, oy);
	//}
	return Vec2D(ox, oy);
}

float Physics::GetOverLapX(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	float dx = std::abs(a->getComponent<CTransform>().prevPos.x - b->getComponent<CTransform>().prevPos.x);
	float ox = dx - (a->getComponent<CBoundingBox>().halfsize.x + b->getComponent<CBoundingBox>().halfsize.x);
  return ox;
}

