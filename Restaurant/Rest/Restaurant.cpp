#include <cstdlib>
#include <time.h>
#include <iostream>
using namespace std;
#include "Restaurant.h"


Restaurant::Restaurant()
{
	pGUI = NULL;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();

	Simulator(mode);		//Calling the simulator function
}



//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event* pE;
	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}


Restaurant::~Restaurant()
{
	if (pGUI)
		delete pGUI;
	while (!DoneOrders.IsEmpty())			//this line will make sure that all memory used for orders is freed
	{										//we don't need to make this operation on the waiting orders and the inservice because at the end all orders must be done
		Order* pOrd;						//this function will always call the first element, remove it from the list and delete it
		DoneOrders.Peek(pOrd);
		DoneOrders.DeleteFirst();
		delete pOrd;
	}
	while (!AvailableCooks.IsEmpty())		//this operation will do the same but for the cooks
	{
		Cook* pCook;
		AvailableCooks.Peek(pCook);
		AvailableCooks.DeleteFirst();
		delete pCook;
	}
}

void Restaurant::FillDrawingList()
{
	//This function should be implemented in phase1
	//It should add ALL orders and Cooks to the drawing list
	//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)
	//To add orders it should call function  void GUI::AddToDrawingList(Order* pOrd);
	//To add Cooks it should call function  void GUI::AddToDrawingList(Cook* pCc);
	pGUI->ResetDrawingList();
	LinkedList<Cook*> Ctemp;
	LinkedList<Order*> Otemp;
	Queue<Order*> Otemp2;
	// Add the cooks to drawing lists
	while (!AvailableCooks.IsEmpty())
	{
		Cook* pCook;
		AvailableCooks.Peek(pCook);
		AvailableCooks.DeleteFirst();
		pGUI->AddToDrawingList(pCook);
		Ctemp.ADD(pCook);
	}
	while (!Ctemp.IsEmpty())
	{
		Cook* pCook;
		Ctemp.Peek(pCook);
		Ctemp.DeleteFirst();
		AvailableCooks.ADD(pCook);
	}
	// Add the orders to the drawing list in the waiting part
	while (!ArrivalOrders.IsEmpty())
	{
		Order* pORD;
		ArrivalOrders.Peek(pORD);
		ArrivalOrders.DeleteFirst();
		if (pORD->GetType() == TYPE_VIP)
		{
			pGUI->AddToDrawingList(pORD);
		}
		Otemp.ADD(pORD);
		//pGUI->UpdateInterface();
	}
	while (!Otemp.IsEmpty())
	{
		Order* pORD;
		Otemp.Peek(pORD);
		Otemp.DeleteFirst();
		ArrivalOrders.ADD(pORD);
	}
	while (!VeganOrders.isEmpty())
	{
		Order* pORD;
		VeganOrders.dequeue(pORD);
		pGUI->AddToDrawingList(pORD);
		Otemp2.enqueue(pORD);
	}
	while (!Otemp2.isEmpty())
	{
		Order* pOrd;
		Otemp2.dequeue(pOrd);
		VeganOrders.enqueue(pOrd);
	}
	while (!NormalOrders.isEmpty())
	{
		Order* pORD;
		NormalOrders.dequeue(pORD);
		pGUI->AddToDrawingList(pORD);
		Otemp2.enqueue(pORD);
	}
	while (!Otemp2.isEmpty())
	{
		Order* pOrd;
		Otemp2.dequeue(pOrd);
		NormalOrders.enqueue(pOrd);
	}
	// Add orders sorted by first in service in the inservice list
	while (!InServiceOrders.IsEmpty())
	{
		Order* pORD;
		InServiceOrders.Peek(pORD);
		InServiceOrders.DeleteFirst();
		pGUI->AddToDrawingList(pORD);
		Otemp.ADD(pORD);
		//pGUI->UpdateInterface();
	}
	while (!Otemp.IsEmpty())
	{
		Order* pORD;
		Otemp.Peek(pORD);
		Otemp.DeleteFirst();
		InServiceOrders.ADD(pORD);
	}
	// Add done orders by the the finished time in the Finished orders list
	while (!DoneOrders.IsEmpty())
	{
		Order* pORD;
		DoneOrders.Peek(pORD);
		DoneOrders.DeleteFirst();
		pGUI->AddToDrawingList(pORD);
		Otemp.ADD(pORD);
		pGUI->UpdateInterface();
	}
	while (!Otemp.IsEmpty())
	{
		Order* pORD;
		Otemp.Peek(pORD);
		Otemp.DeleteFirst();
		DoneOrders.ADD(pORD);
	}
	pGUI->UpdateInterface();
}


