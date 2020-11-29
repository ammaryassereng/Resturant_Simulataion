#include "PromotionEvent.h"

PromotionEvent::PromotionEvent(int TS, int ID, double exmon) :Event(TS, ID)
{
	EXmoney = exmon;
}

void PromotionEvent::Execute(Restaurant* pRest)
{
	Order* pOrd;
	pOrd = pRest->FindOrderInArvOrders(OrderID);
	if (pOrd && pOrd->GetType()==TYPE_NRM)
	{
		pOrd->SetMoney(pOrd->GetMoney() + EXmoney);
		pRest->PromoteOrder(pOrd);
	}
}