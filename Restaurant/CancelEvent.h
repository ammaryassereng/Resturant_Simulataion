#pragma once
#include"Events/Event.h"
#include"Rest/Order.h"
#include"Rest/Restaurant.h"
class CancelEvent : public Event
{


public:
	CancelEvent(int TS, int ID);
	void Execute(Restaurant*);
};