void Restaurant::FileLoadingFunction()
{
	TotalNormalOrdrs = 0;
	pGUI->PrintMessage("Please type the file name: ");
	File = pGUI->GetString();
	File = File + ".txt";
	fstream txtfile;
	txtfile.open(File);
	if (txtfile.fail())
	{
		pGUI->PrintMessage("ERROR_File Not Found_Waiting for mouse click");
		pGUI->waitForClick();
		exit(0);
	}
	int N, G, V;
	int SN_min, SN_max, SG_min, SG_max, SV_min, SV_max;
	int BO, BN_min, BN_max, BG_min, BG_max, BV_min, BV_max;
	txtfile >> N >> G >> V >> SN_min >> SN_max >> SG_min >> SG_max >> SV_min >> SV_max >> BO >> BN_min >> BN_max >> BG_min >> BG_max >> BV_min >> BV_max;
	txtfile >> InjProb >> RstPrd;
	txtfile >> TimeForPromotion >> VIP_WT;
	int id = 1;
	for (int j = 0; j < N; j++)
	{
		Cook* pCook = new Cook;
		pCook->setID(id);
		srand(time(NULL));
		int SN = rand() % (SN_max - SN_min + 1) + SN_min;
		pCook->setspeed(SN);
		if (SN % 2 == 0)
		{
			pCook->setOddSpeed(false);
		}
		else
		{
			pCook->setOddSpeed(true);
		}
		pCook->setType(TYPE_NRM);
		srand(time(NULL));
		int BN = rand() % (BN_max - BN_min + 1) + BN_min;
		pCook->setbreaktime(BN);
		pCook->setBo(BO);
		id++;
		AvailableCooks.InsertEnd(pCook);
	}
	for (int j = 0; j < G; j++)
	{
		Cook* pCook = new Cook;
		pCook->setID(id);
		srand(time(NULL));
		int SG = rand() % (SG_max - SG_min + 1) + SG_min;
		pCook->setspeed(SG);
		if (SG % 2 == 0)
		{
			pCook->setOddSpeed(false);
		}
		else
		{
			pCook->setOddSpeed(true);
		}
		pCook->setType(TYPE_VGAN);
		srand(time(NULL));
		int BG = rand() % (BG_max - BG_min + 1) + BG_min;
		pCook->setbreaktime(BG);
		pCook->setBo(BO);
		id++;
		AvailableCooks.InsertEnd(pCook);
	}
	for (int j = 0; j < V; j++)
	{
		Cook* pCook = new Cook;
		pCook->setID(id);
		srand(time(NULL));
		int SV = rand() % (SV_max - SV_min + 1) + SV_min;
		pCook->setspeed(SV);
		if (SV % 2 == 0)
		{
			pCook->setOddSpeed(false);
		}
		else
		{
			pCook->setOddSpeed(true);
		}
		pCook->setType(TYPE_VIP);
		srand(time(NULL));
		int BV = rand() % (BV_max - BV_min + 1) + BV_min;
		pCook->setbreaktime(BV);
		pCook->setBo(BO);
		id++;
		AvailableCooks.InsertEnd(pCook);
	}
	int n;
	txtfile >> n;
	for (int i = 0; i < n; i++)
	{
		char TypeOfEvent;
		txtfile >> TypeOfEvent;
		if (TypeOfEvent == 'R')
		{
			char type;
			int TS, ID, Size;
			double Money;
			txtfile >> type >> TS >> ID >> Size >> Money;
			ORD_TYPE TYPE;
			if (type == 'N')
			{
				TotalNormalOrdrs++;
				TYPE = TYPE_NRM;
			}
			else if (type == 'V')
			{
				TYPE = TYPE_VIP;
			}
			else
			{
				TYPE = TYPE_VGAN;
			}
			ArrivalEvent* NewOrder = new ArrivalEvent(TS, ID, TYPE, Money, Size);
			EventsQueue.enqueue(NewOrder);
		}
		else if (TypeOfEvent == 'P')
		{
			int TS, ID;
			double exmon;
			txtfile >> TS >> ID >> exmon;
			PromotionEvent* prom = new PromotionEvent(TS, ID, exmon);
			EventsQueue.enqueue(prom);       // Commented because it is told to ignore promotion events
		}
		else if (TypeOfEvent == 'X')
		{
			int TS, ID;
			txtfile >> TS >> ID;
			CancelEvent* pCancel = new CancelEvent(TS, ID);
			EventsQueue.enqueue(pCancel);
		}
	}
}

