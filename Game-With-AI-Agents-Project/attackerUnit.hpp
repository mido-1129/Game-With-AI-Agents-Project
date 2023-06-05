/*
* attackerUnit.hpp
*/

#pragma once

#include "enemyUnit.hpp"

class AttackerUnit : public EnemyUnit
{
public:
	AttackerUnit(Unit* target);
	~AttackerUnit();

	void attack();

protected:
	Unit* _target;
};