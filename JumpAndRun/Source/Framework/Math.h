#pragma once
#include<math.h>

namespace MATH
{

	typedef unsigned int UINT;
	const float PI = 3.1415f;
	
	struct Vec2
	{
		float x, y;
	
		Vec2() : x(0.0f), y(0.0f) {}
		Vec2(float u, float v) : x(u), y(v) {}
	
		Vec2 operator+(const Vec2& v) const { return Vec2(v.x+x, v.y+y); }
		Vec2 operator-(const Vec2& v) const { return Vec2(x-v.x, y-v.y); }
		Vec2 operator*(float f) const { return Vec2(x*f, y*f); }
		Vec2 operator/(float f) const { return Vec2(x/f, y/f); }
		Vec2 operator-() const { return Vec2(-x, -y); }
	
		Vec2& operator+=(const Vec2& v) { x+=v.x; y+=v.y; return *this; }
		Vec2& operator-=(const Vec2& v) { x-=v.x; y-=v.y; return *this; }
		Vec2& operator*=(const Vec2& v) { x*=v.x; y*=v.y; return *this; }
		Vec2& operator/=(const Vec2& v) { x/=v.x; y/=v.y; return *this; }
	
		bool operator>=(const Vec2& v) { return x >= v.x && y >= v.y; }
	
		bool operator==(const Vec2& v) const { return (v.x==x && v.y==y); }
		bool operator!=(const Vec2& v) const { return (v.x!=x || v.y!=y); }
	
		float Length() const { return sqrt(pow(x, 2) + pow(y, 2)); }
		Vec2 Normalize() const { return Vec2(x, y) / Length(); } 
		float Dot(const Vec2& v) const { return x*v.x + y*v.y; }
	
		Vec2& ToInt() { x = (int)x; y = (int)y; return *this; }
		void SetCoord(int i, float v) { if (i == 0) x = v; else y = v; }
	};
	
	struct Vec3
	{
		float x, y, z;
	
		Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
		Vec3(float u, float v, float t) : x(u), y(v), z(t) {}
	
		Vec3 operator+(const Vec3& v) const { return Vec3(v.x+x, v.y+y, v.z+z); }
		Vec3 operator-(const Vec3& v) const { return Vec3(x-v.x, y-v.y, z-v.z); }
		Vec3 operator*(float f) const { return Vec3(x*f, y*f, z*f); }
	};
	
	struct Rect
	{
		float x, y, w, h;
	
		Rect(float _x, float _y, float _w, float _h) : x(_x), y(_y), w(_w), h(_h) {}
		Vec2 GetCenter() const { return Vec2(x+w*0.5f, y+h*0.5f); }
		Rect() {}
	};

};
using namespace MATH;