void Restaurant::countCookTypes(int& N, int& G, int& V)
{
	N = G = V = 0;	//Sets the cooks' counts to 0
	Cook* pC = nullptr;
	AvailableCooks.Peek(pC);
	LinkedList<Cook*> tmp;
	while (pC)
	{
		if (pC->GetType() == TYPE_NRM)
			N++;
		else if (pC->GetType() == TYPE_VGAN)
			G++;
		else if (pC->GetType() == TYPE_VIP)
			V++;
		tmp.InsertEnd(pC);
		AvailableCooks.DeleteFirst();
		if (!AvailableCooks.Peek(pC))
			break;
	}
	tmp.Peek(pC);
	while (pC)
	{
		AvailableCooks.InsertEnd(pC);
		tmp.DeleteFirst();
		if (!tmp.Peek(pC))
			break;
	}
}
void Restaurant::countInserviceTypes(int& N, int& G, int& V)
{
	N = G = V = 0;	//Sets the orders' counts to 0
	Order* pO = nullptr;
	InServiceOrders.Peek(pO);
	LinkedList<Order*> tmp;
	while (pO)
	{
		if (pO->GetType() == TYPE_NRM)
			N++;
		else if (pO->GetType() == TYPE_VGAN)
			G++;
		else if (pO->GetType() == TYPE_VIP)
			V++;
		tmp.InsertEnd(pO);
		InServiceOrders.DeleteFirst();
		if (!InServiceOrders.Peek(pO))
			break;
	}
	tmp.Peek(pO);
	while (pO)
	{
		InServiceOrders.InsertEnd(pO);
		tmp.DeleteFirst();
		if (!tmp.Peek(pO))
			break;
	}
}

void Restaurant::PromoteOrder(Order* pOrd)
{
	pOrd->ChangeType(TYPE_VIP);
	ADDtoVipOrders(pOrd);
	RemoveFromNormalOrders(pOrd);
}

void Restaurant::GeneralPromote(int TS)  // it Should be called each time step;
{
	Order* pOrd;
	while (!NormalOrders.isEmpty())
	{
		NormalOrders.peekFront(pOrd);
		if (pOrd->getWaitT() > TimeForPromotion)
		{
			NormalOrders.dequeue(pOrd);
			pOrd->ChangeType(TYPE_VIP);
			ADDtoVipOrders(pOrd);
			NumberOfPromotion++;
		}
		else
		{
			return;
		}
	}
}

void Restaurant::SortVIPOrders() // it should be called each timestep
{
	Queue<Order*> temp;
	while (!VIPOrders.IsEmpty())
	{
		Order* pOrd;
		VIPOrders.Peek(pOrd);
		temp.enqueue(pOrd);
		VIPOrders.DeleteFirst();
	}
	int size;
	Order** Arr = temp.toArray(size);
	bool sorted = false;
	while (sorted == false)
	{
		sorted = true;
		for (int i = 0; i < size - 1; i++)
		{
			double priority1 = Arr[i]->GetPriority();
			double priority2 = Arr[i + 1]->GetPriority();
			if (priority2 > priority1)
			{
				Order* ptr;
				ptr = Arr[i + 1];
				Arr[i + 1] = Arr[i];
				Arr[i] = ptr;
				sorted = false;
			}
		}
	}
	for (int i = size - 1; i >= 0; i--)
	{
		VIPOrders.ADD(Arr[i]);
	}
}

void Restaurant::IncrementWaiting()
{
	Order* pOrd;
	LinkedList<Order*> temp;
	while (!ArrivalOrders.IsEmpty())
	{
		ArrivalOrders.Peek(pOrd);
		int wait = pOrd->getWaitT();
		wait++;
		pOrd->setWaitT(wait);
		if (pOrd->GetType() == TYPE_VIP)
		{
			pOrd->InitializePriority();
		}
		temp.ADD(pOrd);
		ArrivalOrders.DeleteFirst();
	}
	while (!temp.IsEmpty())
	{
		temp.Peek(pOrd);
		ArrivalOrders.ADD(pOrd);
		temp.DeleteFirst();
	}
}

void Restaurant::SortDone()
{
	LinkedList<Order*> temp1;
	while (!DoneOrders.IsEmpty())
	{
		Order* pOrd1;
		DoneOrders.Peek(pOrd1);
		DoneOrders.DeleteFirst();
		Order* pOrd2;
		if (DoneOrders.Peek(pOrd2))
		{
			if (pOrd1->getArrivalT() + pOrd1->getWaitT() + pOrd1->getServT() == pOrd2->getArrivalT() + pOrd2->getWaitT() + pOrd2->getServT())
			{
				int FT = pOrd1->getArrivalT() + pOrd1->getWaitT() + pOrd1->getServT();
				int size = 0;
				Order** arr;
				Queue<Order*>temp2;
				temp2.enqueue(pOrd1);
				temp2.enqueue(pOrd2);
				DoneOrders.DeleteFirst();
				Order* pOrd;
				while (DoneOrders.Peek(pOrd))
				{
					if (pOrd->getArrivalT() + pOrd->getWaitT() + pOrd->getServT() == FT)
					{
						temp2.enqueue(pOrd);
						DoneOrders.DeleteFirst();
					}
					else
					{
						break;
					}
				}
				arr = temp2.toArray(size);
				bool sorted = false;
				while (sorted == false)
				{
					sorted = true;
					for (int i = 0; i < size - 1; i++)
					{
						int FT1 = arr[i]->getServT();
						int FT2 = arr[i + 1]->getServT();
						if (FT2 > FT1)
						{
							Order* ptr;
							ptr = arr[i + 1];
							arr[i + 1] = arr[i];
							arr[i] = ptr;
							sorted = false;
						}
					}
				}
				for (int i = size - 1; i >= 0; i--)
				{
					temp1.ADD(arr[i]);
				}
			}
			else
			{
				temp1.ADD(pOrd1);
			}
		}
		else
		{
			temp1.ADD(pOrd1);
		}
	}
	while (!temp1.IsEmpty())
	{
		Order* pOrd;
		temp1.Peek(pOrd);
		temp1.DeleteFirst();
		DoneOrders.ADD(pOrd);
	}
}

