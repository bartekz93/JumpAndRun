#include<algorithm>
#include"Player.h"
#include"Controls.h"

using namespace GAME;

void Player::SetBonus(Bonus::BONUSTYPE type, float time)
{
	if (CurrentBonus.BonusType != Bonus::NONE) BonusEnd();

	CurrentBonus.BonusType = type;
	CurrentBonus.StartTime = time;

	if (type == Bonus::ARMOR)
	{
		Armor = true;
	}
	/*
	if (type == Bonus::DRUNK)
	{
	int l = Ctrls.Keys[Controls::LEFT];
	Ctrls.Keys[Controls::LEFT] = Ctrls.Keys[Controls::RIGHT];
	Ctrls.Keys[Controls::RIGHT] = l;

	Dir = Vec2(0.0f, 0.0f);
	}*/

	if (type == Bonus::RUN)
	{
		MoveFactor = DefaultMoveFactor * 2;
	}
}

void Player::Clear()
{
	CurrentBonus.BonusType = Bonus::NONE;
	Points = 0;
	Armor = false;
	Lives = 3;
	State = STAND;
}

void Player::BonusEnd()
{
	if (CurrentBonus.BonusType == Bonus::ARMOR) Armor = false;

	/*if (CurrentBonus.BonusType == Bonus::DRUNK)
	{
	int l = Ctrls.Keys[Controls::LEFT];
	Ctrls.Keys[Controls::LEFT] = Ctrls.Keys[Controls::RIGHT];
	Ctrls.Keys[Controls::RIGHT] = l;

	Dir = Vec2(0.0f, 0.0f);
	}*/

	if (CurrentBonus.BonusType == Bonus::RUN)
	{
		MoveFactor = DefaultMoveFactor;
	}

	if (CurrentBonus.BonusType == Bonus::PERMMOVE)
	{
		Dir = Vec2(0.0f, 0.0f);
	}

	CurrentBonus.BonusType = Bonus::NONE;
}


void Player::OnUpdate(float dt)
{
	AddForce(Dir*MoveFactor);
	//if (Velocity.x > MaxSpeed) Velocity.x = MaxSpeed;
	//if (Velocity.x < -MaxSpeed) Velocity.x = -MaxSpeed;
	//if (Velocity.y > 0) State = FALL;
}

void Player::OnKeyDown(int key)
{
	if (key == Ctrls.Keys[Controls::UP] && State != Player::JUMP2 && State != STAMP)
	{
		Velocity.y = min(Velocity.y, 0.0f);
		//AddForce(Vec2(0.0f, -230));


		if (State == JUMP)
		{
			AddForce(Vec2(0.0f, -180));
			State = JUMP2;
		}
		else
		{
			AddForce(Vec2(0.0f, -230));
			State = JUMP;
		}
	}
	if (key == Ctrls.Keys[Controls::DOWN])
	{
		AddForce(Vec2(0.0f, 400));
		State = STAMP;
	}
	if (key == Ctrls.Keys[Controls::RIGHT]) Dir.x = 1.0f;
	if (key == Ctrls.Keys[Controls::LEFT]) Dir.x = -1.0f;
}

void Player::OnKeyUp(int key)
{
	if (CurrentBonus.BonusType != Bonus::PERMMOVE)
	{
		if (key == Ctrls.Keys[Controls::LEFT] && Dir.x < 0.0f)
			Dir = Vec2(0.0f, 0.0f);
		if (key == Ctrls.Keys[Controls::RIGHT] && Dir.x > 0.0f)
			Dir = Vec2(0.0f, 0.0f);
	}
}

void Player::OnCollision(Object& obj, const Vec2& normal)
{
	if ((obj.Type == Object::PLATFORM || obj.Type == Object::BOX) && normal == Vec2(0.0f, -1.0f))
	{
		if (Velocity.x != 0.0f) State = Player::RUN;
		else State = Player::STAND;
	}
}