#include "Order.h"

Order::Order(int id, ORD_TYPE r_Type)
{
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = r_Type;
	status = WAIT;
	WaitTime = ServTime = 0;
	if (type == TYPE_VIP)
	{
		priority = (double)WaitTime + 0.5 * (double)totalMoney + 1.25 * (double)size;
	}
	else
	{
		priority = 0;
	}
}

Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}

void Order::ChangeType(ORD_TYPE ptype)
{
	type = ptype;
}

ORD_TYPE Order::GetType() const
{
	return type;
}


void Order::SetDistance(int d)
{
	Distance = d>0?d:0;
}

int Order::GetDistance() const
{
	return Distance;
}


void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

void Order::SetSize(int S)
{
	size = S;
}

int Order::GetSize() const
{
	return size;
}

void Order::SetMoney(double mon)
{
	totalMoney = mon;
}

double Order::GetMoney() const
{
	return totalMoney;
}

void Order::setArrivalT(int a)
{
	ArrTime = a;
}

int Order::getArrivalT() const
{
	return ArrTime;
}

int Order::getWaitT() const
{
	return WaitTime;
}

void Order::setWaitT(int w)
{
	WaitTime = w;
}

void Order::setServT(int s)
{
	ServTime = s;
}

int Order::getServT() const
{
	return ServTime;
}


void Order::setFinishT(int f)
{
	FinishTime = f;
}
int Order::getFinishT() const
{
	return FinishTime;
}

void Order::InitializePriority()
{
	if (type == TYPE_VIP)
	{
		priority = (double)WaitTime + 0.5 * (double)totalMoney + 1.25 * (double)size;
	}
	else
	{
		priority = 0;
	}
}

double Order::GetPriority()
{
	return priority;
}

bool Order::operator > (Order* Pord)
{
	if (type == TYPE_VIP && Pord->type != TYPE_VIP)
	{
		return true;
	}
	else if (type != TYPE_VIP && Pord->type == TYPE_VIP)
	{
		return false;
	}
	else if (type != TYPE_VIP && Pord->type != TYPE_VIP)
	{
		return false;
	}
	else if (type == TYPE_VIP && Pord->type == TYPE_VIP)
	{
		if (priority > Pord->priority)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Order::operator < (Order* Pord)
{
	if (type == TYPE_VIP && Pord->type != TYPE_VIP)
	{
		return false;
	}
	else if (type != TYPE_VIP && Pord->type == TYPE_VIP)
	{
		return true;
	}
	else if (type != TYPE_VIP && Pord->type != TYPE_VIP)
	{
		return false;
	}
	else if (type == TYPE_VIP && Pord->type == TYPE_VIP)
	{
		if (priority < Pord->priority)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Order::operator == (Order* Pord)
{
	if (type == TYPE_VIP && Pord->type != TYPE_VIP)
	{
		return false;
	}
	else if (type != TYPE_VIP && Pord->type == TYPE_VIP)
	{
		return false;
	}
	else if (type != TYPE_VIP && Pord->type != TYPE_VIP)
	{
		return true;
	}
	else if (type == TYPE_VIP && Pord->type == TYPE_VIP)
	{
		if (priority == Pord->priority)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
