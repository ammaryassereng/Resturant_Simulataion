#include "Cook.h"


Cook::Cook()
{
	assignedorder = nullptr;
	numofordersassigned = 0;
	Health = NOTINJ;
	injuredBefore = false;
	InBreak = false;
	OrderAssignedLastInjury = nullptr;
}


Cook::~Cook()
{
}


int Cook::GetID() const
{
	return ID;
}


ORD_TYPE Cook::GetType() const
{
	return type;
}


void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}
void Cook::setbreaktime(int s)
{
	breaktime = s;
}

int Cook::getbreaktime()
{
	return breaktime;
}

bool Cook::isavailabe()
{
	int fraction = numofordersassigned % BO;

	if (assignedorder == nullptr && fraction != 1)
	{
		return true;
	}
	if (assignedorder != nullptr)
	{
		return false;
	}
}

void Cook::assignorder(Order* p)
{
	if (p)
		numofordersassigned = numofordersassigned + 1;
	assignedorder = p;
}
void Cook::setspeed(int s)
{
	speed = s;
}
int Cook::getspeed()
{
	return speed;
}
void Cook::setBo(int b)
{
	BO = b;
}
int Cook::getBo()
{
	return BO;
}
bool Cook::needbreak()
{
	if (numofordersassigned % BO == 0 && numofordersassigned != 0)
		return true;

	return false;

}

Order* Cook::GetAssignedOrder()
{
	return assignedorder;
}

void Cook::setHealth(COOK_STATUS b)
{
	Health = b;
}
COOK_STATUS  Cook::getHealth()
{
	return Health;
}

void  Cook::setrestduration(int x)
{
	restduration = x;
}
int  Cook::getrestduration()
{
	return restduration;
}

void Cook::setinjuredtime(int x)
{
	injuredtime = x;
	Health = INJURED;
}
int Cook::getinjuredtime()
{
	return injuredtime;
}

void Cook::setInjuryI(int x)
{
	InjuryInstant = x;
}
int Cook::getInjuryI()
{
	return InjuryInstant;
}

void Cook::setBreakI(int b)
{
	breakInstant = b;
}
int Cook::getBreakI()
{
	return breakInstant;
}

void Cook::setInjury(bool b)
{
	injuredBefore = b;
}
bool Cook::getInjuryBefore() const
{
	return injuredBefore;
}

void Cook::setOddSpeed(bool a)
{
	IsOddSpeed = a;
}

bool Cook::getOddSpeed()
{
	return IsOddSpeed;
}

void Cook::setInBreak(bool a)
{
	InBreak = a;
}

bool Cook::getInBreak()
{
	return InBreak;
}

void Cook::setOrderInLastInjury(Order* pOrd)
{
	OrderAssignedLastInjury = pOrd;
}

Order* Cook::getOrderInLastInjury()
{
	return OrderAssignedLastInjury;
}