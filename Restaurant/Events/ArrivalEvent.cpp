#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType):Event(eTime, oID)
{
	OrdType = oType;
}

ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, double money, int S) : Event(eTime, oID)
{
	OrdType = oType;
	OrdMoney = money;
	size = S;
}

void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create an order and fills its data 
	// Then adds it to normal, vegan, or VIP order lists that you will create in phase1
	Order* pOrd = new Order(OrderID, OrdType);
	pOrd->SetSize(size);
	pOrd->setStatus(WAIT);
	pOrd->SetMoney(OrdMoney);
	pOrd->setArrivalT(EventTime);
	pOrd->InitializePriority();
	pRest->ADDtoArvOrders(pOrd);
	if (OrdType == TYPE_NRM)
	{
		pRest->ADDtoNormalOrders(pOrd);
	}
	else if (OrdType == TYPE_VIP)
	{
		pRest->ADDtoVipOrders(pOrd);
	}
	else
	{
		pRest->ADDtoVeganOrders(pOrd);
	}





	///For the sake of demo, this function will just create an order and add it to DemoQueue
	///Remove the next code lines in phases 1&2
	/*Order* pOrd = new Order(OrderID,OrdType);
	pRest->AddtoDemoQueue(pOrd);*/
}