void Restaurant::OutputFile()
{
	SortDone();
	File = "Out_" + File;
	ofstream out(File);
	int No_Orders = 0, No_Cooks = 0, totalWait = 0, TotalService = 0;
	int Norm_orders = 0, Veg_Orders = 0, VIP_Orders = 0;
	int Norm_Cooks = 0, Veg_Cooks = 0, VIP_Cooks = 0;
	this->countCookTypes(Norm_Cooks, Veg_Cooks, VIP_Cooks);
	No_Cooks = Norm_Cooks + Veg_Cooks + VIP_Cooks;
	out << "FT\tID\tAT\tWT\tST" << endl;
	while (!DoneOrders.IsEmpty())
	{
		Order* pOrd;
		DoneOrders.Peek(pOrd);
		out << pOrd->getArrivalT() + pOrd->getWaitT() + pOrd->getServT() << "\t" << pOrd->GetID() << "\t" << pOrd->getArrivalT() << "\t" << pOrd->getWaitT() << "\t" << pOrd->getServT() << endl;
		No_Orders++;
		if (pOrd->GetType() == TYPE_NRM)
		{
			Norm_orders++;
		}
		else if (pOrd->GetType() == TYPE_VGAN)
		{
			Veg_Orders++;
		}
		else
		{
			VIP_Orders++;
		}
		totalWait += pOrd->getWaitT();
		TotalService += pOrd->getServT();
		delete pOrd;
		DoneOrders.DeleteFirst();
	}
	out << "Orders: " << No_Orders << "\t[Norm:" << Norm_orders << ", Veg:" << Veg_Orders << ", VIP:" << VIP_Orders << "]" << endl;
	out << "Cooks: " << No_Cooks << "\t[Norm:" << Norm_Cooks << ", Veg:" << Veg_Cooks << ", Vip:" << VIP_Cooks << ", injured:" << getInjuredCooks() << "]" << endl;
	out << "Avg Wait = " << (double)totalWait / No_Orders << ", Avg Serv = " << (double)TotalService / No_Orders << endl;
	double percent;
	if (TotalNormalOrdrs != 0)
	{
		percent = ((double)NumberOfPromotion / TotalNormalOrdrs) * 100;
	}
	else
	{
		percent = 0;
	}
	out << "Urgent orders: " << NumberOfUrgent << ", Auto-promoted: " << percent << "%" << endl;
}

// Functions that add to lists

void Restaurant::ADDtoAvailableCooks(Cook* pCook)
{
	AvailableCooks.InsertEnd(pCook);
}
void Restaurant::ADDtoNonAvailableCooks(Cook* pCook)
{
	NonAvailableCooks.InsertEnd(pCook);
}
void Restaurant::ADDtoNormalOrders(Order* pOrd)
{
	NormalOrders.enqueue(pOrd);
}
void Restaurant::ADDtoVipOrders(Order* pOrd)
{
	VIPOrders.InsertEnd(pOrd);  //it should be modified in phase 2 to use InsertSrted Function 

}
void Restaurant::ADDtoVeganOrders(Order* pOrd)
{
	VeganOrders.enqueue(pOrd);
}
void Restaurant::ADDtoArvOrders(Order* pOrd)
{
	ArrivalOrders.InsertEnd(pOrd);
}
void Restaurant::ADDtoInServiceOrders(Order* pOrd)
{
	InServiceOrders.InsertEnd(pOrd);
}
void Restaurant::ADDtoDoneOrders(Order* pOrd)
{
	DoneOrders.InsertEnd(pOrd);
}


//Functions To find Data using the ID

