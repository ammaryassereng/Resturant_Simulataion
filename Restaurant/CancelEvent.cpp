#include "CancelEvent.h"

CancelEvent::CancelEvent(int TS, int ID) :Event(TS, ID)
{
}

void CancelEvent::Execute(Restaurant* x)
{

	Order* canceld;
	canceld = x->FindOrderInArvOrders(OrderID);

	if (canceld != nullptr && canceld->GetType()==TYPE_NRM)
	{
		x->RemoveFromArrivalOrders(canceld);
		x->RemoveFromNormalOrders(canceld);
		delete canceld;
	}
	else
	{
		return;
	}

}