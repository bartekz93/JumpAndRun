#pragma once
#include"Object.h"
#include"Movable.h"
#include"Bonus.h"
#include"Controls.h"
#include"../Framework/Framework.h"

namespace GAME
{

	class Player : public Object, public Movable, public FRAMEWORK::KeyboardListener
	{
	public:
		enum STATE { STAND, RUN, JUMP, JUMP2, STAMP };
		Player() : MaxSpeed(50.0f), DefaultMoveFactor(50), MoveFactor(50.0f), State(STAND), Points(0), Armor(false), Lives(3), Combo(0), ComboTime(0), bCombo(false) {}

		BonusDesc CurrentBonus;
		Vec2 FLV;

		float ArmorTime;

		bool bCombo;
		bool Armor;
		bool OnFL;

		int ComboTime;
		int Combo;


		void OnCollision(Object& obj, const Vec2& normal);
		void AddPoints(int p = 1) { Points += p; }
		void SetControls(Controls& c) { Ctrls = c; }
		void BonusEnd();
		void Clear();
		int GetPoints() const { return Points; }
		int GetLives() const { return Lives; }

		STATE GetState() const { return State; }
		void SetState(STATE s) { State = s; }
		void SetBonus(Bonus::BONUSTYPE type, float time);

		bool Dead(float t)
		{
			if (!Armor)
			{
				Armor = true;
				Lives--;
				if (Lives < 0) return false;
			}
			ArmorTime = t;

			return true;
		}

		void ComboEnd()
		{
			Combo = 0;
			bCombo = false;
		}
	private:
		Controls Ctrls;
		STATE State;

		float MaxSpeed;
		float DefaultMoveFactor;
		float MoveFactor;

		int Points;
		int Lives;

		void OnUpdate(float dt);
		void OnKeyDown(int key);
		void OnKeyUp(int key);
	};

}