Order* Restaurant::FindOrderInArvOrders(int ID)
{
	Order* pOrd;
	LinkedList<Order*> temp;
	bool found = false;
	while (!ArrivalOrders.IsEmpty())
	{
		ArrivalOrders.Peek(pOrd);
		if (pOrd->GetID() == ID)
		{
			found = true;
			break;
		}
		else
		{
			temp.ADD(pOrd);
			ArrivalOrders.DeleteFirst();
		}
	}
	if (found == true)			// found should always be true, the check is just in case something happens
	{
		while (!temp.IsEmpty())
		{
			Order* Ord;
			temp.Peek(Ord);
			temp.DeleteFirst();
			ArrivalOrders.ADD(Ord);
		}
		return pOrd;
	}
	else
	{
		while (!temp.IsEmpty())
		{
			Order* Ord;
			temp.Peek(Ord);
			temp.DeleteFirst();
			ArrivalOrders.ADD(Ord);
		}
		return nullptr;
	}
}
Order* Restaurant::FindOrderInInserviceList(int ID)
{
	Order* pOrd;
	LinkedList<Order*> temp;
	bool found = false;
	while (!InServiceOrders.IsEmpty())
	{
		InServiceOrders.Peek(pOrd);
		if (pOrd->GetID() == ID)
		{
			found = true;
			break;
		}
		else
		{
			temp.ADD(pOrd);
			InServiceOrders.DeleteFirst();
		}
	}
	if (found == true)			// found should always be true, the check is just in case something happens
	{
		while (!temp.IsEmpty())
		{
			Order* Ord;
			temp.Peek(Ord);
			temp.DeleteFirst();
			InServiceOrders.ADD(Ord);
		}
		return pOrd;
	}
	else
	{
		while (!temp.IsEmpty())
		{
			Order* Ord;
			temp.Peek(Ord);
			temp.DeleteFirst();
			InServiceOrders.ADD(Ord);
		}
		return nullptr;
	}
}
Cook* Restaurant::FindCookInAvailableCooks(int ID)
{
	Cook* pCook;
	LinkedList<Cook*> temp;
	bool found = false;
	while (!AvailableCooks.IsEmpty())
	{
		AvailableCooks.Peek(pCook);
		if (pCook->GetID() == ID)
		{
			found = true;
			break;
		}
		else
		{
			temp.ADD(pCook);
			AvailableCooks.DeleteFirst();
		}
	}
	if (found == true)			// found should always be true, the check is just in case something happens
	{
		while (!temp.IsEmpty())
		{
			Cook* COOK;
			temp.Peek(COOK);
			temp.DeleteFirst();
			AvailableCooks.ADD(COOK);
		}
		return pCook;
	}
	else
	{
		while (!temp.IsEmpty())
		{
			Cook* COOK;
			temp.Peek(COOK);
			temp.DeleteFirst();
			AvailableCooks.ADD(COOK);
		}
		return nullptr;
	}
}
Cook* Restaurant::FindCookInNonAvailableCooks(int ID)
{
	Cook* pCook;
	LinkedList<Cook*> temp;
	bool found = false;
	while (!NonAvailableCooks.IsEmpty())
	{
		NonAvailableCooks.Peek(pCook);
		if (pCook->GetID() == ID)
		{
			found = true;
			break;
		}
		else
		{
			temp.ADD(pCook);
			NonAvailableCooks.DeleteFirst();
		}
	}
	if (found == true)			// found should always be true, the check is just in case something happens
	{
		while (!temp.IsEmpty())
		{
			Cook* COOK;
			temp.Peek(COOK);
			temp.DeleteFirst();
			NonAvailableCooks.ADD(COOK);
		}
		return pCook;
	}
	else
	{
		while (!temp.IsEmpty())
		{
			Cook* COOK;
			temp.Peek(COOK);
			temp.DeleteFirst();
			NonAvailableCooks.ADD(COOK);
		}
		return nullptr;
	}
}



//Functions to remove from lists
void Restaurant::RemoveFromAvailableCooks(Cook* pCook)
{
	AvailableCooks.Remove(pCook);
}
void Restaurant::RemoveFromNonAvailableCooks(Cook* pCook)
{
	NonAvailableCooks.Remove(pCook);
}
void Restaurant::RemoveFromNormalOrders(Order* pOrd)
{
	Queue<Order*>temp;
	int ID = pOrd->GetID();
	while (!NormalOrders.isEmpty())
	{
		Order* pOrd2;
		NormalOrders.dequeue(pOrd2);
		if (pOrd2->GetID() != ID)
		{
			temp.enqueue(pOrd2);
		}
	}
	while (!temp.isEmpty())
	{
		Order* pOrd2;
		temp.dequeue(pOrd2);
		NormalOrders.enqueue(pOrd2);
	}
}
void Restaurant::RemoveFromVeganOrders(Order* pOrd)
{
	Queue<Order*>temp;
	int ID = pOrd->GetID();
	while (!VeganOrders.isEmpty())
	{
		Order* pOrd2;
		VeganOrders.dequeue(pOrd2);
		if (pOrd2->GetID() != ID)
		{
			temp.enqueue(pOrd2);
		}
	}
	while (!temp.isEmpty())
	{
		Order* pOrd2;
		temp.dequeue(pOrd2);
		VeganOrders.enqueue(pOrd2);
	}
}
void Restaurant::RemoveFromVipOrders(Order* pOrd)
{
	VIPOrders.Remove(pOrd);
}
void Restaurant::RemoveFromArrivalOrders(Order* pOrd)
{
	ArrivalOrders.Remove(pOrd);
}
void Restaurant::RemoveFromInServiceOrders(Order* pOrd)
{
	InServiceOrders.Remove(pOrd);
}

