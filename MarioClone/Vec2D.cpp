#include "Vec2D.h"
#include <math.h>
#include <iostream>
Vec2D::Vec2D() = default;

Vec2D::Vec2D(float xin, float yin) :
	x(xin), y(yin)
{

}

Vec2D::~Vec2D()
{

}

bool Vec2D::operator==(const Vec2D& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

bool Vec2D::operator!=(const Vec2D& rhs) const
{
	return (x != rhs.x || y != rhs.y);
}

Vec2D Vec2D::operator+(const Vec2D& rhs) const
{
	return Vec2D(x + rhs.x, y + rhs.y);
}

Vec2D& Vec2D::operator+=(const Vec2D& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	return *this;
}

Vec2D& Vec2D::operator*=(float S)
{
	x *= S;
	y *= S;
	return *this;
}

Vec2D Vec2D::operator- (const Vec2D& rhs) const
{
	return Vec2D(x - rhs.x, y - rhs.y);
}

float Vec2D::Distance(const Vec2D& rhs) const
{
	return sqrtf((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
}

Vec2D Vec2D::operator* (float val) const
{
	return Vec2D(x * val, y * val);
}

Vec2D Vec2D::operator/ (float val) const
{
	return Vec2D(x / val, y / val);
}

Vec2D& Vec2D::normalize()
{
	float len = length();
	x /= len;
	y /= len;
	return *this;
}

float Vec2D::length()
{
	return sqrtf(x * x + y * y);
}