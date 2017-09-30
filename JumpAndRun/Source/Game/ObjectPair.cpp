#include "ObjectPair.h"

using namespace GAME;

Object* ObjectPair::GetObject(Object::TYPE t)
{
	return Objects[t];
}

bool ObjectPair::OneOf(Object::TYPE t)
{
	return (obj1->Type == t || obj2->Type == t);
}

Object* ObjectPair::GetObject(int i)
{
	if (i == 0) return obj1;
	return obj2;
}

bool ObjectPair::Is(Object::TYPE t1, Object::TYPE t2)
{
	if (obj1->Type == t1 && obj2->Type == t2)
	{
		Objects[t1] = obj1;
		Objects[t2] = obj2;
		return true;
	}

	else if (obj1->Type == t2 && obj2->Type == t1)
	{
		Objects[t2] = obj1;
		Objects[t1] = obj2;
		return true;
	}

	else return false;
}