//Functions Responsible for serving Orders (Ordinary & Urgent)

void Restaurant::serveUrgents(int timestep)
{
	Cook* server;
	Order* served;
	LinkedList<Cook*> tmpC;
	LinkedList<Order*> tmpO;
	while (VIPOrders.Peek(served))
	{
		if (served->getWaitT() >= VIP_WT)
		{
			if (!AvailableCooks.IsEmpty())
			{
				if (getVc())
					server = getVc();
				else if (getNc())
					server = getNc();
				else
					server = getGc();
				RemoveFromAvailableCooks(server);
				ADDtoNonAvailableCooks(server);
				justAssigned.enqueue(server);
				server->assignorder(served);
				VIPOrders.DeleteFirst();
				RemoveFromArrivalOrders(served);
				ADDtoInServiceOrders(served);
				served->setWaitT(timestep - served->getArrivalT());
				served->setStatus(SRV);
				NumberOfUrgent++;
			}
			else
			{
				bool Taken = false;
				while (NonAvailableCooks.Peek(server))
				{
					if (!server->GetAssignedOrder())
					{
						server->assignorder(served);
						justAssigned.enqueue(server);
						VIPOrders.DeleteFirst();
						RemoveFromArrivalOrders(served);
						ADDtoInServiceOrders(served);
						served->setWaitT(timestep - served->getArrivalT());
						served->setStatus(SRV);
						Taken = true;
						NumberOfUrgent++;
						break;
					}
					else
					{
						RemoveFromNonAvailableCooks(server);
						tmpC.ADD(server);
					}
				}
				if (!Taken)
				{
					while (tmpC.Peek(server))
					{
						ADDtoNonAvailableCooks(server);
						tmpC.DeleteFirst();
					}
					break;
				}

			}
		}
		else
		{
			tmpO.ADD(served);
			VIPOrders.DeleteFirst();
		}
	}
	while (tmpO.Peek(served))
	{
		VIPOrders.ADD(served);
		tmpO.DeleteFirst();
	}
	SortVIPOrders();
}
void Restaurant::serveOrders(int timestep)
{
	serveUrgents(timestep);
	Cook* server;
	Order* served;
	while (VIPOrders.Peek(served))		//As long as there are waiting VIP Orders, loop
	{
		if (!AvailableCooks.IsEmpty())		//if there are available Cooks, assign the Orders
		{
			if (getVc())
				server = getVc();
			else if (getNc())
				server = getNc();
			else
				server = getGc();
			RemoveFromAvailableCooks(server);
			ADDtoNonAvailableCooks(server);
			justAssigned.enqueue(server);
			server->assignorder(served);
			VIPOrders.DeleteFirst();
			RemoveFromArrivalOrders(served);
			ADDtoInServiceOrders(served);
			served->setWaitT(timestep - served->getArrivalT());
			served->setStatus(SRV);
		}
		else break;		//Should be changed in Urgent cases
	}
	while (VeganOrders.peekFront(served))
	{
		if (!AvailableCooks.IsEmpty())		//if there are available Cooks, assign the Orders
			if (getGc())
			{
				server = getGc();
				RemoveFromAvailableCooks(server);
				ADDtoNonAvailableCooks(server);
				VeganOrders.dequeue(served);
				justAssigned.enqueue(server);
				server->assignorder(served);
				RemoveFromArrivalOrders(served);
				ADDtoInServiceOrders(served);
				served->setWaitT(timestep - served->getArrivalT());
				served->setStatus(SRV);
			}
			else break;
		else break;
	}
	while (NormalOrders.peekFront(served))
	{
		if (!AvailableCooks.IsEmpty())		//if there are available Cooks, assign the Orders
			if (getNc())
			{
				server = getNc();
				RemoveFromAvailableCooks(server);
				ADDtoNonAvailableCooks(server);
				NormalOrders.dequeue(served);
				justAssigned.enqueue(server);
				server->assignorder(served);
				RemoveFromArrivalOrders(served);
				ADDtoInServiceOrders(served);
				served->setWaitT(timestep - served->getArrivalT());
				served->setStatus(SRV);
			}
			else if (getVc())
			{
				server = getVc();
				RemoveFromAvailableCooks(server);
				ADDtoNonAvailableCooks(server);
				NormalOrders.dequeue(served);
				justAssigned.enqueue(server);
				server->assignorder(served);
				RemoveFromArrivalOrders(served);
				ADDtoInServiceOrders(served);
				served->setWaitT(timestep - served->getArrivalT());
				served->setStatus(SRV);
			}
			else break;
		else break;
	}
}

Cook* Restaurant::getVc()		//Gets an available VIP Cook
{
	Cook* c;
	Cook* v = nullptr;
	LinkedList<Cook*> tmp;
	while (AvailableCooks.Peek(c))
	{
		if (c->GetType() == TYPE_VIP)
		{
			v = c;
			break;
		}
		else
		{
			tmp.InsertEnd(c);
			AvailableCooks.DeleteFirst();
		}
	}
	while (tmp.Peek(c))
	{
		AvailableCooks.InsertEnd(c);
		tmp.DeleteFirst();
	}
	return v;
}

