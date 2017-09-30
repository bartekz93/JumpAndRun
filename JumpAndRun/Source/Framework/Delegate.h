#pragma once

namespace FRAMEWORK
{

	template <typename T>
	class Delegate
	{
	public:
		void (T::*Fun)();
		T* Obj;

		Delegate()
		{
		}

		Delegate(T* obj, void (T::*fun)())
		{
			Bind(obj, fun);
		}

		void Bind(T* obj, void (T::*fun)())
		{
			Obj = obj;
			Fun = fun;
		}

		void Call()
		{
			(Obj->*Fun)();
		}
	};

}