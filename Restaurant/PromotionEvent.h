#pragma once
#include"Events/Event.h"
#include"Rest/Order.h"
#include"Rest/Restaurant.h"
class PromotionEvent : public Event
{
private:
	double EXmoney;
public:
	PromotionEvent(int TS, int ID, double exmon);
	void Execute(Restaurant* pRest);
};