Cook* Restaurant::getNc()		//Gets an available Normal Cook
{
	Cook* c;
	Cook* v = nullptr;
	LinkedList<Cook*> tmp;
	while (AvailableCooks.Peek(c))
	{
		if (c->GetType() == TYPE_NRM)
		{
			v = c;
			break;
		}
		else
		{
			tmp.InsertEnd(c);
			AvailableCooks.DeleteFirst();
		}
	}
	while (tmp.Peek(c))
	{
		AvailableCooks.InsertEnd(c);
		tmp.DeleteFirst();
	}
	return v;
}

Cook* Restaurant::getGc()		//Gets an available veGan Cook
{
	Cook* c;
	Cook* v = nullptr;
	LinkedList<Cook*> tmp;
	while (AvailableCooks.Peek(c))
	{
		if (c->GetType() == TYPE_VGAN)
		{
			v = c;
			break;
		}
		else
		{
			tmp.InsertEnd(c);
			AvailableCooks.DeleteFirst();
		}
	}
	while (tmp.Peek(c))
	{
		AvailableCooks.InsertEnd(c);
		tmp.DeleteFirst();
	}
	return v;
}

/*=============================================================================================================*/


void Restaurant::FinishOrders(int timestep)
{
	Cook* c;
	LinkedList<Cook*> tmp;
	while (NonAvailableCooks.Peek(c))
	{
		if (c->GetAssignedOrder() && (timestep - c->GetAssignedOrder()->getWaitT() - c->GetAssignedOrder()->getArrivalT() >= ceil((float)c->GetAssignedOrder()->GetSize() / c->getspeed())))		//Greater than or Equal because the Cook may be injured so the expected time to finish is then variable with time as well
		{
			c->GetAssignedOrder()->setServT(timestep - c->GetAssignedOrder()->getWaitT() - c->GetAssignedOrder()->getArrivalT());
			c->GetAssignedOrder()->setStatus(DONE);
			RemoveFromInServiceOrders(c->GetAssignedOrder());
			ADDtoDoneOrders(c->GetAssignedOrder());
			c->assignorder(nullptr);
			if (c->getHealth() == INJURED)
			{
				c->setInjuryI(timestep);		//To start the rest period from the beginning
				RemoveFromNonAvailableCooks(c);
				tmp.ADD(c);
			}
			else
			{
				RemoveFromNonAvailableCooks(c);
				ADDtoAvailableCooks(c);
			}
		}
		else
		{
			tmp.ADD(c);
			NonAvailableCooks.DeleteFirst();
		}
	}
	while (tmp.Peek(c))
	{
		NonAvailableCooks.InsertEnd(c);
		tmp.DeleteFirst();
	}
}
//===================================================================================================
//Cook & Injuries

void Restaurant::injureCooks(int timestep)
{
	Cook* pC;
	LinkedList<Cook*> tmp;
	while (NonAvailableCooks.Peek(pC))
	{
		if (pC->getHealth() == NOTINJ && pC->GetAssignedOrder())		//Responsible for Cook Injury
		{
			if ((float)rand() / RAND_MAX <= InjProb)
			{
				pC->setOrderInLastInjury(pC->GetAssignedOrder());
				pC->setHealth(INJURED);
				if (pC->getspeed() != 1)
				{
					pC->setspeed(pC->getspeed() / 2.0);		//Reduces the Cook's Speed to its half
				}
				injuredCooks.ADD(pC);
				pC->setInjuryI(timestep);
				pC->setInjury(true);
			}
		}
		else if (pC->getHealth() == INJURED && timestep - pC->getInjuryI() >= RstPrd)		//Cook's Recovery
		{
			pC->setHealth(NOTINJ);
			if (pC->getOddSpeed() == false)
			{
				pC->setspeed(pC->getspeed() * 2.0);		//Returns the Cook's Speed to its Normal Speed
			}
			else
			{
				if (pC->getspeed() != 1)
				{
					pC->setspeed((pC->getspeed() * 2.0) + 1);
				}
			}
			injuredCooks.Remove(pC);
		}
		RemoveFromNonAvailableCooks(pC);
		tmp.ADD(pC);
	}
	while (tmp.Peek(pC))
	{
		ADDtoNonAvailableCooks(pC);
		tmp.DeleteFirst();
	}
}

