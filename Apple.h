#pragma once
#include "GamePhysics.h"

class Apple
{
private:
	AABB apple;
public:
	void CreateApple(int size, int x, int y)
	{
		apple.max.x = x + size;
		apple.max.y = y;
		apple.min.x = x;
		apple.min.y = y - size;
	}

	bool CheckCollisionBetweenAppleAndSmth(AABB a)
	{
		if (AABBvsAABB(apple, a)) return true;
		return false;
	}

	void MakeAppleBigger(int size)
	{
		
	}

	PhysicalObject ReturnPhysicalObject()
	{
		PhysicalObject obj;
		obj.borders = apple;
		obj.type = APPLE;
		return obj;
	}

	AABB ReturnBox()
	{
		return apple;
	}

};