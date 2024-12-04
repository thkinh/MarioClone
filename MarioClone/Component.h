#pragma once
#include "Vec2D.h"
#include "Animation.h"


class Component
{
public:
	bool has = false;
};


class CTransform : public Component
{
public:
	Vec2D pos = { 0.0f, 0.0f };
	Vec2D prevPos = {0.0f, 0.0f};
	Vec2D scale = { 1.0f, 1.0f };
	Vec2D velocity = { 0.0,0.0 };
	float angle = 0;
	
	CTransform() {};
	CTransform(const Vec2D& p):pos(p){}
	CTransform(const Vec2D& p, const Vec2D& v, const Vec2D& sc, float a) :
		pos(p), velocity(v), scale(sc), prevPos(p), angle(a)
	{

	}
};


class CLifeSpan : public Component
{
public:
	int total = 0;
	int remaining = 0;
	CLifeSpan() {};
	CLifeSpan(int t) :
		total(t), remaining(t)
	{

	}
};

class CInput : public Component
{
public:
	bool up = false;
	bool down = false;
	bool right = false;
	bool left = false;
	bool action1 = false;
	bool running = false;
	CInput() = default;
};

class CBoundingBox : public Component
{
public:

	Vec2D size;
	Vec2D halfsize;

	CBoundingBox() {};

	CBoundingBox(const Vec2D& s) :
		size(s), halfsize(s.x / 2, s.y / 2)
	{

	}
	
};

class CAnimation : public Component
{
public:
	Animation anmt ;
	CAnimation() {};
	
};

class CGravity : public Component
{
public:
	float gravity = 0;
	CGravity() {};
	CGravity(float g) : gravity(g)
	{

	}
};

class CState : public Component
{
public:
	//0 = on ground, can jump
	//1 = just land on ground, cannot jump
	//2 = falling
	//3 = airing
	//4 = running
	int state = 0;
	CState() {};
	CState(int s) :state(s) {}
};