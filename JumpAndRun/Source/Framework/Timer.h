#pragma once

#include"Delegate.h"

namespace FRAMEWORK
{

	template <typename T>
	class Timer
	{
	private:
		Delegate<T> Callback;
		float StartTime;
		float LastCall;
		float Period;
		float Time;
	public:


		void Create(Delegate<T> dlgt)
		{
			Callback = dlgt;
		}

		void Start(float Time, float period)
		{
			Time = LastCall = StartTime = Time;
			Period = period;
		}

		void Update(float dt)
		{
			Time += dt;

			if (Time - LastCall >= Period)
			{
				Callback.Call();
				LastCall = Time;
			}
		}
	};

}