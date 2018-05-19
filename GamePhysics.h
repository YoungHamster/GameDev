#pragma once

struct XY {
	double x;
	double y;
};

struct AABB
{
	XY max;
	XY min;
};

struct PhysicalObject
{
	AABB borders;
	int type;
};

// Types of objects
#define SNAKE_HEAD_UP 1
#define SNAKE_HEAD_DOWN 2
#define SNAKE_HEAD_LEFT 3
#define SNAKE_HEAD_RIGHT 4
#define SNAKE_BODY 5
#define SNAKE_TAIL_UP 6
#define SNAKE_TAIL_DOWN 7
#define SNAKE_TAIL_LEFT 8
#define SNAKE_TAIL_RIGHT 9
#define APPLE 10
#define BORDER 11



// Checks collision of a and b
bool AABBvsAABB(AABB a, AABB b)
{
	// Exit with no intersection if found separated along an axis
	//if (a.max.x < b.min.x or a.min.x > b.max.x) return false;
	//if (a.max.y < b.min.y or a.min.y > b.max.y) return false;
	if (a.max.x <= b.min.x or a.min.x >= b.max.x) return false; // wrong implementation
	if (a.max.y <= b.min.y or a.min.y >= b.max.y) return false; // wrong implementation

	// No separating axis found, therefor there is at least one overlapping axis
	return true;
}