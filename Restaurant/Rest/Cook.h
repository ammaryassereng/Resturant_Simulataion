#pragma once
#include "Order.h"
#include "..\Defs.h"
#pragma once
class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	Order* assignedorder;
	int numofordersassigned;
	int BO; //the number of orders a cook must prepare before taking a break
	COOK_STATUS	Health;		//Injured - NotInjured - In-rest
	int restduration;
	bool injuredBefore;

	int breaktime;
	int breakInstant;

	int injuredtime;			//Injury Time		
	int InjuryInstant;		//The instant at which a Cook may be Injured

	bool IsOddSpeed;
	bool InBreak;

	Order* OrderAssignedLastInjury;

public:
	Cook();
	virtual ~Cook();
	int GetID() const;
	ORD_TYPE GetType() const;

	void setID(int);
	void setType(ORD_TYPE);

	void setbreaktime(int);
	int getbreaktime();

	void setBo(int);
	int getBo();

	bool needbreak();
	bool isavailabe();

	void setspeed(int);
	int getspeed();

	void assignorder(Order*);
	Order* GetAssignedOrder();

	void setInjuryI(int);
	int getInjuryI();

	void setBreakI(int);
	int getBreakI();

	void setinjuredtime(int);
	int getinjuredtime();

	void setrestduration(int);
	int getrestduration();

	void setHealth(COOK_STATUS);
	COOK_STATUS getHealth();

	void setInjury(bool);
	bool getInjuryBefore() const;

	void setOddSpeed(bool);
	bool getOddSpeed();

	void setInBreak(bool);
	bool getInBreak();

	void setOrderInLastInjury(Order* pOrd);
	Order* getOrderInLastInjury();
};

