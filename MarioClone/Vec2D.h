#pragma once
class Vec2D
{
public:
	float x = 0.0f;
	float y = 0.0f;

	Vec2D();
	Vec2D(float xin, float yin);
	~Vec2D();

	bool operator== (const Vec2D& rhs) const;
	bool operator!= (const Vec2D& rhs) const;

	Vec2D operator+ (const Vec2D& rhs) const;
	Vec2D operator- (const Vec2D& rhs) const;
	Vec2D operator* (float val) const;
	Vec2D operator/ (float val) const;

	Vec2D& operator+= (const Vec2D& rhs);
	Vec2D& operator*= (float S);
	float Distance(const Vec2D& rhs) const;
	Vec2D& normalize();
	float length();
};