void Restaurant::takeBreak(int timestep)
{
	Cook* pC;
	LinkedList<Cook*> tmp;
	while (AvailableCooks.Peek(pC))		//Checks if the Cook needs a break or not
	{
		if (pC->needbreak())
		{
			pC->setInBreak(true);
			RemoveFromAvailableCooks(pC);
			ADDtoNonAvailableCooks(pC);
			pC->setBreakI(timestep);
		}
		else
		{
			RemoveFromAvailableCooks(pC);
			tmp.ADD(pC);
		}
	}
	while (tmp.Peek(pC))
	{
		tmp.DeleteFirst();
		AvailableCooks.ADD(pC);
	}
	while (NonAvailableCooks.Peek(pC))		//Returns the in-Break Cooks to Work
	{
		if (pC->getbreaktime() <= timestep - pC->getBreakI() && !pC->GetAssignedOrder())
		{
			pC->setInBreak(false);
			RemoveFromNonAvailableCooks(pC);
			ADDtoAvailableCooks(pC);
		}
		else
		{
			tmp.ADD(pC);
			RemoveFromNonAvailableCooks(pC);
		}
	}
	while (tmp.Peek(pC))
	{
		ADDtoNonAvailableCooks(pC);
		tmp.DeleteFirst();
	}
}
int Restaurant::getInjuredCooks()
{
	Cook* pC;
	LinkedList<Cook*> tmp;
	int count = 0;
	while (AvailableCooks.Peek(pC))
	{
		tmp.ADD(pC);
		AvailableCooks.DeleteFirst();
		if (pC->getInjuryBefore())
			count++;
	}
	while (tmp.Peek(pC))
	{
		AvailableCooks.ADD(pC);
		tmp.DeleteFirst();
	}
	return count;
}

/*Simulator Function*/
void Restaurant::Simulator(PROG_MODE mode)
{
	NumberOfPromotion = 0;
	NumberOfUrgent = 0;
	FileLoadingFunction();
	int timestep = -1;
	if (mode == MODE_SLNT)		//Should be changed when Silent Mode is ready
	{
		while (!(EventsQueue.isEmpty() && InServiceOrders.IsEmpty() && ArrivalOrders.IsEmpty()))
		{
			timestep++;
			ExecuteEvents(timestep);
			GeneralPromote(timestep);
			SortVIPOrders();
			FinishOrders(timestep);
			injureCooks(timestep);
			takeBreak(timestep);
			serveOrders(timestep);
			IncrementWaiting();
			if (EventsQueue.isEmpty() && InServiceOrders.IsEmpty() && ArrivalOrders.IsEmpty())
			{
				while (!NonAvailableCooks.IsEmpty())
				{
					Cook* pCook;
					NonAvailableCooks.Peek(pCook);
					NonAvailableCooks.DeleteFirst();
					AvailableCooks.InsertEnd(pCook);
				}
			}
		}
		OutputFile();
		return;
	}
	int Nc, Gc, Vc, Ns, Gs, Vs, Nd, Gd, Vd;		//Counts for types of cooks and orders' types
	time_t start = clock();			//Reference to the program's starting time
	while (!(EventsQueue.isEmpty() && InServiceOrders.IsEmpty() && ArrivalOrders.IsEmpty()))
	{
		timestep++;		//Increments the timestep
		if (!EventsQueue.isEmpty())		//In case of an Event, Executes it & Updates the User Interface 
			ExecuteEvents(timestep);
		GeneralPromote(timestep);
		SortVIPOrders();
		FinishOrders(timestep);
		injureCooks(timestep);
		takeBreak(timestep);
		serveOrders(timestep);
		//Updates the User Interface After Changing Order Status at the current timestep
		pGUI->UpdateInterface();
		if (EventsQueue.isEmpty() && InServiceOrders.IsEmpty() && ArrivalOrders.IsEmpty())
		{
			while (!NonAvailableCooks.IsEmpty())
			{
				Cook* pCook;
				NonAvailableCooks.Peek(pCook);
				NonAvailableCooks.DeleteFirst();
				AvailableCooks.InsertEnd(pCook);
			}
		}
		FillDrawingList();
		countInserviceTypes(Ns, Gs, Vs);
		countCookTypes(Nc, Gc, Vc);
		if (!(EventsQueue.isEmpty() && InServiceOrders.IsEmpty() && ArrivalOrders.IsEmpty()))	//Checks whether the loop will continue or not
		{
			pGUI->PrintMessage("TimeStep: " + to_string(timestep) + "\t " + "Click to Proceed...", NormalOrders.getCount(), VeganOrders.getCount(), VIPOrders.getCount(), Nc, Gc, Vc, Ns, Gs, Vs, justAssigned);
			if (mode == MODE_INTR)
				pGUI->waitForClick();		//(User Interactive line)
			else							//(Step-by-Step Mode)
				while (difftime(clock(), start) / 1000 < timestep);		//Prevents the program from proceeding until 1000ms pass
		}
		IncrementWaiting();
	}
	//The Final User Click
	pGUI->PrintMessage("TimeStep: " + to_string(timestep) + "\t " + "All Orders are done, Click to end program...", NormalOrders.getCount(), VeganOrders.getCount(), VIPOrders.getCount(), Nc, Gc, Vc, Ns, Gs, Vs);
	pGUI->waitForClick();
	OutputFile();
}