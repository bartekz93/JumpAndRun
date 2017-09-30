#pragma once
#include<map>
#include"Object.h"

namespace GAME
{

	class ObjectPair
	{
	private:
		Object* obj1;
		Object* obj2;

		std::map<Object::TYPE, Object*> Objects;
	public:
		ObjectPair(Object& o1, Object& o2) : obj1(&o1), obj2(&o2) {}

		Object*		GetObject(Object::TYPE t);
		Object*		GetObject(int i);
		bool		Is(Object::TYPE t1, Object::TYPE t2);
		bool		OneOf(Object::TYPE t);
	};

}